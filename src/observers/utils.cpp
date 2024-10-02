#include "utils.h"

#include <filesystem>

std::int64_t count_fs_entries(const char* path)
{
    int64_t count = 0;

    try {
        for (const auto& _ : std::filesystem::directory_iterator(path)) {
            ++count;
        }
    }
    catch (const std::filesystem::filesystem_error&) {
        count = -1;
    }

    return count;
}
