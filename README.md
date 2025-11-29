LIBRARY_MANAGMENT

This is a console-based **Library Management System** project written in **C**. It uses file handling to store book and issue records persistently.

## Project Overview

The program provides a simple menu-driven interface for basic library operations. It manages two main types of records: **books** and **issued books** (associated with a student).

### Key Features
* **Add Book (Option 1):** Allows a user to add a new book record (ID, name, author) to the library's inventory.
* **Books List (Option 2):** Displays a list of all books currently stored in the system.
* **Remove Book (Option 3):** Deletes a book record based on its ID.
* **Issue Book (Option 4):** Records the issuance of a book to a student, storing both book and student details.
* **Issued Book List (Option 5):** Displays a list of all currently issued books and the associated student information.

---

##  Technical Details

The project utilizes several standard C libraries and key programming concepts:

### 1. Data Structures (`struct`)
Two structures are defined to hold the information for books and students:

* **`struct Book`**:
    * `id` (int): Unique identifier for the book.
    * `bookName` (char[50]): Name of the book.
    * `authorName` (char[50]): Name of the author.
    * `date` (char[12]): Date the book record was added.
* **`struct Student`**:
    * `id` (int): The ID of the issued book (inherited from `struct Book`).
    * `sName` (char[50]): Student's name.
    * `sClass` (char[50]): Student's class.
    * `sRoll` (int): Student's roll number.
    * `bookName` (char[50]): Name of the issued book.
    * `date` (char[12]): Date the book was issued.

### 2. File Handling
The data is stored in binary files using file pointers (`FILE *fp`).

* **`books.txt`**: Stores the records of available books (`struct Book`).
    * Opened in **append binary (`"ab"`)** mode for adding books.
    * Opened in **read binary (`"rb"`)** mode for listing, deleting, and issuing.
* **`issue.txt`**: Stores the records of issued books (`struct Student`).
    * Opened in **append binary (`"ab"`)** mode for issuing books.
    * Opened in **read binary (`"rb"`)** mode for listing issued books.

The `fwrite()` and `fread()` functions are used to write and read entire structure objects to/from the files.

### 3. Record Deletion (`del` function)
The deletion process uses a temporary file approach:

1.  A temporary file (`temp.txt`) is created.
2.  The program reads records from the original file (`books.txt`) one by one.
3.  If a book's ID **does not match** the ID to be deleted, the record is written to `temp.txt`.
4.  The original file is closed.
5.  The original file is **removed** (`remove("books.txt")`).
6.  The temporary file is **renamed** to the original file name (`rename("temp.txt", "books.txt")`).
This effectively overwrites the book list, excluding the deleted record.

### 4. Input Handling
* The `scanf()` function is used for reading integers (like book ID and menu choice).
* The `fgets()` function is used for reading strings (like names) because it can handle spaces in the input.
* The `clearInputBuffer()` function is used to **consume the newline character** left behind by `scanf()` before subsequent calls to `fgets()`, preventing the string input functions from being skipped.
* `b.bookName[strcspn(b.bookName, "\n")] = 0;` is used to remove the trailing newline character that `fgets()` includes.

### 5. Date and Time
The project uses the `time.h` library (`time()`, `localtime()`, `struct tm`) to automatically capture the **current date** when a new book is added or a book is issued, formatting it using `sprintf` into a `DD/MM/YYYY` string.

### 6. System Commands
The `system("cls");` command is used throughout the `main` function and list functions to **clear the console screen**, providing a cleaner user experience for the menu and lists.
