#include "byte_ring.h"
#include "frame_parser.h"

#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>

TEST(ByteRing, EmptyPopPreservesOutput) {
    std::array<uint8_t, 3> storage{};
    ByteRing ring{};
    ASSERT_EQ(byte_ring_init(&ring, storage.data(), storage.size()), BYTE_RING_OK);

    uint8_t value = 99;
    EXPECT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_EMPTY);
    EXPECT_EQ(value, 99);
}

TEST(ByteRing, UsesEverySlotAndRejectsOverflowWithoutLosingData) {
    std::array<uint8_t, 3> storage{};
    ByteRing ring{};
    ASSERT_EQ(byte_ring_init(&ring, storage.data(), storage.size()), BYTE_RING_OK);

    for (uint8_t value : {10, 20, 30}) {
        ASSERT_EQ(byte_ring_push(&ring, value), BYTE_RING_OK);
    }
    EXPECT_EQ(byte_ring_push(&ring, 40), BYTE_RING_FULL);

    uint8_t value = 0;
    for (uint8_t expected : {10, 20, 30}) {
        ASSERT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_OK);
        EXPECT_EQ(value, expected);
    }
    EXPECT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_EMPTY);
    EXPECT_EQ(value, 30);
}

TEST(ByteRing, WrapsWithNonPowerOfTwoCapacity) {
    std::array<uint8_t, 3> storage{};
    ByteRing ring{};
    ASSERT_EQ(byte_ring_init(&ring, storage.data(), storage.size()), BYTE_RING_OK);

    for (uint8_t cycle = 0; cycle < 12; ++cycle) {
        const uint8_t first = static_cast<uint8_t>(cycle * 3);
        ASSERT_EQ(byte_ring_push(&ring, first), BYTE_RING_OK);
        ASSERT_EQ(byte_ring_push(&ring, static_cast<uint8_t>(first + 1)), BYTE_RING_OK);
        uint8_t value = 0;
        ASSERT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_OK);
        EXPECT_EQ(value, first);
        ASSERT_EQ(byte_ring_push(&ring, static_cast<uint8_t>(first + 2)), BYTE_RING_OK);
        ASSERT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_OK);
        EXPECT_EQ(value, first + 1);
        ASSERT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_OK);
        EXPECT_EQ(value, first + 2);
    }
}

TEST(ByteRing, CapacityOneCanBeReused) {
    std::array<uint8_t, 1> storage{};
    ByteRing ring{};
    ASSERT_EQ(byte_ring_init(&ring, storage.data(), storage.size()), BYTE_RING_OK);

    uint8_t value = 0;
    for (uint8_t expected : {0, 255, 17}) {
        ASSERT_EQ(byte_ring_push(&ring, expected), BYTE_RING_OK);
        EXPECT_EQ(byte_ring_push(&ring, 42), BYTE_RING_FULL);
        ASSERT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_OK);
        EXPECT_EQ(value, expected);
    }
}

TEST(ByteRing, RejectsInvalidArgumentsWithoutCorruptingInitializedRing) {
    std::array<uint8_t, 3> storage{};
    ByteRing ring{};
    uint8_t value = 99;
    EXPECT_EQ(byte_ring_init(nullptr, storage.data(), storage.size()), BYTE_RING_INVALID_ARGUMENT);
    EXPECT_EQ(byte_ring_init(&ring, nullptr, storage.size()), BYTE_RING_INVALID_ARGUMENT);
    EXPECT_EQ(byte_ring_init(&ring, storage.data(), 0), BYTE_RING_INVALID_ARGUMENT);
    EXPECT_EQ(byte_ring_push(nullptr, 1), BYTE_RING_INVALID_ARGUMENT);
    EXPECT_EQ(byte_ring_pop(nullptr, &value), BYTE_RING_INVALID_ARGUMENT);
    EXPECT_EQ(value, 99);

    ASSERT_EQ(byte_ring_init(&ring, storage.data(), storage.size()), BYTE_RING_OK);
    ASSERT_EQ(byte_ring_push(&ring, 7), BYTE_RING_OK);
    EXPECT_EQ(byte_ring_pop(&ring, nullptr), BYTE_RING_INVALID_ARGUMENT);
    EXPECT_EQ(byte_ring_init(&ring, nullptr, storage.size()), BYTE_RING_INVALID_ARGUMENT);
    ASSERT_EQ(byte_ring_pop(&ring, &value), BYTE_RING_OK);
    EXPECT_EQ(value, 7);
}

