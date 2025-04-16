#include "sys.h"
#include "usart.h"	
#include "Beep.h"
#include "pwm.h"
#include "delay.h"
#include "led.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//Èç¹ûÊ¹ÓÃucos,Ôò°üÀ¨ÏÂÃæµÄÍ·ÎÄ¼ş¼´¿É.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos Ê¹ÓÃ	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 

//STM32F4¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   

////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êı,¶ø²»ĞèÒªÑ¡Ôñuse MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âĞèÒªµÄÖ§³Öº¯Êı                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êı 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //Èç¹ûÊ¹ÄÜÁË½ÓÊÕ
//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
//×¢Òâ,¶ÁÈ¡USARTx->SRÄÜ±ÜÃâÄªÃûÆäÃîµÄ´íÎó   	
u8 USART_RX_BUF[USART_REC_LEN];     //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.
//½ÓÊÕ×´Ì¬
//bit15£¬	½ÓÊÕÍê³É±êÖ¾
//bit14£¬	½ÓÊÕµ½0x0d
//bit13~0£¬	½ÓÊÕµ½µÄÓĞĞ§×Ö½ÚÊıÄ¿
u16 USART_RX_STA=0;       //½ÓÊÕ×´Ì¬±ê¼Ç	

uint16_t oled_flag = 0;


//³õÊ¼»¯IO ´®¿Ú1 
//bound:²¨ÌØÂÊ
void uart_init(u32 bound)
  {
   //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //Ê¹ÄÜGPIOAÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//Ê¹ÄÜUSART1Ê±ÖÓ
 
	//´®¿Ú1¶ÔÓ¦Òı½Å¸´ÓÃÓ³Éä
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9¸´ÓÃÎªUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10¸´ÓÃÎªUSART1
	
	//USART1¶Ë¿ÚÅäÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9ÓëGPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ËÙ¶È50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure); //³õÊ¼»¯PA9£¬PA10

   //USART1 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êı¾İ¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎŞÆæÅ¼Ğ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎŞÓ²¼şÊı¾İÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
	
  USART_Cmd(USART1, ENABLE);  //Ê¹ÄÜ´®¿Ú1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖĞ¶Ï

	//Usart1 NVIC ÅäÖÃ
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//´®¿Ú1ÖĞ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾İÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯VIC¼Ä´æÆ÷¡¢

#endif
	
}

/*****************  ·¢ËÍÒ»¸ö×Ö½Ú **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ·¢ËÍÒ»¸ö×Ö½ÚÊı¾İµ½USART */
	USART_SendData(pUSARTx,ch);
		
	/* µÈ´ı·¢ËÍÊı¾İ¼Ä´æÆ÷Îª¿Õ */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** ·¢ËÍ8Î»µÄÊı×é ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ·¢ËÍÒ»¸ö×Ö½ÚÊı¾İµ½USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* µÈ´ı·¢ËÍÍê³É */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  ·¢ËÍ×Ö·û´® **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* µÈ´ı·¢ËÍÍê³É */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ·¢ËÍÒ»¸ö16Î»Êı **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* È¡³ö¸ß°ËÎ» */
	temp_h = (ch&0XFF00)>>8;
	/* È¡³öµÍ°ËÎ» */
	temp_l = ch&0XFF;
	
	/* ·¢ËÍ¸ß°ËÎ» */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ·¢ËÍµÍ°ËÎ» */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}



//void Ergodic_USART1_BUF(int CutoffDigit, uint8_t Reseive[CutoffDigit])
//{
//  int i;
//  for(i = 0; i < CutoffDigit-1; i++)
//  {
//    Reseive[i] = USART_RX_BUF[i];
//  }
//}



void USART1_ClearBUF(void)
{
  USART_RX_BUF[0] = 0;
  USART_RX_BUF[1] = 0;
  USART_RX_BUF[2] = 0;
  USART_RX_BUF[4] = 0;
  USART_RX_BUF[5] = 0;
}
  


