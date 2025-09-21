#pragma once

#include <ios>
#include <iostream>
#include <string>
#include <type_traits>

#define TEMP_FILE "temp.dat"

template <typename Stream>
concept FileStream = std::is_base_of_v<std::ios_base, Stream>;

template <FileStream Stream> bool is_file_good(Stream &file);

bool is_file_deleted(const std::string &file_name);
bool is_file_rename(const std::string &file_name, const std::string &new_name);

#include "file.ipp"
