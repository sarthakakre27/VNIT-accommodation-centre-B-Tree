
#include"header.h"
int find = 0;

//this function creates a new tree node and initializes all the child pointers to NULL
Tree_Node* createTreeNode()
{
    Tree_Node* newnode = (Tree_Node*)malloc(sizeof(Tree_Node));
    newnode->count = 0;
    newnode->isleaf = TRUE;
    for(int i = 0;i < MAX;i++)
    {
        newnode->children[i] = NULL;
    }
    return newnode;
}

//function to set record fields for the ind index th block of the Tree_Node
void setRecord(Tree_Node* ptr,int ind,Index_Tree_Node** IndexrootAdr,char allocType,int blockind)
{
    char type[7] = {'t','y','p','e','-','x','\0'};
	printf("Enter first name\n");
	scanf("%s",ptr->records[ind].firstname);
	printf("Enter last name\n");
	scanf("%s",ptr->records[ind].lastname);
	if(ptr->records[ind].idtype == 0)
	{
		//printf("Enter AADHAAR NO.\n");
		//scanf("%s",ptr->records[ind].id.aadhaar);
        ptr->records[ind].id.empcode[0] = '\0';
        ptr->records[ind].id.passport[0] = '\0';
        //setting alloctype
        strcpy(ptr->records[ind].accommodation_type,type);
        ptr->records[ind].accommodation_type[5] = allocType;
        ptr->records[ind].blockind = blockind;
        //while inserting in the corresponding tree insert in the index tree also
        InsertInIndex(IndexrootAdr,ptr->records[ind].idtype,ptr->records[ind].firstname,ptr->records[ind].lastname,ptr->records[ind].id.aadhaar);
	}
	else if(ptr->records[ind].idtype == 1)
	{
		//printf("Enter PASSPORT NO.\n");
		//scanf("%s",ptr->records[ind].id.passport);
        ptr->records[ind].id.aadhaar[0] = '\0';
        ptr->records[ind].id.empcode[0] = '\0';
        //setting alloctype
        strcpy(ptr->records[ind].accommodation_type,type);
        ptr->records[ind].accommodation_type[5] = allocType;
        ptr->records[ind].blockind = blockind;
        //while inserting in the corresponding tree insert in the index tree also
        InsertInIndex(IndexrootAdr,ptr->records[ind].idtype,ptr->records[ind].firstname,ptr->records[ind].lastname,ptr->records[ind].id.passport);
	}
	else
	{
		//printf("Enter EMPLOYEE CODE.\n");
		//scanf("%s",ptr->records[ind].id.empcode);
        ptr->records[ind].id.aadhaar[0] = '\0';
        ptr->records[ind].id.passport[0] = '\0';
        //setting alloctype
        strcpy(ptr->records[ind].accommodation_type,type);
        ptr->records[ind].accommodation_type[5] = allocType;
        ptr->records[ind].blockind = blockind;
        //while inserting in the corresponding tree insert in the index tree also
        InsertInIndex(IndexrootAdr,ptr->records[ind].idtype,ptr->records[ind].firstname,ptr->records[ind].lastname,ptr->records[ind].id.empcode);
	}
	fflush(stdin);
	printf("Enter address\n");
	gets(ptr->records[ind].address);
    
}

//wrapper for the utility function to search for an type and idnumber
//correct tree pointer should be passed
Tree_Node* Search_wrapper(Tree_Node* ptr)
{
    int typeid;
    char numid[15];
    int index;

    //Search input from user
    printf("Enter the idType - \n");
    scanf("%d",&typeid);
    printf("Enter idNum - \n");
    scanf("%s",numid);
    Tree_Node* ret_val = SearchInTree(ptr,typeid,numid,&index);
    return ret_val;
}

//this function takes input as the pointer to the tree node corresponding to the 
//idtype 0 , 1 , 2 tree and returns a pointer to the node containing the idtype
//and idnumber and also makes changes to the index(int) passed as a pointer
// to point to the block in the found node
Tree_Node* SearchInTree(Tree_Node* ptr,int typeid,char* numid,int* index)
{
    Tree_Node* ret_val = NULL;
    //Search in the present node

    //aadhar card holders
    if(typeid == 0)
    {
        int i = 0;
        //within entries in the Node
        while(i < ptr->count && (strcmp(ptr->records[i].id.aadhaar,numid) < 0))
        {
            i++;
        }
        //found
        if(strcmp(ptr->records[i].id.aadhaar,numid) == 0)
        {
            ret_val = ptr;
            *index = i;
        }
        //not found and the current node is a leaf -- not in the Tree -- return null
        else if(ptr->isleaf == TRUE)
        {
            ret_val = NULL;
        }
        //not a leaf -- search recusively in the children with ith index
        else
        {
            ret_val = SearchInTree(ptr->children[i],0,numid,index);
        }
    }
    //passport holders
    else if(typeid == 1)
    {
        int i = 0;
        //within entries in the Node
        while(i < ptr->count && (strcmp(ptr->records[i].id.passport,numid) < 0))
        {
            i++;
        }
        //found
        if(strcmp(ptr->records[i].id.passport,numid) == 0)
        {
            ret_val = ptr;
            *index = i;
        }
        //not found and the current node is a leaf -- not in the Tree -- return null
        else if(ptr->isleaf == TRUE)
        {
            ret_val = NULL;
        }
        //not a leaf -- search recusively in the children with ith index
        else
        {
            ret_val = SearchInTree(ptr->children[i],1,numid,index);
        }
    }
    //employee code holders
    else if(typeid == 2)
    {
        int i = 0;
        //within entries in the Node
        while(i < ptr->count && (strcmp(ptr->records[i].id.empcode,numid) < 0))
        {
            i++;
        }
        //found
        if(strcmp(ptr->records[i].id.empcode,numid) == 0)
        {
            ret_val = ptr;
            *index = i;
        }
        //not found and the current node is a leaf -- not in the Tree -- return null
        else if(ptr->isleaf == TRUE)
        {
            ret_val = NULL;
        }
        //not a leaf -- search recusively in the children with ith index
        else
        {
            ret_val = SearchInTree(ptr->children[i],2,numid,index);
        }
    }
    return ret_val;
}

//this function inserts a record into the correct node as we first search using the key -- idnumber
//and then insert in the given node that is found(leaf), here we are not traversing to the leaf while adding
//but while moving down whenever we find a node that has become full we split it regardless of the insertion 
//being in thet node or not this helps in doing a non recursive insert without traversing back the same path
//for insertion and then splitting
void InsertRecord(Tree_Node** rootAdr,int idtype,Index_Tree_Node** IndexrootAdr,char allocType,int blockind)
{
    int i = 0;
    //root can change if the root itself is split or rootnode is created
    Tree_Node* root = *rootAdr;
    //idnum input from user
    char numid[15];
    printf("Enter idNum - \n");
    scanf("%s",numid);

    //root is NULL -- tree empty
    if(root == NULL)
    {
        //allocate a new root 
        root = (Tree_Node*)malloc(sizeof(Tree_Node));
        *rootAdr = root;
        root->isleaf = TRUE;
        root->count = 1;
        //initializing all children to null
        for(int i = 0;i < MAX;i++)
        {
            root->children[i] = NULL;
        }
        //setting 1st block to the input idnum and idtype
        if(idtype == 0)
        {
            strcpy(root->records[0].id.aadhaar,numid);
            root->records[0].idtype = 0;
            //setting other fields
            setRecord(root,0,IndexrootAdr,allocType,blockind);
        }
        else if(idtype == 1)
        {
            strcpy(root->records[0].id.passport,numid);
            root->records[0].idtype = 1;
            //setting other fields
            setRecord(root,0,IndexrootAdr,allocType,blockind);
        }
        else if(idtype == 2)
        {
            strcpy(root->records[0].id.empcode,numid);
            root->records[0].idtype = 2;
            //setting other fields
            setRecord(root,0,IndexrootAdr,allocType,blockind);
        }

    }
    //root is not NULL
    else
    {
        //root is full and it splits to form a new root
        if(root->count == MAXVAL)
        {
            //allocating new root 
            Tree_Node* newRoot = createTreeNode();
            newRoot->isleaf = FALSE;

            //setting previous root as left child of the new root
            newRoot->children[0] = root;
            //split the left child into left and right and connect it to the new root
            split(0,newRoot,root);

            i = 0;
            //insert the input into the children of the new node as thy are the leaves
            //for aadhar card holders
            if(idtype == 0)
            {
                if(strcmp(newRoot->records[0].id.aadhaar,numid) < 0)
                {
                    i++;
                }
                NodeNotFullInsert(newRoot->children[i],numid,0,IndexrootAdr,allocType,blockind);
                *rootAdr = newRoot;
            }
            //for  passport holders
            else if(idtype == 1)
            {
                if(strcmp(newRoot->records[0].id.passport,numid) < 0)
                {
                    i++;
                }
                NodeNotFullInsert(newRoot->children[i],numid,1,IndexrootAdr,allocType,blockind);
                *rootAdr = newRoot;
            }
            //for employee code holders
            else if(idtype == 2)
            {
                if(strcmp(newRoot->records[0].id.empcode,numid) < 0)
                {
                    i++;
                }
                NodeNotFullInsert(newRoot->children[i],numid,1,IndexrootAdr,allocType,blockind);
                *rootAdr = newRoot;
            }

        }
        //root is not full insert into the leaves
        else
        {
            NodeNotFullInsert(root,numid,idtype,IndexrootAdr,allocType,blockind);
        }
    }
}

