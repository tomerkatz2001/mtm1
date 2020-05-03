#include "map.h"
#include <stdlib.h>
#include <assert.h>
#include<string.h>
struct Map_t{
    char** keys;
    char** values;
    int length;
    int last; //where can you write the next data
    int iterator;
};
/* this function allocates place in the memory for the wanted input 
and after that copys the input data into the new place that was allocated*/
static char* makePlaceAndCopy(const char* input)
{
    assert(input!=NULL);
    int input_size=strlen(input);
    char* copied_input_addres=malloc(input_size+1);
    if(copied_input_addres==NULL){
        return NULL; 
    }    
    memcpy(copied_input_addres,input,input_size+1);
    return copied_input_addres;
}
/*this function find a given key is the map and returns its index in the array
NOTE! YOU NEED TO GIVE THIS FUNCTION ONLY KEYS THAT YOU KNOW THAT ARE IN THE MAP! */
static int findKeyIndex(Map map,const char* key)
{
    assert(mapContains(map,key));
    for (int i=0;i<=map->last;i++)
    {
        if(map->keys[i]==NULL)
        {
            continue;
        }
        if(strcmp(map->keys[i],key)==0)
        {
            return i;
        }
    }
    assert(0);//not suppose to get in here.
    return 0;
}

Map mapCreate()
{
    Map map= malloc(sizeof(*map));
    if(map==NULL)
    {
        return NULL;
    }
    map->length=LENGTH;
    map->keys= malloc(sizeof(char*)*LENGTH);
    if(map->keys==NULL)
    {
		free(map);
        return NULL;
    }
    map->values=malloc(sizeof(char*)*LENGTH);
    if(map->values==NULL)
    {
        mapDestroy(map);
        return NULL;
    }
    for(int i=0;i<map->length;i++)
    {
        map->keys[i]=NULL;
        map->values[i]=NULL;
    }
    map->last=0;
    map->iterator=0;
    return map;
}


void mapDestroy(Map map)
{
    
    if(map==NULL){
    return;
    }
    for(int i=0;i<=map->last;i++)
    {
        free(map->keys[i]);
        free(map->values[i]);
    }
    free(map->keys);
    free(map->values);
    free(map);
    

}


Map mapCopy(Map map)
{
    Map copy= malloc(sizeof(Map));
    copy->keys=malloc(sizeof(char*)*map->length);
    copy->values=malloc(sizeof(char*)*map->length);
    for(int i=0; i<=map->last;i++){
        copy->keys[i]=malloc(sizeof(map->keys[i]));
        copy->values[i]=malloc(sizeof(map->values[i]));
        strcpy(copy->keys[i],map->keys[i]);
        strcpy(copy->values[i],map->values[i]);
    }
    copy->length=map->length;
    copy->last=map->last;
    copy->iterator=map->iterator;
    return map;
}


int mapGetSize(Map map)
{
    if (map==NULL){
        return -1;
    }else{ 
        return map->last;
    }      
}


bool mapContains(Map map, const char* key)
{
    if(map==NULL||key==NULL)
    {
        return false;
    }
    for(int i=0; i<=map->last;i++){
        if(map->keys[i]==NULL)
        {
            continue;
        }
        
        if(strcmp(map->keys[i],key)==0){
            return true;
        }
    }
    return false;
}


MapResult mapPut(Map map, const char* key, const char* data)
{
    if(map==NULL||key==NULL||data==NULL){
        return MAP_NULL_ARGUMENT;
    }
    if (!mapContains(map,key))
    {
        if(map->last>=map->length)// if the array is full. this adds more cells to the array
        {
            char** newkeys;
            newkeys=realloc(map->keys,sizeof(char*)*(map->length)*2);
            if(newkeys==NULL){
                return MAP_OUT_OF_MEMORY;
            }
            map->keys=newkeys;
            char** newvalues;
            newvalues=realloc(map->values,sizeof(char*)*(map->length)*2);
            if(newvalues==NULL){
                return MAP_OUT_OF_MEMORY;
            }
            map->values=newvalues;
            map->length=(map->length)*2;//updateing the current array size    
        }
        char* coppied_key=makePlaceAndCopy(key);
        if (coppied_key==NULL){
            return MAP_OUT_OF_MEMORY;
        }    
        map->keys[map->last]=coppied_key;
        char* coppied_value= makePlaceAndCopy(data);
        if(coppied_value==NULL){
            return MAP_OUT_OF_MEMORY;
        }    
        map->values[map->last++]=coppied_value;
    } 
    else
    {
        int need_to_replace=findKeyIndex(map,key);
        free(map->values[need_to_replace]); //free the old data 
        char* coppied_value=makePlaceAndCopy(data);
        if(coppied_value==NULL){
            return MAP_OUT_OF_MEMORY;
        }    
        map->values[need_to_replace]=coppied_value;//place new data
    }
    return MAP_SUCCESS;
}


char* mapGet(Map map, const char* key){
    if(key==NULL){
        return NULL;
    }
    int index=-1;
    for(int i=0; i<map->last;i++){
        if(strcmp(map->keys[i],key)==0){
            index=i;
        }
    }
    if(index<0){
        return NULL;
    }else{
        return map->values[index];
    }
}

MapResult mapRemove(Map map, const char* key)
{
    if(map==NULL||key==NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    if(!mapContains(map,key))
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    int index=findKeyIndex(map,key);
    free(map->values[index]);
    free(map->keys[index]);
    for(int i=index+1;i<=map->last;i++)
    {
        map->keys[i-1]=map->keys[i];
        map->values[i-1]=map->values[i];
    }
    free(map->keys[map->last-1]);
    free(map->values[map->last-1]);
    map->last--;
    return MAP_SUCCESS;
}

char* mapGetFirst(Map map){
    if(map==NULL){
        return NULL;
    }
    if(map->last==0){
        return NULL;
    }
     map->iterator=0;
     return map->values[0];
}
char* mapGetNext(Map map)
{
    if(map==NULL)
    {
        return NULL;
    }
    if(map->iterator>=map->last||map->iterator<0)
    {
        return NULL;
    }
    map->iterator++;
    return map->keys[map->iterator];

}
 
MapResult mapClear(Map map){
    if(map==NULL){
        return MAP_NULL_ARGUMENT;
    }
    
    for(int i=0; i<=map->last;i++){
        free(map->keys[i]);
        free(map->values[i]);
    }
    map->keys=realloc(map->keys,sizeof(char*)*LENGTH);
    map->values=realloc(map->values,sizeof(char*)*LENGTH);
    map->length=LENGTH;
    map->iterator=0;
    map->last=0;
    return MAP_SUCCESS;
}


