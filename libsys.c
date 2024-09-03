#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include"libsys.h"

// Define the global variable of libsys
struct LibsysInfo repo_handle;


//--------------------------------create()---------------------------------
int libsys_create( char *book_name,char *stud_name,char *issue_name){
	//assuming filename are passed without extension(.dat)
	
	char dfname[30];            char ifname[30];			
    strcpy(dfname, book_name);  strcpy(ifname, book_name);
    strcat(dfname, ".dat");     strcat(ifname, ".ndx");

	char sfname[30];
	strcpy(sfname, stud_name);
	strcat(sfname, ".dat");

	char issfname[30];
	strcpy(issfname, issue_name);
	strcat(issfname, ".dat");

    
    FILE* dat_fp=fopen(dfname, "wb");
    if(dat_fp==NULL)
        return LIB_FILE_ERROR;
    
    FILE* ndx_fp=fopen(ifname, "wb");
    if(ndx_fp==NULL)	
		return LIB_FILE_ERROR;

	int entries=0;
    if(fwrite(&entries, sizeof(int), 1, ndx_fp)!=1)		
		return LIB_FILE_ERROR;


	FILE* stud_fp=fopen(sfname, "wb");
    if(stud_fp==NULL)
        return LIB_FILE_ERROR;

	FILE* iss_fp=fopen(issfname, "wb");
    if(iss_fp==NULL)
        return LIB_FILE_ERROR;
    


    fclose(dat_fp);
    fclose(ndx_fp);
	fclose(stud_fp);
	fclose(iss_fp);

    return LIB_SUCCESS;
}



//--------------------------------open()---------------------------------
int libsys_open(char *book_name,char *stud_name,char *issue_name){
	
	int s1= booksys_open(book_name);
	if(s1!=LIB_SUCCESS)
		return s1;
	
	int s2= studsys_open(stud_name);
	if(s2!=LIB_SUCCESS)
		return s2;
	
	int s3= issuesys_open(issue_name);
	if(s3!=LIB_SUCCESS)
		return s3;
	
	return LIB_SUCCESS;
}

int booksys_open( char *repo_name ){

	if(repo_handle.book_repo_status==LIB_REPO_OPEN)
        return LIB_REPO_ALREADY_OPEN;

    char dfname[30];            char ifname[30];
    strcpy(dfname, repo_name);  strcpy(ifname, repo_name);
    strcat(dfname, ".dat");     strcat(ifname, ".ndx");


	//initialising book system of library
	strcpy(repo_handle.booksys_name, repo_name);			
	repo_handle.booksys_data_fp=fopen(dfname, "rb+");
    if(repo_handle.booksys_data_fp==NULL)
        return LIB_FILE_ERROR;
	repo_handle.book_repo_status=LIB_REPO_OPEN;

	repo_handle.libsys_ndx_fp=fopen(ifname, "rb+");
    if(repo_handle.libsys_ndx_fp==NULL)
        return LIB_FILE_ERROR;
	if(fread(&repo_handle.index_count, sizeof(int), 1, repo_handle.libsys_ndx_fp)!=1)   
        return LIB_FILE_ERROR;
    if(fread(repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, repo_handle.libsys_ndx_fp)!=repo_handle.index_count)
        return LIB_FILE_ERROR;
    fclose(repo_handle.libsys_ndx_fp);  
    return LIB_SUCCESS;
    
}


int issuesys_open( char *repo_name ){
	if(repo_handle.issue_repo_status==LIB_REPO_OPEN)
        return LIB_REPO_ALREADY_OPEN;

    char issfname[30];           
    strcpy(issfname, repo_name);  
    strcat(issfname, ".dat");     


	//initialising issue system of library
	strcpy(repo_handle.issuesys_name, repo_name);			
	repo_handle.issuesys_data_fp=fopen(issfname, "rb+");
    if(repo_handle.issuesys_data_fp==NULL)
        return LIB_FILE_ERROR;
	repo_handle.issue_repo_status=LIB_REPO_OPEN;

    return LIB_SUCCESS;
}


