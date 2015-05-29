/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: sdhc.c
* 文件标识: 见配置管理计划书
* 摘    要: SDHC card操作相关函数, 2~32G, 兼容0~2G SDSC card.
* 函    数: SD_SPI_ReadWriteCmd() - 通过写入命令, 读取相应返回信息
*           SD_SPI_SpeedLow() - SD卡初始化的时候, 需要低速
*           SD_SPI_SpeedHigh() - SD卡正常工作的时候, 可以高速了
*           SD_SPI_Init() - SD对应IO口初始化
*           SD_WaitReady() - 等待SD卡准备好
*           SD_DisSelect() - 取消片选, 释放SPI总线
*           SD_Select() - 选择SD卡, 等待卡准备完成
*           SD_GetResponse() - 等待SD卡回应
*           SD_SendCmd() - 向SD卡发送一个命令
*           SD_ReadBlock() - 从sd卡读取一个数据包的内容
*           SD_SendBlock() - 向sd卡写入一个数据块的内容, 512字节
*           SD_Init() - 初始化SD卡
*           SD_GetCID() - 获取SD卡的CID信息, 包括制造商信息
*           SD_GetCSD() - 获取SD卡的CSD信息, 包括容量和速度信息
*           SD_GetSectorCount() - 获取SD卡的总扇区数
*           SD_ReadDisk() - 从SD卡指定扇区读取数据
*           SD_WriteDisk() - 向SD卡指定扇区写入数据
*           SD_Test() - 测试SD, 读取SD卡指定扇区内容, 通过串口输出
* 
* 当前版本: A0.0.2
* 作    者: Jacob Peng
* 完成日期: 2015年5月20日
* 更改内容: A0.0.1, SD卡初始化通过, 但不能读取SD卡存储器大小, 2015年5月19日
*           A0.0.2, 通过降低AHB时钟, 完成了SD卡容量大小的读取, 2015年5月20日
* 
* 取代版本: A0.0.1
* 原作者  : Jacob Peng
* 完成日期: 2015年5月19日
*/
#include "../include/sdhc.h"

#include <stm32f10x_spi.h>
#include <stdio.h>  // Declare printf().

#include "../include/spi_hardware.h" // 使用硬件SPI

u8 g_sd_type = 0x00;  // SD卡类型

/************************************************ 
* 函  数: SD_SPI_ReadWriteCmd(u8)
* 功  能: SD, 通过写入命令, 读取相应返回信息
*
* 参  数: cmd - 待写入的命令
*
* 返回值: 读到的返回数据
*************************************************/
u8 SD_SPI_ReadWriteCmd(u8 cmd)
{
  return SPI1_ReadWriteCmd(cmd);
} // SD_SPI_ReadWriteCmd()

/************************************************ 
* 函  数: SD_SPI_SpeedLow(void)
* 功  能: SD卡初始化的时候, 需要低速
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void SD_SPI_SpeedLow(void)
{
  SPI1_SetSpeed(SPI_BaudRatePrescaler_256); // 设置到低速模式  
} // SD_SPI_SpeedLow()

/************************************************ 
* 函  数: SD_SPI_SpeedHigh(void)
* 功  能: SD卡正常工作的时候, 可以高速了
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void SD_SPI_SpeedHigh(void)
{
  SPI1_SetSpeed(SPI_BaudRatePrescaler_2); // 设置到高速模式  
} // SD_SPI_SpeedHigh()

/************************************************ 
* 函  数: SD_SPI_Init(void)
* 功  能: SD对应IO口初始化
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void SD_SPI_Init(void)
{
  SPI1_Init();
} // SD_SPI_Init()

/************************************************ 
* 函  数: SD_WaitReady(void)
* 功  能: 等待SD卡准备好
*
* 参  数: 无
*
* 返回值: 0, 已准备好; 1, 未准备完成
*************************************************/
u8 SD_WaitReady(void)
{
  u32 i = 0;

  do {
    if (0xFF == SD_SPI_ReadWriteCmd(0xFF)) return 0; // 准备完成
    ++i;
  } while (i < 0xFFFFFF); // 等待

  return 1;
} // SD_WaitReady()

