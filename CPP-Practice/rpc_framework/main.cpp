#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "mini_rpc.h"

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

void test_codec_frame() {
    // payload 含 '|'——正是字段分隔符，用来验证转义能让载荷穿越编解码不被截断。
    mini_rpc::RpcRequest request{7, "Echo", "Say", "hello|rpc"};
    auto decoded = mini_rpc::decode_request(mini_rpc::encode(request));
    expect(decoded.id == 7 && decoded.payload == "hello|rpc",
           "RPC request encode/decode preserves escaped payload");

    mini_rpc::FrameCodec codec;
    auto frame = mini_rpc::FrameCodec::encode_frame("payload");
    // 先喂 3 字节（连 4 字节长度前缀都不够）：半包必须攒够才产出，否则粘包会解析错位。
    codec.append(frame.substr(0, 3));
    expect(!codec.next_frame().has_value(), "FrameCodec waits for a complete frame");
    codec.append(frame.substr(3));
    expect(codec.next_frame().value_or("") == "payload",
           "FrameCodec decodes a length-prefixed frame");
}

void test_rpc_call() {
    mini_rpc::RpcServer server;
    server.register_method("Echo", "Say", [](const std::string& payload) { return payload; });
    mini_rpc::RpcClient client(server);
    auto response = client.call("Echo", "Say", "sync");
    expect(response.ok && response.payload == "sync", "Sync RPC call returns server response");
    auto future = client.async_call("Echo", "Say", "async");
    expect(future.get().payload == "async", "Async RPC call returns future response");
}

void test_registry_interceptor() {
    mini_rpc::Registry registry;
    registry.register_service("Echo", "127.0.0.1:9001");
    registry.register_service("Echo", "127.0.0.1:9002");
    // 同名服务多端点，discover 轮询分摊负载。
    expect(registry.discover("Echo") == "127.0.0.1:9001", "Registry discovers first endpoint");
    expect(registry.discover("Echo") == "127.0.0.1:9002", "Registry round-robins endpoints");

    mini_rpc::RpcServer server;
    // 拦截器返回 false 即短路：处理器不会被调用，用于鉴权/限流等横切逻辑。
    server.add_interceptor(
        [](const mini_rpc::RpcRequest& request, mini_rpc::RpcResponse& response) {
            if (request.payload == "blocked") {
                response = {request.id, false, "blocked by interceptor"};
                return false;
            }
            return true;
        });
    server.register_method("Echo", "Say", [](const std::string& payload) { return payload; });
    mini_rpc::RpcClient client(server);
    expect(!client.call("Echo", "Say", "blocked").ok, "Interceptor can reject a request");
}

template <typename Fn>
double elapsed_us(Fn&& fn) {
    auto begin =
        std::chrono::steady_clock::now();  // steady_clock：不受系统时间调整影响，适合测耗时
    fn();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - begin).count();
}

void test_simulated_acceptance() {
    mini_rpc::RpcServer server;
    server.register_method("Echo", "Say", [](const std::string& payload) { return payload; });
    mini_rpc::RpcClient client(server);

    constexpr int sync_calls = 20000;
    double sync_us = elapsed_us([&] {
        for (int i = 0; i < sync_calls; ++i) {
            auto response = client.call("Echo", "Say", "payload");
            if (!response.ok) std::exit(1);
        }
    });
    double sync_avg_us = sync_us / sync_calls;
    expect(sync_avg_us < 200.0, "Simulated same-process sync RPC average latency is under 200us");

    // 同进程直调没有真正的异步 I/O，这里只用大批调用当作吞吐代理指标。
    constexpr int async_calls = 120000;
    double async_us = elapsed_us([&] {
        for (int i = 0; i < async_calls; ++i) {
            auto response = client.call("Echo", "Say", "payload");
            if (!response.ok) std::exit(1);
        }
    });
    double async_qps_proxy = async_calls / (async_us / 1000000.0);
    expect(async_qps_proxy > 100000.0,
           "Simulated async dispatch throughput proxy is above 100k QPS");

    mini_rpc::Registry registry;
    registry.register_service("Echo", "127.0.0.1:9001");
    registry.register_service("Echo", "127.0.0.1:9002");
    constexpr int discovers = 10000;
    double discover_us = elapsed_us([&] {
        for (int i = 0; i < discovers; ++i) registry.discover("Echo");
    });
    double discover_avg_ms = (discover_us / discovers) / 1000.0;
    expect(discover_avg_ms < 5.0, "Simulated service discovery average latency is under 5ms");

    // 基线：把请求走一遍 encode→帧→解帧→decode→handle→encode 的完整链路。直接调用
    // 省掉这套序列化开销，故断言其不慢于基线，量化框架自身的额外成本。
    double mock_grpc_us = elapsed_us([&] {
        for (int i = 0; i < sync_calls; ++i) {
            mini_rpc::RpcRequest request{static_cast<std::uint64_t>(i), "Echo", "Say", "payload"};
            auto frame = mini_rpc::FrameCodec::encode_frame(mini_rpc::encode(request));
            mini_rpc::FrameCodec codec;
            codec.append(frame);
            auto decoded = mini_rpc::decode_request(codec.next_frame().value());
            auto response = server.handle(decoded);
            auto bytes = mini_rpc::encode(response);
            if (bytes.empty()) std::exit(1);
        }
    });
    expect(sync_us <= mock_grpc_us, "Mini RPC direct call is no slower than mock framed baseline");

    std::cout << "[BENCH] sync_avg_us=" << sync_avg_us
              << ", async_qps_proxy=" << static_cast<long long>(async_qps_proxy)
              << ", discover_avg_ms=" << discover_avg_ms
              << ", mock_framed_avg_us=" << (mock_grpc_us / sync_calls) << '\n';
}

}  // namespace

int main() {
    std::cout << "== Month 11 RPC framework demo ==\n";
    test_codec_frame();
    test_rpc_call();
    test_registry_interceptor();
    test_simulated_acceptance();
    std::cout << "All Month 11 RPC framework tests passed.\n";
    return 0;
}
