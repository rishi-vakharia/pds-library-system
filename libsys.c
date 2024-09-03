#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include"libsys.h"

// Define the global variable of libsys
struct LibsysInfo repo_handle;

// Open the data file and index file in "wb" mode
// Initialize index file by store "0" to indicate there are zero entries in index file

int libsys_create( char *repo_name)
{   
    char dfname[30];            char ifname[30];
    strcpy(dfname, repo_name);  strcpy(ifname, repo_name);
    strcat(dfname, ".dat");     strcat(ifname, ".ndx");

    
    FILE* dat_fp=fopen(dfname, "wb");
    if(dat_fp==NULL)
        return LIB_FILE_ERROR;
    
    FILE* ndx_fp=fopen(ifname, "wb");
    if(ndx_fp==NULL)
        return LIB_FILE_ERROR;
    

    int entries=0;
    if(fwrite(&entries, sizeof(int), 1, ndx_fp)!=1)
        return LIB_FILE_ERROR;
    
    fclose(dat_fp);
    fclose(ndx_fp);

    return LIB_SUCCESS;
}

// Open the data file and index file in rb+ mode
// Update the fields of LIB_RepoInfo appropriately
// Read number of index entries from index file
// Load index_entries array by reading index entries from the index file
// Close only the index file
int libsys_open( char *repo_name )
{
    if(repo_handle.repo_status==LIB_REPO_OPEN)
        return LIB_REPO_ALREADY_OPEN;


    char dfname[30];            char ifname[30];
    strcpy(dfname, repo_name);  strcpy(ifname, repo_name);
    strcat(dfname, ".dat");     strcat(ifname, ".ndx");


    FILE* ndx_fp=fopen(ifname, "rb+");
    if(ndx_fp==NULL)
        return LIB_FILE_ERROR;
    
    
    strcpy(repo_handle.libsys_name, repo_name);         //initialising library
    repo_handle.libsys_data_fp=fopen(dfname, "rb+");
    if(repo_handle.libsys_data_fp==NULL)
        return LIB_FILE_ERROR;
    repo_handle.repo_status=LIB_REPO_OPEN;
    if(fread(&repo_handle.index_count, sizeof(int), 1, ndx_fp)!=1)   
        return LIB_FILE_ERROR;
    if(fread(repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, ndx_fp)!=repo_handle.index_count)
        return LIB_FILE_ERROR;


    fclose(ndx_fp);  

    return LIB_SUCCESS;
}


// put_rec_by_key
//-----check index file for key
//-----if key already present check for flag 
//-----if key is present but flag is RECORD_DELETED then just add entry at same index i.e update only offset and update flag; return status
//-----if key is present but flag is RECORD_PRSENT return failure as data is already present
//-----if key is not prsent then proceed with following steps:

// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to array using offset returned by ftell
// Write the key at the current data file location
// Write the record after writing the key
// return status

int put_book_by_isbn( int key, struct Book *rec )
{
    if(repo_handle.repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    int i;
    for(i=0; i<repo_handle.index_count; i++){
        struct LIB_NdxInfo entry=repo_handle.index_entries[i];
        if(entry.key==key){
			if(entry.flag==RECORD_DELETED){
                repo_handle.index_entries[i].flag=RECORD_PRESENT;
                fseek(repo_handle.libsys_data_fp, entry.offset, SEEK_SET);
                fseek(repo_handle.libsys_data_fp, sizeof(int), SEEK_CUR);
                if(fwrite(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp)!=1)
                    return LIB_FILE_ERROR;
                return LIB_SUCCESS;
			}
			else	
				return LIB_ADD_FAILED;                      //isbn or book is already present
        }
    }

    fseek(repo_handle.libsys_data_fp, 0, SEEK_END);     
    int location=ftell(repo_handle.libsys_data_fp);

    if(fwrite(&key, sizeof(int), 1, repo_handle.libsys_data_fp) != 1)
		return LIB_FILE_ERROR;

	if(fwrite(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp) != 1)    //add the book
		return LIB_FILE_ERROR;


    struct LIB_NdxInfo index={key, location, RECORD_PRESENT};
    repo_handle.index_entries[repo_handle.index_count++]=index;         //add index entry
    
	return LIB_SUCCESS;   
}	

// get_rec_by_key
//check repo status
// Search for index entry in index_entries array
//-----use flag : RECORD_PRSENT to read valid entries
// Seek to the file location based on offset in index entry
// Read the key at the current file location 
// Read the record after reading the key
int get_book_by_isbn( int key, struct Book *rec )
{
	if(repo_handle.repo_status!=LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;
    
    int i, location=-1;
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
    
    fseek(repo_handle.libsys_data_fp, location, SEEK_SET);  
    int isbn;

	if(fread(&isbn, sizeof(int), 1, repo_handle.libsys_data_fp)!=1)
        return LIB_FILE_ERROR;
    
    if(isbn!=key)
        return LIB_REC_NOT_FOUND;

    if(fread(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp)!=1)
        return LIB_FILE_ERROR;

    return LIB_SUCCESS;
}


//-----delete_rec_by_key
//-----check repo status
//-----Search for index entry in index_entries array
//-----if key matches and flag is RECORD_PRSENT then reset flag
//-----if key matches but flag is RECORD_DELETED  return status LIB_REC_NOT_FOUND

int delete_book_by_isbn( int key )
{
	if(repo_handle.repo_status!=LIB_REPO_OPEN)
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



// libsys_close
// Open the index file in wb mode (write mode, not append mode)
// Write number of index entries at the beginning of index file
// Unload the index array into the index file (overwrite the entire index file)
// Close the index file and data file
int libsys_close()
{
	 if(repo_handle.repo_status==LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;


    char ifname[30];
    strcpy(ifname, repo_handle.libsys_name);
    strcat(ifname, ".ndx");

    
    FILE* ndx_fp=fopen(ifname, "wb");
    if(ndx_fp==NULL)
        return LIB_FILE_ERROR;


    if(fwrite(&repo_handle.index_count, sizeof(int), 1, ndx_fp)!=1)         //deactivate the library
        return LIB_NDX_SAVE_FAILED;
    if(fwrite(repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, ndx_fp)!=repo_handle.index_count)
        return LIB_NDX_SAVE_FAILED;
    strcpy(repo_handle.libsys_name, "");
    repo_handle.index_count=0;
    repo_handle.repo_status=LIB_REPO_CLOSED;
    fclose(repo_handle.libsys_data_fp);     repo_handle.libsys_data_fp=NULL;


	fclose(ndx_fp);

    return LIB_SUCCESS;
}