TEST(FrameParser, ParsesOneFrameAndLeavesTrailingInput) {
    const std::array<uint8_t, 7> input{0x00, 0x03, 0x11, 0x22, 0x33, 0x00, 0x00};
    FrameView frame{};
    size_t consumed = 0;

    ASSERT_EQ(parse_frame(input.data(), input.size(), &frame, &consumed), FRAME_OK);
    EXPECT_EQ(consumed, 5u);
    EXPECT_EQ(frame.payload, input.data() + 2);
    EXPECT_EQ(frame.size, 3u);
}

TEST(FrameParser, AcceptsZeroAndMaximumPayload) {
    const std::array<uint8_t, 2> empty{0x00, 0x00};
    FrameView frame{};
    size_t consumed = 0;
    ASSERT_EQ(parse_frame(empty.data(), empty.size(), &frame, &consumed), FRAME_OK);
    EXPECT_EQ(consumed, 2u);
    EXPECT_EQ(frame.payload, empty.data() + 2);
    EXPECT_EQ(frame.size, 0u);

    std::array<uint8_t, 34> maximum{};
    maximum[1] = 0x20;
    ASSERT_EQ(parse_frame(maximum.data(), maximum.size(), &frame, &consumed), FRAME_OK);
    EXPECT_EQ(consumed, 34u);
    EXPECT_EQ(frame.payload, maximum.data() + 2);
    EXPECT_EQ(frame.size, 32u);
}

TEST(FrameParser, IncompleteInputPreservesOutputs) {
    const std::array<uint8_t, 5> input{0x00, 0x03, 0x11, 0x22, 0x33};
    for (size_t length = 0; length < input.size(); ++length) {
        FrameView frame{input.data(), 17};
        size_t consumed = 91;
        EXPECT_EQ(parse_frame(input.data(), length, &frame, &consumed), FRAME_INCOMPLETE);
        EXPECT_EQ(frame.payload, input.data());
        EXPECT_EQ(frame.size, 17u);
        EXPECT_EQ(consumed, 91u);
    }

    FrameView frame{input.data(), 17};
    size_t consumed = 91;
    EXPECT_EQ(parse_frame(nullptr, 0, &frame, &consumed), FRAME_INCOMPLETE);
    EXPECT_EQ(frame.payload, input.data());
    EXPECT_EQ(frame.size, 17u);
    EXPECT_EQ(consumed, 91u);
}

TEST(FrameParser, RejectsOversizedLengthsBeforeWaitingForPayload) {
    const std::array<std::array<uint8_t, 2>, 3> inputs{{{0x00, 0x21}, {0x01, 0x00}, {0xFF, 0xFF}}};
    for (const auto& input : inputs) {
        FrameView frame{input.data(), 17};
        size_t consumed = 91;
        EXPECT_EQ(parse_frame(input.data(), input.size(), &frame, &consumed), FRAME_INVALID_LENGTH);
        EXPECT_EQ(frame.payload, input.data());
        EXPECT_EQ(frame.size, 17u);
        EXPECT_EQ(consumed, 91u);
    }
}

TEST(FrameParser, RejectsInvalidArgumentsAndPreservesAvailableOutputs) {
    const std::array<uint8_t, 2> input{0x00, 0x00};
    FrameView frame{input.data(), 17};
    size_t consumed = 91;

    EXPECT_EQ(parse_frame(nullptr, 1, &frame, &consumed), FRAME_INVALID_ARGUMENT);
    EXPECT_EQ(parse_frame(input.data(), input.size(), nullptr, &consumed), FRAME_INVALID_ARGUMENT);
    EXPECT_EQ(parse_frame(input.data(), input.size(), &frame, nullptr), FRAME_INVALID_ARGUMENT);
    EXPECT_EQ(frame.payload, input.data());
    EXPECT_EQ(frame.size, 17u);
    EXPECT_EQ(consumed, 91u);
}
