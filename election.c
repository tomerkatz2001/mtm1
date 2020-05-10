#include "election.h"
#include <assert.h>
#include<stdio.h>
#define LENGTH 100
#define MAX_SIZE_OF_INT 10

#define CHECKMALLOC(pointer)\
\
 if(pointer==NULL)\
 {\
 return NULL;\
 }while(0)

struct election_t{
Map ereas;
Map tribes;
Map* votes;   //MAKE SURE THIS IS IN THE SAME SIZE AS EREAS
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

Election electionCreate()
{
    Election election=malloc(sizeof(Election));

    CHECKMALLOC(election);

    election->ereas =mapCreate();
    CHECKMALLOC(election->ereas);

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
    int map_size;
    if(mapGetSize(election->ereas)>LENGTH)
    {
        map_size=mapGetSize(election->ereas);
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
    for(int i=0;i<mapGetSize(election->ereas);i++)
    {
        mapRemove(election->votes[i],id_string);
    }
    mapRemove(election->tribes,id_string);
    return ELECTION_SUCCESS;

}
