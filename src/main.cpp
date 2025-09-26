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
static std::unique_ptr<Task> create_unique_task_ptr(const char **argv);

static constexpr std::size_t NUMBER_OF_FUNCS = 4;
using Func = bool (*)(const char *argv[]);

struct Entry {
    std::string_view key;
    Func func;
};

static constexpr std::array<Entry, NUMBER_OF_FUNCS> func_arr = {{"add", add}};
static constexpr Func find_func(const std::string_view &operation);
static void display_usage_cmd(const std::string_view &prg_exe_name);

int main(int argc, const char **argv) {
    if (argc <= 1) {
        display_usage_cmd(argv[0]);
        return 1;
    }

    std::string_view operation = argv[1];
    if (auto func = find_func(operation)) {
        return func(argv) ? 0 : 1;
    }

    std::cerr << "Unknown command : " << operation << std::endl;
    display_usage_cmd(argv[0]);

    return 1;
}

static constexpr Func find_func(const std::string_view &operation) {
    for (const Entry &entry : func_arr) {
        if (entry.key == operation)
            return entry.func;
    }

    return nullptr;
}

static void display_usage_cmd(const std::string_view &prg_exe_name) {
    std::cerr << "Usage: " << prg_exe_name << " <command>\n"
              << "Commands: add, list, remove, modify" << std::endl;
}

static bool add(const char **argv) {
    std::fstream task_file(TASK_FILE, std::ios::binary | std::ios::app);

    std::unique_ptr<Task> new_unique_task = create_unique_task_ptr(argv);

    if (!new_unique_task) {
        std::cerr << "Task creation failed!\n"
                  << "Usage: add *TaskName" << std::endl;

        return false;
    }

    if (add_task(task_file, *new_unique_task)) {
        std::cout << "Adding task success!" << std::endl;
        return true;
    }

    std::cerr << "Failed to add task!" << std::endl;
    return false;
}

static std::unique_ptr<Task> create_unique_task_ptr(const char **argv) {
    if (!argv || !argv[2])
        return nullptr;

    std::string task_name = argv[2];
    TaskStatus status =
        argv[3] ? num_to_status(std::stoi(argv[3])) : TaskStatus::ONGOING;

    return std::make_unique<Task>(task_name, status);
}
