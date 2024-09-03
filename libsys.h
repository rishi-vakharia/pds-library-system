#ifndef LIBSYS_H
#define LIBSYS_H

// Error codes
#define LIB_SUCCESS 0
#define LIB_FILE_ERROR 1
#define LIB_ADD_FAILED 2
#define LIB_REC_NOT_FOUND 3
#define LIB_REPO_ALREADY_OPEN 12
#define LIB_NDX_SAVE_FAILED 13
#define LIB_REPO_NOT_OPEN 14
#define BOOK_SUCCESS 0
#define BOOK_FAILURE 1

#define RECORD_DELETED 0
#define RECORD_PRESENT 1

// Repository status values
#define LIB_REPO_OPEN 10
#define LIB_REPO_CLOSED 11
#define LIB_MAX_INDEX_ENTRIES 10000

struct LIB_NdxInfo{
	int key;
	int offset;
	int flag; 
};

struct LibsysInfo{
	char booksys_name[30];
	FILE *booksys_data_fp;
	int book_repo_status; 

	char studsys_name[30];
	FILE *studsys_data_fp;
	int stud_repo_status; 

	char issuesys_name[30];
	FILE *issuesys_data_fp;
	int issue_repo_status; 

	FILE *booksys_ndx_fp;
	struct LIB_NdxInfo book_index_entries[LIB_MAX_INDEX_ENTRIES];
	int book_index_count;

	FILE *studsys_ndx_fp;
	struct LIB_NdxInfo stud_index_entries[LIB_MAX_INDEX_ENTRIES];
	int stud_index_count;

};

struct Book{
	int isbn;
	char title[40];
	char author[40];
	float price;
};

struct Student{
	int rollno;
	char name[40];
	char address[40];
	float cgpa;
};

struct Issue{
	int rollno;
	int isbn;
	
};

int hasIssued(int rollno);
int isIssuedBy(int isbn);
int libsys_create( char *book_name,char *stud_name,char *issue_name);
int libsys_open(char *book_name,char *stud_name,char *issue_name);
int booksys_open( char *repo_name );
int issuesys_open( char *repo_name );
int studsys_open( char *repo_name );
int get_book_by_isbn( int key, struct Book *rec );
int put_book_by_isbn(int key, struct Book *rec);
int delete_book_by_isbn( int key );
int put_student_by_rollno( int rollno_to_write, struct Student *rec );
int get_student_by_rollno( int rollno_to_read, struct Student *rec );
int delete_student_by_rollno( int rollno );
int issue(int rollno, int isbn);
int libsys_close();
int booksys_close();
int studsys_close();
int issuesys_close();
int get_book_by_title(char *title, struct Book *rec);
int get_student_by_name(char *name, struct Student *rec);

#endif