//this function takes the parent node and the leftchild of that node value as input and creates
//a new sibbling for the leftchild  --> split operation -- moves the MINVAL records and MIN pointers 
//to the rightchild and also connects the rightchild to the parents MIN pointer --
//during this operation the median value is moved up to the parent node
void split(int ind,Tree_Node* newptr,Tree_Node* ptr)
{
    //allocating space for the right child and settig count to minval
    Tree_Node* rightChild = (Tree_Node*)malloc(sizeof(Tree_Node));
    rightChild->count = MINVAL;
    //if the left child is a leaf the right child also becomes a leaf
    if(ptr->isleaf == TRUE)
    {
        rightChild->isleaf = TRUE;
    }
    else
    {
        rightChild->isleaf = FALSE;
    }

    //shift the records from left child to the right child -- MINVAL records
    for (int j = 0; j < MINVAL; j++)
    {
        rightChild->records[j] = ptr->records[j + MIN];
    }
    //if the left child is not a leaf copy the MIN pointers to the rightchild
    if(ptr->isleaf != TRUE)
    {
        for(int j = 0; j < MIN ; j++)
        {
            rightChild->children[j] = ptr->children[j + MIN];
        }  
    }
    //setting the count of the leftchild to MINVAL after shifting records to the right child
    ptr->count = MINVAL;

    //shifting parent pointers to add the median formed in the split to the parent node
    for (int j = newptr->count; j >= (ind + 1); j--)
    {
        newptr->children[j + 1] = newptr->children[j];
    }
    //attaching the right child to the parent
    newptr->children[ind + 1] = rightChild;

    //shifting the records in the parent node to accommodate the median
    for (int j = ((newptr->count)-1); j >= ind; j--)
    {
        newptr->records[j+1] = newptr->records[j];
    }
    //adding the median to the parent
    newptr->records[ind] = ptr->records[MINVAL];
    newptr->count++;

}

//this function is called when there are < MINVAL records in any node first we chech if the 
//node is a leaf node or not --> if it is a leaf then we add the record at the sorted position
//in the node , else we search for the child of the nodes which should contain the record
//and recursively add the record in that child
void NodeNotFullInsert(Tree_Node* ptr,char* key,int idtype,Index_Tree_Node** IndexrootAdr,char allocType,int blockind)
{
    
    //if the current node is a leaf then add the record
    if(ptr->isleaf == TRUE)
    {
        int i = ptr->count - 1;//last index of the record array
        //for aadhar card holders
        if(idtype == 0)
        {
            //shift and add the record in sorted order
            while(i >= 0 && (strcmp(ptr->records[i].id.aadhaar,key) > 0))
            {
                ptr->records[i + 1] = ptr->records[i];
                i--;
            }

            strcpy(ptr->records[i + 1].id.aadhaar,key);
            setRecord(ptr,i + 1,IndexrootAdr,allocType,blockind);
            ptr->count++;
        }
        //for passport users
        else if(idtype == 1)
        {
            //shift and add the record in sorted order
            while(i >= 0 && (strcmp(ptr->records[i].id.passport,key) > 0))
            {
                ptr->records[i + 1] = ptr->records[i];
                i--;
            }

            strcpy(ptr->records[i + 1].id.passport,key);
            setRecord(ptr,i + 1,IndexrootAdr,allocType,blockind);
            ptr->count++;
        }
        //for empcode users
        else if(idtype == 2)
        {
            //shift and add the record in sorted order
            while(i >= 0 && (strcmp(ptr->records[i].id.empcode,key) > 0))
            {
                ptr->records[i + 1] = ptr->records[i];
                i--;
            }

            strcpy(ptr->records[i + 1].id.empcode,key);
            setRecord(ptr,i + 1,IndexrootAdr,allocType,blockind);
            ptr->count++;
        }
    }
    else//if the current node is not a leaf
    {
        int i = ptr->count - 1;

        if(idtype == 0)
        {
            //search for the child
            while(i >= 0 && (strcmp(ptr->records[i].id.aadhaar,key) > 0))
            {
                i--;
            }
            //split the child found if full
            if(ptr->children[i + 1]->count == MAXVAL)
            {
                split(i + 1,ptr,ptr->children[i + 1]);

                if(strcmp(ptr->records[i + 1].id.aadhaar,key) < 0)
                {
                    i++;
                }
            }
            //insert recursively to add in the leaves
            NodeNotFullInsert(ptr->children[i + 1],key,0,IndexrootAdr,allocType,blockind);
        }
        else if(idtype == 1)
        {
            //search for the child
            while(i >= 0 && (strcmp(ptr->records[i].id.passport,key) > 0))
            {
                i--;
            }

            //split the child found if full
            if(ptr->children[i + 1]->count == MAXVAL)
            {
                split(i + 1,ptr,ptr->children[i + 1]);

                if(strcmp(ptr->records[i + 1].id.passport,key) < 0)
                {
                    i++;
                }
            }
            //insert recursively to add in the leaves
            NodeNotFullInsert(ptr->children[i + 1],key,1,IndexrootAdr,allocType,blockind);
        }
        else if(idtype == 2)
        {
            //search for the child
            while(i >= 0 && (strcmp(ptr->records[i].id.empcode,key) > 0))
            {
                i--;
            }

            //split the child found if full
            if(ptr->children[i + 1]->count == MAXVAL)
            {
                split(i + 1,ptr,ptr->children[i + 1]);

                if(strcmp(ptr->records[i + 1].id.empcode,key) < 0)
                {
                    i++;
                }
            }
            //insert recursively to add in the leaves
            NodeNotFullInsert(ptr->children[i + 1],key,2,IndexrootAdr,allocType,blockind);
        }

    }
}


//this function is used to delete a record with the input the idtypeand the idnum
//first we check for the key in the node then we check if the current node is a leaf node or not 
//if the record cooresponding to the key is found we call the deleteInLeaf or the deleteInNonLeaf
//functions accordingly if not found we go to the corresponding leaf of the node to delete
//during going down to the leaves of the tree if we find a node which is devoid of valus --> val <= MINVAL
//then we adjust the node's record such that the we can borrow from inorder predecessor/successor/merge the nodes 
//if both are devoid
void deleteRecord(Tree_Node* ptr,char* key,int idtype,Index_Tree_Node* Indexptr,block_Node* b1)
{
    int deleteind = 0;
    int deleted = 0;
    //for aadhar users
    if(idtype == 0)
    {
        //search for the key in the current node
        while(deleteind < ptr->count && (strcmp(ptr->records[deleteind].id.aadhaar,key) < 0))
        {
            deleteind++;
        }

        //key found in the current node
        if(deleteind < ptr->count && (strcmp(ptr->records[deleteind].id.aadhaar,key) == 0))
        {
            //key is found so delete in index tree also
            deleteInIndex(Indexptr,0,ptr->records[deleteind].firstname,ptr->records[deleteind].lastname,key);
            deallocateBlock(ptr,deleteind,b1);
            //if the current node is a leaf simply delete it else delete it from a non leaf function
            if(ptr->isleaf == TRUE)
            {
                DeleteInLeaf(ptr,deleteind);
            }
            else
            {
                DeleteInNonLeaf(ptr,deleteind,0,Indexptr,b1);
            }
            deleted = 1;
        }
    }
    //for passport users
    else if(idtype == 1)
    {
        //search for the key in the current node
        while(deleteind < ptr->count && (strcmp(ptr->records[deleteind].id.passport,key) < 0))
        {
            deleteind++;
        }
        //key found in the current node
        if(deleteind < ptr->count && (strcmp(ptr->records[deleteind].id.passport,key) == 0))
        {
            //key is found so delete in index tree also
            deleteInIndex(Indexptr,1,ptr->records[deleteind].firstname,ptr->records[deleteind].lastname,key);
            deallocateBlock(ptr,deleteind,b1);
            //if the current node is a leaf simply delete it else delete it from a non leaf function
            if(ptr->isleaf == TRUE)
            {
                DeleteInLeaf(ptr,deleteind);
            }
            else
            {
                DeleteInNonLeaf(ptr,deleteind,1,Indexptr,b1);
            }
            deleted = 1;
        }
    }
    //for employee code holders
    else if(idtype == 2)
    {
        //search for the key in the current node
        while(deleteind < ptr->count && (strcmp(ptr->records[deleteind].id.empcode,key) < 0))
        {
            deleteind++;
        }
        //key found in the current node
        if(deleteind < ptr->count && (strcmp(ptr->records[deleteind].id.empcode,key) == 0))
        {
            //key is found so delete in index tree also
            deleteInIndex(Indexptr,2,ptr->records[deleteind].firstname,ptr->records[deleteind].lastname,key);
            deallocateBlock(ptr,deleteind,b1);
            //if the current node is a leaf simply delete it else delete it from a non leaf function
            if(ptr->isleaf == TRUE)
            {
                DeleteInLeaf(ptr,deleteind);
            }
            else
            {
                DeleteInNonLeaf(ptr,deleteind,2,Indexptr,b1);
            }
            deleted = 1;
        }
    }


    //if the current node is a leaf and we have reached the end of the values in the node the data is not found
    if(deleted == 0)
    {
        if(ptr->isleaf == TRUE)
        {
            printf("Data not Found");
        }
        //else if the current node is not a leaf
        else
        {
            Boolean track;
            //keep track if we reached the end of the values in the node
            if(deleteind == ptr->count)
            {
                track = TRUE;
            }
            else
            {
                track = FALSE;
            }

            //if the index found has its corresponding child's count less than the minimum value
            //we adjust that node by borrowing from left or right sibling or by merging with the sibling
            if(ptr->children[deleteind]->count < MIN)
            {
                adjust(ptr,deleteind);
            }
            //after adjusting if the delete index has become greater than current node's count
            //that means a merging operation has happened
            //we recursively search and delete for the key in the deleteind - 1 th child
            if(track && deleteind > ptr->count)
            {
                deleteRecord(ptr->children[deleteind - 1],key,idtype,Indexptr,b1);
            }
            //else only borrowing has happened
            //we recursively search and delete in the deleteind th child 
            else
            {
                deleteRecord(ptr->children[deleteind],key,idtype,Indexptr,b1);
            }
        }
    }
    
}

//this function deletes the record at the given index by overwriting the records
void DeleteInLeaf(Tree_Node* ptr,int index)
{
    //start at index + 1 and move every record after index one index back
    //overwriting -- deleting the index element
    int i = index + 1;
    while(i < ptr->count)
    {
        ptr->records[i - 1] = ptr->records[i];
        i++;
    }
    ptr->count--;
}

