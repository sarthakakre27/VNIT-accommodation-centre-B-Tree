/*
	Name - Sarthak Akre
	En no- BT19CSE099
*/
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<conio.h>
#include<stdlib.h>

//max children 6
#define MAX 6
//max values in node 5
#define MAXVAL 5
//min children 3 -- ceil(6/2)
#define MIN 3
//min values in node 2
#define MINVAL 2

typedef enum{FALSE,TRUE} Boolean;

//structure for the record of people
typedef struct accommodation
{ 
	char firstname[50]; 
	char lastname[50]; 
	char accommodation_type [20];
	int idtype; 
	char address[100]; 
	int blockind;
	struct idnum
	{ 
		char aadhaar[15]; 
		char passport[15]; 
		char empcode [15];
	}id; 	
}Record;

//structure for the B-TREE node
typedef struct Tree_Node_tag
{
    int count;
    Record records[MAXVAL];
    struct Tree_Node_tag* children[MAX];
    Boolean isleaf;

}Tree_Node;

//structure for the nodes in the Index tree
typedef struct index_record_tag
{
    char firstname[50]; 
	char lastname[50];
    int idtype;
    
    union idnumber
	{ 
		char aadhaar[15]; 
		char passport[15]; 
		char empcode [15];
	}id;
}IndexRecord;

typedef struct Index_Tree_Node_tag
{
    int count;
    IndexRecord records[MAXVAL];
    struct Index_Tree_Node_tag* children[MAX];
    Boolean isleaf;

}Index_Tree_Node;

//structure for the different types of accommodation blocks
typedef struct block_tag
{
	char accommodation_type [20];
    int serialNumber;
    int accommodated;
	
}block;

//structure for accommodation blocks Tree node
typedef struct block_node_tag
{
    int count;
    block records[MAXVAL];
    struct block_node_tag* children[MAX];
    Boolean isleaf;
}block_Node;


Tree_Node* createTreeNode();
void setRecord(Tree_Node* ptr,int ind,Index_Tree_Node** IndexrootAdr,char allocType,int blockind);
Tree_Node* Search_wrapper(Tree_Node* ptr);
Tree_Node* SearchInTree(Tree_Node* ptr,int typeid,char* numid,int* index);


void InsertRecord(Tree_Node** rootAdr,int idtype,Index_Tree_Node** IndexrootAdr,char allocType,int blockind);
void split(int ind,Tree_Node* newptr,Tree_Node* ptr);
void NodeNotFullInsert(Tree_Node* ptr,char* key,int idtype,Index_Tree_Node** IndexrootAdr,char allocType,int blockind);

void DeleteInLeaf(Tree_Node* ptr,int index);
void DeleteInNonLeaf(Tree_Node* ptr,int deleteind,int idtype,Index_Tree_Node* Indexptr,block_Node* b1);
void deleteRecord(Tree_Node* ptr,char* key,int idtype,Index_Tree_Node* Indexptr,block_Node* b1);
void adjust(Tree_Node* ptr,int deleteind);
Record Predecessor(Tree_Node* ptr,int index);
Record Successor(Tree_Node* ptr,int index);
void NodeMerge(Tree_Node* ptr,int index);
void borrowFromLeft(Tree_Node* ptr,int index);
void borrowFromRight(Tree_Node* ptr,int index);

void InsertInIndex(Index_Tree_Node** IndexrootAdr,int idtype,char* firstname,char* lastname,char* numid);
void Indexsplit(int ind,Index_Tree_Node* newptr,Index_Tree_Node* ptr);
void NodeNotFullInsertIndex(Index_Tree_Node* ptr,int idtype,char* firstname,char* lastname,char* numid);

void DeleteInIndexLeaf(Index_Tree_Node* ptr,int index);
void DeleteInIndexNonLeaf(Index_Tree_Node* ptr,int deleteind,int idtype);
void deleteInIndex(Index_Tree_Node* ptr,int idtype,char* firstname,char* lastname,char* numid);
IndexRecord IndexPredecessor(Index_Tree_Node* ptr,int index);
IndexRecord IndexSuccessor(Index_Tree_Node* ptr,int index);
void Indexmerge(Index_Tree_Node* ptr,int index);
void borrowFromLeftInIdex(Index_Tree_Node* ptr,int index);
void borrowFromRightInIndex(Index_Tree_Node* ptr,int index);
void adjustIndex(Index_Tree_Node* ptr,int deleteind);

void printRecordsWrapper(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr);
void printRecords(Tree_Node* ptr);

void PrintRecordAtIndex(Tree_Node* ptr,int index);

void printSortedRecords(Index_Tree_Node* ptr,Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr);

Tree_Node* SearchAfterSearchInIndex(Tree_Node* ptr,int typeid,char* numid,int* index);

void updateRecord(Index_Tree_Node* Indexptr,Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr,Index_Tree_Node** IndexrootAdr);

int allocateBlock(char type,block_Node* b1);
void deallocateBlock(Tree_Node* ptr,int index,block_Node* b1);
void specialRequestAllocation(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr,block_Node* b1);
void searchAddress_wrapper(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr);
void searchAddress(Tree_Node* ptr,char type,int low,int high);

Index_Tree_Node* searchIndexTree(char* fname,Index_Tree_Node* Indexptr,int* index);

void search(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr,Index_Tree_Node* Indexroot);

void printNameWiseSortedRecords(char* fname,Index_Tree_Node* ptr,Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr);

void InsertInBlock(block_Node** blockrootAdr,char type,int serial);
void blocksplit(int ind,block_Node* newptr,block_Node* ptr);
void blockInsertNotFull(block_Node* ptr,char type,int serial);
block_Node* SearchInBlockTree(block_Node* ptr,char* key,int* index);
void createBlocks(block_Node** blockrootAdr);