/************************************************ 
* 函  数: SD_DisSelect(void)
* 功  能: 取消片选, 释放SPI总线
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void SD_DisSelect(void)
{
  SD_SPI1_CS = 1;
  SD_SPI_ReadWriteCmd(0xFF); // 提供额外的8个时钟
} // SD_DisSelect()

/************************************************ 
* 函  数: SD_Select(void)
* 功  能: 选择SD卡, 等待卡准备完成
*
* 参  数: 无
*
* 返回值: 0, 成功; 1, 失败
*************************************************/
u8 SD_Select(void)
{
  SD_SPI1_CS = 0;
  if (0 == SD_WaitReady()) 
  {
    // printf("sd card is ready!\r\n");
    return 0;  // 等待成功
  } // if
  SD_DisSelect();
  // printf("Failed to wait sd card ready!\r\n");

  return 1; // 等待失败
} // SD_Select()

/************************************************ 
* 函  数: SD_GetResponse(u8)
* 功  能: 等待SD卡回应
*
* 参  数: cmd - 要得到的回应命令
*
* 返回值: 0, 成功获取了回应值; 1, 获取回应值失败
*************************************************/
u8 SD_GetResponse(u8 cmd)
{
  u16 count = 0xFFFF; // 等待次数

  // 等待得到准确的回应
  while ((SD_SPI_ReadWriteCmd(0xFF) != cmd) && count) 
    --count;
  if (0 == count)
    return SD_RESPONSE_FAILURE; // 获取回应失败
  else 
    return SD_RESPONSE_NO_ERROR;  // 成功获得回应
} // SD_GetResponse()

/************************************************ 
* 函  数: SD_ReadBlock(u8, u16)
* 功  能: 从sd卡读取一个数据包的内容
*
* 参  数: buf - 数据缓存区
*         len - 要读取的数据长度
*
* 返回值: 0, 成功; 1, 失败
*************************************************/
u8 SD_ReadBlock(u8 buf[], u16 len)
{           
  u16 i = 0;

  // 等待SD卡发回数据起始令牌0xFE
  if (SD_GetResponse(0xFE)) return 1;
  // 接收数据
  for (i = 0; i < len; ++i)
  {
    buf[i] = SPI1_ReadWriteCmd(0xFF);
  } // for
  // 下面是2个伪CRC(dummy CRC)
  SD_SPI_ReadWriteCmd(0xFF);
  SD_SPI_ReadWriteCmd(0xFF);  

  return 0; // 读取成功
} // SD_ReadBlock()

/************************************************ 
* 函  数: SD_SendBlock(const u8 [], u8)
* 功  能: 向sd卡写入一个数据块的内容, 512字节
*
* 参  数: buf - 数据缓存区
*         cmd - 指令
*
* 返回值: 0, 成功; !0, 失败
*************************************************/
u8 SD_SendBlock(const u8 buf[], u8 cmd)
{ 
  u16 i = 0;

  if (SD_WaitReady()) return 1; // 等待准备失效
  SD_SPI_ReadWriteCmd(cmd);
  // 不是结束指令
  if (cmd != 0XFD) 
  {
    for (i = 0; i < 512; ++i) SPI1_ReadWriteCmd(buf[i]); // 提高速度, 减少函数传参时间
    SD_SPI_ReadWriteCmd(0xFF); // 忽略crc
    SD_SPI_ReadWriteCmd(0xFF);
    if((SD_SPI_ReadWriteCmd(0xFF) & 0x1F) != 0x05) return 2; // 响应错误                                 
  } // if

  return 0; // 发送成功
} // SD_SendBlock()

