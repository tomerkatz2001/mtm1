#include "map.h";


//make sure that the key is found in the map!!!
static int findKeyIndex(Map map,char* key)
{
    assert(mapContains(map,key));
    for (int i=0;i<=map->last;i++)
    {
        if(strcmp(map->keys[i],*key))
        {
            return i;
        }
    }
    assert(0);//not suppose to get in here.
}
MapResult mapRemove(Map map, const char* key)
{
    if(map==NULL)
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
    map->last--;
   
}