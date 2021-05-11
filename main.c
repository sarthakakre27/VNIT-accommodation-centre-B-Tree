#include"BT19CSE099-Assign2-header.h"
extern int find;

int main()
{
    int num;
    Tree_Node* aadharTree = NULL;
    Tree_Node* passportTree = NULL;
    Tree_Node* empcodeTree = NULL;
    Index_Tree_Node* indexTree = NULL;
    block_Node* blockTree = NULL;
    int i,option,idtype;
    char acctype;
    int check = -1;
	int quit = 0;
    char idnumber[15];
    createBlocks(&blockTree);

    printf("WELCOME TO VNIT ACCOMMODATION CENTRE\n");
    while(quit==0)
	{
		printf("Choose the following by pressing corresponding value\n");
		printf("Insert Record - 0 | Print Records - 1 | Print Sorted Records - 2 | Search - 3\n" );
		printf("Delete Record - 4 | Update Record - 5 | Special Request Allocation - 6 | Address Search - 7 | Quit - 8\n");
		scanf("%d",&option);
		idnumber[0]='\0';
		switch(option)
		{
			case 0:
				{
                    printf("Enter Accommodation type - \nA - type-A | B - type-B | C - type-C | D - type-D\n");
                    fflush(stdin);
                    scanf("%c",&acctype);
                    find = 0;
                    check = allocateBlock(acctype,blockTree);
                    find = 0;
                    if(check != -1)
                    {
                        printf("Enter the idtype - \n0 - AADHAR | 1 - PASSPORT | 2 - EMPCODE\n");
                        scanf("%d",&idtype);
                        if(idtype == 0)
                        {
                            InsertRecord(&aadharTree,0,&indexTree,acctype,check);
                        }
                        else if(idtype == 1)
                        {
                            InsertRecord(&passportTree,1,&indexTree,acctype,check);
                        }
                        if(idtype == 2)
                        {
                            InsertRecord(&empcodeTree,2,&indexTree,acctype,check);
                        }
                    }
					else
                    {
                        printf("Block not found Or Wrong input\n");
                    }
				}
				break;
			case 1:
				{
					printRecordsWrapper(aadharTree,passportTree,empcodeTree);
				}
				break;
			case 2:
				{
					printSortedRecords(indexTree,aadharTree,passportTree,empcodeTree);
				}
				break;
			case 3:
				{
					search(aadharTree,passportTree,empcodeTree,indexTree);
				}
				break;
			case 4:
				{
					printf("Enter the idtype - \n0 - AADHAR | 1 - PASSPORT | 2 - EMPCODE\n");
                    scanf("%d",&idtype);
                    printf("Enter idnumber\n");
                    scanf("%s",idnumber);
                    if(idtype == 0)
                    {
                        deleteRecord(aadharTree,idnumber,0,indexTree,blockTree);
                    }
                    else if(idtype == 1)
                    {
                        deleteRecord(passportTree,idnumber,1,indexTree,blockTree);
                    }
                    else if(idtype == 2)
                    {
                        deleteRecord(empcodeTree,idnumber,2,indexTree,blockTree);
                    }
                    else
                    {
                        printf("Wrong Input\n");
                    }
				}
				break;
			case 5:
				{
					updateRecord(indexTree,aadharTree,passportTree,empcodeTree,&indexTree);
				}
				break;
			case 6:
				{
					specialRequestAllocation(aadharTree,passportTree,empcodeTree,blockTree);
				}
				break;
            case 7:
				{
					searchAddress_wrapper(aadharTree,passportTree,empcodeTree);
				}
				break;
			case 8:
				{
					quit = 1;
					printf("THANK YOU\n");
				}
				break;
			default:
				{
					printf("Invalid choice");
				}
		}
	}
    return 0;
}
