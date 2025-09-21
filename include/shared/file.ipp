#pragma once

template <FileStream Stream> bool is_file_good(Stream &file) {
    if constexpr (!std::is_base_of_v<std::basic_fstream<char>, Stream> &&
                  !std::is_base_of_v<std::basic_ifstream<char>, Stream> &&
                  !std::is_base_of_v<std::basic_ofstream<char>, Stream>) {
        std::cerr << "The file is not consider as stream\n";
        return false;
    }

    if (!file.is_open()) {
        std::cerr << "Failed to open task file!\n";
        return false;
    }

    if (file.fail()) {
        std::cerr << "Stream failed to state!\n";
        return false;
    }

    if (file.bad()) {
        std::cerr << "Stream bad state!\n";
        return false;
    }

    return true;
}
