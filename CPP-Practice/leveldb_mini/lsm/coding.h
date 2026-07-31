#pragma once

#include <cstdint>
#include <string>

// 定长小端编解码。WAL 记录帧、SSTable 的 block/index/footer 全用这套定长整数序列化——定长（而非
// 变长 varint）是有意的简化：解析代码更短、更易在崩溃恢复时做边界检查。真 LevelDB 混用 varint 省
// 空间，这里优先可读与可验证。所有多字节整数统一小端，跨端序平台读同一文件才一致。
namespace mini_lsm {

inline void PutFixed32(std::string* dst, std::uint32_t value) {
    char buf[4];
    buf[0] = static_cast<char>(value & 0xFFu);
    buf[1] = static_cast<char>((value >> 8) & 0xFFu);
    buf[2] = static_cast<char>((value >> 16) & 0xFFu);
    buf[3] = static_cast<char>((value >> 24) & 0xFFu);
    dst->append(buf, sizeof(buf));
}

inline void PutFixed64(std::string* dst, std::uint64_t value) {
    char buf[8];
    for (int i = 0; i < 8; ++i) {
        buf[i] = static_cast<char>((value >> (8 * i)) & 0xFFu);
    }
    dst->append(buf, sizeof(buf));
}

// 长度前缀字符串：u32 长度 + 原始字节。用于变长 key/value 的自描述编码。
inline void PutLengthPrefixed(std::string* dst, const std::string& value) {
    PutFixed32(dst, static_cast<std::uint32_t>(value.size()));
    dst->append(value);
}

inline std::uint32_t DecodeFixed32(const char* ptr) {
    return static_cast<std::uint32_t>(static_cast<std::uint8_t>(ptr[0])) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(ptr[1])) << 8) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(ptr[2])) << 16) |
           (static_cast<std::uint32_t>(static_cast<std::uint8_t>(ptr[3])) << 24);
}

inline std::uint64_t DecodeFixed64(const char* ptr) {
    std::uint64_t value = 0;
    for (int i = 0; i < 8; ++i) {
        value |= static_cast<std::uint64_t>(static_cast<std::uint8_t>(ptr[i])) << (8 * i);
    }
    return value;
}

}  // namespace mini_lsm
