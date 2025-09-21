#include <format>

#include "shared/file.hpp"

bool is_file_deleted(const std::string &file_name) {
    if (std::remove(file_name.c_str()) != 0) {
        std::cout << "Failed to delete file : " << file_name << "\n";
        return false;
    }

    return true;
}

bool is_file_rename(const std::string &file_name, const std::string &new_name) {
    if (std::rename(file_name.c_str(), new_name.c_str()) != 0) {
        std::cout << std::format("Failed to rename file: {} to {}\n", file_name,
                                 new_name);
        return false;
    }

    return true;
}
