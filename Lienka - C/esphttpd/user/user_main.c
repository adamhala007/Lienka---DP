/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

/*
This is example code for the esphttpd library. It's a small-ish demo showing off 
the server, including WiFi connection management capabilities, some IO and
some pictures of cats.
*/

#include <esp8266.h>
#include "httpd.h"
#include "io.h"
#include "httpdespfs.h"
#include "cgi.h"
#include "cgiwifi.h"
#include "cgiflash.h"
#include "stdout.h"
#include "auth.h"
#include "espfs.h"
#include "captdns.h"
#include "webpages-espfs.h"
#include "cgiwebsocket.h"
#include "cgi-test.h"
//#include <../gdbstub/gdbstub.h>

/*#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "hw.h"
#include "i2c_mcp.h"
#include "bot.h"*/
//#include "mdns.h"
#include "webprog.h"

#include "bot.h"
#include "hw.h"


//#include "perform.h"

//#include <../gdbstub/gdbstub.h>


//#include "driver/mdns.h"
//static void helper2(void);
//The example can print out the heap use every 3 seconds. You can use this to catch memory leaks.
//#define SHOW_HEAP_USE

//Function that tells the authentication system what users/passwords live on the system.
//This is disabled in the default build; if you want to try it, enable the authBasic line in
//the builtInUrls below.
int myPassFn(HttpdConnData *connData, int no, char *user, int userLen, char *pass, int passLen) {
	if (no==0) {
		os_strcpy(user, "admin");
		os_strcpy(pass, "s3cr3t");
		return 1;
//Add more users this way. Check against incrementing no for each user added.
//	} else if (no==1) {
//		os_strcpy(user, "user1");
//		os_strcpy(pass, "something");
//		return 1;
	}
	return 0;
}

static ETSTimer websockTimer;
static ETSTimer testTimer;
static ETSTimer testTimer2;
static Bot bot, botWeb;
//static mdnsHandle *mdns;




static void ICACHE_FLASH_ATTR mdns()
{
    
    struct ip_info ipConfig;
     struct mdns_info *info = (struct mdns_info *)os_zalloc(sizeof(struct mdns_info));
     
     wifi_get_ip_info(STATION_IF, &ipConfig);
     if (!(wifi_station_get_connect_status() == STATION_GOT_IP && ipConfig.ip.addr != 0)) {
     os_printf("Cannot read station configuration\n");
     return;
     }
     info->host_name = "lienka";
     //info->host_name = (char *) "lienka";
     info->ipAddr = ipConfig.ip.addr; //ESP8266 station IP
    
    //struct station_config config[5];
    //int num = wifi_station_get_ap_info(&config);
    //os_printf("\n Number of IP: %d \n",num);
    /*for (int i = 0; i<num; i++) {
        os_printf("\n IP:");
        //os_printf(config[i].);
        os_printf("\n");
    }*/
    
     info->server_name = "module01";
     info->server_port = 80;
     info->txt_data[0] = "version = now";
     info->txt_data[1] = "user1 = data1";
     info->txt_data[2] = "user2 = data2";
    
     info->txt_data[3] = "vendor = me";
    espconn_mdns_init(info);
     //espconn_mdns_server_register();
    //espconn_mdns_enable();
}

static void ICACHE_FLASH_ATTR testTimerCb(void *arg) {
    mdns();
    //webprog_init();
    //webprog_execute_program(&bot);
}

//Broadcast the uptime in seconds every second over connected websockets
static void ICACHE_FLASH_ATTR websockTimerCb(void *arg) {
	static int ctr=0;
	char buff[128];
	ctr++;
	os_sprintf(buff, "Up for %d minutes %d seconds!\n", ctr/60, ctr%60);
	cgiWebsockBroadcast("/websocket/ws.cgi", buff, os_strlen(buff), WEBSOCK_FLAG_NONE);
}

