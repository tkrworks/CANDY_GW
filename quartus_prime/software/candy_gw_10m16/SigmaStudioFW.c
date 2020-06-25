/*
 * SigmaStudioFW.c
 *
 *  Created on: 2019/01/17
 *      Author: shun
 */

#include "SigmaStudioFW.h"

uint8_t i2c_write_data[5120 + 2] = {0x00};

void i2c_setup(uint8_t per_h_reg, uint8_t per_l_reg)
{
	//IOWR_ALTERA_AVALON_PIO_DATA(I2C_CTR, 0x00);
	IOWR(I2C_BASEADDR, I2C_CTR, 0x00);
	IOWR(I2C_BASEADDR, I2C_CR, 0x01);

	//IOWR_ALTERA_AVALON_PIO_DATA(I2C_PRERL, per_l_reg);
	//IOWR_ALTERA_AVALON_PIO_DATA(I2C_PRERH, per_h_reg);
	//IOWR_ALTERA_AVALON_PIO_DATA(I2C_CTR,   0x80);

	IOWR(I2C_BASEADDR, I2C_PRERL, per_l_reg);
	IOWR(I2C_BASEADDR, I2C_PRERH, per_h_reg);
	IOWR(I2C_BASEADDR, I2C_CTR,   0x80);
}

