# 📝 Task Management System (C++)

A lightweight **task management system written in C++** that helps you organize and track your personal tasks.  
It supports adding, removing, listing, and modifying tasks — keeping things simple and efficient.  

---

## ✨ Features
- ➕ **Add Tasks** – Create new tasks quickly.  
- ➖ **Remove Tasks** – Delete completed or unnecessary tasks.  
- 📋 **List Tasks** – View all tasks in an organized manner.  
- ✏️ **Modify Tasks** – Update tasks with new details or priorities.  

---

## ⚙️ Usage

Run the executable with one of the following commands:

### ➕ Add Task

./task_manager add "TaskName"

- **TaskName** → The name/description of the task to be added.

---

### ➖ Remove Task

./task_manager remove ID

- **ID** → The numeric ID of the task to remove.

---

### 📋 List Tasks

./task_manager list

- Displays all tasks with their **ID, name, and status**.

---

### ✏️ Modify Task

./task_manager modify ID "NewTaskName" NewStatus

- **ID** → The numeric ID of the task to modify.  
- **NewTaskName** → The updated task description.  
- **NewStatusNumber** → The new task status (e.g., `0: ONGOING`, `1: COMPLETED`, `Rest of Nums: UNDEFINED`).  

---

## 🧾 Example

./task_manager add "Finish homework"
./task_manager list
./task_manager modify 1 "Finish math homework" done
./task_manager remove 1


---

## 📌 Notes
- Task IDs are auto-assigned when you create tasks.  
- Status is customizable but typically uses values like `ONGOING`, `COMPLETED`, or `UNDEFINED`.  
- Always use quotes if the task name contains spaces.  