/************************************************ 
* 函  数: SD_SendCmd(u8, u32, u8)
* 功  能: 向SD卡发送一个命令
*
* 参  数: cmd - 命令
*         argv - 命令参数
*         crc - crc校验值
*
* 返回值: SD卡返回的响应值
*************************************************/                           
u8 SD_SendCmd(u8 cmd, u32 argv, u8 crc)
{
  u8 ret_status = 0x00;  
  u8 retry_times = 0x00; 

  SD_DisSelect(); // 取消上次片选
  if (SD_Select()) 
  {
    // printf("Sorry, SD_SendCmd, CS was disabled!\r\n");
    return 0xFF;  // 片选失效 
  } // if
  //printf("SD_SendCmd, CS was enabled!\r\n");
  // 发送命令
  SD_SPI_ReadWriteCmd(cmd | 0x40); // 分别写入命令
  SD_SPI_ReadWriteCmd(argv >> 24);
  SD_SPI_ReadWriteCmd(argv >> 16);
  SD_SPI_ReadWriteCmd(argv >> 8);
  SD_SPI_ReadWriteCmd(argv);    
  SD_SPI_ReadWriteCmd(crc); 
  // Skip a stuff byte when stop reading
  if (SD_CMD12 == cmd) SD_SPI_ReadWriteCmd(0xFF);  
  // 等待响应, 或超时退出
  retry_times = 0x1F;
  do {
    ret_status = SD_SPI_ReadWriteCmd(0xFF);
  } while ((ret_status & 0x80) && retry_times--);
  //printf("SD_SendCmd, cmd: %2d, argv:%d, ret_status:%d.\r\n", cmd, argv, ret_status);

  // 返回状态值
  return ret_status;
} // SD_SendCmd()

/************************************************ 
* 函  数: SD_Init(void)
* 功  能: 初始化SD卡
*
* 参  数: 无
*
* 返回值: 0, 成功; !0, 失败
*************************************************/
u8 SD_Init(void)
{
  u8 i = 0;
  u8 ret_value = 0; // 存放SD卡的返回值
  u16 retry_times = 0;  // 超时次数
  u8 buf[4] = {0};  

  SD_SPI_Init();  // 初始化IO
  SD_SPI_SpeedLow();  // 设置到低速模式 
  // 发送最少74个脉冲
  for (i = 0; i < 10; ++i) SD_SPI_ReadWriteCmd(0xFF);  
  retry_times = 20;
  do {
    ret_value = SD_SendCmd(SD_CMD0, 0, 0x95);  // 进入idle状态
    // printf("SD_SendCmd return value is: 0x%02X.\r\n", ret_value);
  } while ((ret_value != 0x01) && retry_times--);
  g_sd_type = SD_TYPE_ERR;  // 默认无卡
  // printf("SD_Init, SD_SendCmd return value: 0x%02X.\r\n", ret_value);
  if (0x01 == ret_value)
  {    
    if (1 == SD_SendCmd(SD_CMD8, 0x01AA, 0x87))  // SD V2.0
    {
      // Get trailing return value of R7 resp
      for (i = 0; i < 4; ++i) buf[i] = SD_SPI_ReadWriteCmd(0xFF);  
      // 卡是否支持2.7~3.6V
      if (0x01 == buf[2] && 0xAA == buf[3]) 
      {
        retry_times = 0xFFFE;
        do {
          SD_SendCmd(SD_CMD55, 0, 0x01);  // 发送CMD55
          ret_value = SD_SendCmd(SD_CMD41, 0x40000000, 0x01);  // 发送CMD41
        } while (ret_value && retry_times--);
        // 鉴别SD2.0卡版本开始
        if (retry_times && 0 == SD_SendCmd(SD_CMD58, 0, 0x01))
        {
          // 得到OCR值
          for (i = 0; i < 4; ++i) buf[i] = SD_SPI_ReadWriteCmd(0xFF);  
          if (buf[0] & 0x40)
            g_sd_type = SD_TYPE_V2HC;  // 检查CCS
          else 
            g_sd_type = SD_TYPE_V2;   
        } // if
      } // if
    } else {  // SD V1.x or MMC V3.x
      SD_SendCmd(SD_CMD55, 0, 0x01);  // 发送CMD55
      ret_value = SD_SendCmd(SD_CMD41, 0, 0x01);  // 发送CMD41
      if (ret_value <= 1)
      {   
        g_sd_type = SD_TYPE_V1;
        retry_times = 0xFFFE;
        //等待退出IDLE模式
        do {
          SD_SendCmd(SD_CMD55, 0, 0x01);  // 发送CMD55
          ret_value = SD_SendCmd(SD_CMD41, 0, 0x01); // 发送CMD41
        } while (ret_value && retry_times--);
      } else {  // MMC卡不支持CMD55+CMD41识别
        g_sd_type = SD_TYPE_MMC;  // MMC V3
        retry_times = 0xFFFE;
        //等待退出IDLE模式
        do {                         
          ret_value = SD_SendCmd(SD_CMD1, 0, 0x01); // 发送CMD1
        } while (ret_value && retry_times--);  
      } // if else
      if (0 == retry_times || 0 != SD_SendCmd(SD_CMD16, 512, 0x01))
        g_sd_type = SD_TYPE_ERR;  // 错误的卡
    } // if else
  } // if
  SD_DisSelect(); // 取消片选
  SD_SPI_SpeedHigh(); // 高速
  //SD_SPI_ReadWriteCmd(0xFF); // 给SD卡额外提供8个时钟
  printf("SD Card Type: 0x%02x.\r\n", g_sd_type);
  if (g_sd_type) 
    return 0;
  else if (ret_value)
    return ret_value;  

  return 0xAA;  // 其他错误
} // SD_Init()

