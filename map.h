#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>
#include <string.h>
/**
* Map Container
*
* Implements a map container type.
* The type of the key and the value is string (char *)
* The map has an internal iterator for external use. For all functions
* where the state of the iterator after calling that function is not stated,
* it is undefined. That is you cannot assume anything about it.
*
* The following functions are available:
*   mapCreate		- Creates a new empty map
*   mapDestroy		- Deletes an existing map and frees all resources
*   mapCopy		- Copies an existing map
*   mapGetSize		- Returns the size of a given map
*   mapContains	- returns weather or not a key exists inside the map.
*   mapPut		    - Gives a specific key a given value.
*   				  If the key exists, the value is overridden.
*   mapGet  	    - Returns the data paired to a key which matches the given key.
*					  Iterator status unchanged
*   mapRemove		- Removes a pair of (key,data) elements for which the key
*                    matches a given element (using the strcmp function).
*   mapGetFirst	- Sets the internal iterator to the first key in the
*   				  map, and returns it.
*   mapGetNext		- Advances the internal iterator to the next key and
*   				  returns it.
*	 mapClear		- Clears the contents of the map. Frees all the elements of
*	 				  the map using the free function.
* 	 MAP_FOREACH	- A macro for iterating over the map's elements.
*/

/** Type for defining the map */
typedef struct Map_t* Map;

/** Type used for returning error codes from map functions */
typedef enum MapResult_t {
    MAP_SUCCESS,
    MAP_OUT_OF_MEMORY,
    MAP_NULL_ARGUMENT,
    MAP_ITEM_ALREADY_EXISTS,
    MAP_ITEM_DOES_NOT_EXIST,
    MAP_ERROR
} MapResult;

/**
* mapCreate: Allocates a new empty map.
*
* @return
* 	NULL - if allocations failed.
* 	A new Map in case of success.
*/
Map mapCreate();

/**
* mapDestroy: Deallocates an existing map. Clears all elements.
*
* @param map - Target map to be deallocated. If map is NULL nothing will be
* 		done
*/
void mapDestroy(Map map);

/**
* mapCopy: Creates a copy of target map.
* Iterator values for both maps is undefined after this operation.
*
* @param map - Target map.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Map containing the same elements as map otherwise.
*/
Map mapCopy(Map map);

/**
* mapGetSize: Returns the number of elements in a map
* @param map - The map which size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the map.
*/
int mapGetSize(Map map);

/**
* mapContains: Checks if a key element exists in the map. The key element will be
* considered in the map if one of the key elements in the map it determined equal
* using the strcmp function from string.h.
*
* @param map - The map to search in
* @param key - The key to look for.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true - if the key element was found in the map.
*/
bool mapContains(Map map, const char* key);

/**
*	mapPut: Gives a specified key a specific value.
*  Iterator's value is undefined after this operation.
*
* @param map - The map for which to assign/reassign the data element
* @param keyElement - The key element which need to be assigned/reassigned.
*       A copy of the key element will be inserted.
* @param dataElement - The new data element to associate with the given key.
*      A copy of the data element will be inserted and old data memory would be deleted.
* @return
* 	MAP_NULL_ARGUMENT if one of the params is NULL
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	MAP_SUCCESS the paired elements had been inserted successfully
*/
MapResult mapPut(Map map, const char* key, const char* data);

/**
*	mapGet: Returns the data associated with a specific key in the map(not a copy).
*			Iterator status unchanged
*
* @param map - The map for which to get the data element from.
* @param keyElement - The key element which need to be found and who's data
we want to get.
* @return
*  NULL if a NULL pointer was sent or if the map does not contain the requested key.
* 	A pointer to the data element associated with the key otherwise.
*/
char* mapGet(Map map, const char* key);

/**
* 	mapRemove: Removes a pair of key and data elements from the map. The elements
*  are found using the comparison function strcmp. Once found,
*  the elements are removed and deallocated.
*  Iterator's value is undefined after this operation.
*
* @param map -
* 	The map to remove the elements from.
* @param keyElement
* 	The key element to find and remove from the map. The key element will be freed.
* 	The data element associated with this key will also be freed.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent to the function
*  MAP_ITEM_DOES_NOT_EXIST if an equal key item does not already exists in the map
* 	MAP_SUCCESS the paired elements had been removed successfully
*/
MapResult mapRemove(Map map, const char* key);

/**
*	mapGetFirst: Sets the internal iterator (also called current key element) to
*	the first key element in the map. There doesn't need to be an internal order
*  of the keys so the "first" key element is any key element.
*	Use this to start iterating over the map.
*	To continue iteration use mapGetNext
*
* @param map - The map for which to set the iterator and return the first
* 		key element.
* @return
* 	NULL if a NULL pointer was sent or the map is empty.
* 	The first key element of the map otherwise
*/
char* mapGetFirst(Map map);

/**
*	mapGetNext: Advances the map iterator to the next key element and returns it.
*	The next key element is any key element not previously returned by the iterator.
* @param map - The map for which to advance the iterator
* @return
* 	NULL if reached the end of the map, or the iterator is at an invalid state
* 	or a NULL sent as argument
* 	The next key element on the map in case of success
*/
char* mapGetNext(Map map);


/**
* mapClear: Removes all key and data elements from target map.
* The elements are deallocated.
* @param map
* 	Target map to remove all element from.
* @return
* 	MAP_NULL_ARGUMENT - if a NULL pointer was sent.
* 	MAP_SUCCESS - Otherwise.
*/
MapResult mapClear(Map map);

/*!
* Macro for iterating over a map.
* Declares a new iterator for the loop.
*/
#define MAP_FOREACH(iterator, map) \
    for(char* iterator = (char*) mapGetFirst(map) ; \
        iterator ;\
        iterator = mapGetNext(map))

#endif /* MAP_H_ */
