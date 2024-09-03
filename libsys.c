#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include"libsys.h"

// Define the global variable of libsys
struct LibsysInfo repo_handle;


//Return isbn if issued and -1 otherwise
int hasIssued(int rollno){
	struct Issue issue;
	if(repo_handle.issue_repo_status==LIB_REPO_OPEN){
	
		fseek(repo_handle.issuesys_data_fp, 0, SEEK_SET);
		fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
		while(feof(repo_handle.issuesys_data_fp)==0){
			if(issue.rollno==rollno){
				return issue.isbn;
			}
			fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
		}
	}
	return -1;
}


//Return rollno. if issued and -1 otherwise
int isIssuedBy(int isbn){

    struct Issue issue;
	if(repo_handle.issue_repo_status==LIB_REPO_OPEN){
	
		fseek(repo_handle.issuesys_data_fp, 0, SEEK_SET);
		fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
		while(feof(repo_handle.issuesys_data_fp)==0){
			if(issue.isbn==isbn){
				return issue.rollno;
			}
			fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
		}
	}
	return -1;
}


//--------------------------------create()---------------------------------

int libsys_create( char *book_name,char *stud_name,char *issue_name){
	//assuming filename are passed without extension(.dat)
	
	char dfname[30];            char bifname[30];			
    strcpy(dfname, book_name);  strcpy(bifname, book_name);
    strcat(dfname, ".dat");     strcat(bifname, ".ndx");

	char sfname[30];			char sifname[30];
	strcpy(sfname, stud_name);	strcpy(sifname, stud_name);
	strcat(sfname, ".dat");		strcat(sifname, ".ndx");

	char issfname[30];
	strcpy(issfname, issue_name);
	strcat(issfname, ".dat");

    
    FILE* dat_fp=fopen(dfname, "wb");
    if(dat_fp==NULL)
        return LIB_FILE_ERROR;

	FILE* stud_fp=fopen(sfname, "wb");
    if(stud_fp==NULL)
        return LIB_FILE_ERROR;

	FILE* iss_fp=fopen(issfname, "wb");
    if(iss_fp==NULL)
        return LIB_FILE_ERROR;
    

    FILE* ndx_fp=fopen(bifname, "wb");
    if(ndx_fp==NULL)	
		return LIB_FILE_ERROR;

	int entries=0;
    if(fwrite(&entries, sizeof(int), 1, ndx_fp)!=1)		
		return LIB_FILE_ERROR;


	FILE* stndx_fp=fopen(sifname, "wb");
    if(stndx_fp==NULL)	
		return LIB_FILE_ERROR;

	int stentries=0;
    if(fwrite(&stentries, sizeof(int), 1, stndx_fp)!=1)		
		return LIB_FILE_ERROR;


    fclose(dat_fp);
    fclose(ndx_fp);
	fclose(stud_fp);
	fclose(iss_fp);
	fclose(stndx_fp);

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

	repo_handle.booksys_ndx_fp=fopen(ifname, "rb+");
    if(repo_handle.booksys_ndx_fp==NULL)
        return LIB_FILE_ERROR;
	if(fread(&repo_handle.book_index_count, sizeof(int), 1, repo_handle.booksys_ndx_fp)!=1)   
        return LIB_FILE_ERROR;
    if(fread(repo_handle.book_index_entries, sizeof(struct LIB_NdxInfo), repo_handle.book_index_count, repo_handle.booksys_ndx_fp)!=repo_handle.book_index_count)
        return LIB_FILE_ERROR;
    fclose(repo_handle.booksys_ndx_fp);  
    return LIB_SUCCESS;
    
}


