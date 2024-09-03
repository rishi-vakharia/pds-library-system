# Personal Data Store Library Management System

## Overview

This is a menu-driven Library Management System application implemented in C. It uses file-based storage to manage books and students, and implements a book issuing system. The application uses indexing for efficient data retrieval and management.

## Features

1. Add new books to the library
2. Add new students to the system
3. Delete books from the library
4. Delete students from the system
5. Issue books to students
6. Search for books by ISBN
7. Search for books by title
8. Search for students by roll number
9. Search for students by name
10. Efficient data retrieval using indexing

## Data Structures

The system manages two main entities:

1. **Book**: Includes ISBN, title, author, and price
2. **Student**: Includes roll number, name, address, and CGPA

There's also an **Issue** relationship that links students to the books they've borrowed.

## File Structure

- `libsys.h`: Header file containing structure definitions and function prototypes
- `libsys.c`: Implementation of the core library functions
- `menu.c`: Implementation of the menu-driven interface
- `libsys_tester.c`: Test driver for the library functions

## How to Compile

To compile the program, use the following command:

```
gcc menu.c libsys.c -o library_management
```

## How to Run

After compiling, run the program using:

```
./library_management
```

## Usage

Upon running the program, you'll be presented with a menu of options:

1. Add book
2. Add student
3. Delete Book
4. Delete Student
5. Issue Book
6. Search Book by ISBN
7. Search Book by title
8. Search student by roll number
9. Search student by name
10. Exit

Choose the desired option by entering the corresponding number.

## Important Notes

- The system uses separate files for books, students, and issue records.
- Indexing is implemented for both books and students to enable efficient searches.
- When deleting a book, the system checks if it's currently issued to prevent inconsistencies.
- The search functions provide detailed information, including issue status for books and the details of the student who has borrowed a book (if applicable).

## Error Handling

The system includes robust error handling to manage various scenarios such as:
- File I/O errors
- Attempts to add duplicate records
- Attempts to delete non-existent records
- Attempts to issue books that are already issued or to students who already have a book

## Testing

A test driver (`libsys_tester.c`) is provided to verify the functionality of the system. It uses a set of predefined test cases to ensure all features work as expected.

## Versions

v1 - Basic implementation of a Library System - Read and Write Book records to a data file

v2 - Persistent indexing for Book records - Faster Read Book record operation

v3 - Delete Book record operation

v4 - Library System with Two entities (Student, Book) - Issue Book functionality

v5 - Menu Driven application for all the functionalities - Add, Delete, Issue, Search

## Future Enhancements

Potential areas for future development include:
- Implementing a graphical user interface
- Adding support for multiple book issues per student
- Implementing a due date and fine system for overdue books
- Adding more advanced search capabilities (e.g., search by author, partial title matches)
