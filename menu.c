#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include"libsys.h"

//Your code must support menu-driven functionality.
//Function calls rollno_libsys.h

void printMenu(){

printf("\n1.Add book\n\
2.Add student\n\
3.Delete Book\n\
4.Delete Student\n\
5.Issue Book \n\
6.Search Book by isbn\n\
7.Search Book by title\n\
8.Search student by rollno\n\
9.Search student by name\n\
10.Exit\n\n");

}

int main(){

    char *book_repo_name = "book";
    char *stud_repo_name = "stud";	
	char *issue_repo_name= "issue";

    struct Book book;
    struct Student student;

    int rollno, isbn, status;

    char title[40], name[40];


    libsys_create(book_repo_name, stud_repo_name, issue_repo_name);

    printMenu();

    int choice;
    scanf("%d", &choice);

    while(choice!=10){

        switch(choice){
        case 1:
            printf("\nEnter isbn: ");
            scanf("%d", &book.isbn);
            printf("\nEnter title: ");
            scanf("%s", book.title);
            printf("\nEnter author: ");
            scanf("%s", book.author);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            put_book_by_isbn(book.isbn, &book);
            libsys_close();
            break;
        case 2:
            printf("\nEnter rollno: ");
            scanf("%d", &student.rollno);
            printf("\nEnter name: ");
            scanf("%s", student.name);
            printf("\nEnter address: ");
            scanf("%s", student.address);
            
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            put_student_by_rollno(student.rollno, &student);
            libsys_close();
            break;
        case 3:
            printf("\nEnter isbn: ");
            scanf("%d", &isbn);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            delete_book_by_isbn(isbn);
            libsys_close();
            break;
        case 4:
            printf("\nEnter rollno: ");
            scanf("%d", &rollno);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            delete_student_by_rollno(rollno);
            libsys_close();
            break;
        case 5:
            printf("\nEnter isbn: ");
            scanf("%d", &isbn);
            printf("\nEnter rollno: ");
            scanf("%d", &rollno);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            issue(rollno, isbn);
            libsys_close();
            break;
        case 6:
            printf("\nEnter isbn: ");
            scanf("%d", &isbn);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_book_by_isbn(isbn, &book);
            if(status == BOOK_SUCCESS){
                printf("\nBook details:\nisbn = %d\ntitle = %s\nauthor = %s\n", book.isbn, book.title, book.author);
                rollno = isIssuedBy(book.isbn);
                if(rollno==-1)
                    printf("\nIssue status: not issued\n");
                else{
                    printf("\nIssue status: issued\n");
                    get_student_by_rollno(rollno, &student);
                    printf("\nStudent details:\nrollno = %d\nname = %s\naddress = %s\n", student.rollno, student.name, student.address);
                }
            }
            else
                printf("\nBook not found\n");
            libsys_close();
            break;
        case 7:
            printf("\nEnter title: ");
            scanf("%s", title);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_book_by_title(title, &book);
            if(status == BOOK_SUCCESS){
                printf("\nBook details:\nisbn = %d\ntitle = %s\nauthor = %s\n", book.isbn, book.title, book.author);
                rollno = isIssuedBy(book.isbn);
                if(rollno==-1)
                    printf("\nIssue status: not issued\n");
                else{
                    printf("\nIssue status: issued\n");
                    get_student_by_rollno(rollno, &student);
                    printf("\nStudent details:\nrollno = %d\nname = %s\naddress = %s\n", student.rollno, student.name, student.address);
                }
            }
            else
                printf("\nBook not found\n");
            libsys_close();
            break;
        case 8:
            printf("\nEnter rollno: ");
            scanf("%d", &rollno);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_student_by_rollno(rollno, &student);
            if(status == BOOK_SUCCESS) 
                printf("\nStudent details:\nrollno = %d\nname = %s\naddress = %s\n", student.rollno, student.name, student.address);
            else
                printf("\nStudent not found\n");
            libsys_close();
            break;
        case 9:
            printf("\nEnter name: ");
            scanf("%s", name);

            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_student_by_name(name, &student);
            if(status == BOOK_SUCCESS)
                printf("\nStudent details:\nrollno = %d\nname = %s\naddress = %s\n", student.rollno, student.name, student.address);
            else
                printf("\nStudent not found\n");
            libsys_close();
            break;
        }
            
        printMenu();
        scanf("%d", &choice);      
    }
}
