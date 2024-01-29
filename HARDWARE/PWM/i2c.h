#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f4xx.h"
#include "delay.h"

/*********************************************************
                        主机IIC代码
                        Master IIC 
*********************************************************/


    //_i2c_slef->sda_pin_write_bit = /*the sda_pin function you have established*/;
    //_i2c_slef->sda_pin_write_bit = /*the scl_pin function you have established*/;
    //_i2c_slef->sda_pin_read_bit = /*the scl_pin function you have established*/;
    //e.g.
 
    // Set each of your pin functions in turn
    /**
    //_i2c_slef->sda_pin_write_bit = 
    //_i2c_slef->sda_pin_write_bit = 
    //_i2c_slef->sda_pin_read_bit =
        ...
    **/
   
#define I2C_Delay_100ns() __NOP()
#define I2C_Delay_ms(ti)  delay_ms(ti)//Delay_ms(ti)
#define I2C_Delay_us(ti)  delay_us(ti)//delay_us(ti)

#define READ_DATA_BIT(data, x)          ((uint8_t)(((data) >> (x)) & 0x01))
#define OPERATE_DATA_BIT(data, x , bit) ((uint8_t)((bit&0x01)>0?((data)|(0x01<<(x))):((data)&~(0x01<<(x)))))
                                         
#define SDA_OUT()
#define SDA_IN()

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef enum {
    MSG_DELAY_100NANO = 0, 
    MSG_DELAY_10MICRO, 
    MSG_DELAY_MILLI, 
    MSG_DELAY_I2C, 
} IIC_MSG_ENUM;

typedef enum {
    MSG_GPIO_I2C_CLOCK = 0, 
    MSG_GPIO_I2C_DATA, 
} IIC_PIN_MSG_ENUM;


typedef enum {
    LOW  =0,
    HIGH
} IIC_ELC_LEVEL;

struct _i2c_struct
{
    //Drive Address
    char Slave_Addr;
    //char Master_Addr;
    
    //pin setting
    uint8_t (*pin_write_bit)(struct _i2c_struct* _i2c_self, uint8_t msg, uint8_t arg_int);

    /*
      You need to establish a specific function that defines the 
    pin so that the function pointer points to the function 
    you will establish.
    */ 
   //你需要写出特定的定义引脚的函数，使该函数指针指向你所写的函数.
    void (*sda_pin_write_bit)(uint8_t binary);
    void (*scl_pin_write_bit)(uint8_t binary);
    uint8_t (*sda_pin_read_bit)(void);

/*
      //signal
      void (*initiate_sig)(struct _i2c_struct* _i2c_self);
      void (*terminate_sig)(struct _i2c_struct* _i2c_self);
      void (*send_ack)(struct _i2c_struct* _i2c_self);
      void (*send_no_ack)(struct _i2c_struct* _i2c_self);
      void (*wait_ack)(struct _i2c_struct* _i2c_self);
*/
    int8_t (*send_byte)(struct _i2c_struct* _i2c_self, uint8_t slave_addr, uint8_t register_addr,uint8_t data_byte);
    int8_t (*read_byte)(struct _i2c_struct* _i2c_self, uint8_t slave_addr, uint8_t register_addr, uint8_t* data_byte);
    int8_t (*send_bytes)(struct _i2c_struct* _i2c_self, uint8_t slave_addr, uint8_t register_addr,uint8_t* pdata_buffer,uint8_t data_lenth);
    int8_t (*read_bytes)(struct _i2c_struct* _i2c_self, uint8_t slave_addr, uint8_t register_addr,uint8_t* reg_data, uint8_t data_lenth);
};

typedef struct _i2c_struct   I2C_Struct;
typedef struct _i2c_struct*  pI2C_Struct;
typedef struct _i2c_struct** Arr_pI2C_Struct;

uint8_t msg_delay(uint8_t msg);
uint8_t pin_write_bit(pI2C_Struct _i2c_self, uint8_t msg, uint8_t arg_int);

// Arr_pI2C_Struct create_Arr_I2C_Structure(int Arr_n);
pI2C_Struct create_I2C_Structure(void);

void I2C_START(pI2C_Struct _i2c_self);				//发送IIC开始信号
void I2C_STOP(pI2C_Struct _i2c_self);	  			//发送IIC停止信号
uint8_t I2C_WAIT_ACK(pI2C_Struct _i2c_self);
void I2C_ACK(pI2C_Struct _i2c_self);
void I2C_NACK(pI2C_Struct _i2c_self);

int8_t i2c_protecol_send_bytes(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t* pdata_buffer,
    uint8_t data_lenth
);

int8_t i2c_protecol_read_bytes(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t *reg_data,
    uint8_t data_lenth
);

int8_t i2c_protecol_send_byte(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t data_byte
);

int8_t i2c_protecol_read_byte(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t *data_byte
);

#endif //_I2C_H_