//On reception of a message, send "You sent: " plus whatever the other side sent
void ICACHE_FLASH_ATTR myWebsocketRecv(Websock *ws, char *data, int len, int flags) {
	
    hw_keepAwake(300000);
    int i;
	char buff[128];
    int gpio = 5;

    bot_stopProgram(&bot);
    
    if (data[0] == '0') {
        
        switch (data[1]) {
            case '8':
                //bot_handleNormalInput(&bot, BTN_FORWARD, 1);
                bot_handleNormalInput(&botWeb, BTN_FORWARD, 1);
                break;
            case '6':
                bot_handleNormalInput(&botWeb, BTN_RIGHT, 1);
                break;
            case '4':
                bot_handleNormalInput(&botWeb, BTN_LEFT, 1);
                break;
            case '2':
                bot_handleNormalInput(&botWeb, BTN_BACK, 1);
                break;
            case '1':
                bot_handleNormalInput(&botWeb, BTN_STARTSTOP, 1);
                break;
            case '0':
                bot_handleNormalInput(&botWeb, BTN_CLEAR, 1);
                break;
            case '7':
                bot_handleNormalInput(&botWeb, BTN_OP1, 1);
                break;
            case '9':
                bot_handleNormalInput(&botWeb, BTN_OP2, 1);
                break;
            case '3':
                webprog_init("MF;");
                webprog_execute_program(&botWeb);
                break;
            default:
                break;
        }
        
    } else if(data[0] == '1') {
        switch (data[1]) {
            case '8':
                hw_motor_setDir(1,-1);
                break;
            case '6':
                hw_motor_setDir(-1,-1);
                break;
            case '4':
                hw_motor_setDir(1,1);
                break;
            case '2':
                hw_motor_setDir(-1,1);
                break;
            case '0':
                //hw_motor_setDir(0,0);
                hw_allOff();
                break;
            case '7':
                hw_sound(440);
                break;
            case '9':
                hw_setLed(0,1);
                hw_setLed(1,1);
                break;
            default:
                break;
        }
    }else 
    {
        webprog_init(data);
        webprog_execute_program(&botWeb);
    }
    
    
	os_sprintf(buff, "You sent: ");
	for (i=0; i<len; i++) buff[i+10]=data[i];
	buff[i+10]=0;
    os_printf(data);
    os_printf("\n");
	cgiWebsocketSend(ws, buff, os_strlen(buff), WEBSOCK_FLAG_NONE);
}

//Websocket connected. Install reception handler and send welcome message.
void myWebsocketConnect(Websock *ws) {
	ws->recvCb=myWebsocketRecv;
	cgiWebsocketSend(ws, "Hi, Websocket!", 14, WEBSOCK_FLAG_NONE);
}

//On reception of a message, echo it back verbatim
void myEchoWebsocketRecv(Websock *ws, char *data, int len, int flags) {
	os_printf("EchoWs: echo, len=%d\n", len);
	cgiWebsocketSend(ws, data, len, flags);
}

//Echo websocket connected. Install reception handler.
void myEchoWebsocketConnect(Websock *ws) {
	os_printf("EchoWs: connect\n");
	ws->recvCb=myEchoWebsocketRecv;
}


#ifdef ESPFS_POS
CgiUploadFlashDef uploadParams={
	.type=CGIFLASH_TYPE_ESPFS,
	.fw1Pos=ESPFS_POS,
	.fw2Pos=0,
	.fwSize=ESPFS_SIZE,
};
#define INCLUDE_FLASH_FNS
#endif
#ifdef OTA_FLASH_SIZE_K
CgiUploadFlashDef uploadParams={
	.type=CGIFLASH_TYPE_FW,
	.fw1Pos=0x1000,
	.fw2Pos=((OTA_FLASH_SIZE_K*1024)/2)+0x1000,
	.fwSize=((OTA_FLASH_SIZE_K*1024)/2)-0x1000,
	.tagName=OTA_TAGNAME
};
#define INCLUDE_FLASH_FNS
#endif

