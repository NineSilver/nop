#ifndef __BSON_H__
#define __BSON_H__

void *bson_find_entry(void *doc, const char *name);

void *bson_find(void *doc, const char *name);
int   bson_type(void *doc, const char *name);

void *bson_array_find(void *doc, int index);
int   bson_array_type(void *doc, int index);

#endif
