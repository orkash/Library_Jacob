/* 
* Copyright (c) 2015, Jacob Peng.
* All rights reserved. 
* 
* 文件名称: sys.c
* 文件标识: 见配置管理计划书
* 摘    要: stm32, NVIC, RCC配置, 对IO口进行位带操作参考正点原子源代码
* 函    数: NVIC_Configuration() - NVIC中断设置
*           RCC_Configuration() - 系统高速外部时钟HSE配置
* 
* 当前版本: E0.0.2
* 作    者: Jacob Peng
* 完成日期: 2015年5月21日
* 更改内容: E0.0.1, 实现了NVIC, RCC基本设置, 2015年4月23日
*           E0.0.2, 为了SD卡初始化, 降低了系统时钟频率, 2015年5月21日
* 
* 取代版本: E0.0.1
* 原作者  : Jacob Peng
* 完成日期: 2015年4月23日
*/
#include "../include/sys.h"

#include <stm32f10x.h>
#include <stm32f10x_flash.h>  // Declare FLASH_PrefetchBufferCmd(), FLASH_SetLatency().

/************************************************ 
* 函  数: NVIC_Configuration(void)
* 功  能: NVIC中断设置
*
* 参  数: 无
*
* 返回值: 无
*************************************************/ 
void NVIC_Configuration(void)
{
  // 设置NVIC中断分组2:2, 2位抢占优先级, 2位响应优先级
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
} // NVIC_Configuration()

/************************************************ 
* 函  数: RCC_Configuration(void)
* 功  能: 系统高速外部时钟HSE配置: 
*         为了SD卡初始化, 降低了系统时钟频率, 
*         将RCC_PCLK2Config(RCC_HCLK_Div1),
*         修改为RCC_PCLK2Config(RCC_HCLK_Div2)
*
* 参  数: 无
*
* 返回值: 无
*************************************************/
void RCC_Configuration(void)
{
  /* 这里是重置了RCC的设置，类似寄存器复位 */
  RCC_DeInit();
  /* 使能外部高速晶振 */
  RCC_HSEConfig(RCC_HSE_ON);
  /* 等待高速晶振稳定 */
  if (SUCCESS == RCC_WaitForHSEStartUp())
  {
    /* 使能flash预读取缓冲区 */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* 令Flash处于等待状态，2是针对高频时钟的，这两句跟RCC没直接关系，可以暂且略过 */
    FLASH_SetLatency(FLASH_Latency_2);    
    /*AHB, HCLK = SYSCLK 设置高速总线时钟=系统时钟*/
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    /*高速AHB, PCLK2 = HCLK 设置低速总线2时钟=高速总线时钟*/
    // RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PCLK2Config(RCC_HCLK_Div2); // 72/2=36MHz, 为了SD卡初始化降低速度 
    /* PCLK1 = HCLK/2 设置低速总线1的时钟=高速时钟的二分频*/
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /*低速AHB, ADCCLK = PCLK2/6 设置ADC外设时钟=低速总线2时钟的六分频*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    /* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */   
    /* 利用锁相环将外部8Mhz晶振9倍频到72Mhz, 这句很关键*/ 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    /* Enable PLL 使能锁相环*/
    RCC_PLLCmd(ENABLE); 
    /* Wait till PLL is ready 等待锁相环输出稳定*/
    while (RESET == RCC_GetFlagStatus(RCC_FLAG_PLLRDY)) continue;
    /* Select PLL as system clock source 将锁相环输出设置为系统时钟 */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source 等待校验成功*/
    while (RCC_GetSYSCLKSource() != 0x08) continue;
  } // if
    /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    // 使能外围接口总线时钟，注意各外设的隶属情况，不同芯片的分配不同，到时候查手册就可以;
    // 要使用那一个引脚或功能, 就只使能要使用的引脚或功能, 这样可以降低功耗与提升稳定性
    /*RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
                           RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);*/ 
} // RCC_Configuration()
