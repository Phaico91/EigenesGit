const int k = 42;
char *const s1 = "Test1";
const char *s2 = "Test2";
const char *const *s3 = "Test3";

k = 4;           // error: k ist const
s1 = "New test"; // error: pointer is const
*s1 = 'P';       // okay, characters are not const
s2 = "New test"; // okay, pointer is not const
*s2 = 'P';       // error: characters are const
