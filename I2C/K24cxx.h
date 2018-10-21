#ifndef __K24CXX_H
#define __K24CXX_H

#include "gpio_api.h"
#include "i2c_api.h"
#include "app_debug.h"

//IO��������
#define MBED_I2C_MTR_SCL    PB_2
#define MBED_I2C_MTR_SDA    PB_3


//IO��������	 
#define IIC_SCL_H    gpio_write(&i2c_gpio_scl, 1) //SCL
#define IIC_SCL_L    gpio_write(&i2c_gpio_scl, 0)

#define IIC_SDA_H    gpio_write(&i2c_gpio_sda, 1) //SDA
#define IIC_SDA_L    gpio_write(&i2c_gpio_sda, 0) //SDA

#define IIC_SDA_READ     gpio_read(&i2c_gpio_sda)//����SDA 

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	        8191
#define AT24C128	16383
#define AT24C256	32767  

#define EEROM_PAGESIZE  32

#define EEROM_SYS_NAME   3936      //�洢��
#define EEPROM_FIRSTADR 3954       //APP���º��־��ַ   

#define APPFIRSTFLAG 0x5A       //APP���º��־ 

//Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE AT24C32


//IIC���в�������
void IIC_Init(void);                           //IIC��IO��				 
void IIC_Start(void);				//IIC��ʼ�ź�
void IIC_Stop(void);	  			//IICֹͣ�ź�
uint8_t IIC_Send_Byte(u8 sdata);			//IIC����һ���ֽ�
uint8_t IIC_Wait_Ack(void); 		        //IIC�ȴ�ACK�ź�
void IIC_Ack(void);			        //IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
u8 IIC_Read_Byte(uint8_t nend);


uint8_t AT24CXX_ReadOneByte(u16 raddr,u8* rdata);
uint8_t AT24CXX_ReadLenByte(u16 raddr,u8* rdata,u16 len);

uint8_t AT24CXX_WriteOneByte(u16 waddr,u8  wdata);
uint8_t AT24CXX_WriteLenByte(u16 waddr,u8 *wdata,u16 len);
#endif