//this function helps in the deletion in non leaf nodes --> in such case we find for the inorder 
//predecessor/successor to replace the given record value and if both are not found then we merge the
//two children and delete the record
void DeleteInNonLeaf(Tree_Node* ptr,int deleteind,int idtype,Index_Tree_Node* indexptr,block_Node* b1)
{
    //store the value to be deleted in temp
    Record temp = ptr->records[deleteind];

    //if the preceeding child has more than MIN values
    //we find the inorder predecessor and replace with the deletind value
    //and then recursively delete the predecessor
    if(ptr->children[deleteind]->count >= MIN)
    {
        Record InorderPred = Predecessor(ptr,deleteind);
        ptr->records[deleteind] = InorderPred;
        if(idtype == 0)
            deleteRecord(ptr->children[deleteind],InorderPred.id.aadhaar,0,indexptr,b1);
        else if(idtype == 1)
            deleteRecord(ptr->children[deleteind],InorderPred.id.passport,1,indexptr,b1);
        else
            deleteRecord(ptr->children[deleteind],InorderPred.id.empcode,2,indexptr,b1);
    }

    //if the succeeding child has more than MIN values
    //we find the inorder successor and replace with the deletind value
    //and then recursively delete the successor
    else if(ptr->children[deleteind + 1]->count >= MIN)
    {
        Record InorderSucc = Successor(ptr,deleteind);
        ptr->records[deleteind] = InorderSucc;
        if(idtype == 0)
            deleteRecord(ptr->children[deleteind + 1],InorderSucc.id.aadhaar,0,indexptr,b1);
        else if(idtype == 1)
            deleteRecord(ptr->children[deleteind + 1],InorderSucc.id.passport,1,indexptr,b1);
        else
            deleteRecord(ptr->children[deleteind + 1],InorderSucc.id.empcode,2,indexptr,b1);
    }

    //else the children need to be mergerd as there are < MIN values in both the
    //preceeding child and the succeeding child 
    else
    {
        NodeMerge(ptr,deleteind);
        //after merging again call for the delete on the same value
        if(idtype == 0)
            deleteRecord(ptr->children[deleteind],temp.id.aadhaar,0,indexptr,b1);
        else if(idtype == 1)
            deleteRecord(ptr->children[deleteind],temp.id.passport,1,indexptr,b1);
        else
            deleteRecord(ptr->children[deleteind],temp.id.empcode,2,indexptr,b1);
    }

}


//this function helps to adjust the node if the records <= MINVAL
//if the deleteindex record's leftchild has enough records then we borrow the record
//from the leftchild else we borrow from the right vhild of that record
//and if no enough in both we merge both the r and l child
void adjust(Tree_Node* ptr,int deleteind)
{
    //if the preceeding child has >= MIN count then borrow a value from the preceeding child 
    if(deleteind != 0 && (ptr->children[deleteind - 1]->count >= MIN ))
    {
        borrowFromLeft(ptr,deleteind);
    }
    //if the succeeding child has >= MIN count then borrow a value from the succeeding child
    else if(deleteind != ptr->count && ptr->children[deleteind + 1]->count >= MIN)
    {
        borrowFromRight(ptr,deleteind);
    }
    else
    {
        //if the deleteind between 0 ans current node's count merge left child with the right child 
        if(deleteind != ptr->count)
        {
            NodeMerge(ptr,deleteind);
        }
        //else -- last index merge second last and last children
        else
        {
            NodeMerge(ptr,deleteind - 1);
        }
    }
}

//this function finds the inorder predecessor of the record to be deleted in the node
//by going to the leftchild and then traversing to the extreme right to get the last record
Record Predecessor(Tree_Node* ptr,int index)
{
    //first go to the left child of the index
    Tree_Node* travptr = ptr->children[index];

    //and then traverse as right as possible till we reach a leaf node
    while(travptr->isleaf != TRUE)
    {
        travptr = travptr->children[travptr->count];
    }

    //return the last value in the leaf node
    return travptr->records[(travptr->count) - 1];

}

//this function finds the inorder successor of the record to be deleted in the node
//by going to the rightchildchild and then traversing to the extreme left to get the first record
Record Successor(Tree_Node* ptr,int index)
{
    //first go to the right child of the index
    Tree_Node* travptr = ptr->children[index + 1];

    //and then traverse as left as possible till we reach a leaf node
    while(travptr->isleaf != TRUE)
    {
        travptr = travptr->children[0];
    }

    //return the first value in the leaf node
    return travptr->records[0];
}

//this function helps in to merge the nodes whose records are <= MINVAL
//we shift the records in the leftchild from the rightchild -- also the children pointers
//and the parents inde record goes to the leftchild and the free the rightchild node
void NodeMerge(Tree_Node* ptr,int index)
{
    //get the left child in leftchild and its right sibling in rightSibling
    Tree_Node* leftchild = ptr->children[index];
    Tree_Node* rightSibling = ptr->children[index + 1];

    //bringing the parent node's index value to the leftchilds last value
    leftchild->records[MIN - 1] = ptr->records[index];

    int i = 0;
    //bringing all the values from the rightsibling to the leftsibling
    while(i < rightSibling->count)
    {
        leftchild->records[i + MIN] = rightSibling->records[i];
        i++;
    }
    //if the left sibling is not a eaf so is not the right sibling and thus we bring all the pointers to the 
    //children to the left child
    if(leftchild->isleaf == FALSE)
    {
        i = 0;
        while(i <= rightSibling->count)
        {
            leftchild->children[i + MIN] = rightSibling->children[i];
            i++;
        }
    }

    //then we move all the values in the parent after the index one step back to 
    //delete the index value which has been brought to the left child
    i = index + 1;
    while(i < ptr->count)
    {
        ptr->records[i - 1] = ptr->records[i];
        i++;
    }

    //also we bring all the children pointers one step back as the index element is deleted
    i = index + 2;
    while(i <= ptr->count)
    {
        ptr->children[i - 1] = ptr->children[i];
        i++;
    }

    //finally we update the count of the left child and parent node
    leftchild->count = leftchild->count + rightSibling->count + 1;
    ptr->count--;

    //free the space of the rightsibling
    free(rightSibling);

}

//this function helps in borrowing the record from the leftchild of the parent's index record
//first we bring first key from parent to the rchild and then move a record up to the parent
void borrowFromLeft(Tree_Node* ptr,int index)
{
    //mark the child of the parent node as the child and its leftSibling as the leftSibling
    Tree_Node* child = ptr->children[index];
    Tree_Node* leftSibling = ptr->children[index - 1];

    //first we shift all values in the leftchild one step ahead to accommodate the value from the parent
    int i = (child->count - 1);
    while(i >= 0)
    {
        child->records[i + 1] = child->records[i];
        i--;
    }

    //if the child is not a leaf shift all the child pointers to right
    if(child->isleaf == FALSE)
    {
        i = child->count;
        while(i >= 0)
        {
            child->children[i + 1] = child->children[i];
            i--;
        }
    }

    //bring the value from the parent to the child
    child->records[0] = ptr->records[index - 1];

    //bring the left child pointer to the child if not a leaf
    if(child->isleaf == FALSE)
    {
        child->children[0] = leftSibling->children[leftSibling->count];
    }

    //replace the parents index value to that of the last value of the leftsibling
    ptr->records[index - 1] = leftSibling->records[(leftSibling->count) - 1];

    //update the values of the child's count and that of the leftSibling
    child->count++;
    leftSibling->count--;

}

//this function helps in borrowing the record from the rightchild of the parent's index record
//first we bring last key from parent to the rightchild and move last record from lchild to the parent
void borrowFromRight(Tree_Node* ptr,int index)
{
    //mark the child of the parent node as the child and its rightSibling as the rigthSibling
    Tree_Node* child = ptr->children[index];
    Tree_Node* rightSibling = ptr->children[index + 1];

    //bring the parent's index value to the child's last value
    child->records[child->count] = ptr->records[index];

    //if child is a leaf bring the pointer from the rightSibling to the child
    if(child->isleaf == FALSE)
    {
        child->children[(child->count) + 1] = rightSibling->children[0];
    }

    //set the parents index value to that of the rightChild's first value
    ptr->records[index] = rightSibling->records[0];

    //bring all the values in the rightSibling one step back
    int i = 1;
    while(i < rightSibling->count)
    {
        rightSibling->records[i - 1] = rightSibling->records[i];
        i++;
    }

    //if right sibling is not a leaf bring all the pointers one step back
    if(rightSibling->isleaf == FALSE)
    {
        i = 1;
        while(i <= rightSibling->count)
        {
            rightSibling->children[i - 1] = rightSibling->children[i];
            i++;
        }
    }

    //update the count of the child and that of the rightSibling
    child->count++;
    rightSibling->count--;

}


/*------------Similar functions for insertion and deletion for the index tree-------------------------------------*/

