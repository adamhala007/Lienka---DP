#ifndef HAVE_BEEBOT_USER_I2C_MCP_H
#define HAVE_BEEBOT_USER_I2C_MCP_H 1
#include "c_types.h"

#define MCP23017_addr 0x40
#define MCP23017_registerIODIR 0x00
#define MCP23017_registerIPOL 0x02
#define MCP23017_registerINTEN 0x04
#define MCP23017_registerDEFVAL 0x06
#define MCP23017_registerINTCON 0x08
#define MCP23017_registerIOCON 0x0a
#define MCP23017_registerGPPU 0x0c
#define MCP23017_registerINTF 0x0e
#define MCP23017_registerINTCAP 0x10
#define MCP23017_registerGPIO 0x12
#define MCP23017_registerOLAT 0x14

#define MCP9800_addr 0x90
#define MCP9800_registerTEMP 0x00
#define MCP9800_registerCONFIG 0x01
#define MCP9800_registerHYST 0x02
#define MCP9800_registerSET 0x03

/**
 * Writes data to a register, MSB first.
 *
 * @param addr device address
 * @param reg register number
 * @param val16 value to write
 * @param size how many bytes from value to write (1 - only LSB)
 */
bool mcp_writeRegister(uint8 addr, uint8 reg, uint16 val16, uint8 size);

/**
 * Select register for reading.
 *
 * @param addr device address
 * @param reg register number
 */
bool mcp_selectRegister(uint8 addr, uint8 reg);

/**
 * Read data from register. MSB first.
 *
 * @param addr device address
 * @param size how many bytes to read (1 - read only LSB)
 */
uint16 mcp_read(uint8 addr, uint8 size);


bool mcp23017_setDirection(uint16 dir);
uint16 mcp23017_getDirection();
bool mcp23017_setGpio(uint16 gpio);
bool mcp23017_setGpioA(uint8 gpio);
bool mcp23017_setGpioB(uint8 gpio);
uint16 mcp23017_readGpio();
uint8 mcp23017_readGpioA();
uint8 mcp23017_readGpioB();
uint8 mcp23017_readIntcapA();

#endif //HAVE_BEEBOT_USER_I2C_MCP_H
/* vim: set sw=4 sts=4 ts=4 et : */
