#include "mini_reactor.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

using mini_net::Buffer;
using mini_net::Channel;
using mini_net::EventLoop;
using mini_net::EventLoopThreadPool;
using mini_net::HttpParser;
using mini_net::HttpRequest;
using mini_net::HttpServerCore;
using mini_net::RpcClientCore;
using mini_net::RpcServerCore;

TEST(Buffer, FrameRoundTripsPayload) {
    Buffer buffer;
    buffer.append(Buffer::frame("hello"));
    auto frame = buffer.read_frame();
    ASSERT_TRUE(frame.has_value());
    EXPECT_EQ(*frame, "hello");
    EXPECT_EQ(buffer.readable_bytes(), 0u);
}

TEST(Buffer, ReadFrameWaitsForCompleteBody) {
    // A length-prefixed frame must not be surfaced until every declared byte has arrived.
    Buffer buffer;
    std::string encoded = Buffer::frame("payload");
    buffer.append(encoded.substr(0, encoded.size() - 2));
    EXPECT_FALSE(buffer.read_frame().has_value());
    buffer.append(encoded.substr(encoded.size() - 2));
    auto frame = buffer.read_frame();
    ASSERT_TRUE(frame.has_value());
    EXPECT_EQ(*frame, "payload");
}

TEST(Buffer, ReadFrameHandlesBackToBackFrames) {
    Buffer buffer;
    buffer.append(Buffer::frame("a"));
    buffer.append(Buffer::frame("bb"));
    EXPECT_EQ(*buffer.read_frame(), "a");
    EXPECT_EQ(*buffer.read_frame(), "bb");
    EXPECT_FALSE(buffer.read_frame().has_value());
}

TEST(Buffer, RetrieveClampsToAvailable) {
    Buffer buffer;
    buffer.append("abc");
    EXPECT_EQ(buffer.retrieve(100), "abc");
    EXPECT_EQ(buffer.readable_bytes(), 0u);
}

TEST(HttpParser, ParsesRequestLineAndHeaders) {
    const std::string raw =
        "GET /index HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Accept: text/html\r\n"
        "\r\n";
    HttpRequest request = HttpParser::parse(raw);
    EXPECT_EQ(request.method, "GET");
    EXPECT_EQ(request.path, "/index");
    EXPECT_EQ(request.headers.at("Host"), "example.com");
    EXPECT_EQ(request.headers.at("Accept"), "text/html");
}

TEST(HttpParser, TrimsLeadingSpacesInHeaderValue) {
    // Values carry arbitrary leading whitespace after the colon; only that padding is stripped.
    HttpRequest request = HttpParser::parse("POST /x HTTP/1.1\r\nX-Token:    abc def\r\n\r\n");
    EXPECT_EQ(request.method, "POST");
    EXPECT_EQ(request.headers.at("X-Token"), "abc def");
}

TEST(HttpParser, IgnoresMalformedHeaderLineWithoutColon) {
    HttpRequest request = HttpParser::parse("GET / HTTP/1.1\r\ngarbage-line\r\nOk: 1\r\n\r\n");
    EXPECT_EQ(request.headers.count("garbage-line"), 0u);
    EXPECT_EQ(request.headers.at("Ok"), "1");
}

TEST(HttpParser, HandlesRequestWithNoHeaders) {
    HttpRequest request = HttpParser::parse("GET /ping HTTP/1.1\r\n\r\n");
    EXPECT_EQ(request.path, "/ping");
    EXPECT_TRUE(request.headers.empty());
}

TEST(EventLoop, DispatchesOnlyRegisteredActivatedChannels) {
    // loop_once must fire read callbacks exactly for fds that are both registered and active.
    EventLoop loop;
    int fired = 0;
    Channel channel(7);
    channel.set_read_callback([&fired] { ++fired; });
    loop.update(&channel);

    loop.activate(9);  // unregistered fd is ignored
    EXPECT_EQ(loop.loop_once(), 0);
    EXPECT_EQ(fired, 0);

    loop.activate(7);
    EXPECT_EQ(loop.loop_once(), 1);
    EXPECT_EQ(fired, 1);
}

TEST(EventLoopThreadPool, NextLoopRotatesRoundRobin) {
    EventLoopThreadPool pool(3);
    EXPECT_EQ(pool.size(), 3u);
    EventLoop* first = &pool.next_loop();
    EventLoop* second = &pool.next_loop();
    EventLoop* third = &pool.next_loop();
    EventLoop* wrap = &pool.next_loop();
    EXPECT_NE(first, second);
    EXPECT_NE(second, third);
    EXPECT_EQ(first, wrap);
}

TEST(HttpServerCore, RoutesGetRequestToHandler) {
    HttpServerCore server;
    server.get("/hi", [](const HttpRequest&) { return "world"; });
    std::string response = server.handle("GET /hi HTTP/1.1\r\n\r\n");
    EXPECT_NE(response.find("200 OK"), std::string::npos);
    EXPECT_NE(response.find("world"), std::string::npos);
    EXPECT_NE(response.find("Content-Length: 5"), std::string::npos);
}

TEST(HttpServerCore, UnknownPathReturns404) {
    HttpServerCore server;
    std::string response = server.handle("GET /missing HTTP/1.1\r\n\r\n");
    EXPECT_NE(response.find("404 Not Found"), std::string::npos);
}

TEST(HttpServerCore, NonGetMethodReturns404) {
    HttpServerCore server;
    server.get("/hi", [](const HttpRequest&) { return "world"; });
    std::string response = server.handle("POST /hi HTTP/1.1\r\n\r\n");
    EXPECT_NE(response.find("404 Not Found"), std::string::npos);
}

TEST(RpcCore, ClientCallDispatchesToRegisteredMethod) {
    RpcServerCore server;
    server.register_method("echo", [](const std::string& body) { return body; });
    RpcClientCore client(server);
    EXPECT_EQ(client.call("echo", "ping"), "OK:ping");
}

TEST(RpcCore, UnknownMethodAndMalformedFrameReportErrors) {
    RpcServerCore server;
    EXPECT_EQ(server.handle_frame("nope:body"), "ERR:not found");
    EXPECT_EQ(server.handle_frame("no-colon-here"), "ERR:bad request");
}