/*
This is the main url->function dispatching data struct.
In short, it's a struct with various URLs plus their handlers. The handlers can
be 'standard' CGI functions you wrote, or 'special' CGIs requiring an argument.
They can also be auth-functions. An asterisk will match any url starting with
everything before the asterisks; "*" matches everything. The list will be
handled top-down, so make sure to put more specific rules above the more
general ones. Authorization things (like authBasic) act as a 'barrier' and
should be placed above the URLs they protect.
*/
HttpdBuiltInUrl builtInUrls[]={
	{"*", cgiRedirectApClientToHostname, "esp8266.nonet"},
	{"/", cgiRedirect, "/index.tpl"},
	{"/led.tpl", cgiEspFsTemplate, tplLed},
	{"/index.tpl", cgiEspFsTemplate, tplCounter},
	{"/led.cgi", cgiLed, NULL},
#ifdef INCLUDE_FLASH_FNS
	{"/flash/next", cgiGetFirmwareNext, &uploadParams},
	{"/flash/upload", cgiUploadFirmware, &uploadParams},
#endif
	{"/flash/reboot", cgiRebootFirmware, NULL},

	//Routines to make the /wifi URL and everything beneath it work.

//Enable the line below to protect the WiFi configuration with an username/password combo.
//	{"/wifi/*", authBasic, myPassFn},

	{"/wifi", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/wifiscan.cgi", cgiWiFiScan, NULL},
	{"/wifi/wifi.tpl", cgiEspFsTemplate, tplWlan},
	{"/wifi/connect.cgi", cgiWiFiConnect, NULL},
	{"/wifi/connstatus.cgi", cgiWiFiConnStatus, NULL},
	{"/wifi/setmode.cgi", cgiWiFiSetMode, NULL},

	{"/websocket/ws.cgi", cgiWebsocket, myWebsocketConnect},
	{"/websocket/echo.cgi", cgiWebsocket, myEchoWebsocketConnect},

	{"/test", cgiRedirect, "/test/index.html"},
	{"/test/", cgiRedirect, "/test/index.html"},
	{"/test/test.cgi", cgiTestbed, NULL},

	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};


#ifdef SHOW_HEAP_USE
static ETSTimer prHeapTimer;

/*static void start_mdns_service()
{
    //initialize mDNS service
    esp_err_t err = mdns_init();
    if (err) {
        printf("MDNS Init failed: %d\n", err);
        return;
    }
    
    //set hostname
    mdns_hostname_set("lienka");
    //set default instance
    mdns_instance_name_set("ESP8266 Lienka");
}*/


static void ICACHE_FLASH_ATTR prHeapTimerCb(void *arg) {
	os_printf("Heap: %ld\n", (unsigned long)system_get_free_heap_size());
}
#endif

//Main routine. Initialize stdout, the I/O, filesystem and the webserver and we're done.
void ICACHE_FLASH_ATTR user_init(void) {
	stdoutInit();
	ioInit();
	captdnsInit();
    wifi_set_phy_mode(PHY_MODE_11B);

    //gdbstub_init();

	// 0x40200000 is the base address for spi flash memory mapping, ESPFS_POS is the position
	// where image is written in flash that is defined in Makefile.
#ifdef ESPFS_POS
	espFsInit((void*)(0x40200000 + ESPFS_POS));
#else
	espFsInit((void*)(webpages_espfs_start));
#endif
	httpdInit(builtInUrls, 80);
#ifdef SHOW_HEAP_USE
	os_timer_disarm(&prHeapTimer);
	os_timer_setfn(&prHeapTimer, prHeapTimerCb, NULL);
	os_timer_arm(&prHeapTimer, 3000, 1);
#endif
	os_timer_disarm(&websockTimer);
	os_timer_setfn(&websockTimer, websockTimerCb, NULL);
	os_timer_arm(&websockTimer, 1000, 1); //1
    
    //uart_init(BIT_RATE_115200, BIT_RATE_115200);
    hw_init();
   bot_init(&bot, &botWeb);
    //bot_init(&bot);
    
    os_printf("SDK version:%s\n", system_get_sdk_version());
    
    os_timer_disarm(&testTimer);
    os_timer_setfn(&testTimer, testTimerCb, NULL);
    os_timer_arm(&testTimer, 10000, 0);
    
	os_printf("\nReady\n");
    
    
}

void user_rf_pre_init() {
	//Not needed, but some SDK versions want this defined.
}




//Sdk 2.0.0 needs extra sector to store rf cal stuff. Place that at the end of the flash.
/*uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
	enum flash_size_map size_map = system_get_flash_size_map();
	uint32 rf_cal_sec = 0;

	switch (size_map) {
		case FLASH_SIZE_4M_MAP_256_256:
			rf_cal_sec = 128 - 8;
			break;

		case FLASH_SIZE_8M_MAP_512_512:
			rf_cal_sec = 256 - 5;
			break;

		case FLASH_SIZE_16M_MAP_512_512:
		case FLASH_SIZE_16M_MAP_1024_1024:
			rf_cal_sec = 512 - 5;
			break;

		case FLASH_SIZE_32M_MAP_512_512:
		case FLASH_SIZE_32M_MAP_1024_1024:
			rf_cal_sec = 1024 - 5;
			break;

		default:
			rf_cal_sec = 0;
			break;
	}

	return rf_cal_sec;
}*/

