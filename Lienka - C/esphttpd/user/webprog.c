

//#include "webprog.h"
#include "xml_parser.h"
#include "bot.h"



#include <string.h>

#include "osapi.h"
#include "ets_sys.h"
#include "os_type.h"


void ICACHE_FLASH_ATTR webprog_init(char * cmds){
	
    
	//parse("<variables></variables>");
    //parse("#1;2;0");
    //init("MF,2,0;NS;MO;S,440;SL,1000;"); //TEXTOVE
    //init("ML;MF;SL;MR;SO;MB;");
    //init("MF;");
    init(cmds);
	//print_result();
}

// TODO: change return type to cmd array
void ICACHE_FLASH_ATTR webprog_execute_program(Bot *bot){
	
	//int actualCmd = 0;
	//Bot *bot = arg;

	while(!isEmptyCommandList()){
		Command command;
		command = scanCommand();
		webprog_process_program(&command, bot);
    }

	os_printf("STARTSTOP\n");
    bot_handleNormalInput(bot, BTN_STARTSTOP, 1);

    // add command to cmd array
    // return cmd array
}

void ICACHE_FLASH_ATTR webprog_process_program(Command *command, Bot *bot){

	//Command *command = arg1;
	//Bot *bot = arg2;

	/*os_printf("cmd: %s; \n", command->type);
	for (int i = 0; i < 20; ++i)
	{
		os_printf("cmd: %c; num: %d \n", command->type[i], command->type[i]);
	}*/
	if (strcmp(command->type, "MF") == 0){
		//os_printf("FORWARD\n");
		bot_handleNormalInput(bot, BTN_FORWARD, 1);

	}
	
	else if (strcmp(command->type, "MB") == 0){
		
		bot_handleNormalInput(bot, BTN_BACK, 1);
	}

	else if (strcmp(command->type, "ML") == 0){
		
		bot_handleNormalInput(bot, BTN_LEFT, 1);
	}

	else if (strcmp(command->type, "MR") == 0){
		bot_handleNormalInput(bot, BTN_RIGHT, 1);
	}

	else if (strcmp(command->type, "MO") == 0){
		// TODO OFF
	}

	else if (strcmp(command->type, "SO") == 0){
		// TODO SOUND ON
		bot_handleNormalInput(bot, BTN_OP2, 1);
	}

	else if (strcmp(command->type, "NS") == 0){
		// TODO NO SOUND
	}

	else if (strcmp(command->type, "SL") == 0){
		bot_handleNormalInput(bot, BTN_OP1, 1);
	}

	else if (strcmp(command->type, "NO") == 0){
		// TODO NONE
	}

	else if (strcmp(command->type, "GO") == 0){
		// TODO GOTO
	}

	else if (strcmp(command->type, "LE") == 0){
		// TODO LEDS
	}

	else if (strcmp(command->type, "LL") == 0){
		// TODO LED LEFT
	}

	else if (strcmp(command->type, "LR") == 0){
		// TODO LED RIGHT
	}
	else{
		os_printf("ELSE\n");
	}



}