bool i2c_start(uint8_t devAddress, uint8_t rw)
{
	IOWR(I2C_BASEADDR, I2C_TXR, (devAddress << 0) + (rw & 0x01));
	IOWR(I2C_BASEADDR, I2C_CR, 0x90);

	while (IORD(I2C_BASEADDR, I2C_SR) & 0x02)
	{
		usleep(10);
	}

	usleep(10);

	if (IORD(I2C_BASEADDR, I2C_SR) & 0x80)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool i2c_write_byte(uint8_t data)
{
	IOWR(I2C_BASEADDR, I2C_TXR, data);
	IOWR(I2C_BASEADDR, I2C_CR,  0x10); //wr

	while (IORD(I2C_BASEADDR, I2C_SR) & 0x02)
	{
		usleep(10);
	}

	usleep(10);

	if (IORD(I2C_BASEADDR, I2C_SR) & 0x80)
	{
		printf("I2C_SR %d\n", IORD(I2C_BASEADDR, I2C_SR));
		return false;
	}
	else
	{
		return true;
	}
}

bool i2c_write_byte_stop(uint8_t data)
{
	IOWR(I2C_BASEADDR, I2C_TXR, data);
	IOWR(I2C_BASEADDR, I2C_CR, 0x50); //stop

	while (IORD(I2C_BASEADDR, I2C_SR) & 0x02)
	{
		usleep(10);
	}

	usleep(10);

	if (IORD(I2C_BASEADDR, I2C_SR) & 0x80)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool i2c_multiple_write(uint8_t devAddress, uint16_t controlAddress, uint8_t data[], uint16_t len)
{
	if (!i2c_start(devAddress, 0x00))
	{
		printf("I2C start failed.\n");
		return false;
	}

	usleep(100);

	if (!i2c_write_byte((controlAddress >> 8) & 0xFF))
	{
		printf("I2C caddrH write failed.\n");
		//return false;
	}

	usleep(100);

	if (!i2c_write_byte(controlAddress & 0xFF))
	{
		printf("I2C caddrL write failed.\n");
		return false;
	}

	usleep(100);

	for (int i = 0; i < len; i++)
	{
		if (i == len - 1)
		{
			if (!i2c_write_byte_stop(data[i]))
			{
				printf("I2C stop failed.\n");
				return false;
			}
		}
		else
		{
			if (!i2c_write_byte(data[i]))
			{
				printf("I2C data[%d] write failed.\n", i);
				return false;
			}
		}

		usleep(100);
	}

	return true;
}

bool SIGMA_WRITE_REGISTER_BLOCK(uint8_t devAddress, uint16_t address, uint16_t length, ADI_REG_TYPE *pData)
{
#if 0
	I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t  i2c_read_data[2];
#endif
  //static uint8_t i2c_write_data[5120 + 2];
  //uint8_t i2c_write_data[length + 2];

  bool ret;

#if 0
  seq.addr  = devAddress;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_write_data[0] = (address >> 8) & 0x00FF;
  i2c_write_data[1] = address & 0x00FF;

  for (uint32_t i = 0; i < length; i++)
  {
    i2c_write_data[i + 2] = pData[i];
  }
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = length + 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(I2C0, &seq);
#endif

  //ret = i2c_multiple_write(PIO_2_BASE, PIO_3_BASE, devAddress, address, pData, length);
  ret = i2c_multiple_write(devAddress, address, pData, length);

  usleep(1000);

  //if (ret != i2cTransferDone)
  if (!ret)
  {
    //return((uint32_t) ret);
	  return false;
  }

  //return((uint32_t) 0);
  return true;
}

bool SIGMA_WRITE_REGISTER_CONTROL(uint8_t devAddress, uint16_t address, uint16_t length, ADI_REG_U8 *pData)
{
#if 0
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t  i2c_read_data[2];
  static uint8_t i2c_write_data[16 + 2];
  //uint8_t i2c_write_data[length + 2];
  //uint8_t *i2c_write_data = (uint8_t *)malloc(sizeof(uint8_t) * (length + 2));
#endif
  bool ret;

#if 0
  seq.addr  = devAddress;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_write_data[0] = (address >> 8) & 0x00FF;
  i2c_write_data[1] = address & 0x00FF;

  for (uint32_t i = 0; i < length; i++)
  {
    i2c_write_data[i + 2] = pData[i];
  }
  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = length + 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(I2C0, &seq);
#endif

  //ret = i2c_multiple_write(PIO_2_BASE, PIO_3_BASE, devAddress, address, pData, length);
  ret = i2c_multiple_write(devAddress, address, pData, length);

  //if (ret != i2cTransferDone)
  if (!ret)
  {
    //return((uint32_t) ret);
	  return false;
  }

  //free(i2c_write_data);

  //return((uint32_t) 0);
  return true;
}

bool SIGMA_SAFELOAD_WRITE_ADDR(uint8_t devAddress, uint16_t addrAddress, uint16_t address)
{
#if 0
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t  i2c_read_data[2];
  static uint8_t i2c_write_data[4];
#endif
  bool ret;
  uint8_t wdata[2];

#if 0
  seq.addr  = devAddress;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_write_data[0] = (addrAddress >> 8) & 0x00FF;
  i2c_write_data[1] = addrAddress & 0x00FF;
  i2c_write_data[2] = (address >> 8) & 0x00FF;
  i2c_write_data[3] = address & 0x00FF;

  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = 4;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(I2C0, &seq);
#endif

  wdata[0] = (address >> 8) & 0x00FF;
  wdata[1] = address & 0x00FF;
  //ret = i2c_multiple_write(PIO_2_BASE, PIO_3_BASE, devAddress, address, wdata, 2);
  ret = i2c_multiple_write(devAddress, address, wdata, 2);

  //if (ret != i2cTransferDone)
  if (!ret)
  {
    //return((uint32_t) ret);
	  return false;
  }

  //return((uint32_t) 0);
  return true;
}

bool SIGMA_SAFELOAD_WRITE_DATA(uint8_t devAddress, uint16_t dataAddress, uint16_t length, ADI_REG_U8 *pData)
{
#if 0
  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t  i2c_read_data[2];
  static uint8_t i2c_write_data[16];

  seq.addr  = devAddress;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_write_data[0] = (dataAddress >> 8) & 0x00FF;
  i2c_write_data[1] = dataAddress & 0x00FF;
  i2c_write_data[2] = 0x00;

  for (uint32_t i = 0; i < length; i++)
  {
    i2c_write_data[i + 3] = pData[i];
  }

  seq.buf[0].data   = i2c_write_data;
  seq.buf[0].len    = length + 3;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(I2C0, &seq);
#endif

  bool ret;
  uint8_t wdata[16] = {0};
  for (int i = 0; i < length; i++)
  {
	  wdata[i + 1] = pData[i];
  }

  //ret = i2c_multiple_write(PIO_2_BASE, PIO_3_BASE, devAddress, dataAddress, wdata, length + 1);
  ret = i2c_multiple_write(devAddress, dataAddress, wdata, length + 1);

  //if (ret != i2cTransferDone)
  if (!ret)
  {
    //return((uint32_t) ret);
	  return false;
  }

  //return((uint32_t) 0);
  return true;
}

bool SIGMA_SAFELOAD_WRITE_TRANSFER_BIT(uint8_t devAddress)
{
#if 0
    I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t  i2c_read_data[2];
  uint8_t i2c_safeload_data[4];

  seq.addr  = devAddress;
  seq.flags = I2C_FLAG_WRITE;
  /* Select command to issue */
  i2c_safeload_data[0] = 0x08;
  i2c_safeload_data[1] = 0x1C;
  i2c_safeload_data[2] = 0x00;
  i2c_safeload_data[3] = 0x3C;

  seq.buf[0].data   = i2c_safeload_data;
  seq.buf[0].len    = 4;//length + 2;
  /* Select location/length of data to be read */
  seq.buf[1].data = i2c_read_data;
  seq.buf[1].len  = 0;

  ret = I2CSPM_Transfer(I2C0, &seq);
#endif

  bool ret;
  uint16_t address = 0x081C;
  uint8_t wdata[2] = {0x00, 0x3C};
  //ret = i2c_multiple_write(PIO_2_BASE, PIO_3_BASE, devAddress, address, wdata, 2);
  ret = i2c_multiple_write(devAddress, address, wdata, 2);

  //if (ret != i2cTransferDone)
  if (!ret)
  {
    //return((uint32_t) ret);
	  return false;
  }

  //return((uint32_t) 0);
  return true;
}
