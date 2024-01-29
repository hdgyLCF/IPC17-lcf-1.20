#include"i2c.h"
#include"stdlib.h"
#include"OLED.h"
#define NULL 0

// Arr_pI2C_Struct create_Arr_I2C_Structure(int Arr_n){
//      Arr_pI2C_Struct Arr_i2c_slef = NULL;
//     Arr_i2c_slef = (Arr_pI2C_Struct)malloc(Arr_n * sizeof(pI2C_Struct));
//     for(int i = 0; i < Arr_n; i++){
//         Arr_i2c_slef[i] = (pI2C_Struct)malloc(sizeof(I2C_Struct));
//         Arr_i2c_slef[i]->pin_write_bit =  pin_write_bit;
//         Arr_i2c_slef[i]->send_byte = i2c_protecol_send_byte;
//         Arr_i2c_slef[i]->read_byte = i2c_protecol_read_byte;
//         Arr_i2c_slef[i]->send_bytes = i2c_protecol_send_bytes;
//         Arr_i2c_slef[i]->read_bytes = i2c_protecol_read_bytes;
//     }

//     //Arr_i2c_slef[i]->sda_pin_write_bit = /*the sda_pin function you have established*/;
//     //Arr_i2c_slef[i]->scl_pin_write_bit = /*the scl_pin function you have established*/;
//     //Arr_i2c_slef[i]->sda_pin_read_bit = /*the scl_pin function you have established*/;
//     //e.g.
 
//     // Set each of your pin functions in turn
//     /**
//         //Arr_i2c_slef[0]->sda_pin_write_bit = 
//         //Arr_i2c_slef[0]->scl_pin_write_bit = 
//         //Arr_i2c_slef[0]->sda_pin_read_bit =
//         ...
//     **/
//     return Arr_i2c_slef;
// }

pI2C_Struct create_I2C_Structure(void){
    pI2C_Struct _i2c_slef = NULL;
    _i2c_slef = (pI2C_Struct)malloc(sizeof(I2C_Struct));    
    _i2c_slef->pin_write_bit =  pin_write_bit;
    _i2c_slef->send_byte = i2c_protecol_send_byte;
    _i2c_slef->read_byte = i2c_protecol_read_byte;
    _i2c_slef->send_bytes = i2c_protecol_send_bytes;
    _i2c_slef->read_bytes = i2c_protecol_read_bytes;
    return _i2c_slef;
}


uint8_t msg_delay(uint8_t msg){
    switch (msg) {
        case MSG_DELAY_100NANO: // delay 100 nano seconds
            I2C_Delay_100ns();
            break;
        case MSG_DELAY_10MICRO: // delay 10 micro seconds
            I2C_Delay_us(10);
            break;
        case MSG_DELAY_MILLI:   // delay 1 milli second
            I2C_Delay_ms(1);
            break;
        case MSG_DELAY_I2C:     //I2C speed
            I2C_Delay_us(1);    
            break;             
        default:
            break;
    }
    return 1;
}

uint8_t pin_write_bit(pI2C_Struct _i2c_self, uint8_t msg, uint8_t arg_int){
    switch (msg) {
        case MSG_GPIO_I2C_CLOCK: 
            arg_int > 0 ? 
                _i2c_self->scl_pin_write_bit(1) 
            : 
                _i2c_self->scl_pin_write_bit(0)
            ;
            break;
        case MSG_GPIO_I2C_DATA:     
            arg_int > 0 ? 
                _i2c_self->sda_pin_write_bit(1) 
            : 
                _i2c_self->sda_pin_write_bit(0)
            ;
            break;
        default:
            break;
    }
    return 1;
 }


void I2C_START(pI2C_Struct _i2c_self){
    SDA_OUT();
    //先拉高时钟线
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_CLOCK, HIGH);
    //再拉高数据线
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_DATA, HIGH);

    //msg_delay(MSG_DELAY_100NANO);

    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_DATA, LOW);

   // msg_delay(MSG_DELAY_100NANO);

    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, LOW);

    //msg_delay(MSG_DELAY_100NANO);
}

void I2C_STOP(pI2C_Struct _i2c_self){
    SDA_OUT();
    //先拉低数据线
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_DATA,LOW);
    //再拉高时钟线
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_CLOCK, HIGH);

    //msg_delay(MSG_DELAY_100NANO);

    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_DATA, HIGH);

    //msg_delay(MSG_DELAY_100NANO);

    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, LOW);

    //msg_delay(MSG_DELAY_100NANO);
}


void I2C_ACK(pI2C_Struct _i2c_self){
    //SCL线先拉低，方便数据线上变化数据
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_CLOCK, LOW);
    SDA_IN();
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_DATA, LOW);
   
    //msg_delay(MSG_DELAY_100NANO);

    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, HIGH);

    msg_delay(MSG_DELAY_I2C);

    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, LOW);
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_DATA, HIGH);
}

