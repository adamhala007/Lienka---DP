#include "i2c_mcp.h"
#include "driver/i2c_master.h"
#include "osapi.h"

bool ICACHE_FLASH_ATTR mcp_writeRegister(uint8 addr, uint8 reg, uint16 val16, uint8 size)
{
    uint8 ack;

    i2c_master_start();
    i2c_master_writeByte(addr & 0xfe);
    ack = i2c_master_getAck();
    if (ack) {
        os_printf("%s: addr not acked\n", __FUNCTION__);
        i2c_master_stop();
        return false;
    }

    i2c_master_writeByte(reg);
    ack = i2c_master_getAck();
    if (ack) {
        os_printf("%s: reg not acked\n", __FUNCTION__);
        i2c_master_stop();
        return false;
    }

    while(size) {
        size--;
        uint8 val8 = (val16 >> (size * 8)) & 0xff;
        i2c_master_writeByte(val8);
        ack = i2c_master_getAck();
        if (ack) {
            os_printf("%s: byte %d not acked\n", __FUNCTION__);
            i2c_master_stop();
            return false;
        }
    }

    i2c_master_stop();
    return true;
}

bool ICACHE_FLASH_ATTR mcp_selectRegister(uint8 addr, uint8 reg)
{
    return mcp_writeRegister(addr, reg, 0, 0);
}

uint16 ICACHE_FLASH_ATTR mcp_read(uint8 addr, uint8 size)
{
    uint8 ack;
    uint16 val16 = 0;
    uint8 val8 = 0;

    i2c_master_start();
    i2c_master_writeByte(addr | 1);
    ack = i2c_master_getAck();

    if (ack) {
        os_printf("%s: addr not acked\n", __FUNCTION__);
        i2c_master_stop();
        return 0xff;
    }

    while(size) {
        size--;
        val8 = i2c_master_readByte();
        i2c_master_setAck(size == 0);
        val16 = ((val16 << 8) | val8);
    }

    i2c_master_stop();

    return val16;
}


void ICACHE_FLASH_ATTR test_mcp9800()
{
    uint16 temp;

    os_printf("\n\nmcp9800\n");

    if (!mcp_selectRegister(MCP9800_addr, MCP9800_registerCONFIG)) {
        os_printf("cannot select register!\n");
        return;
    }
    temp = mcp_read(MCP9800_addr, 1);
    os_printf("conf %x\n", temp);

    if (!mcp_writeRegister(MCP9800_addr, MCP9800_registerCONFIG, 0x60, 1)) {
        os_printf("cannot write conf register!");
        return;
    }

    if (!mcp_selectRegister(MCP9800_addr, MCP9800_registerCONFIG)) {
        os_printf("cannot select register!\n");
        return;
    }
    temp = mcp_read(MCP9800_addr, 1);
    os_printf("conf %x\n", temp);

    if (!mcp_selectRegister(MCP9800_addr, MCP9800_registerTEMP)) {
        os_printf("cannot select register!\n");
        return;
    }
    os_printf("set register, reading temp\n");
    temp = mcp_read(MCP9800_addr, 2);
    os_printf("temp %x\n", temp);
    os_printf("temp %f\n", temp / 256.0);
    os_printf("mcp9800 done\n");
}


bool ICACHE_FLASH_ATTR mcp23017_setDirection(uint16 dir)
{
    // set to all output
    if (!mcp_writeRegister(MCP23017_addr, MCP23017_registerIODIR, dir, 2)) {
        os_printf("error writing IODIR\n");
        return false;
    }
    return true;
}
uint16 ICACHE_FLASH_ATTR mcp23017_getDirection(void)
{
    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerIODIR)) {
        os_printf("error selecting IODIR\n");
        return 0;
    }
    return mcp_read(MCP23017_addr, 2);
}

bool ICACHE_FLASH_ATTR mcp23017_setGpio(uint16 gpio)
{
    if (!mcp_writeRegister(MCP23017_addr, MCP23017_registerGPIO, gpio, 2)) {
        os_printf("error writing GPIO\n");
        return false;
    }
    return true;
}

bool ICACHE_FLASH_ATTR mcp23017_setGpioA(uint8 gpio)
{
    if (!mcp_writeRegister(MCP23017_addr, MCP23017_registerGPIO, gpio, 1)) {
        os_printf("error writing GPIO\n");
        return false;
    }
    return true;
}

bool ICACHE_FLASH_ATTR mcp23017_setGpioB(uint8 gpio)
{
    if (!mcp_writeRegister(MCP23017_addr, MCP23017_registerGPIO + 1, gpio, 1)) {
        os_printf("error writing GPIO\n");
        return false;
    }
    return true;
}

uint16 ICACHE_FLASH_ATTR mcp23017_readGpio()
{
    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerGPIO)) {
        os_printf("error selecting GPIO\n");
        return 0;
    }
    return mcp_read(MCP23017_addr, 2);
}

uint8 ICACHE_FLASH_ATTR mcp23017_readGpioA()
{
    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerGPIO)) {
        os_printf("error selecting GPIO\n");
        return 0;
    }
    return mcp_read(MCP23017_addr, 1);
}

uint8 ICACHE_FLASH_ATTR mcp23017_readGpioB()
{
    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerGPIO + 1)) {
        os_printf("error selecting GPIO\n");
        return 0;
    }
    return mcp_read(MCP23017_addr, 1);
}

uint8 ICACHE_FLASH_ATTR mcp23017_readIntcapA()
{
    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerINTCAP)) {
        os_printf("error selecting GPIO\n");
        return 0;
    }
    return mcp_read(MCP23017_addr, 1);
}

void ICACHE_FLASH_ATTR test_mcp23017()
{
    static uint16 gpio = 0b1010101010101010;
    uint16 temp;

    os_printf("\n\nmcp23017\n");

    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerIODIR)) {
        os_printf("error selecting IODIR\n");
        return;
    }
    temp = mcp_read(MCP23017_addr, 2);
    os_printf("IODIR %x\n", temp);

    // set to all output
    if (!mcp_writeRegister(MCP23017_addr, MCP23017_registerIODIR, 0, 2)) {
        os_printf("error writing IODIR\n");
        return;
    }

    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerIODIR)) {
        os_printf("error selecting IODIR\n");
        return;
    }
    temp = mcp_read(MCP23017_addr, 2);
    os_printf("IODIR after set %x\n", temp);




    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerGPIO)) {
        os_printf("error selecting GPIO\n");
        return;
    }
    temp = mcp_read(MCP23017_addr, 2);
    os_printf("GPIO %x\n", temp);

    gpio = (gpio << 1) | (~gpio&1);
    os_printf("writing GPIO %x\n", gpio);
    if (!mcp_writeRegister(MCP23017_addr, MCP23017_registerGPIO, gpio, 2)) {
        os_printf("error writing GPIO\n");
        return;
    }

    if (!mcp_selectRegister(MCP23017_addr, MCP23017_registerGPIO)) {
        os_printf("error selecting GPIO\n");
        return;
    }
    temp = mcp_read(MCP23017_addr, 2);
    os_printf("GPIO after write %x\n", temp);

    os_printf("mcp23017 done\n");
}


/* vim: set sw=4 sts=4 ts=4 et : */
