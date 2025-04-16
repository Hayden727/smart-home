/*
 * @Description: 
 * @FilePath: /LQ_TC26xB_LIBtasking/src/AppSw/Tricore/MY/API/MPU6050.c
 * @Author: Wei Zhou  //NB
 * @Github: https://github.com/zromyk
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-05-22 21:00:17
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */ 

#include "mpu6050.h"
#include "math.h"
//#include "My_Math.h"
#include "key.h"
#include "oled.h"
//#include "KalmanFilter.h"

MPUNode mpu = 
{
    .elem.kp    = 1.0f,     /*比例增益*/
    .elem.ki    = 0.001f,   /*积分增益*/
    .elem.exInt = 0.0f,
    .elem.eyInt = 0.0f,
    .elem.ezInt = 0.0f,     /*积分误差累计*/

    .elem.q0 = 1.0f,        /*四元数*/
    .elem.q1 = 0.0f,
    .elem.q2 = 0.0f,
    .elem.q3 = 0.0f
};

// 定义MPU6050内部地址
#define MPU6050_DEV_ADDR        0x68    //IIC写入时的地址字节数据，+1为读取

#define SMPLRT_DIV              0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG                  0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG             0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG            0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_CONFIG2           0X1D    //加速度计低通滤波器 0x06 5hz
#define INT_PIN_CFG             0x37    //设置9250辅助I2C为直通模式寄存器
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define TEMP_OUT_H              0X41    //温度计输出数据
#define TEMP_OUT_L              0X42
#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44    
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48
#define USER_CTRL               0x6A    //关闭9250对辅助I2C设备的控制,用户配置当为0X10时使用SPI模式
#define PWR_MGMT_1              0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I                0x75    //器件ID MPU6050默认ID为0X71
#define SlaveAddress            0xD0    //IIC写入时的地址字节数据，+1为读取

// 简单延时
void iic_delay(void)
{
    uint16_t t = 10;
    while(t--);
}

// 产生IIC起始信号
void mpu6050_iic_start(void)
{
    MPU6050_SDA_OUT();
    MPU6050_SDA_H();
    MPU6050_SCL_H();   
    iic_delay();
    MPU6050_SDA_L();
    iic_delay();
    MPU6050_SCL_L();   //为啥要拉低时钟线？
}

// 产生IIC停止信号
void mpu6050_iic_stop(void)   //时钟线保持高电平的同时拉高数据线
{
    MPU6050_SDA_OUT();   
		MPU6050_SCL_L();  
    MPU6050_SDA_L();
    iic_delay();
    MPU6050_SCL_H();// 不延迟一下吗？
    MPU6050_SDA_H();
    iic_delay();
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t mpu6050_iic_wait_ack(void)
{
    uint8_t err_time = 0;
    MPU6050_SDA_IN();
    MPU6050_SDA_H(); iic_delay();   //都作为输入了为啥要设置电平高低
    MPU6050_SCL_H(); iic_delay();
		while (MPU6050_READ_SDA())   //在SDA设置为输入的时候还要保持高电位来等待低电位的到来？如果不设置高电位的话默认是什么电位
    {
        ++err_time;
        if (err_time > 250)   //重试250次？
        {
            mpu6050_iic_stop();
            return 1;
        }
    }
    MPU6050_SCL_L();
    return 0;
}

// 产生ACK应答
void mpu6050_iic_ack(void)
{
    MPU6050_SCL_L();
    MPU6050_SDA_OUT();
    MPU6050_SDA_L();
    iic_delay();
    MPU6050_SCL_H();
    iic_delay();
    MPU6050_SCL_L();
}

// 不产生ACK应答
void mpu6050_iic_nack(void)
{
    MPU6050_SCL_L();
    MPU6050_SDA_OUT();
    MPU6050_SDA_H();
    iic_delay();
    MPU6050_SCL_H();
    iic_delay();
    MPU6050_SCL_L();
}

// IIC发送一个字节
void mpu6050_iic_send_byte(uint8_t txd)  //get it
{
    uint8_t t;
    MPU6050_SDA_OUT();
    MPU6050_SCL_L();
    for (t=0; t<8; ++t)
    {
        if ((txd&0x80) >> 7)    MPU6050_SDA_H();   
        else                    MPU6050_SDA_L();
        txd <<= 1;
        iic_delay();              //保证有效电平
        MPU6050_SCL_H();
        iic_delay();
        MPU6050_SCL_L();
        iic_delay();
    }
    mpu6050_iic_wait_ack(); //没有判断有没有应答成功啊
}

// IIC读一个字节
// ack=1时，发送ACK；ack=0时，发送nACK
uint8_t mpu6050_iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    MPU6050_SDA_IN();
    for (i=0; i<8; ++i)
    {
        MPU6050_SCL_L();
        iic_delay();
        MPU6050_SCL_H();
        receive <<= 1;
        if (MPU6050_READ_SDA()) ++receive;
        iic_delay();
    }
    if (!ack)
        mpu6050_iic_nack();
    else
        mpu6050_iic_ack();
    return receive;
}

