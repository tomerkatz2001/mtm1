
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "election.h"

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
* this function adds a new, embty, tribe slots in the voting board.
* if the area coulm that was given has already tribes slots(it is not new):
*       the function adds a new score slot for the given tribe id and puts '0' for how much votes.
*if the given area has no tribes slotes at all(it is a new area):
*       the function creates slotes for all the existing tribes.
*/
static MapResult addEmptyScore(Election election, Map area, const char* tribe_id)
{
    assert(election!=NULL);
    assert(area!=NULL);
    assert(tribe_id!=NULL);
    if(mapGetSize(area)==0)
    {
        if(mapPut(area,mapGetFirst(election->tribes),"0")==MAP_OUT_OF_MEMORY)
        {
            return MAP_OUT_OF_MEMORY;
        }
        for(int i=1;i<mapGetSize(election->tribes);i++)
        {
            if(mapPut(area,mapGetNext(election->tribes),"0")==MAP_OUT_OF_MEMORY)
            {
                return MAP_OUT_OF_MEMORY;
            }
        }
    }
    else
    {
        if(mapPut(area,tribe_id,"0")==MAP_OUT_OF_MEMORY)
        {
            return MAP_OUT_OF_MEMORY;
        }
    }
    return MAP_SUCCESS;
}

Election electionCreate()
{
    Election election=(Election)malloc(sizeof(Election));
    CHECKMALLOC(election);

    election->areas =mapCreate();
    CHECKMALLOC(election->areas);

    election->tribes=mapCreate();
    CHECKMALLOC(election->tribes);

    election->votes=(Map*)malloc(sizeof(*election->votes)*LENGTH);
    CHECKMALLOC(election->votes);
    for(int i=0 ; i< LENGTH ; i++)
    {
        election->votes[i]=mapCreate();
        CHECKMALLOC(election->votes[i]);
    }
    election->area_ids=malloc(sizeof(*election->area_ids)*LENGTH);
    CHECKMALLOC(election->area_ids);
    election->last=0;
    election->size=LENGTH;

    return election;
}

/**
 *  electionDestroy: Destroys the election database, and frees all of its allocated memory
 *  @param election- The election to destroy.
 */
