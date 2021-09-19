// include self
#include "shared_includes.h"

// shared functions

/*!
    
	<summary>
        this function prints the current time to console
    </summary>

    <returns>void<returns>

*/
void myTime () {

	// variable definition
	time_t curr_time;
	struct tm * format;
	char * tmp;
	tmp = (char *)calloc(MAXIN, sizeof(char *));
	int replaced = 0, start;

	// get and translate time
	time(&curr_time);
	format = localtime(&curr_time);

	// copy the time into the temp
	strcpy(tmp, asctime(format));

	// trim down the string to just hours minutes seconds
	for (int x=0; ((x < MAXIN) && (replaced < 4)); x++)
		if (tmp[x] == ' ') {

			tmp[x] = '\0';
			replaced++;

			if (replaced == 3)
				start = x + 1;

		}

	// copy the time into
	printf("%s", (tmp + start));

}

/*!
    
	<summary>
        this function replaces the first space it finds with a \0 and return a char*
		pointer to the character after it
    </summary>

	<param name="str">the string which this function concerns</param>

    <returns>a char pointer to the string after the first<returns>

*/
char* popFront (char * str) {

	for (int x=0; x < (MAXIN - 1); x++)
		if ((str[x] == '\0') || (str[x] == ' ')) {

			str[x] = '\0';
			return (str + x + 1);

		}

}

/*!
    
	<summary>
        first of all, frankly this feels like bad design
		but this function reverses the effect of the popFront function. upon giving it
		the orginal string it will remove the \0 (replacing it with a space) which
		seperated the command from the args
    </summary>

	<param name="str">the string which this function concerns</param>

    <returns>void<returns>

*/
void unPopFront (char * str) {

	if (str[0] != '\0')
		return unPopFront(str + 1);

	str[0] = ' ';

}

/*!
    
	<summary>
        this function cleans up after fget's crap
    </summary>

	<param name="str">the string which this function concerns</param>

    <returns>void<returns>

*/
void fgetsClean (char * str) {

	int len = strlen(str);
	str[len - 1] = '\0';
	str[len] = '\0';

}