/************************************************ 
* 函  数: SD_GetCID(u8 [])
* 功  能: 获取SD卡的CID信息, 包括制造商信息
*
* 参  数: cid_data - 存放CID的缓冲区, 至少16 bytes
*
* 返回值: 0, 成功; 1, 失败
*************************************************/                          
u8 SD_GetCID(u8 cid_data[])
{
  u8 ret_status = 0x00;  

  // 发CMD10命令, 读CID
  ret_status = SD_SendCmd(SD_CMD10, 0, 0x01);
  // 接收16个字节的数据 
  if (0x00 == ret_status) ret_status = SD_ReadBlock(cid_data, 16);
  SD_DisSelect(); // 取消片选
  if (ret_status)  return 1;

  return 0;
} // SD_GetCID()

/************************************************ 
* 函  数: SD_GetCSD(u8 [])
* 功  能: 获取SD卡的CSD信息, 包括容量和速度信息
*
* 参  数: csd_data - 存放CSD的缓冲区, 至少16 bytes
*
* 返回值: 0, 成功; 1, 失败
*************************************************/                          
u8 SD_GetCSD(u8 csd_data[])
{
  u8 ret_status = 0x00;     

  // 发CMD9命令, 读CSD
  ret_status = SD_SendCmd(SD_CMD9, 0, 0x01);
  // printf("SD_GetCSD return value0: 0x%02X.\r\n", ret_status);
  // 接收16个字节的数据 
  if (0x00 == ret_status) ret_status = SD_ReadBlock(csd_data, 16);  
  SD_DisSelect(); // 取消片选
  // printf("SD_GetCSD return value1: 0x%02X.\r\n", ret_status);
  if (ret_status)  return 1;

  return 0;
} // SD_GetCSD()

/************************************************ 
* 函  数: SD_GetSectorCount(void)
* 功  能: 获取SD卡的总扇区数. (SD_GetSectorCount() >> 11)为SD卡容量MB
*         这里每扇区的字节数是512, 因为如果不是512, 则初始化不能通过;
*         当然有的SD卡扇区字节数是1024.
*
* 参  数: 无
*
* 返回值: 0, 失败; !0, SD卡的容量(扇区数/512字节)
*************************************************/  
u32 SD_GetSectorCount(void)
{
  u8 n = 0;
  u16 capacity_size = 0;
  u32 capacity = 0;  
  u8 csd_buf[16] = {0};

  // 取CSD信息, 如果期间出错, 返回0
  if (SD_GetCSD(csd_buf) != 0) 
  {
    printf("Failed to get CSD information!\r\n");
    return 0; 
  } // if
  // 如果为SDHC卡, 按照下面方式计算
  if (0x40 == (csd_buf[0] & 0xC0))  // V2.00的卡
  {
    capacity_size = csd_buf[9] + ((u16)csd_buf[8] << 8) + 1;
    capacity = (u32)capacity_size << 10;  // 得到扇区数 
    printf("SD type: V2.0.\r\n");
  } else {  // V1.XX的卡
    n = (csd_buf[5] & 15) + ((csd_buf[10] & 128) >> 7) + ((csd_buf[9] & 3) << 1) + 2;
    capacity_size = (csd_buf[8] >> 6) + ((u16)csd_buf[7] << 2) + ((u16)(csd_buf[6] & 3) << 10) + 1;
    capacity= (u32)capacity_size << (n - 9);  // 得到扇区数   
    printf("SD type: V1.XX.\r\n");
  } // if else
   printf("The capacity is: %dMB\r\n", capacity >> 11);

  return capacity;
} // SD_GetSectorCount()

