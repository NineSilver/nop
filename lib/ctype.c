#include <ctype.h>

int isdigit(char chr) {
  return (chr >= '0' && chr <= '9');
}

char toupper(char chr) {
  if (chr >= 'a' && chr <= 'z') {
    return (chr - 'a') + 'A';
  }
  
  return chr;
}

char tolower(char chr) {
  if (chr >= 'A' && chr <= 'Z') {
    return (chr - 'A') + 'a';
  }
  
  return chr;
}
