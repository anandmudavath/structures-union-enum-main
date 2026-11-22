# C Programming: Structures, Unions & Enumerations

A collection of C programming projects demonstrating **Structures, Unions, Enumerations, Dynamic Memory Allocation, Pointers, and Memory Management** through practical programs.

## 📌 About the Project

This repository contains five C programming assignments developed as part of the Fundamentals of Computer Programming (FCP) coursework.

The programs demonstrate how structures and unions organize data, how enumerations represent named constants, and how dynamic memory allocation helps build flexible and efficient applications.

Each program focuses on practical implementation, modular programming, input validation, and memory safety.

## 📂 Project Structure

| File | Program | Main Concepts |
|---|---|---|
| `4.c` | Student Records Management System | Structures, malloc, realloc, CRUD |
| `5.c` | Rectangle Inventory & Geometry Analysis | Structures, Dynamic Memory, Calculations |
| `6.c` | Structure vs Union Memory Analysis | Structures, Unions, Memory Layout |
| `7.c` | Interactive Command-Driven Task Manager | Enums, Structures, String Handling |
| `10.c` | Safe Heterogeneous Data Container | Unions, Enums, Pointers, Memory Management |

## 🚀 Programs Overview

### 1. Student Records Management System (`4.c`)

A dynamic student management application that stores and manages student information.

**Features:**
- Add new student records.
- Display all student details.
- Search students using roll numbers.
- Update existing records.
- Delete student records.
- Calculate highest, lowest, and average GPA.
- Sort students based on GPA.
- Dynamically allocate and resize memory.

**Concepts:** Structures, Pointers, `malloc()`, `realloc()`, `free()`, Arrays and Functions.

### 2. Rectangle Inventory & Geometry Analysis (`5.c`)

A geometry-based application that manages rectangle records and performs mathematical calculations.

**Features:**
- Store rectangle dimensions.
- Calculate area and perimeter.
- Add, search, update, and delete rectangles.
- Identify the largest and smallest rectangles.
- Calculate total and average area.
- Identify squares and non-squares.
- Sort rectangles by area.

**Concepts:** Structures, Dynamic Memory Allocation, Functions, Mathematical Operations and Input Validation.

### 3. Structure vs Union Memory Analysis (`6.c`)

A demonstration of how structures and unions store data differently in computer memory.

**Features:**
- Compare structure and union sizes.
- Display memory addresses of members.
- Understand memory sharing in unions.
- Explore memory alignment and padding.
- Demonstrate safe union access using enumerations.

**Concepts:** Structures, Unions, `sizeof()`, Memory Layout, `offsetof()` and Enumerations.

### 4. Interactive Command-Driven Task Manager (`7.c`)

A command-based task management application that uses enumerations to interpret user commands.

**Available Commands:**

- `ADD` – Create a new task.
- `DELETE` – Remove an existing task.
- `VIEW` – Display all tasks.
- `UPDATE` – Modify task descriptions.
- `COMPLETE` – Mark a task as completed.
- `FIND` – Search for a task.
- `HELP` – Display available commands.
- `EXIT` – Terminate the application.

**Concepts:** Enumerations, Structures, Switch Statements, String Handling, Dynamic Memory Allocation and Command Processing.

### 5. Safe Heterogeneous Data Container (`10.c`)

A type-safe data container capable of storing different data types using unions and enumerations.

**Features:**
- Store integers, floats, characters, and strings.
- Retrieve values using type-safe getter functions.
- Change stored data types safely.
- Dynamically allocate memory for strings.
- Copy containers using deep copying.
- Validate data types before accessing union members.
- Release dynamically allocated memory.

**Concepts:** Unions, Enumerations, Structures, Pointers, `malloc()`, `free()`, Deep Copying and Memory Management.

## 🛠️ Technologies Used

- **Programming Language:** C
- **Compiler:** GCC
- **Code Editor:** Visual Studio Code
- **Version Control:** Git & GitHub

## 💻 How to Run the Programs

### Step 1: Clone the Repository

```bash
git clone https://github.com/anandmudavath/structures-union-enum-main.git
```

### Step 2: Navigate to the Project Folder

```bash
cd structures-union-enum-main/structures-union-enum-main
```

### Step 3: Compile a Program

For example, to compile the student management program:

```bash
gcc 4.c -o student -std=c11 -Wall -Wextra -pedantic -lm
```

### Step 4: Execute the Program

**Windows:**
```bash
student.exe
```

**Linux / macOS:**
```bash
./student
```

Repeat the compilation process for the other `.c` files by changing the filename and output executable name.

## 📚 Key Learning Outcomes

Through these programs, I explored:

- Understanding structures and unions in C.
- Working with enumerations and user-defined data types.
- Managing memory dynamically.
- Understanding pointers and memory allocation.
- Implementing CRUD operations.
- Building modular and reusable functions.
- Handling invalid input and edge cases.
- Understanding memory efficiency and safe union access.
- Developing practical problem-solving skills using C.

## 👨‍💻 Author

**Anand Mudavath**

B.Tech – Computer Science and Engineering  
Faculty of Technology, University of Delhi

- GitHub: [@anandmudavath](https://github.com/anandmudavath)

---

⭐ If you find this repository useful, consider giving it a star!

**Built with C | Learning by Building | Continuous Improvement**