void electionDestroy(Election election)
{
    mapDestroy(election->tribes);
    mapDestroy(election->areas);
    for (int i=0;i<mapGetSize(election->areas);i++)
    {
        mapDestroy(election->votes[i]);
    }
    free(election->votes);
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
    for(int i=0;i<election->last;i++)
    {
         if(addEmptyScore(election,election->votes[i],id_string)==MAP_OUT_OF_MEMORY)
         {
             return ELECTION_OUT_OF_MEMORY;
         }
    }

    return ELECTION_SUCCESS;
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
    if(election==NULL || area_name==NULL)//Check if any of the params is NULL.
    { 
        return ELECTION_NULL_ARGUMENT;
    }

    if(area_id<0)//Check if the area id is invalid
    {
        return ELECTION_INVALID_ID;
    }

    for(int i=0;i<mapGetSize(election->areas);i++)//Iterate through all of the areas to check if the area already exists
    {
        if(area_id == election->area_ids[i])
        {
            return ELECTION_AREA_ALREADY_EXIST;
        }
    }

    if(checkForbiddenKeys(area_name))//Check if the area name is invalid
    {
        return ELECTION_INVALID_NAME;
    }

    /* Add the area to the database. */
    char to_str[MAX_SIZE_OF_INT+1];
    sprintf(to_str,"%d",area_id);
    if(mapPut(election->areas, to_str , area_name)==MAP_OUT_OF_MEMORY)//Add the area to the areas map.
    {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    if(election->last>=election->size)//Check if the Areas array is full, if it is full then allocate more memory.
    {
        if( realloc(election->area_ids,sizeof(int)*(election->size+LENGTH)) == NULL || realloc(election->votes,sizeof(Map)*(election->size+LENGTH))==NULL){
            electionDestroy(election);
            return ELECTION_OUT_OF_MEMORY;
        }
    }

    election->area_ids[election->last]=area_id;//Add the area id to the ids array.
    election->votes[election->last]=mapCreate();//Add the area to the votes array.
    if(election->votes[election->last]==NULL)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    
    char* tribe_id=mapGetFirst(election->tribes);
    while(tribe_id!=NULL){//Add all of the tribes to the areas voting map with 0 votes.
        if(mapPut(election->votes[election->last],tribe_id,"0") == MAP_OUT_OF_MEMORY){
            electionDestroy(election);
            return ELECTION_OUT_OF_MEMORY;
        }
        tribe_id=mapGetNext(election->tribes);
    }
    election->last++;
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
    char* copy_name_pointer=(char*)malloc(strlen(name_pointer)+1);
    CHECKMALLOC(copy_name_pointer);
    strcpy(copy_name_pointer, name_pointer);
    return copy_name_pointer;
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
    if(election==NULL || tribe_name==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }

    if(tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }

    char id_string[MAX_SIZE_OF_INT+1];
    sprintf(id_string,"%d",tribe_id);
    if(!mapContains(election->tribes,id_string))//Check if the tribe exists in the database.
    {
        return ELECTION_TRIBE_NOT_EXIST;
    }

    if(checkForbiddenKeys(tribe_name))
    {
        return ELECTION_INVALID_NAME;
    }

    /* Change the tribe name in the tribes map inside the elections database */
    char to_str[MAX_SIZE_OF_INT+1];
    sprintf(to_str,"%d",tribe_id);
    if(mapPut(election->tribes,to_str,tribe_name)==MAP_OUT_OF_MEMORY)
    {
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
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
    
    for(int i=0;i<mapGetSize(election->areas);i++)
    {
        mapRemove(election->votes[i],id_string);
    }
    mapRemove(election->tribes,id_string);
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
    for(int i=0;i<election->last;i++)//Iterate through all of the areas.
    {
        if(should_delete_area(election->area_ids[i]))//Use the parameter function to check for each area if it should be removed
        {
            char to_str[MAX_SIZE_OF_INT+1];
            sprintf(to_str,"%d",election->area_ids[i]);
            mapRemove(election->areas, to_str);//Remove the area from the areas map.
            free(election->votes[i]);//Free the memory allocated for the area voting map
            for(int j=i;j<election->last-1;j++)//For every area with an index higher then the one removed, in the voting maps array and area ids array, move them to the index below them.
            {
                election->votes[j]=election->votes[j+1];
                election->area_ids[j]=election->area_ids[j+1];
            }
            election->votes[election->last-1]=NULL;//Set the end of the arrays
            election->area_ids[election->last-1]=0;
            election->last-=1;//Decrease the last item counter. 
        }
    }
    return ELECTION_SUCCESS; 
} 

ElectionResult electionAddVote (Election election, int area_id,int tribe_id, int num_of_votes)
{
    if(election==NULL)
    {
        return ELECTION_NULL_ARGUMENT;
    }
    if(area_id<0||tribe_id<0)
    {
        return ELECTION_INVALID_ID;
    }
    if(num_of_votes<=0)
    {
        return ELECTION_INVALID_VOTES;
    }
    char area_id_string[MAX_SIZE_OF_INT+1];
    sprintf(area_id_string,"%d",tribe_id);
    if(!mapContains(election->areas,area_id_string))
    {
        return ELECTION_AREA_NOT_EXIST;
    }
    char tribe_id_string[MAX_SIZE_OF_INT+1];
    sprintf(tribe_id_string,"%d",tribe_id);
    if(!mapContains(election->tribes,tribe_id_string))
    {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    int area_index=0;
    if(strcmp(mapGetFirst(election->areas),area_id_string)!=0)
    {
        for(area_index=1;area_index<mapGetSize(election->tribes);area_index++)
        {
            if(strcmp(mapGetNext(election->areas),area_id_string)==0)
            {
                break;
            }
        }
    }
    
    int new_votes=num_of_votes+atoi(mapGet(election->votes[area_index],tribe_id_string));
    char new_votes_string[MAX_SIZE_OF_INT+1];
    sprintf(new_votes_string,"%d",new_votes);
    if(mapPut(election->votes[area_index],tribe_id_string,new_votes_string)==MAP_OUT_OF_MEMORY)
    {
        return ELECTION_OUT_OF_MEMORY;
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
    if(election==NULL)
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
    char area_id_str[MAX_SIZE_OF_INT+1];
    sprintf(area_id_str,"%d",tribe_id);
    if(!mapContains(election->tribes,area_id_str))//Check if the tribe exists in the database.
    {
        return ELECTION_TRIBE_NOT_EXIST;
    }

    char tribe_id_str[MAX_SIZE_OF_INT+1];
    sprintf(tribe_id_str,"%d",area_id);
    if(!mapContains(election->areas,tribe_id_str))//Check if the tribe exists in the database.
    {
        return ELECTION_AREA_NOT_EXIST;
    }

    /* Add the votes */
    int area_num;//Find the number of the area in the voting map array using the area ids array.
    for(int i=0;i<election->last;i++){
        if( area_id==election->area_ids[i]){
            area_num=i;
        }
    }
    char to_str[MAX_SIZE_OF_INT+1];
    sprintf(to_str,"%d",tribe_id);
    int votes=atoi(mapGet(election->votes[area_num],to_str));//Retrieve the current number of votes from the voting map.
    votes+=num_of_votes;//Add the votes.
    char to_str2[MAX_SIZE_OF_INT+1];
    sprintf(to_str2,"%d",votes);
    if(mapPut(election->votes[area_num],to_str,to_str2)==MAP_OUT_OF_MEMORY)//Insert the updated number of votes to the voting map.
    {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
} 
Map electionComputeAreasToTribesMapping (Election election)
{
    if(election==NULL)
    {
        return NULL;
    }
    if(election->last==0)//if there are no areas in the system
    {
        return NULL;
    }
    Map results=mapCopy(election->areas);
    if(results==NULL)
    {
        return NULL;
    }
   

    for(int i=0;i<election->last;i++)
    {
        char current_area[MAX_SIZE_OF_INT+1];
        sprintf(current_area,"%d",election->area_ids[i]);// this is the area id (string) we are counting votes now.

        char* winning_tribe_id=mapGetFirst(election->votes[i]);//taking the first tribe in the list
        assert(winning_tribe_id!=NULL);
        char* winning_tribe_score =mapGet(election->votes[i],winning_tribe_id);// getting its score (string)
        assert(winning_tribe_id!=NULL);
        int highest_score=atoi(winning_tribe_score);//converting the score into an int;
        for(int j=1;j<mapGetSize(election->votes[i]);j++)//scannig the whole score map of the eara to find biggest score
        {
            char* current_tribe_id=mapGetNext(election->votes[i]);
            assert(current_tribe_id!=NULL);
            char* current_tribe_score=mapGet(election->votes[i],current_tribe_id);
            assert(current_tribe_score!=NULL);
            int current_tribe_score_int=atoi(current_tribe_score);
            if(current_tribe_score_int>highest_score)
            {
                highest_score=current_tribe_score_int;
                winning_tribe_id=current_tribe_id;
            }
            if(current_tribe_score_int==highest_score)
            {
                int current_tribe_id_int=atoi(current_tribe_id);
                int winning_tribe_id_int=atoi(winning_tribe_id);
                if(current_tribe_id_int<winning_tribe_id_int)
                {
                    winning_tribe_id=current_tribe_id;
                    highest_score=current_tribe_score_int;
                }
            }
        }    
        mapPut(results,current_area,winning_tribe_id);
    
    }
return results;
}










