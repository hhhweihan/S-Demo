#include "mini_rpc.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <stdexcept>

using mini_rpc::decode_request;
using mini_rpc::decode_response;
using mini_rpc::encode;
using mini_rpc::FrameCodec;
using mini_rpc::parse_fields;
using mini_rpc::Registry;
using mini_rpc::RpcClient;
using mini_rpc::RpcRequest;
using mini_rpc::RpcResponse;
using mini_rpc::RpcServer;

TEST(Codec, RequestEncodeDecodeRoundTrip) {
    RpcRequest request{7, "user", "get", "id=1"};
    RpcRequest decoded = decode_request(encode(request));
    EXPECT_EQ(decoded.id, 7u);
    EXPECT_EQ(decoded.service, "user");
    EXPECT_EQ(decoded.method, "get");
    EXPECT_EQ(decoded.payload, "id=1");
}

TEST(Codec, RoundTripPreservesDelimiterCharactersViaEscaping) {
    // Payloads containing the wire delimiters must survive escape/parse without corruption.
    RpcRequest request{1, "svc", "m", "a=b|c\\d"};
    RpcRequest decoded = decode_request(encode(request));
    EXPECT_EQ(decoded.payload, "a=b|c\\d");
}

TEST(Codec, ResponseEncodeDecodeRoundTrip) {
    RpcResponse response{9, false, "boom"};
    RpcResponse decoded = decode_response(encode(response));
    EXPECT_EQ(decoded.id, 9u);
    EXPECT_FALSE(decoded.ok);
    EXPECT_EQ(decoded.payload, "boom");
}

TEST(Codec, ParseFieldsSplitsKeyValuePairs) {
    auto fields = parse_fields("type=req|id=3|service=echo");
    EXPECT_EQ(fields.at("type"), "req");
    EXPECT_EQ(fields.at("id"), "3");
    EXPECT_EQ(fields.at("service"), "echo");
}

TEST(FrameCodec, EncodesAndDecodesBackToBackFrames) {
    FrameCodec codec;
    codec.append(FrameCodec::encode_frame("first"));
    codec.append(FrameCodec::encode_frame("second"));
    EXPECT_EQ(*codec.next_frame(), "first");
    EXPECT_EQ(*codec.next_frame(), "second");
    EXPECT_FALSE(codec.next_frame().has_value());
}

TEST(FrameCodec, WaitsForCompleteFrame) {
    FrameCodec codec;
    std::string frame = FrameCodec::encode_frame("payload");
    codec.append(frame.substr(0, 5));
    EXPECT_FALSE(codec.next_frame().has_value());
    codec.append(frame.substr(5));
    EXPECT_EQ(*codec.next_frame(), "payload");
}

TEST(RpcServer, RegisteredMethodDispatchesAndReturnsResult) {
    RpcServer server;
    server.register_method("math", "double", [](const std::string& body) { return body + body; });
    RpcClient client(server);
    RpcResponse response = client.call("math", "double", "ab");
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.payload, "abab");
}

TEST(RpcServer, UnknownMethodReturnsFailure) {
    RpcServer server;
    RpcClient client(server);
    RpcResponse response = client.call("math", "missing", "x");
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.payload, "method not found");
}

TEST(RpcServer, InterceptorsRunInRegistrationOrderBeforeHandler) {
    RpcServer server;
    std::vector<std::string> log;
    server.add_interceptor([&log](const RpcRequest&, RpcResponse&) {
        log.push_back("first");
        return true;
    });
    server.add_interceptor([&log](const RpcRequest&, RpcResponse&) {
        log.push_back("second");
        return true;
    });
    server.register_method("s", "m", [&log](const std::string& body) {
        log.push_back("handler");
        return body;
    });
    RpcClient client(server);
    RpcResponse response = client.call("s", "m", "ok");
    EXPECT_TRUE(response.ok);
    ASSERT_EQ(log.size(), 3u);
    EXPECT_EQ(log[0], "first");
    EXPECT_EQ(log[1], "second");
    EXPECT_EQ(log[2], "handler");
}

TEST(RpcServer, RejectingInterceptorShortCircuitsHandler) {
    // A false-returning interceptor must stop the chain and skip the handler entirely.
    RpcServer server;
    bool handler_called = false;
    server.add_interceptor([](const RpcRequest&, RpcResponse& response) {
        response.ok = false;
        response.payload = "denied";
        return false;
    });
    server.register_method("s", "m", [&handler_called](const std::string& body) {
        handler_called = true;
        return body;
    });
    RpcClient client(server);
    RpcResponse response = client.call("s", "m", "ok");
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.payload, "denied");
    EXPECT_FALSE(handler_called);
}

TEST(Registry, DiscoverRoundRobinsAcrossEndpoints) {
    Registry registry;
    registry.register_service("cache", "10.0.0.1");
    registry.register_service("cache", "10.0.0.2");
    EXPECT_EQ(registry.discover("cache"), "10.0.0.1");
    EXPECT_EQ(registry.discover("cache"), "10.0.0.2");
    EXPECT_EQ(registry.discover("cache"), "10.0.0.1");
}

TEST(Registry, DiscoverUnknownServiceThrows) {
    Registry registry;
    EXPECT_THROW(registry.discover("nope"), std::runtime_error);
}

TEST(RpcClient, AsyncCallResolvesToHandlerResult) {
    RpcServer server;
    server.register_method("svc", "echo", [](const std::string& body) { return body; });
    RpcClient client(server);
    auto future = client.async_call("svc", "echo", "hello");
    RpcResponse response = future.get();
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.payload, "hello");
}