// 向I2C设备寄存器写入一个字节数据
void mpu6050_i2c_write_reg(uint8_t reg_addr, uint8_t reg_data)
{
    mpu6050_iic_start();                    // 发送起始信号
		mpu6050_iic_send_byte(SlaveAddress);    // 发送设备地址+写信号   
    mpu6050_iic_send_byte(reg_addr);        // 内部寄存器地址
    mpu6050_iic_send_byte(reg_data);        // 内部寄存器数据
    mpu6050_iic_stop();                     // 发送停止信号
}

// 向I2C设备寄存器读取一个字节数据
uint8_t mpu6050_i2c_read_reg(uint8_t reg_addr)
{
    uint8_t reg_data;
    mpu6050_iic_start();                    // 发送起始信号
    mpu6050_iic_send_byte(SlaveAddress);    // 发送设备地址+写信号
    mpu6050_iic_send_byte(reg_addr);        // 发送存储单元地址，从0开始
    mpu6050_iic_start();                    // 发送起始信号
    mpu6050_iic_send_byte(SlaveAddress+1);  // 发送设备地址+读信号
    reg_data = mpu6050_iic_read_byte(0);    // 读出寄存器数据  0   NACK   1  ACK
    mpu6050_iic_stop();                     // 发送停止信号
    return reg_data;
}

// 向I2C设备寄存器读取多个字节数据
// dev_add  设备地址(低七位地址)
// reg      寄存器地址
// dat_add  数据保存的地址指针
// num      读取字节数量
void mpu6050_i2c_read_regs(uint8_t reg_addr, uint8_t *dat_add, uint8_t num)
{
    mpu6050_iic_start();                    // 发送起始信号
    mpu6050_iic_send_byte(SlaveAddress);    // 发送设备地址+写信号
    mpu6050_iic_send_byte(reg_addr);        // 内部寄存器地址
    
    mpu6050_iic_start();                    // 发送起始信号//？？
    mpu6050_iic_send_byte(SlaveAddress+1);  // 发送设备地址+读信号
    while(--num)
    {
        *dat_add = mpu6050_iic_read_byte(1);// 读出寄存器数据  0   NACK   1  ACK
        dat_add++;
    }
    *dat_add = mpu6050_iic_read_byte(0);    // 读出寄存器数据  0   NACK   1  ACK
    mpu6050_iic_stop();                     // 发送停止信号
}

/**
 * @description: 初始化MPU6050
 * 调用该函数前，请先调用模拟IIC的初始化
 * 
 *  ACCEL_CONFIG
 *      0x00: 2g
 *      0x08: 4g
 *      0x10: 8g
 *      0x18: 16g
 *  GYRO_CONFIG:
 *      0x00: 250deg/s
 *      0x08: 500deg/s
 *      0x10: 1000deg/s 
 *      0x18: 2000deg/s 
 */
#define ACC_SENSITIVITY     (4095.875f) // 32767.0f / 8.0f    //??不懂
#define GYRO_SENSITIVITY    (16.384f)   // 131.072 / 65.536f / 32.768f / 16.384f

#define DEG2RAD     0.017453293f    /* 度转弧度 π/180 */
#define RAD2DEG     57.29578f       /* 弧度转度 180/π */

