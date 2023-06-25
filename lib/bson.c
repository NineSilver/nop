#include <endian.h>
#include <string.h>
#include <bson.h>

void *bson_find_entry(void *doc, const char *name) {
  const void *doc_start = doc;
  
  int32_t size = int32le_to_host(*((int32le_t *)(doc)));
  doc += 4;
  
  while (doc - doc_start < size) {
    uint8_t type = *((uint8_t *)(doc));
    
    if (!type) {
      break;
    }
    
    if (!strcmp(doc + 1, name)) {
      return doc;
    }
    
    /* Here we exploit the fact that strlen will count non-null
       bytes, even if unprintable. */
    
    doc += strlen(doc) + 1;
    
    if (type == 0x01 || type == 0x09 || type == 0x11 || type == 0x12) {
      doc += 8;
    } else if (type == 0x02) {
      doc += int32le_to_host(*((int32le_t *)(doc))) + 4;
    } else if (type == 0x03 || type == 0x04) {
      doc += int32le_to_host(*((int32le_t *)(doc)));
    } else if (type == 0x05) {
      doc += int32le_to_host(*((int32le_t *)(doc))) + 5;
    } else if (type == 0x08) {
      doc += 1;
    } else if (type == 0x0A || type == 0x7F || type == 0xFF) {
      /* doc += 0; */
    } else if (type == 0x10) {
      doc += 4;
    } else if (type == 0x13) {
      doc += 16;
    }
  }
  
  return NULL;
}

void *bson_find(void *doc, const char *name) {
  void *entry = bson_find_entry(doc, name);
  return entry + strlen(entry) + 1;
}

int bson_type(void *doc, const char *name) {
  void *entry = bson_find_entry(doc, name);
  return *((uint8_t *)(entry));
}

void *bson_array_find(void *doc, int index) {
  char buffer[12];
  /* TODO: Fill in? */
  
  return bson_find(doc, buffer);
}

int bson_array_type(void *doc, int index) {
  char buffer[12];
  /* TODO: Fill in? */
  
  return bson_type(doc, buffer);
}