void InsertInIndex(Index_Tree_Node** IndexrootAdr,int idtype,char* firstname,char* lastname,char* numid)
{
    int i = 0;
    //root can change if the root itself is split or rootnode is created
    Index_Tree_Node* Indexroot = *IndexrootAdr;
    char space = ' ';

    char key[30];
    strcpy(key,firstname);
    strncat(key,&space,1);
    strcat(key,lastname);
    char checkkey[30];

    //root is NULL -- tree empty
    if(Indexroot == NULL)
    {
        //allocate a new root 
        Indexroot = (Index_Tree_Node*)malloc(sizeof(Index_Tree_Node));
        *IndexrootAdr = Indexroot;
        Indexroot->isleaf = TRUE;
        Indexroot->count = 1;
        //initializing all children to null
        for(int i = 0;i < MAX;i++)
        {
            Indexroot->children[i] = NULL;
        }
        //setting 1st block to the input idnum and idtype
        if(idtype == 0)
        {
            strcpy(Indexroot->records[0].id.aadhaar,numid);
            Indexroot->records[0].idtype = 0;
            //setting other fields
            strcpy(Indexroot->records[0].firstname,firstname);
            strcpy(Indexroot->records[0].lastname,lastname);
        }
        else if(idtype == 1)
        {
            strcpy(Indexroot->records[0].id.passport,numid);
            Indexroot->records[0].idtype = 1;
            //setting other fields
            strcpy(Indexroot->records[0].firstname,firstname);
            strcpy(Indexroot->records[0].lastname,lastname);
        }
        else if(idtype == 2)
        {
            strcpy(Indexroot->records[0].id.empcode,numid);
            Indexroot->records[0].idtype = 2;
            //setting other fields
            strcpy(Indexroot->records[0].firstname,firstname);
            strcpy(Indexroot->records[0].lastname,lastname);
        }

    }
    //root is not NULL
    else
    {
        //root is full and it splits to form a new root
        if(Indexroot->count == MAXVAL)
        {
            //allocating new root 
            Index_Tree_Node* IndexnewRoot = (Index_Tree_Node*)malloc(sizeof(Index_Tree_Node));
            for(int i = 0;i < MAX;i++)
            {
                IndexnewRoot->children[i] = NULL;
            }
            IndexnewRoot->isleaf = FALSE;
            IndexnewRoot->count = 0;

            //setting previous root as left child of the new root
            IndexnewRoot->children[0] = Indexroot;
            //split the left child into left and right and connect it to the new root
            Indexsplit(0,IndexnewRoot,Indexroot);
            *IndexrootAdr = IndexnewRoot;

            i = 0;
            //insert the input into the children of the new node as they are the leaves
            strcpy(checkkey,IndexnewRoot->records[0].firstname);
            strncat(checkkey,&space,1);
            strcat(checkkey,IndexnewRoot->records[0].lastname);
            if(strcmp(checkkey,key) < 0)//changed key for IndexTree
            {
                i++;
            }
            NodeNotFullInsertIndex(IndexnewRoot->children[i],idtype,firstname,lastname,numid);

        }
        //root is not full insert into the leaves
        else
        {
            NodeNotFullInsertIndex(Indexroot,idtype,firstname,lastname,numid);
        }
    }
}


void Indexsplit(int ind,Index_Tree_Node* newptr,Index_Tree_Node* ptr)
{
    //allocating space for the right child and setting count to minval
    Index_Tree_Node* rightChild = (Index_Tree_Node*)malloc(sizeof(Index_Tree_Node));
    rightChild->count = MINVAL;
    //if the left child is a leaf the right child also becomes a leaf
    if(ptr->isleaf == TRUE)
    {
        rightChild->isleaf = TRUE;
    }
    else
    {
        rightChild->isleaf = FALSE;
    }

    //shift the records from left child to the right child -- MINVAL records
    int j = 0;
    while(j < MINVAL)
    {
        rightChild->records[j] = ptr->records[j + MIN];
        j++;
    }
    //if the left child is not a leaf copy the MIN pointers to the rightchild
    if(ptr->isleaf != TRUE)
    {
        j = 0;
        while(j < MIN)
        {
            rightChild->children[j] = ptr->children[j + MIN];
            j++;
        }  
    }
    //setting the count of the leftchild to MINVAL after shifting records to the right child
    ptr->count = MINVAL;

    //shifting parent pointers to add the median formed in the split to the parent node
    j = newptr->count;
    while (j >= (ind + 1))
    {
        newptr->children[j + 1] = newptr->children[j];
        j--;
    }
    //attaching the right child to the parent
    newptr->children[ind + 1] = rightChild;

    //shifting the records in the parent node to accommodate the median
    j = ((newptr->count)-1);
    while(j >= ind)
    {
        newptr->records[j+1] = newptr->records[j];
        j--;
    }
    //adding the median to the parent
    newptr->records[ind] = ptr->records[MINVAL];
    newptr->count++;

}


void NodeNotFullInsertIndex(Index_Tree_Node* ptr,int idtype,char* firstname,char* lastname,char* numid)
{
    char key[30];
    char space = ' ';
    strcpy(key,firstname);
    strncat(key,&space,1);
    strcat(key,lastname);
    char checkkey[30];
    //if the current node is a leaf then add the record
    if(ptr->isleaf == TRUE)
    {
        int i = ptr->count - 1;//last index of the record array

        strcpy(checkkey,ptr->records[i].firstname);
        strncat(checkkey,&space,1);
        strcat(checkkey,ptr->records[i].lastname);
        //shift and add the record in sorted order
        while(i >= 0 && (strcmp(checkkey,key) > 0))
        {
            ptr->records[i + 1] = ptr->records[i];
            i--;
            strcpy(checkkey,ptr->records[i].firstname);
            strncat(checkkey,&space,1);
            strcat(checkkey,ptr->records[i].lastname);
        }
        //setRecord(ptr,i + 1);
        ptr->count++;
        strcpy(ptr->records[i + 1].firstname,firstname);
        strcpy(ptr->records[i + 1].lastname,lastname);
        ptr->records[i + 1].idtype = idtype;

        if(idtype == 0)
        {
            strcpy(ptr->records[i + 1].id.aadhaar,numid);
        }
        else if(idtype == 1)
        {
            strcpy(ptr->records[i + 1].id.passport,numid);
        }
        if(idtype == 2)
        {
            strcpy(ptr->records[i + 1].id.empcode,numid);
        }
    }
    else//if the current node is not a leaf
    {
        int i = ptr->count - 1;

        strcpy(checkkey,ptr->records[i].firstname);
        strncat(checkkey,&space,1);
        strcat(checkkey,ptr->records[i].lastname);
        //search for the child
        while(i >= 0 && (strcmp(checkkey,key) > 0))
        {
            i--;
            strcpy(checkkey,ptr->records[i].firstname);
            strncat(checkkey,&space,1);
            strcat(checkkey,ptr->records[i].lastname);
        }
        //split the child found if full
        if(ptr->children[i + 1]->count == MAXVAL)
        {
            Indexsplit(i + 1,ptr,ptr->children[i + 1]);
            
            strcpy(checkkey,ptr->records[i + 1].firstname);
            strncat(checkkey,&space,1);
            strcat(checkkey,ptr->records[i + 1].lastname);
            if(strcmp(checkkey,key) < 0)
            {
                i++;
            }
        }
        
        //insert recursively to add in the leaves
        NodeNotFullInsertIndex(ptr->children[i + 1],idtype,firstname,lastname,numid);
    }
}

/*-------------------------Similar functions for deltetion in index----------------------------------------------*/

void deleteInIndex(Index_Tree_Node* ptr,int idtype,char* firstname,char* lastname,char* numid)
{
    int deleteind = 0;
    char key[30];
    char space = ' ';
    strcpy(key,firstname);
    strncat(key,&space,1);
    strcat(key,lastname);
    char checkkey[30];
    int deleted = 0;

    strcpy(checkkey,ptr->records[deleteind].firstname);
    strncat(checkkey,&space,1);
    strcat(checkkey,ptr->records[deleteind].lastname);
    //search for the key in the current node
    while(deleteind < ptr->count && (strcmp(checkkey,key) < 0))
    {
        deleteind++;
        strcpy(checkkey,ptr->records[deleteind].firstname);
        strncat(checkkey,&space,1);
        strcat(checkkey,ptr->records[deleteind].lastname);
    }


    strcpy(checkkey,ptr->records[deleteind].firstname);
    strncat(checkkey,&space,1);
    strcat(checkkey,ptr->records[deleteind].lastname);
    //key found in the current node
    if(deleteind < ptr->count && (strcmp(checkkey,key) == 0))
    {
        if(ptr->isleaf == TRUE)
        {
            //if the current node is a leaf simply delete it else delete it from a non leaf function
            DeleteInIndexLeaf(ptr,deleteind);
            deleted = 1;
        }
        else
        {
            ////DeleteInNonLeaf(ptr,deleteind,0);
            deleted = 1;
        }
    }

    else
    {
        //keep track if we reached the end of the values in the node
        Boolean track;
        if(deleteind == ptr->count)
        {
            track = TRUE;
        }
        else
        {
            track = FALSE;
        }

        //if the index found has its corresponding child's count less than the minimum value
        //we adjust that node by borrowing from left or right sibling or by merging with the sibling
        if(ptr->children[deleteind]->count < MIN)
        {
            adjustIndex(ptr,deleteind);
        }

        //after adjusting if the delete index has become greater than current node's count
        //that means a merging operation has happened
        //we recursively search and delete for the key in the deleteind - 1 th child
        if(track && deleteind > ptr->count)
        {
            deleteInIndex(ptr->children[deleteind - 1],idtype,firstname,lastname,numid);
        }
        //else only borrowing has happened
        //we recursively search and delete in the deleteind th child 
        else
        {
            deleteInIndex(ptr->children[deleteind],idtype,firstname,lastname,numid);
        }
    }
    
}

void DeleteInIndexLeaf(Index_Tree_Node* ptr,int index)
{
    //start at index + 1 and move every record after index one index back
    //overwriting -- deleting the index element
    int i = index + 1;
    while(i < ptr->count)
    {
        ptr->records[i - 1] = ptr->records[i];
        i++;
    }
    ptr->count--;
}


