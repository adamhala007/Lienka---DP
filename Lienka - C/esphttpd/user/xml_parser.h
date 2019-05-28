
#define MAX_ATTRIBUTE_COUNT 10

#define NOTHING 0
#define NUMBER 1
#define WORD 2
#define SYMBOL 3

/*typedef struct Xml_Tree {
	char *tag_name;
    char *attribute_names[MAX_ATTRIBUTE_COUNT];
    char *attribute_values[MAX_ATTRIBUTE_COUNT];
    struct Xml_Tree* xml_tree;
} Xml_Tree;*/

typedef struct _Command {
    char type[32];
    char arg1[16];
    char arg2[16];
} Command;

//Xml_Tree parse(char* input);
//char* concat(const char *s1, const char *s2);
void init(char* code);
void next();
void print_result();
int isEmptyCommandList();
int charToInt(char c);
int appendCharToInt(int n, char c);
void findEndOfCommand();

Command scanCommand();


