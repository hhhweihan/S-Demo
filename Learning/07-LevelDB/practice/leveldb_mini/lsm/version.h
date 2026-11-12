#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

// 版本/文件元信息。真 LevelDB 用 MANIFEST 日志记录每次 compaction 后的文件集变更（VersionEdit），
// 重启时回放 MANIFEST 重建当前 Version。mini 版做**无 MANIFEST 简化**：把 level 与文件号直接编进
// SSTable 文件名，Recover 时扫描目录即可重建分层文件集。代价是丢失原子的多文件切换语义，但对
// "单线程、flush/compaction 串行"的教学实现足够，且崩溃安全仍由 WAL 保证。
namespace mini_lsm {

// 一个 SSTable 文件的元信息：文件号（单调递增，定序与命名）、所在层、key 范围、磁盘路径。
struct FileMetaData {
    std::uint64_t number = 0;
    int level = 0;
    std::string smallest;  // 最小 user_key
    std::string largest;   // 最大 user_key
    std::filesystem::path path;
};

// 文件名：sst_L{level}_{number:06}.sst。level 与 number 直接可从名字解析，供无 MANIFEST 恢复。
inline std::string SSTableFileName(int level, std::uint64_t number) {
    std::string num = std::to_string(number);
    if (num.size() < 6) {
        num = std::string(6 - num.size(), '0') + num;
    }
    return "sst_L" + std::to_string(level) + "_" + num + ".sst";
}

inline std::filesystem::path SSTablePath(const std::filesystem::path& dir, int level,
                                         std::uint64_t number) {
    return dir / SSTableFileName(level, number);
}

// 解析文件名 → level + number。非 SSTable 文件返回 false。
inline bool ParseSSTableFileName(const std::string& name, int* level, std::uint64_t* number) {
    // 期望前缀 "sst_L"、中缀 "_"、后缀 ".sst"。
    constexpr const char* kPrefix = "sst_L";
    constexpr std::size_t kPrefixLen = 5;
    if (name.size() < kPrefixLen + 4 || name.compare(0, kPrefixLen, kPrefix) != 0) {
        return false;
    }
    if (name.compare(name.size() - 4, 4, ".sst") != 0) {
        return false;
    }
    const std::size_t underscore = name.find('_', kPrefixLen);
    if (underscore == std::string::npos) {
        return false;
    }
    const std::string level_str = name.substr(kPrefixLen, underscore - kPrefixLen);
    const std::string number_str = name.substr(underscore + 1, name.size() - 4 - (underscore + 1));
    if (level_str.empty() || number_str.empty()) {
        return false;
    }
    for (char c : level_str) {
        if (c < '0' || c > '9') return false;
    }
    for (char c : number_str) {
        if (c < '0' || c > '9') return false;
    }
    *level = std::stoi(level_str);
    *number = std::stoull(number_str);
    return true;
}

}  // namespace mini_lsm
