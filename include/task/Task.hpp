#pragma once

#include <string>

#define TASK_FILE "task.dat"

enum TaskStatus { ONGOING, COMPLETED };

class Task final {
  private:
    std::string name;
    TaskStatus status = TaskStatus::ONGOING;

  public:
    Task() = default;
    Task(std::string name, TaskStatus status = TaskStatus::ONGOING)
        : name(std::move(name)), status(status) {}

    void setName(std::string newName) { this->name = std::move(newName); }
    const std::string &getName() const { return name; }

    void setStatus(TaskStatus newStatus) { this->status = newStatus; }
    TaskStatus getStatus() const { return status; }
};

bool add_task(std::fstream &task_file, const Task &task);
bool list_task(std::fstream &task_file);
bool remove_task(std::fstream &task_file, const unsigned int task_num);
bool modify_task(std::fstream &task_file, const unsigned int task_num,
                 const Task &newTask);

constexpr const char *status_to_str(TaskStatus status);
TaskStatus num_to_status(const unsigned int num);