void MPU6050_Init(void)
{
    int i = 10000;
    MPU6050_SCL_OUT();
    MPU6050_SDA_OUT();
    
    //gpio_init(MPU6050_SCL_PIN, GPO, 1);
    //gpio_init(MPU6050_SDA_PIN, GPO, 1);
    
    MPU6050_SCL_H();
    MPU6050_SDA_H();

    

    for(i = 0; i < 10000; i++);
    mpu6050_i2c_write_reg(PWR_MGMT_1, 0x00);      // 解除休眠状态
    for(i = 0; i < 10000; i++);
    mpu6050_i2c_write_reg(SMPLRT_DIV, 0x07);      // 采样率, 8kHz / (1 + SMPLRT_DIV)
    for(i = 0; i < 10000; i++);
    mpu6050_i2c_write_reg(CONFIG, 0x01);
    mpu6050_i2c_write_reg(ACCEL_CONFIG, 0x00);
    //mpu6050_i2c_write_reg(ACCEL_CONFIG2, 0x06);   // 加速度采样频率460HZ
    mpu6050_i2c_write_reg(GYRO_CONFIG, 0x18);
    mpu6050_i2c_write_reg(USER_CTRL, 0x00);
    mpu6050_i2c_write_reg(INT_PIN_CFG, 0x02);
    
//    OLED_P6x8Int(0, 0,  mpu6050_i2c_read_reg(PWR_MGMT_1), 5);
//    OLED_P6x8Int(1, 0,  mpu6050_i2c_read_reg(SMPLRT_DIV), 5);
//    OLED_P6x8Int(2, 0,  mpu6050_i2c_read_reg(CONFIG), 5);
//    OLED_P6x8Int(3, 0,  mpu6050_i2c_read_reg(ACCEL_CONFIG), 5);
//    OLED_P6x8Int(4, 0,  mpu6050_i2c_read_reg(ACCEL_CONFIG2), 5);
//    OLED_P6x8Int(5, 0,  mpu6050_i2c_read_reg(GYRO_CONFIG), 5);
//    OLED_P6x8Int(6, 0,  mpu6050_i2c_read_reg(USER_CTRL), 5);
//    OLED_P6x8Int(7, 0,  mpu6050_i2c_read_reg(INT_PIN_CFG), 5);
}

/**
 * @description: 采集一次加速度计的值
 */
static void GetAccData(MPUNode *mpu)
{
    uint8_t dat[6];
    
    mpu6050_i2c_read_regs(ACCEL_XOUT_H, dat, 6);  
    mpu->acc.x = (int16_t)((uint16_t)dat[0]<<8 | dat[1]);
    mpu->acc.y = (int16_t)((uint16_t)dat[2]<<8 | dat[3]);
    mpu->acc.z = (int16_t)((uint16_t)dat[4]<<8 | dat[5]);

    mpu->accRaw.x  = mpu->acc.x; // 向前的加速度
    mpu->accRaw.y  = mpu->acc.y; // 左右的加速度
    mpu->accRaw.z  = mpu->acc.z; // 向下的加速度
	
		
}

/**
 * @description: 采集一次陀螺仪的值
 */
static void GetGyroData(MPUNode *mpu)
{
    uint8_t dat[6];
    
    mpu6050_i2c_read_regs(GYRO_XOUT_H, dat, 6);  
    mpu->gyro.x = (int16_t)((uint16_t)dat[0]<<8 | dat[1]);
    mpu->gyro.y = (int16_t)((uint16_t)dat[2]<<8 | dat[3]);
    mpu->gyro.z = (int16_t)((uint16_t)dat[4]<<8 | dat[5]);
	
    mpu->gyro.x = mpu->gyro.x + (0) ;    //系统误差
    mpu->gyro.y = mpu->gyro.y + (0);
    mpu->gyro.z = mpu->gyro.z + (-35);
	
    mpu->gyro.x = _abs(mpu->gyro.x) < 25 ? 0 : mpu->gyro.x;  //数据太小的话就直接当作0(静止)
    mpu->gyro.y = _abs(mpu->gyro.y) < 25 ? 0 : mpu->gyro.y;
    mpu->gyro.z = _abs(mpu->gyro.z) < 25 ? 0 : mpu->gyro.z; 
		
    mpu->gyroRaw.x = mpu->gyro.x/GYRO_SENSITIVITY;
    mpu->gyroRaw.y = mpu->gyro.y/GYRO_SENSITIVITY;
    mpu->gyroRaw.z = mpu->gyro.z/GYRO_SENSITIVITY;
    
    mpu->pitch.gyro = mpu->gyroRaw.y;
    
}

static void YJHB_Acc_Pitch(MPUNode* mpu,float dt)   //一阶互补算法求Pitch角
{
	float k1=0.03f;  //加速度的权重
	float acc_m_pitch;  //加速度求出来的角度
	
	acc_m_pitch = myMath_fast_atan2(mpu->accRaw.x,mpu->accRaw.z)*RAD2DEG;
	
	mpu->pitch.angle = k1*acc_m_pitch + (1-k1)*( mpu->pitch.angle + mpu->gyroRaw.y * dt );//加权   角速度求积分（累加）
}


void MPU6050_GetInfo(float dt)
{
    GetAccData(&mpu);
    GetGyroData(&mpu);
		//Kalman_Filter(&mpu,&kal,dt);
		YJHB_Acc_Pitch(&mpu,dt);
}
