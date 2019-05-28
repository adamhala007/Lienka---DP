#include <xml_parser.h>
/*#include <stdlib.h>
#include <mem.h>
 #include <ctype.h>*/

#include "osapi.h"
#include "ets_sys.h"
#include "os_type.h"

char look[2] = "";
int indexx = 0;
char * input;

char token[] = "";
int kind;
int position;

/*Xml_Tree parse(char* xml_input){
	input = xml_input;
	indexx = 0;
	next();
	//scan();
	Xml_Tree xml_tree;
	return xml_tree;
}*/

void ICACHE_FLASH_ATTR init(char* code){
    input = code;
    indexx = 0;
    next();
}

void ICACHE_FLASH_ATTR next(){
	if (indexx >= strlen(input))
	{
		look[0] = '\0';
		
	}else{
		//os_printf("Index before; (%s) (%d) \n", look, indexx);
		look[0] = input[indexx]; 
		indexx = indexx + 1;
		os_printf("look: %c;\n", look[0]);
		//os_printf("Index; (%s) (%d) \n", look, indexx);
	}
}

/*
 Command pattern: type,arg1,arg2;
 type, arg1, arg2 - numbers
 For example: MF;2;0;
 
 Commands:
 type: 0 - error, arg1:     0 - no command              arg2: 0 - no meaning yet :)
                            1 - delimiter count > 2
                            2 - wrong command format
 */
Command ICACHE_FLASH_ATTR scanCommand(){
    Command c;
    c.type[0] = 0;
    c.arg1[0] = 0;
    c.arg2[0] = 0;

    
    
    if (look[0] == '\0') {
        return c;
    }

	int delimiterCount = 0;

	while(look[0] != '\0'){

		switch(look[0]){
			case ',':
				delimiterCount++;
				break;
			case ';':
				next();
				return c;
			default:
				if (delimiterCount == 0)
				{
					strcat(c.type,&look[0]);
					os_printf("TYPE: %s \n", c.type);
				}
				else if (delimiterCount == 1)
				{
					strcat(c.arg1,&look[0]);
				}
				else if (delimiterCount == 1)
				{
					strcat(c.arg2,&look[0]);
				}
				break;
		}
		next();
	}
	return c;
}

int ICACHE_FLASH_ATTR isEmptyCommandList(){
	return look[0] == '\0';
}

int ICACHE_FLASH_ATTR charToInt(char c){
    return c-'0';
}

int ICACHE_FLASH_ATTR appendCharToInt(int n, char c){
    return n*10 + charToInt(c);
}

void ICACHE_FLASH_ATTR findEndOfCommand(){
    while (look[0] != '\0') {
        if (look[0] == ';') {
            next();
            return;
        }
        next();
    }
}



void ICACHE_FLASH_ATTR print_result(){
	

    while (look[0]!='\0') {
        os_printf("%s, %d, %d, %c \n", token, kind, position, look[0]);
        next();
    }
	
	
}