void I2C_NACK(pI2C_Struct _i2c_self){
        //SCL线先拉低，方便数据线上变化数据
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_CLOCK, LOW);
    SDA_IN();
    _i2c_self->pin_write_bit(_i2c_self,  MSG_GPIO_I2C_DATA, HIGH);
   
    //msg_delay(MSG_DELAY_100NANO);

    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, HIGH);

    msg_delay(MSG_DELAY_I2C);

    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, LOW);
    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_DATA, HIGH);
}


uint8_t I2C_WAIT_ACK(pI2C_Struct _i2c_self){
    uint8_t ucErrTime = 0;
    SDA_IN();
    _i2c_self->pin_write_bit(_i2c_self,MSG_GPIO_I2C_DATA, HIGH);
   // msg_delay(MSG_DELAY_100NANO);
    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, HIGH);
   // msg_delay(MSG_DELAY_100NANO);

    while (_i2c_self->sda_pin_read_bit()){
        ucErrTime++;
        msg_delay(MSG_DELAY_100NANO);
        if(ucErrTime > 200){
            I2C_STOP(_i2c_self);
            return 1;
        }
    }
    _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, LOW);
    //msg_delay(MSG_DELAY_100NANO);
    return 0;
}

void I2C_SEND_BYTE(pI2C_Struct _i2c_self, uint8_t data_byte){
    SDA_OUT();
    for (int i = 7; i >= 0; --i){
        uint8_t data_bit =  READ_DATA_BIT(data_byte, i);
        _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_DATA, data_bit);
        //msg_delay(MSG_DELAY_100NANO);
        _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, HIGH);
       // msg_delay(MSG_DELAY_100NANO);
        _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, LOW);
    }  
}

uint8_t I2C_READ_BYTE(pI2C_Struct _i2c_self){
    SDA_IN();
    uint8_t data_byte = 0x00;
    for (int i = 7; i >= 0; --i){
        _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, LOW);
        //msg_delay(MSG_DELAY_100NANO);
        _i2c_self->pin_write_bit(_i2c_self, MSG_GPIO_I2C_CLOCK, HIGH);
        uint8_t data_bit = _i2c_self->sda_pin_read_bit();
        data_byte = OPERATE_DATA_BIT(data_byte, i , data_bit);
       // msg_delay(MSG_DELAY_100NANO);    
    }  
    return data_byte;
}

int8_t i2c_protecol_send_byte(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t data_byte
){
    I2C_START(_i2c_self);
    I2C_SEND_BYTE(_i2c_self, slave_addr & 0xFE);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -1;
    I2C_SEND_BYTE(_i2c_self, register_addr);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -2;
    I2C_SEND_BYTE(_i2c_self, data_byte);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -3;
    I2C_STOP(_i2c_self);
    return 0;
}

int8_t i2c_protecol_read_byte(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t *data_byte
){
    *data_byte = 0x00;
    I2C_START(_i2c_self);
    I2C_SEND_BYTE(_i2c_self, slave_addr & 0xFE);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -1;
    I2C_SEND_BYTE(_i2c_self, register_addr);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -2;
    I2C_START(_i2c_self);
    I2C_SEND_BYTE(_i2c_self, slave_addr | 0x01);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -3;
    *data_byte = I2C_READ_BYTE(_i2c_self);
    I2C_NACK(_i2c_self);
    I2C_STOP(_i2c_self);
    return 0;
}

int8_t i2c_protecol_send_bytes(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t* pdata_buffer,
    uint8_t data_lenth
){
    I2C_START(_i2c_self);
    I2C_SEND_BYTE(_i2c_self, slave_addr & 0xFE);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -1;
    I2C_SEND_BYTE(_i2c_self, register_addr);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -2;
    for(int i =0; i < data_lenth; ++i){
        I2C_SEND_BYTE(_i2c_self, pdata_buffer[i]);
        if(I2C_WAIT_ACK(_i2c_self)) 
            return -3;
    }
    I2C_STOP(_i2c_self);
    return 0;
}

int8_t i2c_protecol_read_bytes(
    pI2C_Struct _i2c_self, 
    uint8_t slave_addr, 
    uint8_t register_addr,
    uint8_t *reg_data,
    uint8_t data_lenth
    
){  
    I2C_START(_i2c_self);
    I2C_SEND_BYTE(_i2c_self, slave_addr & 0xFE);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -1;
    I2C_SEND_BYTE(_i2c_self, register_addr);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -2;
    I2C_START(_i2c_self);
    I2C_SEND_BYTE(_i2c_self, slave_addr | 0x01);
    if(I2C_WAIT_ACK(_i2c_self)) 
        return -3;
    for(int i = 0; i < data_lenth; ++i){
        reg_data[i] = I2C_READ_BYTE(_i2c_self);
        if(i == data_lenth - 1){
            I2C_NACK(_i2c_self);
        }else{
            I2C_ACK(_i2c_self);
        }
        // i == data_lenth - 1 ? : ;
    }
    I2C_STOP(_i2c_self);
    return 0;
}