void USART1_IRQHandler(void)                	//´®¿Ú1ÖĞ¶Ï·şÎñ³ÌĞò
{
  u8 Res;
  #if SYSTEM_SUPPORT_OS 		//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòĞèÒªÖ§³ÖOS.
  OSIntEnter();    
  #endif
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //½ÓÊÕÖĞ¶Ï(½ÓÊÕµ½µÄÊı¾İ±ØĞëÊÇ0x0d 0x0a½áÎ²)
  {
    Res = USART_ReceiveData(USART1);//(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊı¾İ
    switch(Res)
    {
      //
      case('1'):
      {
        //å‘é€1æ¿€æ´»        
        printf("Welcome to smart home system!!\r\n");
        printf("Please send order to choose the mode:\r\n");
        oled_flag = 1;   //å±å¹•å¼€å§‹æ˜¾ç¤º
        printf("2--DoorBell\r\n3--Open Door\r\n4--Lighting\r\n5--Draw the Curtain\r\n6--Close thr Curtain\r\no--Close All\r\n");
        LED_Bling(2);
        USART1_ClearBUF();  //æ¸…é™¤ä¸²å£1ç¼“å†²åŒºæ•°æ®
        break;
      }
      case('2'):
      {
        Beep_Bling();
        USART1_ClearBUF();
        break;
      }
      case('3'):
      {
        TIM_SetCompare2(TIM4,1300);
        delay_ms(2000);
        TIM_SetCompare2(TIM4,300);
        USART1_ClearBUF();
        break;
      }
      case('4'):
      {
        printf("Please send l,m or h to choose Brightness!");
        break;
      }
      case('l'):
      {
        TIM_SetCompare1(TIM4,2000);
        TIM_SetCompare3(TIM4,2000);
        break;
      } 
      case('m'):
      {
        TIM_SetCompare1(TIM4,6000);
        TIM_SetCompare3(TIM4,6000);
        break;
      }
      case('h'):
      {
        TIM_SetCompare1(TIM4,10000);
        TIM_SetCompare3(TIM4,10000);
        break;
      }
      case('5'):
      {
        TIM_SetCompare4(TIM4,1300);
//        delay_ms(3000);
//        TIM_SetCompare4(TIM4,800);
        USART1_ClearBUF();
        break;   
      }
      case('6'):
      {
        TIM_SetCompare4(TIM4,300);
        USART1_ClearBUF();
        break;
      }
      case('o'):
      {
        printf("GoodBye!");
        oled_flag = 0;
        TIM_SetCompare4(TIM4,300);
        TIM_SetCompare1(TIM4,0);
        TIM_SetCompare3(TIM4,0);
        USART1_ClearBUF();
        LED_Bling(1);
        break;
      }
    }
    if((USART_RX_STA&0x8000)==0)//½ÓÊÕÎ´Íê³É
    {
      if(USART_RX_STA&0x4000)//½ÓÊÕµ½ÁË0x0d
      {
        if(Res!=0x0a)USART_RX_STA=0;//½ÓÊÕ´íÎó,ÖØĞÂ¿ªÊ¼
        else USART_RX_STA|=0x8000;	//½ÓÊÕÍê³ÉÁË 
      }
      else //»¹Ã»ÊÕµ½0X0D
      {	
        if(Res==0x0d)USART_RX_STA|=0x4000;
        else
        {
          USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
          USART_RX_STA++;
          if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//½ÓÊÕÊı¾İ´íÎó,ÖØĞÂ¿ªÊ¼½ÓÊÕ	  
        }		 
      }
    }   		 
  } 
  

  
#if SYSTEM_SUPPORT_OS 	//Èç¹ûSYSTEM_SUPPORT_OSÎªÕæ£¬ÔòĞèÒªÖ§³ÖOS.
	OSIntExit();  											 
#endif
} 
#endif	

 