void DeleteInIndexNonLeaf(Index_Tree_Node* ptr,int deleteind,int idtype)
{
    //store the value to be deleted in temp
    IndexRecord temp = ptr->records[deleteind];

    //if the preceeding child has more than MIN values
    //we find the inorder predecessor and replace with the deletind value
    //and then recursively delete the predecessor
    if(ptr->children[deleteind]->count >= MIN)
    {
        IndexRecord InorderPred = IndexPredecessor(ptr,deleteind);
        ptr->records[deleteind] = InorderPred;
        if(InorderPred.idtype == 0)
            deleteInIndex(ptr->children[deleteind],InorderPred.idtype,InorderPred.firstname,InorderPred.lastname,InorderPred.id.aadhaar);
        else if(InorderPred.idtype == 1)
            deleteInIndex(ptr->children[deleteind],InorderPred.idtype,InorderPred.firstname,InorderPred.lastname,InorderPred.id.passport);
        else
            deleteInIndex(ptr->children[deleteind],InorderPred.idtype,InorderPred.firstname,InorderPred.lastname,InorderPred.id.empcode);
    }

    //if the succeeding child has more than MIN values
    //we find the inorder successor and replace with the deletind value
    //and then recursively delete the successor
    else if(ptr->children[deleteind + 1]->count >= MIN)
    {
        IndexRecord InorderSucc = IndexSuccessor(ptr,deleteind);
        ptr->records[deleteind] = InorderSucc;
        if(InorderSucc.idtype == 0)
            deleteInIndex(ptr->children[deleteind + 1],InorderSucc.idtype,InorderSucc.firstname,InorderSucc.lastname,InorderSucc.id.aadhaar);
        else if(InorderSucc.idtype == 1)
            deleteInIndex(ptr->children[deleteind + 1],InorderSucc.idtype,InorderSucc.firstname,InorderSucc.lastname,InorderSucc.id.passport);
        else
            deleteInIndex(ptr->children[deleteind + 1],InorderSucc.idtype,InorderSucc.firstname,InorderSucc.lastname,InorderSucc.id.empcode);
    }

    //else the children need to be mergerd as there are < MIN values in both the
    //preceeding child and the succeeding child 
    //and recursively delete the temp record
    else
    {
        Indexmerge(ptr,deleteind);
        if(temp.idtype == 0)
            deleteInIndex(ptr->children[deleteind],temp.idtype,temp.firstname,temp.lastname,temp.id.aadhaar);
        else if(temp.idtype == 1)
            deleteInIndex(ptr->children[deleteind],temp.idtype,temp.firstname,temp.lastname,temp.id.passport);
        else
            deleteInIndex(ptr->children[deleteind],temp.idtype,temp.firstname,temp.lastname,temp.id.empcode);
    }

}

IndexRecord IndexPredecessor(Index_Tree_Node* ptr,int index)
{
    //first go to the left child of the index
    Index_Tree_Node* travptr = ptr->children[index];

    //and then traverse as right as possible till we reach a leaf node
    while(travptr->isleaf != TRUE)
    {
        travptr = travptr->children[travptr->count];
    }
    //return the last value in the leaf node
    return travptr->records[(travptr->count) - 1];

}

IndexRecord IndexSuccessor(Index_Tree_Node* ptr,int index)
{
    //first go to the right child of the index
    Index_Tree_Node* travptr = ptr->children[index + 1];

    //and then traverse as left as possible till we reach a leaf node
    while(travptr->isleaf != TRUE)
    {
        travptr = travptr->children[0];
    }

    //return the first value in the leaf node
    return travptr->records[0];
}

void Indexmerge(Index_Tree_Node* ptr,int index)
{
    //child and right sibling are to be merged
    //get the left child in leftchild and its right sibling in rightSibling
    Index_Tree_Node* child = ptr->children[index];
    Index_Tree_Node* rightSibling = ptr->children[index + 1];

    //parents index key is inserted in the child
    //bringing the parent node's index value to the leftchilds last value
    child->records[MIN - 1] = ptr->records[index];

    //bringing all the values from the rightsibling to the leftsibling
    int i = 0;
    while(i < rightSibling->count)
    {
        child->records[i + MIN] = rightSibling->records[i];
        i++;
    }

    //if the left sibling is not a eaf so is not the right sibling and thus we bring all the pointers to the 
    //children to the left child
    if(child->isleaf == FALSE)
    {
        i = 0;
        while(i <= rightSibling->count)
        {
            child->children[i + MIN] = rightSibling->children[i];
            i++;
        }
    }

    //then we move all the values in the parent afet the index one tsep back to 
    //delete the index value which has been brought to the left child
    i = index + 1;
    while(i < ptr->count)
    {
        ptr->records[i - 1] = ptr->records[i];
        i++;
    }

    //also we bring all the children pointers one step back as the index element is deleted
    i = index + 2;
    while(i <= ptr->count)
    {
        ptr->children[i - 1] = ptr->children[i];
        i++;
    }

    //finally we update the count of the left child and parent node
    child->count = child->count + rightSibling->count + 1;
    ptr->count--;

    //free the space of the rightsibling
    free(rightSibling);

}


void borrowFromLeftInIdex(Index_Tree_Node* ptr,int index)
{
    //mark the child of the parent node as the child and its leftSibling as the leftSibling
    Index_Tree_Node* child = ptr->children[index];
    Index_Tree_Node* leftSibling = ptr->children[index - 1];

    //first we shift all values in the leftchild one step ahead to accommodate the value from the parent
    int i = (child->count - 1);
    while(i >= 0)
    {
        child->records[i + 1] = child->records[i];
        i--;
    }
    //if the child is not a leaf shift all the child pointers to right
    if(child->isleaf == FALSE)
    {
        i = child->count;
        while(i >= 0)
        {
            child->children[i + 1] = child->children[i];
            i--;
        }
    }
    //bring the value from the parent to the child
    child->records[0] = ptr->records[index - 1];

    //bring the left child pointer to the child if not a leaf
    if(child->isleaf == FALSE)
    {
        child->children[0] = leftSibling->children[leftSibling->count];
    }

    //replace the parents index value to that of the last value of the leftsibling
    ptr->records[index - 1] = leftSibling->records[(leftSibling->count) - 1];

    //update the values of the child's count and that of the leftSibling
    child->count++;
    leftSibling->count--;

}


void borrowFromRightInIndex(Index_Tree_Node* ptr,int index)
{
    //mark the child of the parent node as the child and its rightSibling as the rigthSibling
    Index_Tree_Node* child = ptr->children[index];
    Index_Tree_Node* rightSibling = ptr->children[index + 1];

    //bring the parent's index value to the child's last value
    child->records[child->count] = ptr->records[index];

    //if child is a leaf bring the pointer from the rightSibling to the child
    if(child->isleaf == FALSE)
    {
        child->children[(child->count) + 1] = rightSibling->children[0];
    }

    //set the parents index value to that of the rightChild's first value
    ptr->records[index] = rightSibling->records[0];

    //bring all the values in the rightSibling one step back
    int i = 1;
    while(i < rightSibling->count)
    {
        rightSibling->records[i - 1] = rightSibling->records[i];
        i++;
    }

    //if right sibling is not a leaf bring all the pointers one step back
    if(rightSibling->isleaf == FALSE)
    {
        i = 1;
        while(i <= rightSibling->count)
        {
            rightSibling->children[i - 1] = rightSibling->children[i];
            i++;
        }
    }

    //update the count of the child and that of the rightSibling
    child->count++;
    rightSibling->count--;

}

void adjustIndex(Index_Tree_Node* ptr,int deleteind)
{
    //if the preceeding child has >= MIN count then borrow a value from the preceeding child
    if(deleteind != 0 && (ptr->children[deleteind - 1]->count >= MIN ))
    {
        borrowFromLeftInIdex(ptr,deleteind);
    }
    //if the succeeding child has >= MIN count then borrow a value from the succeeding child
    else if(deleteind != ptr->count && ptr->children[deleteind + 1]->count >= MIN)
    {
        borrowFromRightInIndex(ptr,deleteind);
    }
    else
    {
        //if the deleteind between 0 ans current node's count merge left child with the right child
        if(deleteind != ptr->count)
        {
            Indexmerge(ptr,deleteind);
        }
        //else -- last index merge second last and last children
        else
        {
            Indexmerge(ptr,deleteind - 1);
        }
    }
}


//wrapper function to print records in the three trees 
//first aadhar --> passport --> empcode
void printRecordsWrapper(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr)
{
    printRecords(aadharptr);
    printRecords(passportptr);
    printRecords(empcodeptr);
}

//function for the inorder traversal of the tree and printing the records
void printRecords(Tree_Node* ptr)
{
    if(ptr != NULL)
    {
        int i = 0;
        //print every value record in the current node
        while(i < ptr->count)
        {
            //if the current node is not a leaf
            //then recusively call for the ith child
            if(ptr->isleaf != TRUE)
            {
                printRecords(ptr->children[i]);
            }
            //after covering the child print the value record at tthe ith index in the current node
            PrintRecordAtIndex(ptr,i);
            i++;
        }
        //last child gets left as we are going through count of the node
        //call printrecords for the ith child
        if(ptr->isleaf != TRUE)
        {
            printRecords(ptr->children[i]);
        }
    }
}

//function to print the record contents in the index th block of the node
void PrintRecordAtIndex(Tree_Node* ptr,int index)
{
    printf("Firstname- %s\n",ptr->records[index].firstname);
	printf("Lastname- %s\n",ptr->records[index].lastname);
	printf("Accommodation type- %s\n",ptr->records[index].accommodation_type);
	printf("ID type- %d\n",ptr->records[index].idtype);
    if(ptr->records[index].idtype == 0)
    {
        printf("AADHAR- %s\n",ptr->records[index].id.aadhaar);
    }
    else if(ptr->records[index].idtype == 1)
    {
        printf("PASSPORT- %s\n",ptr->records[index].id.passport);
    }
    else
    {
        printf("EMPCODE- %s\n",ptr->records[index].id.empcode);
    }
	printf("Block number- %d\n",ptr->records[index].blockind);
	printf("Address- %s\n\n",ptr->records[index].address);
}


