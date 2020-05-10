
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#define LENGTH 100
#define MAX_SIZE_OF_INT 10

#define CHECKMALLOC(pointer)\
\
 if(pointer==NULL)\
 {\
 return NULL;\
 }while(0)

struct election_t{
Map areas;      //A map that contains all of the area IDs(key) and Names(value).
Map tribes;     //A map that contains all of the tribe IDs(key) and Names(value).
Map* votes;     //An array that contains a voting map for each area, a voting map contains the tribe ids(key) and how many votes they got(value).
int last;       //A counter that counts the amount of areas
int* area_ids;  //An array that contains the ids for each area, the array works in a way that in each index there will be the id of the array that his voting map is in the same index in the maps array.
int size;       //The size allocated for the voting maps array and ids array.
};

/**
*   toString: This function converts an integer into a string.
*   @param num- the integer to convert.
*   @return 
*       The converted string.
*/
char* toString(int num)
{
    char str[MAX_SIZE_OF_INT+1];
    sprintf(str,"%d",area_id);
    return str;
}

/**
*   checkForbiddenKeys: This function returns true if the word has forbidden keys (not lowercase letters nor spaces)
*   and returns false is the word is fine (has only lowercase letters or spaces)
*   @param word- The word that we need to check
*   @return
*       A boolean that states wether the word is valid or invalid(true=invalid, false=valid) 
*/
static bool checkForbiddenKeys(const char* word)
{
    assert(word!=NULL);
    int length=strlen(word);
    for (int i=0;i<length;i++)//Iterate through the word's charachters and check if they are ilegal.
    {
        if(word[i]<'a'||word[i]>'z')
        {
            if(word[i]!=' ')
            {
            return true;
            }
        }
    }
    return false;

}

/**
 *  electionDestroy: Destroys the election database, and frees all of its allocated memory
 *  @param election- The election to destroy.
 */
void electionDestroy(Election election)
{
    mapDestroy(election->tribes);
    mapDestroy(election->areas);
    for (i=0;i<mapGetSize(election->areas);i++)
    {
        mapDestroy(election->votes[i]);
    }
    free(election->votes);
}

/** 
 *  electionAddArea: Add an area to the election database.
 *  @param election- The election database that the area is added to.
 *  @param area_id- The ID of the area that we want to add.
 *  @param area_name- The name of the area that we want to add.
 *  @return
 *      The result of the action, as specified in the excercise.
 */
