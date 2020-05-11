#include "election.h"
#include <assert.h>
#include<stdio.h>
#include <stdlib.h>
#define LENGTH 100
#define MAX_SIZE_OF_INT 10

#define CHECKMALLOC(pointer)\
\
 if(pointer==NULL)\
 {\
 return NULL;\
 }while(0)

struct election_t{
Map areas;
Map tribes;
Map* votes;
int last;
int* area_ids;
int size; 
};

/*
checkForbiddenKeys:

this function returns true if the word has forbidden keys (not lowercase letters nor spaces)
and returns false is the word is fine (has only lowercase letters or spaces)
*/
static bool checkForbiddenKeys(const char* word)
{
    assert(word!=NULL);
    int length=strlen(word);
    for (int i=0;i<length;i++)
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
/*
** this function adds a new, embty, tribe slots in the voting board.

** if the area coulm that was given has already tribes slots(it is not new):
        the function adds a new score slot for the given tribe id and puts '0' for how much votes.

**if the given area has no tribes slotes at all(it is a new area):
        the function creates slotes for all the existing tribes.

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
    Election election=malloc(sizeof(Election));

    CHECKMALLOC(election);

    election->areas =mapCreate();
    CHECKMALLOC(election->areas);

    election->tribes=mapCreate();
    CHECKMALLOC(election->tribes);

    election->votes=malloc(sizeof(*election->votes)*LENGTH);
    CHECKMALLOC(election->votes);
    for(int i=0 ; i< LENGTH ; i++)
    {
        election->votes[i]=mapCreate();
        CHECKMALLOC(election->votes[i]);
    }

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
    for(int i=0;i<election->last;i++)
    {
         if(addEmptyScore(election,election->votes[i],id_string)==MAP_OUT_OF_MEMORY)
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
    
    for(int i=0;i<mapGetSize(election->areas);i++)
    {
        mapRemove(election->votes[i],id_string);
    }
    mapRemove(election->tribes,id_string);
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
Map electionComputeAreasToTribesMapping (Election election)
{
    if(election==NULL)
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







int main()
{
   Election election= electionCreate();
   assert (election!=NULL);
   printf("ok\n");
   assert(electionAddTribe(election,55,"tomer")==ELECTION_SUCCESS);
   assert(electionAddTribe(election,55,"katz")==ELECTION_TRIBE_ALREADY_EXIST);
   assert(electionAddTribe(election,76,"toM")==ELECTION_INVALID_NAME);
   assert(electionAddTribe(election,-55,"tomer")==ELECTION_INVALID_ID);
printf("ok2\n");
char name[10]="tomer";
char* check=electionGetTribeName(election,55);
    assert(!strcmp(name,check));
   printf("ok3\n");
   electionAddTribe(election,57,"moshe");
   assert(electionRemoveTribe(election,59)==ELECTION_TRIBE_NOT_EXIST);
   
   printf("ok4\n");
}