//function for the inorder traversal of the indextree 
//so that we get the values sorted on the basis of the firstnames and lastnames
//gives the id type and the id number to be searched in the respective
//idtype tree and print the value contents there
void printSortedRecords(Index_Tree_Node* ptr,Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr)
{
    if(ptr == NULL)
    {
        printf("Empty Tree\n");
    }
    else
    {
        int i = 0;
        int index;//to be modified by SearchAfterSearchInIndex
        Tree_Node* treeptr = NULL;//to be modified with the returned value of the SearchAfterSearchInIndex
        //print every value record in the current node
        while(i < ptr->count)
        {
            //if the current node is not a leaf
            //then recusively call for the ith child
            if(ptr->isleaf != TRUE)
            {
                printSortedRecords(ptr->children[i],aadharptr,passportptr,empcodeptr);
            }
            //after covering the child print the value record at the ith index in the current node
            ////PrintRecordAtIndex(ptr,i);
            //SearchAfterSearchInIndex()
            if(ptr->records[i].idtype == 0)
            {
                treeptr = SearchAfterSearchInIndex(aadharptr,0,ptr->records[i].id.aadhaar,&index);
            }
            else if(ptr->records[i].idtype == 1)
            {
                treeptr = SearchAfterSearchInIndex(passportptr,1,ptr->records[i].id.passport,&index);
            }
            else
            {
                treeptr = SearchAfterSearchInIndex(empcodeptr,2,ptr->records[i].id.empcode,&index);
            }

            //has to be non null as we are searching from index --> initial tree -- extra guard condition
            if(treeptr != NULL)
            {
                PrintRecordAtIndex(treeptr,index);//printing the value given by the search in the original tree
            }

            i++;
        }
        //last child gets left as we are going through count of the node
        //call printrecords for the last child
        if(ptr->isleaf != TRUE)
        {
            printSortedRecords(ptr->children[i],aadharptr,passportptr,empcodeptr);
        }
    }
}

//this function searches for the record in the aadhartree/passporttree/empcodetree after searching the name in the indextree 
Tree_Node* SearchAfterSearchInIndex(Tree_Node* ptr,int typeid,char* numid,int* index)
{
    Tree_Node* ret_val = NULL;
    //Search in the present node

    //aadhar card holders
    if(typeid == 0)
    {
        int i = 0;
        //within entries in the Node
        while(i < ptr->count && (strcmp(ptr->records[i].id.aadhaar,numid) < 0))
        {
            i++;
        }
        //found
        if(strcmp(ptr->records[i].id.aadhaar,numid) == 0)
        {
            ret_val = ptr;
            *index = i;
        }
        //not found and the current node is a leaf -- not in the Tree -- return null
        else if(ptr->isleaf == TRUE)
        {
            ret_val = NULL;
        }
        //not a leaf -- search recusively in the children with ith index
        else
        {
            ret_val = SearchAfterSearchInIndex(ptr->children[i],0,numid,index);
        }
    }
    //passport holders
    else if(typeid == 1)
    {
        int i = 0;
        //within entries in the Node
        while(i < ptr->count && (strcmp(ptr->records[i].id.passport,numid) < 0))
        {
            i++;
        }
        //found
        if(strcmp(ptr->records[i].id.passport,numid) == 0)
        {
            ret_val = ptr;
            *index = i;
        }
        //not found and the current node is a leaf -- not in the Tree -- return null
        else if(ptr->isleaf == TRUE)
        {
            ret_val = NULL;
        }
        //not a leaf -- search recusively in the children with ith index
        else
        {
            ret_val = SearchAfterSearchInIndex(ptr->children[i],1,numid,index);
        }
    }
    //employee code holders
    else if(typeid == 2)
    {
        int i = 0;
        //within entries in the Node
        while(i < ptr->count && (strcmp(ptr->records[i].id.empcode,numid) < 0))
        {
            i++;
        }
        //found
        if(strcmp(ptr->records[i].id.empcode,numid) == 0)
        {
            ret_val = ptr;
            *index = i;
        }
        //not found and the current node is a leaf -- not in the Tree -- return null
        else if(ptr->isleaf == TRUE)
        {
            ret_val = NULL;
        }
        //not a leaf -- search recusively in the children with ith index
        else
        {
            ret_val = SearchAfterSearchInIndex(ptr->children[i],2,numid,index);
        }
    }
    return ret_val;
}

//this function is used for the updation in the three main trees if the updation is the firname or the lastname 
//then we update in the indextree
void updateRecord(Index_Tree_Node* Indexptr,Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr,Index_Tree_Node** IndexrootAdr)
{
    int idtype;
    int idtypechoice;
    char idnum[15];
    //take input from user to update
    printf("Enter idtype to update - \n");
    scanf("%d",&idtype);
    printf("Enter idnum - \n");
    scanf("%s",idnum);
    int index = -1;//to be modified by Search
    int choice;
    //temp fname and lname for user input and function calls
    char fname[50];
    fname[0] = '\0';
    char lname[50];
    lname[0] = '\0';
    char address[100];
    address[0] = '\0';

    Tree_Node* ptr = NULL;

    //first we search for the given idtype and idnumber in the respective trees
    if(idtype == 0)
    {
        ptr = SearchInTree(aadharptr,idtype,idnum,&index);
    }
    else if(idtype == 1)
    {
        ptr = SearchInTree(passportptr,idtype,idnum,&index);
    }
    else if(idtype == 2)
    {
        ptr = SearchInTree(empcodeptr,idtype,idnum,&index);
    }

    //if the record is found ptr != NULL
    //found condition
    if(ptr != NULL)
    {
        printf("press the follwing number to update accordingly\n");
	    printf("firstname - 0 | lastname - 1 | accommodation type - 2 | IdNumber - 3 | address - 5 \n");
	    scanf("%d",&choice);
	
	    switch(choice)
	    {
		    case 0:
			    {
                    //in case of firstname change --> changes made to the IndexTree
                    if(idtype == 0)
                    {
                        deleteInIndex(Indexptr,ptr->records[index].idtype,ptr->records[index].firstname,ptr->records[index].lastname,ptr->records[index].id.aadhaar);
                    }
                    else if(idtype == 1)
                    {
                        deleteInIndex(Indexptr,ptr->records[index].idtype,ptr->records[index].firstname,ptr->records[index].lastname,ptr->records[index].id.passport);
                    }
                    else if(idtype == 2)
                    {
                        deleteInIndex(Indexptr,ptr->records[index].idtype,ptr->records[index].firstname,ptr->records[index].lastname,ptr->records[index].id.empcode);
                    }
				    printf("Enter firstname to update\n");
				    scanf("%s",&fname);
				    strcpy(ptr->records[index].firstname,fname);

                    if(idtype == 0)
                    {
                        InsertInIndex(IndexrootAdr,0,fname,ptr->records[index].lastname,ptr->records[index].id.aadhaar);
                    }
                    else if(idtype == 1)
                    {
                        InsertInIndex(IndexrootAdr,1,fname,ptr->records[index].lastname,ptr->records[index].id.passport);
                    }
                    else if(idtype == 2)
                    {
                        InsertInIndex(IndexrootAdr,2,fname,ptr->records[index].lastname,ptr->records[index].id.empcode);
                    }
			    }
			    break;
		    case 1:
			    {
                    //in case of lastname change --> changes made to the IndexTree
                    if(idtype == 0)
                    {
                        deleteInIndex(Indexptr,ptr->records[index].idtype,ptr->records[index].firstname,ptr->records[index].lastname,ptr->records[index].id.aadhaar);
                    }
                    else if(idtype == 1)
                    {
                        deleteInIndex(Indexptr,ptr->records[index].idtype,ptr->records[index].firstname,ptr->records[index].lastname,ptr->records[index].id.passport);
                    }
                    else if(idtype == 2)
                    {
                        deleteInIndex(Indexptr,ptr->records[index].idtype,ptr->records[index].firstname,ptr->records[index].lastname,ptr->records[index].id.empcode);
                    }
				    printf("Enter lastname to update\n");
				    scanf("%s",&lname);
				    strcpy(ptr->records[index].lastname,lname);
                    
                    if(idtype == 0)
                    {
                        InsertInIndex(IndexrootAdr,0,ptr->records[index].firstname,lname,ptr->records[index].id.aadhaar);
                    }
                    else if(idtype == 1)
                    {
                        InsertInIndex(IndexrootAdr,1,ptr->records[index].firstname,lname,ptr->records[index].id.passport);
                    }
                    else if(idtype == 2)
                    {
                        InsertInIndex(IndexrootAdr,0,ptr->records[index].firstname,lname,ptr->records[index].id.empcode);
                    }
			    }
			    break;
		    case 2:
			    {
				    printf("Accommodation type can be requested to change in main menu with SpecialRequestAllocation\n");
			    }
			    break;
            case 3:
			    {
                    //idnumber is unique and cannot be updated
                    //delete entry with the idnumber in main menu and insert a new entry with new idnumber
				    printf("IdNumber once entered cannot be updated, to record entry with \n");
                    printf("new idtype delete record in main menu and insert with update \n");
			    }
			    break;
		    case 4:
			    {
				    printf("Enter address to update\n");
				    scanf("%d",&address);
				    strcpy(ptr->records[index].address,address);
			    }
			    break;
		    default:
			    {
				    printf("INVALID CHOICE\n");
			    }
			    break;
			
	    }
    }
    else//Not found
    {
        printf("Record Not Found\n");
    }
}



