#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

// CRC32C（Castagnoli 多项式 0x1EDC6F41，反射实现）。header-only、无外部依赖。
// WAL/SSTable 的每条记录都带一个 CRC32C 校验和：回放时算一遍比对，不符即判定该记录被崩溃/坏盘
// 截断或篡改，据此丢弃损坏尾部。选 CRC32C 而非 CRC32 是因为它有硬件指令（SSE4.2 / ARMv8 CRC）且
// 是 LevelDB/RocksDB 的既定选择——这里用软件表驱动实现，保证跨平台可跑。
namespace mini_lsm::crc32c {

// 反射后的 Castagnoli 多项式：0x1EDC6F41 位反转即 0x82F63B78。
inline constexpr std::uint32_t kPoly = 0x82F63B78u;

// 编译期生成 256 项查表。constexpr 保证是只读静态数据，无运行期初始化竞态。
inline constexpr std::array<std::uint32_t, 256> MakeTable() {
    std::array<std::uint32_t, 256> table{};
    for (std::uint32_t i = 0; i < 256; ++i) {
        std::uint32_t crc = i;
        for (int bit = 0; bit < 8; ++bit) {
            crc = (crc & 1u) ? (crc >> 1) ^ kPoly : (crc >> 1);
        }
        table[i] = crc;
    }
    return table;
}

inline constexpr std::array<std::uint32_t, 256> kTable = MakeTable();

// 计算 [data, data+n) 的 CRC32C。标准做法：初值全 1，逐字节查表，最后再取反。
inline std::uint32_t Value(const char* data, std::size_t n) {
    std::uint32_t crc = 0xFFFFFFFFu;
    for (std::size_t i = 0; i < n; ++i) {
        const std::uint8_t byte = static_cast<std::uint8_t>(data[i]);
        crc = kTable[(crc ^ byte) & 0xFFu] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}

inline std::uint32_t Value(const std::string& data) {
    return Value(data.data(), data.size());
}

}  // namespace mini_lsm::crc32c
