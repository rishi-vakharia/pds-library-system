#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include"libsys.h"

// Define the global variable of libsys
//Info. of data file(repo) is encapsulated in this struct
struct LibsysInfo repo_handle;		

//Function to create the file where data will be stored
int libsys_create( char *repo_name)
{
	FILE* fptr=fopen(repo_name,"wb");
	if(fptr==NULL)
		return LIB_FILE_ERROR;
	fclose(fptr);
	return LIB_SUCCESS;

	//1. create file pointer to open a file in 'wb' mode
	//2. handle file pointer error if value is NULL return appropriate error code referring to the error codes in libsys.h
	//3. close file pointer
	//4. return libsys_success 
}

//Function to open the data file(repo) and set values of repo_handle struct
int libsys_open( char *repo_name )
{
	repo_handle.libsys_data_fp=fopen(repo_name, "rb+");
	if(repo_handle.libsys_data_fp==NULL)
		return LIB_FILE_ERROR;

	strcpy(repo_handle.libsys_name, repo_name);
	repo_handle.repo_status=LIB_REPO_OPEN;

	return LIB_SUCCESS;

	//1. assign repo handle a file pointer by opening file in 'rb+' mode
	//2. handle file pointer error if value is NULL return appropriate error code referring to the error codes in libsys.h
	//3. assign values (repo_name) to libsys_repo_handle
	//4. assign value to repo_handle.repo_status=LIB_REPO_OPEN;
	//5. return libsys_success 

}

//Function to insert a book with a given isbn into the library
int put_book_by_isbn( int isbn_to_write, struct Book *rec )
{
	if(repo_handle.repo_status==LIB_REPO_CLOSED)
		return LIB_FILE_ERROR;
	
	//set the file pointer to end
	fseek(repo_handle.libsys_data_fp, 0, SEEK_END);

	//Check success of fwrite - fwrite returns the no. of elements successfully written
	if(fwrite(&isbn_to_write, sizeof(int), 1, repo_handle.libsys_data_fp) != 1)
		return LIB_ADD_FAILED;
	if(fwrite(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp) != 1)
		return LIB_ADD_FAILED;
	
	return LIB_SUCCESS;

	//1. check if repo status is closed then return appropriate error code referring to the error codes in libsys.h
	//2. else continue with following action sequence
	//3. set the file pointer to end
	//4. write isbn_to_write 
	//5. write book record
	//6. if both actions are successful then return libsys_success 
	//7. else return libsys_add_failed
    
}

//Retrieve a book with given isbn from the library
int get_book_by_isbn( int isbn_to_read, struct Book *rec )
{
	int isbn;

	if(repo_handle.repo_status==LIB_REPO_CLOSED)
		return LIB_FILE_ERROR;
	
	//set the file pointer to start
	fseek(repo_handle.libsys_data_fp, 0, SEEK_SET);
	
	fread(&isbn, sizeof(int), 1, repo_handle.libsys_data_fp);

	//loop till eof is not reached - feof returns non-zero if eof is reached and 0 otherwise
	while(feof(repo_handle.libsys_data_fp)==0){
		if(isbn==isbn_to_read){
			fread(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp);
			return LIB_SUCCESS;
		}
		//skip the record	
		fseek(repo_handle.libsys_data_fp, sizeof(struct Book), SEEK_CUR);
		fread(&isbn, sizeof(int), 1, repo_handle.libsys_data_fp);
	}
	return LIB_REC_NOT_FOUND;
	
	//1. check if repo status is closed then return appropriate error code referring to the error codes in libsys.h
	//2. else continue with following action sequence
	//3. set the file pointer to start
	//4.1 read isbn
	//4.2 check if isbn is equal to the isbn_to_read
	//4.3 if yes then read entire record of a book and return libsys_success
	//4.4 else skip the record and read next isbn of the book
	//5. Repeat step 4.1 to 4.4 till end of file
	//6. Return record not found : appropriate error code referring to the error codes in libsys.h

}

int libsys_close()
{
	if(repo_handle.repo_status==LIB_REPO_CLOSED)
		return LIB_FILE_ERROR;

	fclose(repo_handle.libsys_data_fp);
	strcpy(repo_handle.libsys_name, "");
	repo_handle.repo_status=LIB_REPO_CLOSED;

	return LIB_SUCCESS;

	//1. check if repo status is closed then  return appropriate error code referring to the error codes in libsys.h
	//2. else continue with following action sequence
	//3. close file pointer
	//4. set libsys_name as "" 
	//5. set repo_status=LIB_REPO_CLOSED
    //6. return LIB_SUCCESS;
}