int studsys_open( char *repo_name ){

	if(repo_handle.stud_repo_status==LIB_REPO_OPEN)
        return LIB_REPO_ALREADY_OPEN;

    char sfname[30];           
    strcpy(sfname, repo_name);  
    strcat(sfname, ".dat");     


	//initialising student system of library
	strcpy(repo_handle.studsys_name, repo_name);			
	repo_handle.studsys_data_fp=fopen(sfname, "rb+");
    if(repo_handle.studsys_data_fp==NULL)
        return LIB_FILE_ERROR;
	repo_handle.stud_repo_status=LIB_REPO_OPEN;

    return LIB_SUCCESS;
}




//------------------------------book:NO change ----------------------------

int get_book_by_isbn( int key, struct Book *rec ){

	if(repo_handle.book_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;
    
    int i, location = -1;
    for(i=0; i<repo_handle.index_count; i++){
        struct LIB_NdxInfo entry = repo_handle.index_entries[i];
        if(entry.key==key){
			if(entry.flag==RECORD_PRESENT){
            	location=entry.offset;
            	break;
			}
			else	
				return LIB_REC_NOT_FOUND;
        } 
    }
    if(location == -1)
        return LIB_REC_NOT_FOUND;
    
    fseek(repo_handle.booksys_data_fp, location, SEEK_SET);  
    int isbn;

	if(fread(&isbn, sizeof(int), 1, repo_handle.booksys_data_fp)!=1)
        return LIB_FILE_ERROR;
    
    if(isbn!=key)
        return LIB_REC_NOT_FOUND;

    if(fread(rec, sizeof(struct Book), 1, repo_handle.booksys_data_fp)!=1)
        return LIB_FILE_ERROR;

    return LIB_SUCCESS;
}


int put_book_by_isbn( int key, struct Book *rec ){

	if(repo_handle.book_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    int i;
    for(i=0; i<repo_handle.index_count; i++){
        struct LIB_NdxInfo entry=repo_handle.index_entries[i];
        if(entry.key==key){
			if(entry.flag==RECORD_DELETED){
                repo_handle.index_entries[i].flag=RECORD_PRESENT;
                fseek(repo_handle.booksys_data_fp, entry.offset, SEEK_SET);
                fseek(repo_handle.booksys_data_fp, sizeof(int), SEEK_CUR);
                if(fwrite(rec, sizeof(struct Book), 1, repo_handle.booksys_data_fp)!=1)
                    return LIB_FILE_ERROR;
                return LIB_SUCCESS;
			}
			else	
				return LIB_ADD_FAILED;                      						//isbn or book is already present
        }
    }

    fseek(repo_handle.booksys_data_fp, 0, SEEK_END);     
    int location=ftell(repo_handle.booksys_data_fp);

    if(fwrite(&key, sizeof(int), 1, repo_handle.booksys_data_fp) != 1)
		return LIB_FILE_ERROR;

	if(fwrite(rec, sizeof(struct Book), 1, repo_handle.booksys_data_fp) != 1)    	//add the book
		return LIB_FILE_ERROR;


    struct LIB_NdxInfo index={key, location, RECORD_PRESENT};
    repo_handle.index_entries[repo_handle.index_count++]=index;         			//add index entry

	return LIB_SUCCESS;   
}


int delete_book_by_isbn( int key ){

	if(repo_handle.book_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

	int i;
    for(i=0; i<repo_handle.index_count; i++){
        struct LIB_NdxInfo entry = repo_handle.index_entries[i];
        if(entry.key==key){
			if(entry.flag==RECORD_PRESENT){
            	repo_handle.index_entries[i].flag=RECORD_DELETED;
				return LIB_SUCCESS;
			}
			else	
				return LIB_REC_NOT_FOUND;
        } 
    }
    return LIB_REC_NOT_FOUND;
}



//-----------------------------student: additional code---------------------------
int put_student_by_rollno( int rollno_to_write, struct Student *rec ){

	if(repo_handle.stud_repo_status!=LIB_REPO_OPEN)
		return LIB_REPO_NOT_OPEN;
	
	//set the file pointer to end
	fseek(repo_handle.studsys_data_fp, 0, SEEK_END);

	//Check success of fwrite - fwrite returns the no. of elements successfully written
	if(fwrite(&rollno_to_write, sizeof(int), 1, repo_handle.studsys_data_fp) != 1)
		return LIB_ADD_FAILED;
	if(fwrite(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp) != 1)
		return LIB_ADD_FAILED;
	
	return LIB_SUCCESS;
       
}

int get_student_by_rollno( int rollno_to_read, struct Student *rec ){

	int roll;

	if(repo_handle.stud_repo_status!=LIB_REPO_OPEN)
		return LIB_REPO_NOT_OPEN;
	
	//set the file pointer to start
	fseek(repo_handle.studsys_data_fp, 0, SEEK_SET);
	fread(&roll, sizeof(int), 1, repo_handle.studsys_data_fp);

	//loop till eof is not reached - feof returns non-zero if eof is reached and 0 otherwise
	while(feof(repo_handle.studsys_data_fp)==0){
		if(roll==rollno_to_read){
			fread(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp);
			return LIB_SUCCESS;
		}
		//skip the record	
		fseek(repo_handle.studsys_data_fp, sizeof(struct Student), SEEK_CUR);
		fread(&roll, sizeof(int), 1, repo_handle.studsys_data_fp);
	}
	return LIB_REC_NOT_FOUND;
}



//--------------------------------issue()---------------------------------
int issue(int rollno, int isbn){

	if(repo_handle.book_repo_status!=LIB_REPO_OPEN)
		return LIB_REPO_NOT_OPEN;
	
	if(repo_handle.stud_repo_status!=LIB_REPO_OPEN)
		return LIB_REPO_NOT_OPEN;

	if(repo_handle.issue_repo_status!=LIB_REPO_OPEN)
		return LIB_REPO_NOT_OPEN;


	struct Student s;
	int st1= get_student_by_rollno(rollno, &s);
	if(st1!=BOOK_SUCCESS)
		return st1;

	struct Book b;
	int st2= get_book_by_isbn(isbn, &b);
	if(st2!=BOOK_SUCCESS)
		return st2;


	struct Issue issue={rollno, isbn};
	fseek(repo_handle.issuesys_data_fp, 0, SEEK_END);
	if(fwrite(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp) != 1)
		return LIB_ADD_FAILED;

	
	return LIB_SUCCESS;
}



//----------------------------------close()---------------------------------
int libsys_close(){

	int s1= booksys_close();
	if(s1!=LIB_SUCCESS)
		return s1;
	
	int s2= studsys_close();
	if(s2!=LIB_SUCCESS)
		return s2;
	
	int s3= issuesys_close();
	if(s3!=LIB_SUCCESS)
		return s3;
	
	return LIB_SUCCESS;
}

int booksys_close(){

	if(repo_handle.book_repo_status==LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;

    char ifname[30];
    strcpy(ifname, repo_handle.booksys_name);
    strcat(ifname, ".ndx");


    repo_handle.libsys_ndx_fp=fopen(ifname, "wb");
	if(repo_handle.libsys_ndx_fp==NULL)
        return LIB_FILE_ERROR;
    if(fwrite(&repo_handle.index_count, sizeof(int), 1, repo_handle.libsys_ndx_fp)!=1)         		
        return LIB_NDX_SAVE_FAILED;
    if(fwrite(repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, repo_handle.libsys_ndx_fp)!=repo_handle.index_count)
        return LIB_NDX_SAVE_FAILED;
	

	fclose(repo_handle.libsys_ndx_fp);		
	repo_handle.libsys_ndx_fp=NULL;
	repo_handle.index_count=0;


    strcpy(repo_handle.booksys_name, "");
    repo_handle.book_repo_status=LIB_REPO_CLOSED;
    fclose(repo_handle.booksys_data_fp);     
	repo_handle.booksys_data_fp=NULL;


    return LIB_SUCCESS;
}

int studsys_close(){

	if(repo_handle.stud_repo_status==LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;

	strcpy(repo_handle.studsys_name, "");
	repo_handle.stud_repo_status=LIB_REPO_CLOSED;
	fclose(repo_handle.studsys_data_fp);
	repo_handle.studsys_data_fp=NULL;

	return LIB_SUCCESS;
}

int issuesys_close(){

	if(repo_handle.issue_repo_status==LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;

	strcpy(repo_handle.issuesys_name, "");
	repo_handle.issue_repo_status=LIB_REPO_CLOSED;
	fclose(repo_handle.issuesys_data_fp);
	repo_handle.issuesys_data_fp=NULL;

	return LIB_SUCCESS;
}