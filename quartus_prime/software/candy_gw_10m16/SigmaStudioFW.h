/*
 * File:			SigmaStudioFW.h
 *
 * Description:  	SigmaStudio System Framwork macro definitions. These 
 *				macros should be implemented for your system's software.
 *
 * This software is distributed in the hope that it will be useful,
 * but is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * This software may only be used to program products purchased from
 * Analog Devices for incorporation by you into audio products that 
 * are intended for resale to audio product end users. This software
 * may not be distributed whole or in any part to third parties.
 *
 * Copyright � 2008 Analog Devices, Inc. All rights reserved.
 */	
 
#ifndef __SIGMASTUDIOFW_H__
#define __SIGMASTUDIOFW_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "io.h"
//#include "i2c.h"
#include "oc_i2c_master.h"

// 0x40 = 0100 0000 -> 0x10 = 0001 0000
// 0x30 ] 0011 0000 -> 0x0C = 0000 1100
#define I2C_BASEADDR  (AVALON_WB_BASE + 0x40)

#define I2C_PRERL 0x00//(AVALON_WB_0_BASE + ((I2C_BASEADDR + 0x00) << 2))//(*(volatile unsigned char *) (I2C_BASEADDR + 0x0))
#define I2C_PRERH 0x01//(AVALON_WB_0_BASE + ((I2C_BASEADDR + 0x01) << 2))//(*(volatile unsigned char *) (I2C_BASEADDR + 0x1))
#define I2C_CTR   0x02//(AVALON_WB_0_BASE + ((I2C_BASEADDR + 0x02) << 2))//(*(volatile unsigned char *) (I2C_BASEADDR + 0x2))
#define I2C_TXR   0x03//(AVALON_WB_0_BASE + ((I2C_BASEADDR + 0x03) << 2))//(*(volatile unsigned char *) (I2C_BASEADDR + 0x3))
#define I2C_RXR   0x03//(AVALON_WB_0_BASE + ((I2C_BASEADDR + 0x03) << 2))//(*(volatile unsigned char *) (I2C_BASEADDR + 0x3))
#define I2C_CR    0x04//(AVALON_WB_0_BASE + ((I2C_BASEADDR + 0x04) << 2))//(*(volatile unsigned char *) (I2C_BASEADDR + 0x4))
#define I2C_SR    0x04//(AVALON_WB_0_BASE + ((I2C_BASEADDR + 0x04) << 2))//(*(volatile unsigned char *) (I2C_BASEADDR + 0x4))

void i2c_setup(uint8_t per_h_reg, uint8_t per_l_reg);
bool i2c_start(uint8_t devAddress, uint8_t rw);

/* 
 * TODO: Update for your system's data type
 */
typedef unsigned short ADI_DATA_U16;
typedef unsigned char  ADI_REG_TYPE;
typedef uint8_t ADI_REG_U8;

/* 
 * Parameter data format
 */
#define SIGMASTUDIOTYPE_FIXPOINT 	0
#define SIGMASTUDIOTYPE_INTEGER 	1

#define SIGMA_SAFELOAD_DATA_0 0x0810
#define SIGMA_SAFELOAD_DATA_1 0x0811
#define SIGMA_SAFELOAD_DATA_2 0x0812
#define SIGMA_SAFELOAD_DATA_3 0x0813
#define SIGMA_SAFELOAD_DATA_4 0x0814

#define SIGMA_SAFELOAD_ADDR_0 0x0815
#define SIGMA_SAFELOAD_ADDR_1 0x0816
#define SIGMA_SAFELOAD_ADDR_2 0x0817
#define SIGMA_SAFELOAD_ADDR_3 0x0818
#define SIGMA_SAFELOAD_ADDR_4 0x0819

#define SIGMA_SAFELOAD_TRANSFER_BIT 0x081C

#if 0
int32_t SIGMA_WRITE_REGISTER_BLOCK(uint8_t devAddress, uint16_t address, uint16_t length, ADI_REG_TYPE *pData);
int32_t SIGMA_WRITE_REGISTER_CONTROL(uint8_t devAddress, uint16_t address, uint16_t length, ADI_REG_U8 *pData);
int32_t SIGMA_SAFELOAD_WRITE_ADDR(uint8_t devAddress, uint16_t addrAddress, uint16_t address);
int32_t SIGMA_SAFELOAD_WRITE_DATA(uint8_t devAddress, uint16_t dataAddress, uint16_t length, ADI_REG_U8 *pData);
int32_t SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(uint8_t devAddress);
#else
bool SIGMA_WRITE_REGISTER_BLOCK(uint8_t devAddress, uint16_t address, uint16_t length, ADI_REG_TYPE *pData);
bool SIGMA_WRITE_REGISTER_CONTROL(uint8_t devAddress, uint16_t address, uint16_t length, ADI_REG_U8 *pData);
bool SIGMA_SAFELOAD_WRITE_ADDR(uint8_t devAddress, uint16_t addrAddress, uint16_t address);
bool SIGMA_SAFELOAD_WRITE_DATA(uint8_t devAddress, uint16_t dataAddress, uint16_t length, ADI_REG_U8 *pData);
bool SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(uint8_t devAddress);
#endif
/* 
 * Write to a single Device register
 */
#define SIGMA_WRITE_REGISTER( devAddress, address, dataLength, data ) {/*TODO: implement macro or define as function*/}

/* 
 * TODO: CUSTOM MACRO IMPLEMENTATION
 * Write to multiple Device registers 
 */
//#define SIGMA_WRITE_REGISTER_BLOCK( devAddress, address, length, pData ) {/*TODO: implement macro or define as function*/}

/* 
 * TODO: CUSTOM MACRO IMPLEMENTATION
 * Writes delay (in ms) 
 */
#define SIGMA_WRITE_DELAY( devAddress, length, pData ) {/*TODO: implement macro or define as function*/}

/*
 * Read device registers 
 */
#define SIGMA_READ_REGISTER( devAddress, address, length, pData ) {/*TODO: implement macro or define as function*/} 

/*
 * Set a register field's value
 */
#define SIGMA_SET_REGSITER_FIELD( regVal, fieldVal, fieldMask, fieldShift )  \
		{ (regVal) = (((regVal) & (~(fieldMask))) | (((fieldVal) << (fieldShift)) && (fieldMask))) }
  
/*
 * Get the value of a register field
 */
#define SIGMA_GET_REGSITER_FIELD( regVal, fieldMask, fieldShift )  \
		{ ((regVal) & (fieldMask)) >> (fieldShift) }
 
/* 
 * Convert a floating-point value to SigmaDSP (5.23) fixed point format 
 *    This optional macro is intended for systems having special implementation
 *    requirements (for example: limited memory size or endianness)
 */
#define SIGMASTUDIOTYPE_FIXPOINT_CONVERT( _value ) {/*TODO: IMPLEMENT MACRO*/}

/* 
 * Convert integer data to system compatible format
 *    This optional macro is intended for systems having special implementation
 *    requirements (for example: limited memory size or endianness)
 */
#define SIGMASTUDIOTYPE_INTEGER_CONVERT( _value ) {/*TODO: IMPLEMENT MACRO*/}

#endif