void specialRequestAllocation(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr,block_Node* b1)
{
    int typeid;
    char numid[15];
    printf("Enter your IDType - 0 - AADHAR | 1 - PASSPORT | 2 - EMPCODE - \n");
    scanf("%d",&typeid);
    printf("Enter NumberId - \n");
    scanf("%s",numid);
    int choice;
    Tree_Node* searchptr = NULL;
    int index;//to be mdified by the search function
    int i = 0;
    int found = 0;
    int blockind;

    //search according to the id type
    //search returns the pointer to the node and also makes changes to the index variable
    if(typeid == 0)
    {
        searchptr = SearchInTree(aadharptr,typeid,numid,&index);
    }
    else if(typeid == 1)
    {
        searchptr = SearchInTree(passportptr,typeid,numid,&index);
    }
    else if(typeid == 2)
    {
        searchptr = SearchInTree(empcodeptr,typeid,numid,&index);
    }


    if(searchptr != NULL && typeid < 3)//found extra guard condition for invalid input
    {
        printf("Enter the type of accommodation to check for\nOr chose same type to change the block\nEnter number accordingly\n");
		printf("Type-A - 1 |  Type-B - 2 | Type-C - 3 | Type-D - 4\n");
		scanf("%d",&choice);

        if(choice == 1)
        {
            //first allocate block and then deallocate previous block if the block is available
            find = 0;
            blockind = allocateBlock('A',b1);
            find = 0;
            if(blockind != -1)
            {
                deallocateBlock(searchptr,index,b1);
                searchptr->records[index].blockind = blockind;
                searchptr->records[index].accommodation_type[5] = 'A';
            }
            else
            {
                printf("Block not available\n");
            }
        }
        else if(choice == 2)
        {
            //first allocate block and then deallocate previous block if the block is available
            find = 0;
            blockind = allocateBlock('B',b1);
            find = 0;
            if(blockind != -1)
            {
                deallocateBlock(searchptr,index,b1);
                searchptr->records[index].blockind = blockind;
                searchptr->records[index].accommodation_type[5] = 'B';
            }
            else
            {
                printf("Block not available\n");
            }
        }
        else if(choice == 3)
        {
            //first allocate block and then deallocate previous block if the block is available
            find = 0;
            blockind = allocateBlock('C',b1);
            find = 0;
            if(blockind != -1)
            {
                deallocateBlock(searchptr,index,b1);
                searchptr->records[index].blockind = blockind;
                searchptr->records[index].accommodation_type[5] = 'C';
            }
            else
            {
                printf("Block not available\n");
            }
        }
        else if(choice == 4)
        {
            //first allocate block and then deallocate previous block if the block is available
            find = 0;
            blockind = allocateBlock('D',b1);
            find = 0;
            if(blockind != -1)
            {
                deallocateBlock(searchptr,index,b1);
                searchptr->records[index].blockind = blockind;
                searchptr->records[index].accommodation_type[5] = 'D';
            }
            else
            {
                printf("Block not available\n");
            }
        }
        else
        {
            printf("Invalid Choice\n");
        }
    }
    else
    {
        printf("Record Not Found\n");
    }
}

//as the adress of the various records are scattered and not in sorted order
//we traverse through each record and check if the record value is in the 
//bounds of the given input and print them
//althogh we can maintain fields in the blockTree for the fields of idtype and idnum
//which then can be traversed and then the found address can be searched in the 3 main
//trees but first the allocation in the block i.e. the search for the availability of the 
//block and then marking it allocated is happening thus we will have to traverse the trees
void searchAddress_wrapper(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr)
{
    char type;
    int low,high;
    printf("Enter the Accommodation Type - \n");
    printf("A - type-A | B - type-B | C - type-C | D - type-D\n");
    fflush(stdin);
    scanf("%c",&type);
    printf("Enter the lower bound - \n");
    scanf("%d",&low);
    printf("Enter the upper bound - \n");
    scanf("%d",&high);
    searchAddress(aadharptr,type,low,high);
    searchAddress(passportptr,type,low,high);
    searchAddress(empcodeptr,type,low,high);
}


void searchAddress(Tree_Node* ptr,char type,int low,int high)
{
    if(ptr != NULL)
    {
        int i = 0;
        //print every value record in the current node
        while(i < ptr->count)
        {
            //if the current node is not a leaf
            //then recusively call for the ith child
            if(ptr->isleaf != TRUE)
            {
                searchAddress(ptr->children[i],type,low,high);
            }
            //after covering the child print the value record at tthe ith index in the current node
            //printing only the records which are in the range
            if(ptr->records[i].accommodation_type[5] ==  type && ptr->records[i].blockind >= low && ptr->records[i].blockind <= high)
            {
                PrintRecordAtIndex(ptr,i);
            }
            i++;
        }
        //last child gets left as we are going through count of the node
        //call printrecords for the ith child
        if(ptr->isleaf != TRUE)
        {
            searchAddress(ptr->children[i],type,low,high);
        }
    }
}

//this function takes input as the name of the person and prints the same name people in alphabetical order

void search(Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr,Index_Tree_Node* Indexroot)
{
    //first we input from the user the firstname to be searched
    //then we search for the firstname in the index tree
    char fname[50];
    fname[0] = '\0';
    int index = -1;//to be change by the searchIndexTree
    printf("Enter the firstname to search - \n");
    scanf("%s",fname);
    Index_Tree_Node* foundptr = NULL;

    //now we search for the given name in the index tree
    foundptr = searchIndexTree(fname,Indexroot,&index);
    //searchIndexTree gives the pointer and the index of the first occurance of the fname given
    //there will not be any record with same name in the parent of the pointer that is returned
    //as it would have stopped there and also the fname cannot be in the sibling of the pointer returned
    //as again search would have stopped on the parent --> so the fname containing records are present
    //in the node whose pointer is returned and its subtrees --> so we do a inorder traversal on 
    //that node and its subtrees to get the names in the alphabetical order

    if(foundptr != NULL)
    {
        //now we do a inorder traversal of the node pointed by foundptr and print te records
        //this is done by the function printNameWiseSortedRecords
        //which saves us from traversing all all the nodes for inorder traversal and just traverse the 
        //node which has all the names in alphabetic order
        printNameWiseSortedRecords(fname,foundptr,aadharptr,passportptr,empcodeptr);
    }

}

//this function searches for a given nmae in the indextree and returns the pointer to the
//node containing it also changes the index pointer to the index of the block in the node
Index_Tree_Node* searchIndexTree(char* fname,Index_Tree_Node* Indexptr,int* index)
{
    Index_Tree_Node* ret_val = NULL;//default NULL if not found
    int i = 0;
    //within entries in the Node
    while(i < Indexptr->count && (strcmp(Indexptr->records[i].firstname,fname) < 0))
    {
        i++;
    }
    //found
    if(strcmp(Indexptr->records[i].firstname,fname) == 0)
    {
        ret_val = Indexptr;
        *index = i;
    }
    //not found and the current node is a leaf -- not in the Tree -- return null
    else if(Indexptr->isleaf == TRUE)
    {
        ret_val = NULL;
    }
    //not a leaf -- search recusively in the children with ith index
    else
    {
        ret_val = searchIndexTree(fname,Indexptr->children[i],index);
    }
    return ret_val;
}


//this function traverses the given node pointer for the first occurance of the name and traverses inorder so that
//alphabetical order of the names is printed
void printNameWiseSortedRecords(char* fname,Index_Tree_Node* ptr,Tree_Node* aadharptr,Tree_Node* passportptr,Tree_Node* empcodeptr)
{
    if(ptr == NULL)
    {
        printf("Empty Tree\n");
    }
    else
    {
        int i = 0;
        int index;//to be modified by SearchAfterSearchInIndex
        Tree_Node* treeptr = NULL;//to be modified with the returned value of the SearchAfterSearchInIndex
        //print every value record in the current node
        while(i < ptr->count)
        {
            //if the current node is not a leaf
            //then recusively call for the ith child
            if(ptr->isleaf != TRUE)
            {
                printNameWiseSortedRecords(fname,ptr->children[i],aadharptr,passportptr,empcodeptr);
            }
            //after covering the child print the value record at the ith index in the current node
            //if the record mathches the firstname -->search in corresponding tree --> print the record
            if(strcmp(ptr->records[i].firstname,fname) == 0)
            {
                if(ptr->records[i].idtype == 0)
                {
                    treeptr = SearchAfterSearchInIndex(aadharptr,0,ptr->records[i].id.aadhaar,&index);
                }
                else if(ptr->records[i].idtype == 1)
                {
                    treeptr = SearchAfterSearchInIndex(passportptr,1,ptr->records[i].id.passport,&index);
                }
                else
                {
                    treeptr = SearchAfterSearchInIndex(empcodeptr,2,ptr->records[i].id.empcode,&index);
                }

                //has to be non null as we are searching from index --> initial tree -- extra guard condition
                if(treeptr != NULL)
                {
                    PrintRecordAtIndex(treeptr,index);//printing the value given by the search in the original tree
                }
            }

            i++;
        }
        //last child gets left as we are going through count of the node
        //call printrecords for the last child
        if(ptr->isleaf != TRUE)
        {
            printNameWiseSortedRecords(fname,ptr->children[i],aadharptr,passportptr,empcodeptr);
        }
    }
}

/*-----------------------------------Similar insertion functions for block tree---------------------------------------------------*/

void InsertInBlock(block_Node** blockrootAdr,char type,int serial)
{
    int i = 0;
    //root can change if the root itself is split or rootnode is created
    block_Node* blockroot = *blockrootAdr;

    char defaultType[6] = {'t','y','p','e','-','\0'};
    char ten[3] = {'1','0','\0'};
    char zero = '0';
    char charserial = serial + '0';

    char insertType[9];
    strcpy(insertType,defaultType);
    strncat(insertType,&type,1);

    char key[9];
    strcpy(key,defaultType);
    strncat(key,&type,1);
    if(serial == 10)
    {
        strcat(key,ten);
    }
    else
    {
        strncat(key,&zero,1);
        strncat(key,&charserial,1);
    }
    char checkkey[9];

    //root is NULL -- tree empty
    if(blockroot == NULL)
    {
        //allocate a new root 
        blockroot = (block_Node*)malloc(sizeof(block_Node));
        *blockrootAdr = blockroot;
        blockroot->isleaf = TRUE;
        blockroot->count = 1;
        //initializing all children to null
        for(int i = 0;i < MAX;i++)
        {
            blockroot->children[i] = NULL;
        }
        
        blockroot->records[0].accommodated = 0;
        strcpy(blockroot->records[0].accommodation_type,insertType);
        blockroot->records[0].serialNumber = serial;

    }
    //root is not NULL
    else
    {
        //root is full and it splits to form a new root
        if(blockroot->count == MAXVAL)
        {
            //allocating new root 
            block_Node* blocknewRoot = (block_Node*)malloc(sizeof(block_Node));
            for(int i = 0;i < MAX;i++)
            {
                blocknewRoot->children[i] = NULL;
            }
            blocknewRoot->isleaf = FALSE;
            blocknewRoot->count = 0;

            //setting previous root as left child of the new root
            blocknewRoot->children[0] = blockroot;
            //split the left child into left and right and connect it to the new root
            blocksplit(0,blocknewRoot,blockroot);
            *blockrootAdr = blocknewRoot;

            i = 0;
            //insert the input into the children of the new node as they are the leaves
            strcpy(checkkey,blocknewRoot->records[0].accommodation_type);
            if(blocknewRoot->records[0].serialNumber == 10)
            {
                strcat(checkkey,ten);
            }
            else
            {
                strncat(checkkey,&zero,1);
                charserial = blocknewRoot->records[0].serialNumber + '0';
                strncat(checkkey,&charserial,1);
            }
            if(strcmp(checkkey,key) < 0)//changed key for IndexTree
            {
                i++;
            }
            blockInsertNotFull(blocknewRoot->children[i],type,serial);

        }
        //root is not full insert into the leaves
        else
        {
            blockInsertNotFull(blockroot,type,serial);
        }
    }
}

