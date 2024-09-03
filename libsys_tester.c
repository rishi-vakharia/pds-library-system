#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "libsys.h"

#define TREPORT(a1,a2) printf("Status %s: %s\n\n",a1,a2); fflush(stdout);

void process_line( char *test_case );

int main(int argc, char *argv[])
{
	FILE *cfptr;
	char test_case[50];

	if( argc != 2 ){
		fprintf(stderr, "Usage: %s testcasefile\n", argv[0]);
		exit(1);
	}

	cfptr = (FILE *) fopen(argv[1], "r");
	while(fgets(test_case, sizeof(test_case)-1, cfptr)){
		// printf("line:%s",test_case);
		if( !strcmp(test_case,"\n") || !strcmp(test_case,"") )
			continue;
		process_line( test_case );
	}
	return 0;
}


void process_line( char *test_case )
{
	char book_repo_name[30], stud_repo_name[30], issue_repo_name[30];
	char command[10], param1[10], param2[10], param3[10], info[1000];

	int isbn, rollno, status, book_size, student_size, expected_status;
	char title[40], name[40];
	struct Book testBook;
	struct Student testStudent;
	book_size = sizeof(struct Book);
	student_size = sizeof(struct Student);

	strcpy(testStudent.name, "Rishi");
	strcpy(testStudent.address, "IIITB");
	testStudent.cgpa= 3.4;

	strcpy(testBook.title, "1984");
	strcpy(testBook.author, "George Orwell");
	testBook.price = 100.50;


	sscanf(test_case, "%s", command);
	// sscanf(test_case, "%s%s%s", command, param1, param2);
	printf("Test case: %s", test_case); 

	if( !strcmp(command,"CREATE") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		strcpy(book_repo_name, param1);
		strcat(book_repo_name, "_book");
		strcpy(stud_repo_name, param1);
		strcat(stud_repo_name, "_stud");
		strcpy(issue_repo_name, param1);
		strcat(issue_repo_name, "_issue");

		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;
		
		status = libsys_create( book_repo_name, stud_repo_name, issue_repo_name );
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"libsys_create returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	if( !strcmp(command,"OPEN") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		strcpy(book_repo_name, param1);
		strcat(book_repo_name, "_book");
		strcpy(stud_repo_name, param1);
		strcat(stud_repo_name, "_stud");
		strcpy(issue_repo_name, param1);
		strcat(issue_repo_name, "_issue");

		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		status = libsys_open( book_repo_name, stud_repo_name, issue_repo_name );
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"libsys_open returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	else if( !strcmp(command,"ADD_BOOK") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &isbn);
		testBook.isbn = isbn;
		status = put_book_by_isbn( isbn, &testBook );
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"put_book_by_isbn returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	else if( !strcmp(command,"ADD_STUDENT") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &rollno);
		testStudent.rollno=rollno;
		status = put_student_by_rollno(rollno, &testStudent);
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"put_student_by_rollno returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	else if( !strcmp(command,"SEARCH_BOOK_ISBN") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &isbn);
		// testBook.isbn = -1;
		status = get_book_by_isbn( isbn, &testBook );
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",isbn, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match with what was stored earlier
			if( expected_status == 0 ){
				if (testBook.isbn == isbn && 
					strcmp(testBook.title,"1984") == 0 &&
					strcmp(testBook.author,"George Orwell") == 0 )
				{
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"Book data not matching... Expected:{%d,%s,%s,%f} Got:{%d,%s,%s,%f}\n",
						isbn, "1984", "George Orwell",100.50, 
						testBook.isbn, testBook.title, testBook.author, testBook.price
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}

	else if( !strcmp(command,"SEARCH_BOOK_TITLE") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		strcpy(title, param1);
		// strcpy(testBook.title, "");
		status = get_book_by_title(title, &testBook);
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search title: %s; Got status %d",title, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match with what was stored earlier
			if( expected_status == 0 ){
				if ( strcmp(testBook.title, title) == 0 &&
					strcmp(testBook.author,"George Orwell") == 0 ){
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"Book data not matching... Expected:{%d,%s,%s,%f} Got:{%d,%s,%s,%f}\n",
						testBook.isbn, title, "George Orwell",100.50, 
						testBook.isbn, testBook.title, testBook.author, testBook.price
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}

	else if( !strcmp(command,"SEARCH_STUDENT_ROLLNO") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &rollno);
		// testStudent.rollno =-1;
		status = get_student_by_rollno(rollno, &testStudent);
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",rollno, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match with what was stored earlier
			if( expected_status == 0 ){
				if (testStudent.rollno == rollno && 
					strcmp(testStudent.name, "Rishi") == 0 &&
					strcmp(testStudent.address,"IIITB") == 0 )
				{
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"Student data not matching... Expected:{%d,%s,%s,%f} Got:{%d,%s,%s,%f}\n",
						rollno, "Rishi", "IIITB", 3.4, 
						testStudent.rollno, testStudent.name, testStudent.address, testStudent.cgpa
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}

	else if( !strcmp(command,"SEARCH_STUDENT_NAME") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		strcpy(name, param1);
		// strcpy(testStudent.name, "");
		status = get_student_by_name(name, &testStudent);
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status != expected_status ){
			sprintf(info,"search name: %s; Got status %d", name, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match with what was stored earlier
			if( expected_status == 0 ){
				if (strcmp(testStudent.name, name) == 0 &&
					strcmp(testStudent.address,"IIITB") == 0 ){
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"Student data not matching... Expected:{%d,%s,%s,%f} Got:{%d,%s,%s,%f}\n",
						testStudent.rollno, name, "IIITB", 3.4, 
						testStudent.rollno, testStudent.name, testStudent.address, testStudent.cgpa
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}

	else if( !strcmp(command,"DELETE_BOOK") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &isbn);
		// testBook.isbn = isbn;
		status = delete_book_by_isbn( isbn );
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"delete_book_by_isbn returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	else if( !strcmp(command,"DELETE_STUDENT") ){
		sscanf(test_case, "%s%s%s", command, param1, param2);
		if( !strcmp(param2,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &rollno);
		// testStudent.rollno = rollno;
		status = delete_student_by_rollno(rollno);
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"delete_student_by_rollno returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	else if( !strcmp(command,"ISSUE") ){
		sscanf(test_case, "%s%s%s%s", command, param1, param2, param3);
		if( !strcmp(param3,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		sscanf(param1, "%d", &isbn);
		sscanf(param2, "%d", &rollno);
		status = issue(rollno, isbn);
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"issue returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

	else if( !strcmp(command,"CLOSE") ){
		sscanf(test_case, "%s%s", command, param1);
		if( !strcmp(param1,"0") )
			expected_status = BOOK_SUCCESS;
		else
			expected_status = BOOK_FAILURE;

		status = libsys_close();
		if(status == LIB_SUCCESS)
			status = BOOK_SUCCESS;
		else
			status = BOOK_FAILURE;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"libsys_close returned status %d",status);
			TREPORT("FAIL", info);
		}
	}

}


