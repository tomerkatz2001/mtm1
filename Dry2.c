#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

char* duplicateString(char* str,int times){
	assert(!str);
	assert(times>0);

	int len=strlen(str);
	char* out=malloc(len*times+1);

	if(out==NULL){
	    return NULL;
	}

	for(int i=0;i<times;i++){
		strcpy(out,str);
		out=out+len;
	}

	return out-(len*times);
}