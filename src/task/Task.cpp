#include <cstdio>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>

#include "shared/file.hpp"
#include "task/Task.hpp"

static bool write_task(std::fstream &task_file, const Task &task);
static bool process_task(
    std::fstream &task_file,
    const std::function<void(int, const std::unique_ptr<Task> &)> &func);

static bool is_name_len_err(const int name_len);

bool add_task(std::fstream &task_file, const Task &task) {
    if (!is_file_good(task_file))
        return false;

    return write_task(task_file, task);
}

bool list_task(std::fstream &task_file) {
    std::cout << "List of Task (No. - Name - Status)\n";

    return process_task(
        task_file,
        [](int task_count, const std::unique_ptr<Task> &unique_task) {
            std::cout << std::format("{} - {} - {}\n", task_count,
                                     unique_task->getName(),
                                     status_to_str(unique_task->getStatus()));
        });
}

bool remove_task(std::fstream &task_file, const unsigned int task_num) {
    std::fstream temp_file(TEMP_FILE, std::ios::out | std::ios::binary);

    bool process_success =
        process_task(task_file, [&](unsigned int task_count,
                                    const std::unique_ptr<Task> &unique_task) {
            if (task_num == task_count)
                return;

            if (!write_task(temp_file, *unique_task))
                std::cout << "Failed to rewrite tasks\n";
        });

    temp_file.close();
    task_file.close();

    return process_success && is_file_deleted(TASK_FILE) &&
           is_file_rename(TEMP_FILE, TASK_FILE);
}

bool modify_task(std::fstream &task_file, const unsigned int task_num,
                 const Task &newTask) {
    std::fstream temp_file(TEMP_FILE, std::ios::out | std::ios::binary);

    bool process_success =
        process_task(task_file, [&](unsigned int task_count,
                                    const std::unique_ptr<Task> &task) {
            if (!write_task(temp_file,
                            (task_count == task_num ? newTask : *task)))
                std::cout << "Failed to rewrite the tasks\n";
        });

    return process_success && is_file_deleted(TASK_FILE) &&
           is_file_rename(TEMP_FILE, TASK_FILE);
}

static bool write_task(std::fstream &task_file, const Task &task) {
    unsigned int task_name_len = task.getName().size();
    TaskStatus status = task.getStatus();

    return task_file.write(reinterpret_cast<char *>(&task_name_len),
                           sizeof(task_name_len)) &&
           task_file.write(task.getName().data(), task_name_len) &&
           task_file.write(reinterpret_cast<char *>(&status), sizeof(status));
}

static bool process_task(
    std::fstream &task_file,
    const std::function<void(int, const std::unique_ptr<Task> &)> &func) {
    if (!is_file_good(task_file))
        return false;

    unsigned int task_name_len;
    int task_count = 0;

    std::string buffer;
    TaskStatus status;

    while (task_file.read(reinterpret_cast<char *>(&task_name_len),
                          sizeof(task_name_len))) {
        if (is_name_len_err(task_name_len))
            break;

        if (buffer.capacity() < task_name_len)
            buffer.reserve(task_name_len);

        buffer.resize(task_name_len);
        if (!task_file.read(buffer.data(), task_name_len) ||
            !task_file.read(reinterpret_cast<char *>(&status),
                            sizeof(status))) {
            std::cout << "End of file\n";
            break;
        }

        task_count++;

        func(task_count, std::make_unique<Task>(std::move(buffer), status));
    }

    return true;
}

static bool is_name_len_err(const int name_len) {
    if (name_len < 0 || name_len > 10000) {
        std::cerr << "Currupted Length\n";
        return true;
    }

    return false;
}

constexpr const char *status_to_str(TaskStatus status) {
    switch (status) {
    case TaskStatus::ONGOING:
        return "Ongoing";
    case TaskStatus::COMPLETED:
        return "Complete";
    }

    return "Unknown";
}

TaskStatus num_to_status(const unsigned int num) {
    switch (num) {
    case 0:
        return TaskStatus::ONGOING;
    case 1:
        return TaskStatus::COMPLETED;
    }

    throw "Invalid number status";
}
