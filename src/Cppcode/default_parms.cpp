/* Code does not compile, as argument to print is missing. So there is not
   corresponding goal to compile it. That is intended. */
#include <string>
void print(char *string, int nl = 1);
print("Test",0);
print("Test");   // is equal to print("Test",1)
print();         // wrong, char * parameter is missing
