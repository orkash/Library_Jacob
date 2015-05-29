/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: mpu6050.h 
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
#ifndef MPU6050_H_
#define MPU6050_H_

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif  // end of #ifndef

// 是否使用MPU6050_CalcResult()函数,
// 可以消除编译器未使用某一个函数的警告。
#define ENABLE_MPU6050_CalcResult TRUE

/* 定义MPU6050内部地址 */
// 陀螺仪采样率，典型值：0x07(125Hz)
#define SMPLRT_DIV 0x19 
// 低通滤波频率，典型值：0x06(5Hz)
#define CONFIG 0x1A 
// 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define GYRO_CONFIG 0x1B  
// 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_CONFIG 0x1C 
// 加速度输出地址
#define ACCEL_XOUT_ADDR_H 0x3B
#define ACCEL_XOUT_ADDR_L 0x3C
#define ACCEL_YOUT_ADDR_H 0x3D
#define ACCEL_YOUT_ADDR_L 0x3E
#define ACCEL_ZOUT_ADDR_H 0x3F
#define ACCEL_ZOUT_ADDR_L 0x40
// 角速度输出地址
#define GYRO_XOUT_ADDR_H 0x43
#define GYRO_XOUT_ADDR_L 0x44 
#define GYRO_YOUT_ADDR_H 0x45
#define GYRO_YOUT_ADDR_L 0x46
#define GYRO_ZOUT_ADDR_H 0x47
#define GYRO_ZOUT_ADDR_L 0x48
// 温度输出地址
#define TEMP_OUT_ADDR_H 0x41
#define TEMP_OUT_ADDR_L 0x42
// 其他寄存器
#define PWR_MGMT 0x6B // 电源管理，典型值：0x00(正常启用)
// 加速度、角速度输出
#define ACCEL_XOUT MPU6050_GetData(ACCEL_XOUT_ADDR_H)
#define ACCEL_YOUT MPU6050_GetData(ACCEL_YOUT_ADDR_H)
#define ACCEL_ZOUT MPU6050_GetData(ACCEL_ZOUT_ADDR_H)
#define GYRO_XOUT MPU6050_GetData(GYRO_XOUT_ADDR_H)
#define GYRO_YOUT MPU6050_GetData(GYRO_YOUT_ADDR_H)
#define GYRO_ZOUT MPU6050_GetData(GYRO_ZOUT_ADDR_H)
// 温度输出
#define TEMP_OUT MPU6050_GetData(TEMP_OUT_ADDR_H)
// 计算实际结果通道
#define ACCEL_CHANNEL (0)
#define GYRO_CHANNEL (1)
#define TEMP_CHANNEL (2)
// MPU6050的零点校准值
#define MPU6050_ZERO_ACCELL (-465.522)
#define MPU6050_ZERO_GYRO (-58.8427)

// 初始化MPU6050
void MPU6050_Init(void);
// 合成数据
int MPU6050_GetData(unsigned char reg_address);
// 计算实际加速度、角速度、温度结果
#if ENABLE_MPU6050_CalcResult
float MPU6050_CalcResult(int input_data, unsigned char channel);
#endif // #if ENABLE_MPU6050_CalcResult

#endif // end of #ifndef MPU6050_H_
