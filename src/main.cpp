#include <cstddef>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "shared/file.hpp"
#include "task/Task.hpp"

static bool add(const char **argv);
static bool remove(const char **argv);
static bool list(const char **argv);
static bool modify(const char **argv);

static std::unique_ptr<Task> create_shr_task_ptr(const char *argv[]);

constexpr std::size_t NUMBER_OF_FUNCS = 4;
using Func = bool (*)(const char *argv[]);

struct Entry {
    std::string_view key;
    Func func;
};

constexpr std::array<Entry, NUMBER_OF_FUNCS> func_arr = {{"add", add}};

constexpr Func find_func(std::string_view &operation) {
    for (const Entry &entry : func_arr) {
        if (entry.key == operation)
            return entry.func;
    }

    return nullptr;
}

int main(int argc, const char **argv) {
    if (argc <= 1) {
        std::cerr << "Usage: " << argv[0] << " <command>\n"
                  << "Commands: add, list, remove, modify" << std::endl;

        return 1;
    }

    std::string_view operation = argv[1];
    if (auto func = find_func(operation)) {
        return func(argv) ? 0 : 1;
    }

    std::cerr << "Unknown command : " << operation << std::endl;

    return 0;
}

bool add(const char **argv) {
    std::fstream task_file(TASK_FILE, std::ios::binary | std::ios::app);

    if (add_task(task_file, *create_shr_task_ptr(argv))) {
        std::cout << "Adding task success!" << std::endl;
        return true;
    }

    std::cerr << "Failed to add task!" << std::endl;
    return false;
}

static std::unique_ptr<Task> create_shr_task_ptr(const char *argv[]) {
    std::string task_name;
    TaskStatus status;

    if (!argv && !argv[2])
        return nullptr;

    task_name = argv[2];
    status = argv[3] ? num_to_status(std::stoi(argv[3])) : TaskStatus::ONGOING;

    return std::make_unique<Task>(task_name, status);
}
