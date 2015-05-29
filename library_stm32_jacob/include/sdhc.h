/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: sdhc.h
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
#ifndef SDHC_H_
#define SDHC_H_

#include "sys.h"

// SD卡类型
#define SD_TYPE_ERR   0x00
#define SD_TYPE_MMC   0x01
#define SD_TYPE_V1    0x02
#define SD_TYPE_V2    0x04
#define SD_TYPE_V2HC  0x06
// SD卡指令
#define SD_CMD0   0   // 卡复位
#define SD_CMD1   1
#define SD_CMD8   8   // 命令8 , SEND_IF_COND
#define SD_CMD9   9   // 命令9 , 读CSD数据
#define SD_CMD10  10  // 命令10, 读CID数据
#define SD_CMD12  12  // 命令12, 停止数据传输
#define SD_CMD16  16  // 命令16, 设置SectorSize 应返回0x00
#define SD_CMD17  17  // 命令17, 读sector
#define SD_CMD18  18  // 命令18, 读multiple sectors
#define SD_CMD23  23  // 命令23, 设置多sector写入前预先擦除n个block
#define SD_CMD24  24  // 命令24, 写sector
#define SD_CMD25  25  // 命令25, 写multiple sectors
#define SD_CMD41  41  // 命令41, 应返回0x00
#define SD_CMD55  55  // 命令55, 应返回0x01
#define SD_CMD58  58  // 命令58, 读OCR信息
#define SD_CMD59  59  // 命令59, 使能/禁止CRC，应返回0x00
// 数据写入回应字
#define SD_DATA_OK              0x05
#define SD_DATA_CRC_ERROR       0x0B
#define SD_DATA_WRITE_ERROR     0x0D
#define SD_DATA_OTHER_ERROR     0xFF
// SD卡回应标记字
#define SD_RESPONSE_NO_ERROR    0x00
#define SD_IN_IDLE_STATE        0x01
#define SD_ERASE_RESET          0x02
#define SD_ILLEGAL_COMMAND      0x04
#define SD_COM_CRC_ERROR        0x08
#define SD_ERASE_SEQUENCE_ERROR 0x10
#define SD_ADDRESS_ERROR        0x20
#define SD_PARAMETER_ERROR      0x40
#define SD_RESPONSE_FAILURE     0xFF

/*----- High layer function -----*/
// 初始化SD卡
u8 SD_Init(void);
// 获取SD卡的CID信息, 包括制造商信息
u8 SD_GetCID(u8 cid_data[]);
// 获取SD卡的CSD信息, 包括容量和速度信息
u8 SD_GetCSD(u8 csd_data[]);
// 获取SD卡的总扇区数
u32 SD_GetSectorCount(void);
// 从SD卡指定扇区读取数据
u8 SD_ReadDisk(u8 buf[], u32 sector, u8 sector_size);
// 向SD卡指定扇区写入数据
u8 SD_WriteDisk(const u8 buf[], u32 sector, u8 sector_size);
// 测试SD, 读取SD卡指定扇区内容, 通过串口输出
void SD_Test(u32 sector);

/*----- Medium layer function -----*/
// SD, 通过写入命令, 读取相应返回信息
u8 SD_SPI_ReadWriteCmd(u8 cmd);
// SD卡初始化的时候, 需要低速
void SD_SPI_SpeedLow(void);
// SD卡正常工作的时候, 可以高速了
void SD_SPI_SpeedHigh(void);
// SD对应IO口初始化
void SD_SPI_Init(void);
// 等待SD卡准备好
u8 SD_WaitReady(void);
// 取消片选, 释放SPI总线
void SD_DisSelect(void);
// 选择SD卡, 等待卡准备完成
u8 SD_Select(void);
// 等待SD卡回应
u8 SD_GetResponse(u8 cmd);
// 向SD卡发送一个命令
u8 SD_SendCmd(u8 cmd, u32 argv, u8 crc);
// 从sd卡读取一个数据包的内容
u8 SD_ReadBlock(u8 buf[], u16 len);
// 向sd卡写入一个数据块的内容, 512字节
u8 SD_SendBlock(const u8 buf[], u8 cmd);

/*----- Low layer function -----*/

#endif  // #ifndef SDHC_H_
