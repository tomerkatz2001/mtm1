#include "map.h"
#include <stdlib.h>
struct Map_t{
    char** keys;
    char** values;
    int length;
    int last;
    int iterator;
}


Map mapCreate()
{
    Map map= malloc(sizeof(Map));
    map->keys= malloc(sizeof(*char)*LENGTH);
    map->values=malloc(sizeof(*char)*LENGTH);
    map->length=LENGTH;
    map->last=0;
    map->iterator=0;
    return map;
}


void mapDestroy(Map map)
{
    if(map==NULL)
    return;

    for(int i=0;i<=map->last;i++)
    {
        free(keys[i]);
        free(values[i]);
    }
    free(map);
}


Map mapCopy(Map map)
{
    Map copy= malloc(sizeof(Map));
    copy->keys=malloc(sizeof(*char)*map->length);
    copy->values=malloc(sizeof(*char)*map->length);
    for(int i=0; i<=map->last;i++){
        copy->keys[i]=malloc(sizeof(map->keys[i]));
        copy->values[i]=malloc(sizeof(map->values[i]));
        strcpy(copy->keys[i],map->keys[i]);
        strcpy(copy->values[i],map->values[i]);
    }
    copy->length=map->length;
    copy->last=map->last;
    copy->iterator=map->iterator;
}


int mapGetSize(Map map)
{
    if (map==NULL)
        return -1;
    
    else 
        return map->last +1;

    
}


bool mapContains(Map map,string key)
{
    for(int i=0; i<=map->last;i++){
        if(strcmp(map->key[i],key)==0){
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
     if (!mapContains(map,*key))
     {
         if(map->last>=map->length)
         {
             map->keys=realloc(map->keys,sizeof(char*)(map->length)*2)
             if(map->keys==NULL)
                return MAP_OUT_OF_MEMORY;
              map->values=realloc(map->values,sizeof(char*)(map->values)*2)
             if(map->values==NULL)
                return MAP_OUT_OF_MEMORY;
         }
         map->keys[++last]=malloc(strlen(key)+1);
         if(map->keys[last]==NULL)
            return MAP_OUT_OF_MEMORY;
        memcpy(map->keys[last],key,strnlen(key)+1)
        map->values[last]= malloc(strlen(data)+1) 
        if(map->values[last]==NULL)
            return MAP_OUT_OF_MEMORY;
        memcpy(map->values[last],data,strnlen(data)+1); 
     } 
     else
     {
         for(int i=0;i<=map->last;i++)
         {
             if(strcmp(map->key[i],key)==0)
             {
                 free(map->values[i]);
                 map->values[i]=malloc(strlen(data)+1);
                 
             }
             
         }
    
     }    
    
}


char* mapGet(char* key,Map map){
    if(key==NULL){
        return NULL;
    }
    int index=-1;
    for(int i=0; i<=map->last;i++){
        if(strcmp(map->key[i],key)==0){
            index=i;
        }
    }
    if(index<0){
        return NULL;
    }else{
        return map->values[index];
    }
}

char* mapGetFirst(Map map){
    if(map==NULL){
        return NULL;
    }
    if(map->keys[0]==NULL){
        return NULL;
    }
     map->iterator=0;
     return map->values[0];
}
 
MapResult mapClear(Map map){
    if(map==NULL){
        return MAP_NULL_ARGUMENT;
    }
    
    for(int i=0; i<=map->last;i++){
        free(map->keys[i]);
        free(map->value[i]);
    }
    map->keys=realloc(map->keys,sizeof(*char)*LENGTH);
    map->values=realloc(map->values,sizeof(*char)*LENGTH);
    map->length=LENGTH;
    map->iterator=0;
    map->last=0;
    return MAP_SUCCESS;
}