/************************************************ 
* 函  数: SD_ReadDisk(u8 [], u32, u8)
* 功  能: 从SD卡指定扇区读取数据
*
* 参  数: buf - 数据缓存区
*         sector - 扇区编号
*         sector_size - 扇区数, 可以读取多个扇区
*
* 返回值: 0, 成功; !0, 失败
*************************************************/
u8 SD_ReadDisk(u8 buf[], u32 sector, u8 sector_size)
{
  u8 ret_value = 0x00;
  u8 *ptr = buf;

  if (SD_TYPE_V2HC != g_sd_type) sector <<= 9;  // 转换为字节地址
  if (1 == sector_size)
  {
    ret_value = SD_SendCmd(SD_CMD17, sector, 0x01); // 读命令
    // 指令发送成功
    if (0 == ret_value) ret_value = SD_ReadBlock(ptr, 512);  // 接收512个字节     
  } else {
    ret_value = SD_SendCmd(SD_CMD18, sector, 0X01); // 连续读命令
    do {
      ret_value = SD_ReadBlock(ptr, 512); // 接收512个字节   
      ptr +=512;
    } while (--sector_size && 0 == ret_value);   
    SD_SendCmd(SD_CMD12, 0, 0X01); // 发送停止命令
  } // if else
  SD_DisSelect(); // 取消片选

  return ret_value;
} // SD_ReadDisk()

/************************************************ 
* 函  数: SD_WriteDisk(const u8 [], u32, u8)
* 功  能: 向SD卡指定扇区写入数据
*
* 参  数: buf - 数据缓存区
*         sector - 扇区编号
*         sector_size - 扇区数, 可以写入多个扇区
*
* 返回值: 0, 成功; !0, 失败
*************************************************/
u8 SD_WriteDisk(const u8 buf[], u32 sector, u8 sector_size)
{
  u8 ret_value = 0x00;
  const u8 *ptr = buf;

  if (SD_TYPE_V2HC != g_sd_type) sector *= 512;  // 转换为字节地址
  if (1 == sector_size)
  {
    ret_value = SD_SendCmd(SD_CMD24, sector, 0X01);  // 读命令
    // 指令发送成功 
    if (0 == ret_value) ret_value = SD_SendBlock(ptr, 0xFE); // 写512个字节    
  } else {
    if (SD_TYPE_MMC != g_sd_type)
    {
      SD_SendCmd(SD_CMD55, 0, 0X01); 
      SD_SendCmd(SD_CMD23, sector_size, 0X01);  // 发送指令 
    } // if 
    ret_value = SD_SendCmd(SD_CMD25, sector, 0X01);  // 连续读命令
    if (0 == ret_value)
    {
      do {
        ret_value = SD_SendBlock(ptr, 0xFC);  // 接收512个字节   
        ptr +=512;  
      } while (--sector_size && 0 == ret_value);
      ret_value = SD_SendBlock(0, 0xFD);  // 接收512个字节 
    } // if
  } // if else
  SD_DisSelect(); // 取消片选

  return ret_value;
} // SD_WriteDisk() 

/************************************************ 
* 函  数: SD_Test(u32)
* 功  能: 测试SD, 读取SD卡指定扇区内容, 通过串口输出
*
* 参  数: sector - 扇区编号
*
* 返回值: 无
*************************************************/
void SD_Test(u32 sector)
{
  u16 i = 0;
/*  u8 write_buf[16] = {0xAA, 0x00, 0x01, 0x02, 
                      0x03, 0x04, 0x05, 0x06, 
                      0x07, 0x08, 0x09, 0x10, 
                      0x11, 0x12, 0x13, 0xDD};*/
  u8 buf[512] = {0};
  // 向0扇区写入数据
  // SD_WriteDisk(write_buf, 0, 1);
  // 读取0扇区内容
  if (0 == SD_ReadDisk(buf, sector, 1))
  {
    printf("Sector 0: \r\n");
    for (i = 0; i < 16; ++i)
    {
      // 显示扇区内容
      printf("0x%02X ", buf[i]);
    } // for
    printf("\r\nEnded here!\r\n");
  } // if
} // SD_Test()
