#include "status.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace {

constexpr auto KIBIBYTE_IN_BYTES = 1024;

using handler_t = void (*)(long long int, status_data&);

/* Hashing logic was generated with gperf */

// clang-format off
constexpr std::array<std::uint8_t, 256> asso_values{
    /* 00 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* 10 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* 20 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* 30 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* 40 */ 19,  8, 19, 19, 19, 19,  3, 19,  0, 19, 19, 19, 19, 19, 19, 19,
    /* 50 */ 19, 19,  1,  0,  2, 19,  0,  9, 19, 19, 19, 19, 19, 19, 19, 19,
    /* 60 */ 19, 19, 19,  8, 19,  6, 19, 19, 19,  1, 19, 19, 19, 19, 19, 19,
    /* 70 */ 19, 19, 19, 19, 19, 19, 19,  2, 19, 19, 19, 19, 19, 19, 19, 19,
    /* 80 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* 90 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* A0 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* B0 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* C0 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* D0 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* E0 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    /* F0 */ 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19
};
// clang-format on

constexpr auto MIN_WORD_LENGTH = 5;
constexpr auto MAX_WORD_LENGTH = 12;
constexpr auto MAX_HASH_VALUE  = 18;

constexpr std::array<std::string_view, MAX_HASH_VALUE + 1> wordlist{
    "", "", "", "", "",
    "VmRSS",     // 5
    "VmPin",     // 6
    "VmSize",    // 7
    "VmSwap",    // 8
    "RssShmem",  // 9
    "FDSize",    // 10
    "RssFile",   // 11
    "VmPeak",    // 12
    "VmLck",     // 13
    "VmHWM",     // 14
    "Threads",   // 15
    "RssAnon",   // 16
    "",
    "HugetlbPages"  // 18
};

constexpr std::array<handler_t, MAX_HASH_VALUE + 1> handlers{
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    [](long long int value, status_data& data) { data.rss = value * KIBIBYTE_IN_BYTES; },              // 5, VmRSS
    [](long long int value, status_data& data) { data.pinned_vm_size = value * KIBIBYTE_IN_BYTES; },   // 6, VmPin
    [](long long int value, status_data& data) { data.vm_size = value * KIBIBYTE_IN_BYTES; },          // 7, VmSize
    [](long long int value, status_data& data) { data.swapped_vm_size = value * KIBIBYTE_IN_BYTES; },  // 8, VmSwap
    [](long long int value, status_data& data) { data.rss_shmem = value * KIBIBYTE_IN_BYTES; },        // 9, RssShmem
    [](long long int value, status_data& data) { data.fd_size = value; },                              // 10, FDSize
    [](long long int value, status_data& data) { data.rss_file = value * KIBIBYTE_IN_BYTES; },         // 11, RssFile
    [](long long int value, status_data& data) { data.peak_vm_size = value * KIBIBYTE_IN_BYTES; },     // 12, VmPeak
    [](long long int value, status_data& data) { data.locked_vm_size = value * KIBIBYTE_IN_BYTES; },   // 13, VmLck
    [](long long int value, status_data& data) { data.peak_rss = value * KIBIBYTE_IN_BYTES; },         // 14, VmHWM
    [](long long int value, status_data& data) { data.threads = value; },                              // 15, Threads
    [](long long int value, status_data& data) { data.rss_anon = value * KIBIBYTE_IN_BYTES; },         // 16, RssAnon
    nullptr,
    [](long long int value, status_data& data) { data.huge_tlb_pages = value * KIBIBYTE_IN_BYTES; }  // 18, HugetlbPages
};

std::size_t hash(std::string_view s)
{
    assert(s.size() >= 4);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    return s.size() + asso_values[static_cast<unsigned char>(s[3])] + asso_values[static_cast<unsigned char>(s[0])];
}

handler_t get_handler(std::string_view s)
{
    if (s.size() <= MAX_WORD_LENGTH && s.size() >= MIN_WORD_LENGTH) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
        if (auto key = hash(s); key <= MAX_HASH_VALUE && wordlist[key] == s) {
            return handlers[key];  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }
    }

    return nullptr;
}

}  // namespace

proc_pid_status_observer::proc_pid_status_observer()
{
    this->observe();
}

void proc_pid_status_observer::observe()
{
    std::memset(&this->data, 0, sizeof(this->data));

    try {
        std::ifstream f("/proc/self/status");
        std::string line;
        std::string key;

        while (std::getline(f, line)) {
            std::istringstream iss(line);
            iss >> key;
            if (auto handler = get_handler(key); handler) {
                long long int value;  // NOLINT(*-init-variables)
                iss >> value;
                handler(value, this->data);
            }
        }

        this->data.ok = true;
    }
    catch (const std::exception&) {
        this->data.ok = false;
    }
}
