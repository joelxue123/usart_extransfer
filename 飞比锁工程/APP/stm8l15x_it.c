/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "global.h"
#include "protocol.h"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  zigbee_interrupt =1;
  EXTI->SR1=0XFF;
  EXTI->SR2=0XFF;
	
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

  lock_interrupt =1;
  EXTI->SR1=0XFF;
  EXTI->SR2=0XFF;
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
#if 0
  zigbee_interrupt =1;
  EXTI->SR1=0XFF;
  EXTI->SR2=0XFF;
#endif
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */		
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */


extern BFCT_protocol BFCT_protocol_Zigbee;

extern u16 data3_tc;
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
   char Res;
   static u8 sum = 0;

  if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
  {

    Res =   USART_ReceiveData8(USART1);
    
/******************
    **从zigbee接收数据
******************/
    if(1 == BFCT_protocol_Zigbee.receive_enable)
    {		
          data3_tc = 0; ////////此为全局变量，需要谨慎使用 
        if(0 == BFCT_protocol_Zigbee.receive_len)
        {
            if( protocal_header == Res) { }
            else
            {
                BFCT_protocol_Zigbee.receive_len = 0;
                sum = 0;
                BFCT_protocol_Zigbee.receive_fail =1;
                return ;
            }
        }
        else if( 1 == BFCT_protocol_Zigbee.receive_len)
        {
            if( Res == protocal_header ) { 
			
		 BFCT_protocol_Zigbee.receive_len = 0;
                sum = 0;
                return ;
	    }

        }
/************************
       // else if(BFCT_protocol.receive_len ==  (BFCT_protocol.receive_data[1] + 2))
	else if(BFCT_protocol.receive_len ==  (BFCT_protocol.receive_data[1] + 3))
        {
            if(sum == Res ) {}
            else
            {
                BFCT_protocol.receive_len = 0;
                sum = 0;
                fail_flag = 1;
                return 0 ;
            }
        }
************************/////  暂时屏蔽
        //else if(BFCT_protocol.receive_len ==  (BFCT_protocol.receive_data[1] + 3))
	else if(BFCT_protocol_Zigbee.receive_len > 3 )	
        { 
          data3_tc = 0; ////////此为全局变量，需要谨慎使用
#if    defined( Nan_Jing)
          if(BFCT_protocol_Zigbee.receive_len ==  protocal_len )
          {
              BFCT_protocol_Zigbee.receive_flag = 1;             
              BFCT_protocol_Zigbee.receive_data[BFCT_protocol_Zigbee.receive_len] = Res;
              BFCT_protocol_Zigbee.receive_len ++;
              sum = 0;
              return ;
          }
#elif defined(Fei_Bi)
            if(BFCT_protocol_Zigbee.receive_len ==   protocal_len )	
           {
              if(protocal_tail == Res)
              {
                sum = 0;
                BFCT_protocol_Zigbee.receive_flag = 1;
                BFCT_protocol_Zigbee.receive_data[BFCT_protocol_Zigbee.receive_len] = Res;
                BFCT_protocol_Zigbee.receive_len ++;
                sum = 0;
                return ;
                
              }
              else
              {
                  BFCT_protocol_Zigbee.receive_len = 0;
                  sum = 0;
                  BFCT_protocol_Zigbee.receive_fail =1;
                  return ;
              }
          }
#endif         
        }

          
            BFCT_protocol_Zigbee.receive_data[BFCT_protocol_Zigbee.receive_len] = Res;
            if(BFCT_protocol_Zigbee.receive_len > 0 && BFCT_protocol_Zigbee.receive_len <  USART_LEN)
            {
                sum = Res + sum;
            }
            BFCT_protocol_Zigbee.receive_len++;
            if(BFCT_protocol_Zigbee.receive_len > (USART_LEN-1))
              BFCT_protocol_Zigbee.receive_len =(USART_LEN-1);


    }
    
    /******************
    **从lock接收数据
******************/
    else if(1 == BFCT_protocol_Lock.receive_enable)
    {		
        data3_tc = 0; ////////此为全局变量，需要谨慎使用
        if(0 == BFCT_protocol_Lock.receive_len)
        {
            if(0x02 == Res) { 
            Res=0x02;
            }
            else
            {
                BFCT_protocol_Lock.receive_len = 0;
                sum = 0;
                return ;
            }
        }
	else if(BFCT_protocol_Lock.receive_len > 3 )	
        {
          data3_tc = 0; ////////此为全局变量，需要谨慎使用
          if(BFCT_protocol_Lock.receive_len ==  (BFCT_protocol_Lock.receive_data[1]+ (BFCT_protocol_Lock.receive_data[2]<<8)+ 4) )	
          {
            if(0Xff == Res)
            {
              sum = 0;
              BFCT_protocol_Lock.receive_flag = 1;
              BFCT_protocol_Lock.receive_data[BFCT_protocol_Lock.receive_len] = Res;
              BFCT_protocol_Lock.receive_len ++;
              sum = 0;
              return ;
              
            }
            else
            {
                BFCT_protocol_Lock.receive_len = 0;
                sum = 0;
                BFCT_protocol_Lock.receive_fail =1;
                return ;
            }
          }
        }

          
            BFCT_protocol_Lock.receive_data[BFCT_protocol_Lock.receive_len] = Res;
            if(BFCT_protocol_Lock.receive_len > 0 && BFCT_protocol_Lock.receive_len <  USART_LEN)//BFCT_protocol_Lock.receive_data[1] + 8)
            {
                sum = Res + sum;
            }
            BFCT_protocol_Lock.receive_len++;
            if(BFCT_protocol_Lock.receive_len > (USART_LEN-1))
              BFCT_protocol_Lock.receive_len = (USART_LEN-1);

    }
    else
    {
        sum = 0;
    }
  }
  
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/