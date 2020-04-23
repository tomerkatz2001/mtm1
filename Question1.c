typedef struct node_t {
int x;
struct node_t *next;
} *Node;

typedef enum {
SUCCESS=0,
MEMORY_ERROR,
EMPTY_LIST,
UNSORTED_LIST,
NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out){

    if(list1==NULL){
        return EMPTY_LIST;
    }

    if(list2==NULL){
        return EMPTY_LIST;
        }

    if(!isListSorted(list1)){
        return UNSORTED_LIST;
    }
    if(!isListSorted(list2)){
        return UNSORTED_LIST;
        }
    Node current=list1;
    while(current!=NULL){
        if(addToSortedList(&merged_out,current)==MEMORY_ERROR){
            destroyList(&merged_out);
            merged_out=NULL;
            return MEMORY_ERROR;
            }
        current=current->next;
    } 
    current=list2;
    while(current!=NULL){
        if(addToSortedList(&merged_out,current)==MEMORY_ERROR){
            destroyList(&merged_out);
            merged_out=NULL;
            return MEMORY_ERROR;
            }
        current=current->next;
    }

}

void destroyList(Node list){
    Node current=list;
    Node temp;
    while(current!=NULL){
        temp=current->next;
        free(current);
        current=temp;
    }
}

ErrorCode addToSortedList(Node list, Node toAdd){
        Node current=list;
        Node newNode,temp;
        while(toAdd->x < current->next->x){
            current=current->next;
            if(current==NULL){
                break;
            }
        }
        newNode=malloc(sizeof(Node));
        if(newNode==NULL){return MEMORY_ERROR;}
        newNode->x=toAdd->x;
        newNode->next=toAdd->next;
        temp=current->next;
        current->next=newNode;
        newNode->next=temp;
        return SUCCESS
}