void blocksplit(int ind,block_Node* newptr,block_Node* ptr)
{
    //allocating space for the right child and setting count to minval
    block_Node* rightChild = (block_Node*)malloc(sizeof(block_Node));
    rightChild->count = MINVAL;
    //if the left child is a leaf the right child also becomes a leaf
    if(ptr->isleaf == TRUE)
    {
        rightChild->isleaf = TRUE;
    }
    else
    {
        rightChild->isleaf = FALSE;
    }

    //shift the records from left child to the right child -- MINVAL records
    int j = 0;
    while(j < MINVAL)
    {
        rightChild->records[j] = ptr->records[j + MIN];
        j++;
    }
    //if the left child is not a leaf copy the MIN pointers to the rightchild
    if(ptr->isleaf != TRUE)
    {
        j = 0;
        while(j < MIN)
        {
            rightChild->children[j] = ptr->children[j + MIN];
            j++;
        }  
    }
    //setting the count of the leftchild to MINVAL after shifting records to the right child
    ptr->count = MINVAL;

    //shifting parent pointers to add the median formed in the split to the parent node
    j = newptr->count;
    while (j >= (ind + 1))
    {
        newptr->children[j + 1] = newptr->children[j];
        j--;
    }
    //attaching the right child to the parent
    newptr->children[ind + 1] = rightChild;

    //shifting the records in the parent node to accommodate the median
    j = ((newptr->count)-1);
    while(j >= ind)
    {
        newptr->records[j+1] = newptr->records[j];
        j--;
    }
    //adding the median to the parent
    newptr->records[ind] = ptr->records[MINVAL];
    newptr->count++;

}

void blockInsertNotFull(block_Node* ptr,char type,int serial)
{
    char defaultType[6] = {'t','y','p','e','-','\0'};
    char key[9];
    strcpy(key,defaultType);
    strncat(key,&type,1);

    char ten[3] = {'1','0','\0'};
    char zero = '0';
    char charserial = serial + '0';
    if(serial == 10)
    {
        strcat(key,ten);
    }
    else
    {
        strncat(key,&zero,1);
        strncat(key,&charserial,1);
    }
    char checkkey[9];
    //if the current node is a leaf then add the record
    if(ptr->isleaf == TRUE)
    {
        int i = ptr->count - 1;//last index of the record array

        strcpy(checkkey,ptr->records[i].accommodation_type);
        if(ptr->records[i].serialNumber == 10)
        {
            strcat(checkkey,ten);
        }
        else
        {
            strncat(checkkey,&zero,1);
            charserial = ptr->records[i].serialNumber + '0';
            strncat(checkkey,&charserial,1);
        }
        //shift and add the record in sorted order
        while(i >= 0 && (strcmp(checkkey,key) > 0))
        {
            ptr->records[i + 1] = ptr->records[i];
            i--;
            strcpy(checkkey,ptr->records[i].accommodation_type);
            if(ptr->records[i].serialNumber == 10)
            {
                strcat(checkkey,ten);
            }
            else
            {
                strncat(checkkey,&zero,1);
                charserial = ptr->records[i].serialNumber + '0';
                strncat(checkkey,&charserial,1);
            }
        }
        //setRecord(ptr,i + 1);
        ptr->count++;
        strcpy(ptr->records[i + 1].accommodation_type,defaultType);
        strncat(ptr->records[i + 1].accommodation_type,&type,1);
        ptr->records[i + 1].serialNumber = serial;
        ptr->records[i + 1].accommodated = 0;
    }
    else//if the current node is not a leaf
    {
        int i = ptr->count - 1;

        strcpy(checkkey,ptr->records[i].accommodation_type);
        if(ptr->records[i].serialNumber == 10)
        {
            strcat(checkkey,ten);
        }
        else
        {
            strncat(checkkey,&zero,1);
            charserial = ptr->records[i].serialNumber + '0';
            strncat(checkkey,&charserial,1);
        }
        //search for the child
        while(i >= 0 && (strcmp(checkkey,key) > 0))
        {
            i--;
            strcpy(checkkey,ptr->records[i].accommodation_type);
            if(ptr->records[i].serialNumber == 10)
            {
                strcat(checkkey,ten);
            }
            else
            {
                strncat(checkkey,&zero,1);
                charserial = ptr->records[i].serialNumber + '0';
                strncat(checkkey,&charserial,1);
            }
        }
        //split the child found if full
        if(ptr->children[i + 1]->count == MAXVAL)
        {
            blocksplit(i + 1,ptr,ptr->children[i + 1]);
            
            strcpy(checkkey,ptr->records[i + 1].accommodation_type);
            if(ptr->records[i + 1].serialNumber == 10)
            {
                strcat(checkkey,ten);
            }
            else
            {
                strncat(checkkey,&zero,1);
                charserial = ptr->records[i + 1].serialNumber + '0';
                strncat(checkkey,&charserial,1);
            }
            if(strcmp(checkkey,key) < 0)
            {
                i++;
            }
        }
        
        //insert recursively to add in the leaves
        blockInsertNotFull(ptr->children[i + 1],type,serial);
    }
}

//creating 10 blocks of each type
void createBlocks(block_Node** blockrootAdr)
{
    int i = 0;
    for(i = 0;i < 10;i++)
    {
        InsertInBlock(blockrootAdr,'A',i + 1);
    }
    i = 0;
    for(i = 0;i < 10;i++)
    {
        InsertInBlock(blockrootAdr,'B',i + 1);
    }
    i = 0;
    for(i = 0;i < 10;i++)
    {
        InsertInBlock(blockrootAdr,'C',i + 1);
    }
    i = 0;
    for(i = 0;i < 10;i++)
    {
        InsertInBlock(blockrootAdr,'D',i + 1);
    }
}

//this function searches for the block of given type and returns the block index of the 
//allocated block
int allocateBlock(char type,block_Node* blockptr)
{
    int ret_val = -1;
    if(blockptr != NULL)
    {
    
        int i = 0;
        while(find == 0 && i < blockptr->count)
        {
            //if the current node is not a leaf
            //then recusively call for the ith child
            if(blockptr->isleaf != TRUE)
            {
                ret_val = allocateBlock(type,blockptr->children[i]);
            }
            //after covering the child print the value record at the ith index in the current node
            if(find == 0 && blockptr->records[i].accommodation_type[5] == type && blockptr->records[i].accommodated == 0)
            {
                blockptr->records[i].accommodated = 1;
                find = 1;
                ret_val =  blockptr->records[i].serialNumber;
            }
            i++;
        }
        //last child gets left as we are going through count of the node
        //call printrecords for the last child
        if(find == 0 && blockptr->isleaf != TRUE)
        {
            ret_val = allocateBlock(type,blockptr->children[i]);
        }
    }
    return ret_val;
}

//search for a particular type and serial number in the block tree and return the pointer and 
//change the index pointer to the record in the node found
block_Node* SearchInBlockTree(block_Node* ptr,char* key,int* index)
{
    block_Node* ret_val = NULL;
    //Search in the present node
    char checkkey[9];
    char ten[3] = {'1','0','\0'};
    char charserial;

    int i = 0;
    //within entries in the Node
    strcpy(checkkey,ptr->records[i].accommodation_type);
    if(ptr->records[i].serialNumber == 10)
    {
        strcat(checkkey,ten);
    }
    else
    {
        charserial = ptr->records[i].serialNumber + '0';
        strncat(checkkey,&charserial,1);
    }
    while(i < ptr->count && (strcmp(checkkey,key) < 0))
    {
        i++;
        strcpy(checkkey,ptr->records[i].accommodation_type);
        if(ptr->records[i].serialNumber == 10)
        {
            strcat(checkkey,ten);
        }
        else
        {
            charserial = ptr->records[i].serialNumber + '0';
            strncat(checkkey,&charserial,1);
        }
    }
    //found
    if(strcmp(checkkey,key) == 0)
    {
        ret_val = ptr;
        *index = i;
    }
    //not found and the current node is a leaf -- not in the Tree -- return null
    else if(ptr->isleaf == TRUE)
    {
        ret_val = NULL;
    }
    //not a leaf -- search recusively in the children with ith index
    else
    {
        ret_val = SearchInBlockTree(ptr->children[i],key,index);
    }
    return ret_val;
}

//searches for the blocktype and serial number in the blocktree and change its accommodation status to 0
void deallocateBlock(Tree_Node* ptr,int index,block_Node* blockRoot)
{
    int ind;
    block_Node* bptr = NULL;
    char searchkey[9];
    char ten[3] = {'1','0','\0'};
    char charserial;

    strcpy(searchkey,ptr->records[index].accommodation_type);
    if(ptr->records[index].blockind == 10)
    {
        strcat(searchkey,ten);
    }
    else
    {
        charserial = ptr->records[index].blockind + '0';
        strncat(searchkey,&charserial,1);
    }

    bptr = SearchInBlockTree(blockRoot,searchkey,&ind);

    if(bptr != NULL)
    {
        bptr->records[ind].accommodated = 0;
    }
}
