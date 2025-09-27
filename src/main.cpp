#include <cstddef>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include "shared/file.hpp"
#include "task/Task.hpp"

static bool add(const char **argv);
static bool remove(const char **argv);
static bool list(const char **argv);
static bool modify(const char **argv);

static std::unique_ptr<Task> create_unique_task_ptr(const char **argv);
static TaskStatus create_task_status(const char **argv);

static constexpr std::size_t NUMBER_OF_FUNCS = 4;
using Func = bool (*)(const char *argv[]);

struct Entry {
    std::string_view key;
    Func func;
};

static constexpr std::array<Entry, NUMBER_OF_FUNCS> func_arr = {
    {{"add", add}, {"list", list}}};

static constexpr Func find_func(const std::string_view &operation);

int main(int argc, const char **argv) {
    auto handle_usage_cmd = [](const std::string_view &prg_exe_name) {
        std::cerr << "Usage: " << prg_exe_name << " <command>\n"
                  << "Commands: add, list, remove, modify" << std::endl;
    };

    if (argc <= 1) {
        handle_usage_cmd(argv[0]);
        return 1;
    }

    std::string_view operation = argv[1];
    if (auto func = find_func(operation)) {
        return func(argv) ? 0 : 1;
    }

    std::cerr << "Unknown command : " << operation << std::endl;
    handle_usage_cmd(argv[0]);

    return 1;
}

static constexpr Func find_func(const std::string_view &operation) {
    for (const Entry &entry : func_arr) {
        if (entry.key == operation)
            return entry.func;
    }

    return nullptr;
}

static bool add(const char **argv) {
    std::fstream task_file(TASK_FILE, std::ios::binary | std::ios::app);

    std::unique_ptr<Task> new_unique_task = create_unique_task_ptr(argv);

    if (!new_unique_task) {
        std::cerr << "Task creation failed!\n"
                  << "Usage: add *TaskName" << std::endl;

        return false;
    }

    if (!add_task(task_file, *new_unique_task)) {
        std::cerr << "Failed to add task!" << std::endl;
        return false;
    }

    std::cout << "Adding task success!" << std::endl;
    return true;
}

static bool list(const char **) {
    std::fstream task_file(TASK_FILE, std::ios::in | std::ios::binary);

    return list_task(task_file);
}

static bool remove(const char **argv) {}

static std::unique_ptr<Task> create_unique_task_ptr(const char **argv) {
    if (!argv || !argv[2])
        return nullptr;

    std::string task_name = argv[2];
    TaskStatus status = create_task_status(argv);

    return std::make_unique<Task>(task_name, status);
}

TaskStatus create_task_status(const char **argv) {
    auto handle_status_error = [](const char *msg) {
        std::cerr << msg << ", defaulting to Undefined\n";
    };

    TaskStatus status = TaskStatus::ONGOING;

    if (argv[3]) {
        try {
            status = num_to_status(std::stoi(argv[3]));
        } catch (const std::invalid_argument &) {
            handle_status_error("Invalid status! (must a number)");
            return TaskStatus::UNDEFINED;
        } catch (const std::out_of_range &) {
            handle_status_error("Status out of range");
            return TaskStatus::UNDEFINED;
        }
    }

    return status;
}
