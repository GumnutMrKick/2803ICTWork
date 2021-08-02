#include <string.h>

//-- Removes trailing blanks from s --
void trailing(char *s) {

    int slength = strlen(s);

    // while((s[slength] == ' ' || s[slength] == '\0')) // fixed code
    while((s[slength] == ' ' || s[slength] == '\t')) // yucky error code
        --slength;

    s[++slength] = '\0';

}