int studsys_open( char *repo_name ){

	if(repo_handle.stud_repo_status==LIB_REPO_OPEN)
        return LIB_REPO_ALREADY_OPEN;

    char sfname[30];            char sifname[30];
    strcpy(sfname, repo_name);  strcpy(sifname, repo_name);
    strcat(sfname, ".dat");     strcat(sifname, ".ndx");


	//initialising book system of library
	strcpy(repo_handle.studsys_name, repo_name);			
	repo_handle.studsys_data_fp=fopen(sfname, "rb+");
    if(repo_handle.studsys_data_fp==NULL)
        return LIB_FILE_ERROR;
	repo_handle.stud_repo_status=LIB_REPO_OPEN;

	repo_handle.studsys_ndx_fp=fopen(sifname, "rb+");
    if(repo_handle.studsys_ndx_fp==NULL)
        return LIB_FILE_ERROR;
	if(fread(&repo_handle.stud_index_count, sizeof(int), 1, repo_handle.studsys_ndx_fp)!=1)   
        return LIB_FILE_ERROR;
    if(fread(repo_handle.stud_index_entries, sizeof(struct LIB_NdxInfo), repo_handle.stud_index_count, repo_handle.studsys_ndx_fp)!=repo_handle.stud_index_count)
        return LIB_FILE_ERROR;
    fclose(repo_handle.studsys_ndx_fp);  
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


//------------------------------book:NO change ----------------------------

int get_book_by_isbn( int key, struct Book *rec ){

	if(repo_handle.book_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;
    
    int i, location = -1;
    for(i=0; i<repo_handle.book_index_count; i++){
        struct LIB_NdxInfo entry = repo_handle.book_index_entries[i];
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
    for(i=0; i<repo_handle.book_index_count; i++){
        struct LIB_NdxInfo entry=repo_handle.book_index_entries[i];
        if(entry.key==key){
			if(entry.flag==RECORD_DELETED){
                repo_handle.book_index_entries[i].flag=RECORD_PRESENT;
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
    repo_handle.book_index_entries[repo_handle.book_index_count++]=index;         	//add index entry

	return LIB_SUCCESS;   
}


int delete_book_by_isbn( int key ){

	if(repo_handle.book_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

	int i;
    for(i=0; i<repo_handle.book_index_count; i++){
        struct LIB_NdxInfo entry = repo_handle.book_index_entries[i];
        if(entry.key==key){
			if(entry.flag==RECORD_PRESENT && isIssuedBy(entry.key)==-1){
            	repo_handle.book_index_entries[i].flag=RECORD_DELETED;
				return LIB_SUCCESS;
			}
			else	
				return LIB_REC_NOT_FOUND;
        } 
    }
    return LIB_REC_NOT_FOUND;
}



//-----------------------------student: additional code---------------------------

int get_student_by_rollno( int rollno_to_read, struct Student *rec ){

	if(repo_handle.stud_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;
    
    int i, location = -1;
    for(i=0; i<repo_handle.stud_index_count; i++){
        struct LIB_NdxInfo entry = repo_handle.stud_index_entries[i];
        if(entry.key==rollno_to_read){
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
    
    fseek(repo_handle.studsys_data_fp, location, SEEK_SET);  
    int roll;

	if(fread(&roll, sizeof(int), 1, repo_handle.studsys_data_fp)!=1)
        return LIB_FILE_ERROR;
    
    if(roll!=rollno_to_read)
        return LIB_REC_NOT_FOUND;

    if(fread(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp)!=1)
        return LIB_FILE_ERROR;

    return LIB_SUCCESS;

}

int put_student_by_rollno( int rollno_to_write, struct Student *rec ){

	if(repo_handle.stud_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    int i;
    for(i=0; i<repo_handle.stud_index_count; i++){
        struct LIB_NdxInfo entry=repo_handle.stud_index_entries[i];
        if(entry.key==rollno_to_write){
			if(entry.flag==RECORD_DELETED){
                repo_handle.stud_index_entries[i].flag=RECORD_PRESENT;
                fseek(repo_handle.studsys_data_fp, entry.offset, SEEK_SET);
                fseek(repo_handle.studsys_data_fp, sizeof(int), SEEK_CUR);
                if(fwrite(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp)!=1)
                    return LIB_FILE_ERROR;
                return LIB_SUCCESS;
			}
			else	
				return LIB_ADD_FAILED;                      							//rollno or student is already present
        }
    }

    fseek(repo_handle.studsys_data_fp, 0, SEEK_END);     
    int location=ftell(repo_handle.studsys_data_fp);

    if(fwrite(&rollno_to_write, sizeof(int), 1, repo_handle.studsys_data_fp) != 1)
		return LIB_FILE_ERROR;

	if(fwrite(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp) != 1)    	//add the student
		return LIB_FILE_ERROR;


    struct LIB_NdxInfo index={rollno_to_write, location, RECORD_PRESENT};
    repo_handle.stud_index_entries[repo_handle.stud_index_count++]=index;         		//add index entry

	return LIB_SUCCESS;
       
}

int delete_student_by_rollno( int rollno ){

	if(repo_handle.stud_repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

	int i;
    for(i=0; i<repo_handle.stud_index_count; i++){
        struct LIB_NdxInfo entry = repo_handle.stud_index_entries[i];
        if(entry.key==rollno){
			if(entry.flag==RECORD_PRESENT){
            	repo_handle.stud_index_entries[i].flag=RECORD_DELETED;
				return LIB_SUCCESS;
			}
			else	
				return LIB_REC_NOT_FOUND;
        } 
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

	//issue failed: book or rollno. were unavailable
	if(isIssuedBy(isbn)!=-1 || hasIssued(rollno)!=-1)		
		return BOOK_FAILURE;	

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


    repo_handle.booksys_ndx_fp=fopen(ifname, "wb");
	if(repo_handle.booksys_ndx_fp==NULL)
        return LIB_FILE_ERROR;
    if(fwrite(&repo_handle.book_index_count, sizeof(int), 1, repo_handle.booksys_ndx_fp)!=1)         		
        return LIB_NDX_SAVE_FAILED;
    if(fwrite(repo_handle.book_index_entries, sizeof(struct LIB_NdxInfo), repo_handle.book_index_count, repo_handle.booksys_ndx_fp)!=repo_handle.book_index_count)
        return LIB_NDX_SAVE_FAILED;
	

	fclose(repo_handle.booksys_ndx_fp);		
	repo_handle.booksys_ndx_fp=NULL;
	repo_handle.book_index_count=0;


    strcpy(repo_handle.booksys_name, "");
    repo_handle.book_repo_status=LIB_REPO_CLOSED;
    fclose(repo_handle.booksys_data_fp);     
	repo_handle.booksys_data_fp=NULL;


    return LIB_SUCCESS;
}

int studsys_close(){

	if(repo_handle.stud_repo_status==LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;

    char sifname[30];
    strcpy(sifname, repo_handle.studsys_name);
    strcat(sifname, ".ndx");


    repo_handle.studsys_ndx_fp=fopen(sifname, "wb");
	if(repo_handle.studsys_ndx_fp==NULL)
        return LIB_FILE_ERROR;
    if(fwrite(&repo_handle.stud_index_count, sizeof(int), 1, repo_handle.studsys_ndx_fp)!=1)         		
        return LIB_NDX_SAVE_FAILED;
    if(fwrite(repo_handle.stud_index_entries, sizeof(struct LIB_NdxInfo), repo_handle.stud_index_count, repo_handle.studsys_ndx_fp)!=repo_handle.stud_index_count)
        return LIB_NDX_SAVE_FAILED;
	

	fclose(repo_handle.studsys_ndx_fp);		
	repo_handle.studsys_ndx_fp=NULL;
	repo_handle.stud_index_count=0;


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

//---------------------------------------------------Additional Functions -----------------------

int get_book_by_title(char *title, struct Book *rec){

	int isbn;
    struct Book book;

	if(repo_handle.book_repo_status!=LIB_REPO_OPEN)
		return LIB_REPO_NOT_OPEN;
	
	fseek(repo_handle.booksys_data_fp, 0, SEEK_SET);
	fread(&isbn, sizeof(int), 1, repo_handle.booksys_data_fp);
    fread(&book, sizeof(struct Book), 1, repo_handle.booksys_data_fp);

	while(feof(repo_handle.booksys_data_fp)==0){
		if(strcmp(book.title, title)==0){
			return get_book_by_isbn(book.isbn, rec);
		}
		fread(&isbn, sizeof(int), 1, repo_handle.booksys_data_fp);
        fread(&book, sizeof(struct Book), 1, repo_handle.booksys_data_fp);
	}
	return LIB_REC_NOT_FOUND;
}

int get_student_by_name(char *name, struct Student *rec){

    int roll;
    struct Student stud;

	if(repo_handle.stud_repo_status!=LIB_REPO_OPEN)
		return LIB_REPO_NOT_OPEN;
	
	fseek(repo_handle.studsys_data_fp, 0, SEEK_SET);
	fread(&roll, sizeof(int), 1, repo_handle.studsys_data_fp);
    fread(&stud, sizeof(struct Student), 1, repo_handle.studsys_data_fp);

	while(feof(repo_handle.studsys_data_fp)==0){
		if(strcmp(stud.name, name)==0){
            return get_student_by_rollno(stud.rollno, rec);
		}
		fread(&roll, sizeof(int), 1, repo_handle.studsys_data_fp);
        fread(&stud, sizeof(struct Student), 1, repo_handle.studsys_data_fp);
	}
	return LIB_REC_NOT_FOUND;
}