ElectionResult electionAddArea(Election election, int area_id, const char* area_name)
{
    /* Checking the validity of the parameters. */
    if(election==NULL || area_id==NULL || area_name==NULL)//Check if any of the params is NULL.
    { 
        return ELECTION_NULL_ARGUMENT;
    }

    if(area_id<0)//Check if the area id is invalid
    {
        return ELECTION_INVALID_ID;
    }

    for(i=0;i<mapGetSize(election->areas);i++)//Iterate through all of the areas to check if the area already exists
    {
        if(id_string==election->areas->keys[i])
        {
            return ELECTION_AREA_ALREADY_EXIST;
        }
    }

    if(checkForbiddenKeys(area_name))//Check if the area name is invalid
    {
        return ELECTION_INVALID_NAME;
    }

    /* Add the area to the database. */
    if(mapPut(election->areas, toString(area_id) , area_name)==MAP_OUT_OF_MEMORY)//Add the area to the areas map.
    {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    if(election->last>=election->size)//Check if the Areas array is full, if it is full then allocate more memory.
    {
        if(realloc(election->area_ids,sizeof(int)*(election->size+LENGTH)==NULL || realloc(election->votes,sizeof(Map)*(election->size+LENGTH)==NULL)){
            electionDestroy(election);
            return ELECTION_OUT_OF_MEMORY;
        }
        
    }
    election->area_ids[last]=area_id;//Add the area id to the ids array.
    election->votes[last]=mapCreate();//Add the area to the votes array.
    for(i=0;i<mapGetSize(election->tribes);i++){//Add all of the tribes to the areas voting map with 0 votes.
        if(mapPut(election->votes[last],election->tribes->keys[i],"0") == MAP_OUT_OF_MEMORY){
            electionDestroy(election);
            return ELECTION_OUT_OF_MEMORY;
        }
    }
    return ELECTION_SUCCESS;
     
}

/** 
 *  electionSetTribeName: Change the name of a tribe in the election database.
 *  @param election- The election database to edit.
 *  @param tribe_id- The id of the tribe that we want to edit.
 *  @param tribe_name- The name that the tribe will be changed to.
 *  @return
 *      The result of the action, as specified in the excercise.
 */
ElectionResult electionSetTribeName (Election election, int tribe_id, const char* tribe_name)
{
    /* Checking the validity of the parameters */
    if(election==NULL || tribe_id==NULL || tribe_name==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }

    if(tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }

    bool tribe_exists=false;//Check if hte tribe exists in the database.
    for( i=0 ; i<mapGetSize(election->tribes) ; i++ )
    {
        if(id_string==election->tribes->keys[i])
        {
            tribe_exists=true;
        }
    }
    if(!tribe_exists)
    {
        return ELECTION_TRIBE_NOT_EXIST;
    }

    if(checkForbiddenKeys(tribe_name))
    {
        return ELECTION_INVALID_NAME;
    }

    /* Change the tribe name in the tribes map inside the elections database */
    if(mapPut(election->tribes,toString(tribe_id),tribe_name)==MAP_OUT_OF_MEMORY)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
}

/** 
 *  electionRemoveAreas: remove a set of areas from the election database.
 *  @param election- The election database to edit.
 *  @param should_delete_area- A pointer to the function that dictates wether an area should be removed.
 *  @return
 *      The result of the action, as specified in the excercise.
 */
ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area)
{
    /* Checking the validity of the parameters */
    if(election==NULL || should_delete_area==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }

    /* Remove the areas from the election database. */
    for(i=0;i<election->last;i++)//Iterate through all of the areas.
    {
        if(should_delete_area(election->area_ids[i])//Use the parameter function to check for each area if it should be removed
        {
            mapRemove(election->areas, election->areas->keys[i]);//Remove the area from the areas map.
            free(election->votes[i]);//Free the memory allocated for the area voting map
            for(j=i;j<election->last-1;j++)//For every area with an index higher then the one removed, in the voting maps array and area ids array, move them to the index below them.
            {
                election->votes[j]=election->votes[j+1];
                election->area_ids[j]=election->area_ids[j+1];
            }
            election->votes[last-1]=NULL;//Set the end of the arrays
            election->area_ids[last-1]=0;
            election->last-=1;//Decrease the last item counter. 
        }
    }
    return ELECTION_SUCCESS; 
} 

/** 
 *  electionRemoveVote: Remove a certain number of votes from a certain area to a certain tribe.
 *  @param election- The election database to edit.
 *  @param area_id- The id of the area that the votes come from. 
 *  @param tribe_id- The id of the tribe that was voted to.
 *  @param num_of_votes- The amount of votes.
 *  @return
 *      The result of the action, as specified in the excercise.
 */
ElectionResult electionRemoveVote (Election election, int area_id, int tribe_id,int num_of_votes)
{
    /* Checking the validity of the parameters */
    if(election==NULL || area_id==NULL || tribe_id==NULL || num_of_votes==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }

    if(area_id<0 || tribe_id<0){//Check if any of the ids is invalid.
        return ELECTION_INVALID_ID;
    }

    if(num_of_votes<=0){//Check if hte number of votes is invalid.
        return ELECTION_INVALID_VOTES;
    }

    /* Check if both the tribe and the area exist in the database */
    bool tribe_exists=false;
    for( i=0 ; i<mapGetSize(election->tribes) ; i++ ){
        if(id_string==election->tribes->keys[i]){
            tribe_exists=true;
        }
    }
    if(!tribe_exists){
        return ELECTION_TRIBE_NOT_EXIST;
    }

    bool area_exists=false;
    for( i=0 ; i<mapGetSize(election->areas) ; i++ ){
        if(id_string==election->areas->keys[i]){
            area_exists=true;
        }
    }
    if(!area_exists){
        return ELECTION_AREA_NOT_EXIST;
    }

    /* Add the votes */
    int area_num;//Find the number of the area in the voting map array using the area ids array.
    for(i=0;i<election->last;i++){
        if(area_id==election->area_ids){
            area_num=i;
        }
    }
    int votes=atoi(mapGet(election->votes[i],toString(tribe_id)));//Retrieve the current number of votes from the voting map.
    votes+=num_of_votes;//Add the votes.
    if(mapPut(election->votes[area_num],toString(tribe_id),toString(votes))==MAP_OUT_OF_MEMORY)//Insert the updated number of votes to the voting map.
    {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
} 



Election electionCreate()
{
    Election election=malloc(sizeof(Election));

    CHECKMALLOC(election);

    election->areas =mapCreate();
    CHECKMALLOC(election->areas);

    election->tribes=mapCreate();
    CHECKMALLOC(election->tribes);

    election->votes=malloc(sizeof(Map)*LENGTH);
    CHECKMALLOC(election->votes);
    for(int i=0 ; i< LENGTH ; i++)
    {
        election->votes[i]=mapCreate();
        CHECKMALLOC(election->votes[i]);
    }
    election->last=0;
    election->size=LENGTH;
    election->area_ids=malloc(sizeof(int)*LENGTH);
    return election;
}

ElectionResult electionAddTribe(Election election, int tribe_id, const char* tribe_name)
{
    if(election==NULL||tribe_name==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if(tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }
    char id_string[MAX_SIZE_OF_INT+1];
    sprintf(id_string,"%d",tribe_id);
    if(mapContains(election->tribes,id_string))
    {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if(checkForbiddenKeys(tribe_name))
    {
        return ELECTION_INVALID_NAME;
    }
    if(mapPut(election->tribes,id_string,tribe_name)!=MAP_SUCCESS)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    int map_size;
    if(mapGetSize(election->areas)>LENGTH)
    {
        map_size=mapGetSize(election->areas);
    }
    else
    {
        map_size=LENGTH;
    }
    
     for(int i=0;i<map_size;i++)
     {
         if(mapPut(election->votes[i],id_string,"0")==MAP_OUT_OF_MEMORY)
         {
             return ELECTION_OUT_OF_MEMORY;
         }
     }

    return ELECTION_SUCCESS;
}

char* electionGetTribeName (Election election, int tribe_id)
{
    if(election==NULL)
    {
        return NULL;
    }
    char id_string[MAX_SIZE_OF_INT+1];
    sprintf(id_string,"%d",tribe_id);
    char* name_pointer=mapGet(election->tribes,id_string);
    if (name_pointer==NULL)
    {
        return NULL;
    }
    char* copy_name_pointer=malloc(strlen(name_pointer)+1);
    CHECKMALLOC(copy_name_pointer);
    strcpy(copy_name_pointer, name_pointer);
    return copy_name_pointer;
}
ElectionResult electionRemoveTribe (Election election, int tribe_id)
{
    if(election==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if(tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }
    char id_string[MAX_SIZE_OF_INT+1];
    sprintf(id_string,"%d",tribe_id);
    if(!mapContains(election->tribes,id_string))
    {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    
    /*int tribe_index=0;
    if(strcmp(mapGetFirst(election->tribes),id_string)!=0)
    {
        for(tribe_index=1;tribe_index<mapGetSize(election->tribes);tribe_index++)
        {
            if(strcmp(mapGetNext(election->tribes),id_string)==0)
            {
                break;
            }
        }
    }*/
    for(int i=0;i<mapGetSize(election->areas);i++)
    {
        mapRemove(election->votes[i],id_string);
    }
    mapRemove(election->tribes,id_string);
    return ELECTION_SUCCESS;

}

