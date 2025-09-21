#include <cstddef>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string_view>

#include "task/Task.hpp"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "No command has been executed" << std::endl;
        return 0;
    }

    std::string operation = argv[1];

    if (operation == "add") {
        std::fstream task_file(TASK_FILE, std::ios::out | std::ios::app |
                                              std::ios::binary);

        std::string task_name = argv[2];
        auto task = std::make_shared<Task>(task_name, TaskStatus::ONGOING);

        add_task(task_file, *task);

        task_file.close();
    }

    if (operation == "list") {
        std::fstream task_file(TASK_FILE, std::ios::in | std::ios::binary);

        list_task(task_file);

        task_file.close();
    }

    if (operation == "remove") {
        unsigned int task_num = std::stoi(argv[2]);

        std::fstream task_file(TASK_FILE, std::ios::in | std::ios::binary);

        remove_task(task_file, task_num);
    }

    if (operation == "modify") {
        std::fstream task_file(TASK_FILE, std::ios::in | std::ios::binary);

        const unsigned int task_num = std::stoi(argv[2]);
        const std::string task_name = argv[3];
        const TaskStatus task_status = num_to_status(std::stoi(argv[4]));

        auto task_ptr = std::make_unique<Task>(task_name, task_status);

        modify_task(task_file, task_num, *task_ptr);
    }

    return 0;
}
