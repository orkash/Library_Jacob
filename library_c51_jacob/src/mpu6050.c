/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: mpu6050.c  
* 文件标识: 见配置管理计划书
* 摘    要: MPU6050三轴陀螺仪相关函数的实现
* 函    数: MPU6050_Init() - 初始化MPU6050
*           MPU6050_GetData() - 合成从mpu6050内部寄存器读取到的数据
*           MPU6050_CalcResult() - 计算实际加速度、角速度、温度结果
* 
* 当前版本: E0.0.1
* 作    者: Jacob Peng
* 完成日期: 2015年3月2日
* 更改内容: E0.0.1, 完成了I2C相关8个函数, 2015年3月2日
* 
* 取代版本: None
* 原作者  : Unknown
* 完成日期: Unknown
*/
#include "../include/mpu6050.h"

#include "../include/i2c.h"

/************************************************ 
* 函  数: MPU6050_Init(void)
* 功  能: 初始化MPU6050
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void MPU6050_Init(void)
{
	I2C_WriteByteToSlave(PWR_MGMT, 0x00);	// 解除休眠状态
	I2C_WriteByteToSlave(SMPLRT_DIV, 0x07);
	I2C_WriteByteToSlave(CONFIG, 0x06);
	I2C_WriteByteToSlave(GYRO_CONFIG, 0x18);
	I2C_WriteByteToSlave(ACCEL_CONFIG, 0x01);
} // end of MPU6050_Init().

/************************************************ 
* 函  数: MPU6050_GetData(unsigned char)
* 功  能: 合成从mpu6050内部寄存器读取到的数据
*
* 参  数: reg_address - mpu6050内部寄存器地址
*
* 返回值: 合成以后的数据, int型
*************************************************/
int MPU6050_GetData(unsigned char reg_address)
{
	unsigned char high_8bits = 0, low_8bits = 0;
  
	high_8bits = I2C_ReadByteFromSlave(reg_address);
	low_8bits = I2C_ReadByteFromSlave(reg_address + 1);
  
	return (high_8bits << 8) + low_8bits; // 合成数据
} // end of MPU6050_GetData().

/************************************************ 
* 函  数: MPU6050_CalcResult(int, unsigned char)
* 功  能: 计算实际加速度、角速度、温度结果
*
* 参  数: input_data - mpu6050原始数据
*         channel - 计算通道, 0: 加速度, 1: 角速度, 2: 温度
*
* 返回值: 计算完成以后的实际结果, float型
*************************************************/
#if ENABLE_MPU6050_CalcResult

float MPU6050_CalcResult(int input_data, unsigned char channel)
{
  /* 计算公式
  // 加速度计算
  读出加速度数据/32768.0*16g(g为重力加速度，可取9.8m/s2)
  // 角速度计算
  读出角速度数据/32768.0*2000(°/s)
  // 角度计算
  读出角度数据/32768.0*180(°)
  // 温度计算
  读出温度数据/340.0+36.25℃
  */     
  float result = 0.0;
  
  if (channel != 0 && channel != 1 && channel != 2) return -1;  // 通道不合法
  switch (channel)
  {
    case ACCEL_CHANNEL: { // 利用加速度计算角度
      result = (input_data - MPU6050_ZERO_ACCELL)/16384.0;
      // result = result*180.0/PI/0.92; // 弧度转换为度，角度较小时，可这样近似计算
      result = asin(result)*180.0/PI; // 反正弦函数计算出弧度，然后转换为度
      break;
    } // case
    case GYRO_CHANNEL: { // 角速度
      // 去除零点偏移，计算角速度值，负号为方向处理  
      result = -(input_data - MPU6050_ZERO_GYRO)/16.4;    
      break;
    } // case
    case TEMP_CHANNEL: { // 温度
      result = input_data/340.0+36.25;
      break;
    } // case
    default: {
      break;
    } // default
  } // end of switch ().
  
  return result;
} // end of MPU6050_CalcResult().

#endif // #if ENABLE_MPU6050_CalcResult
