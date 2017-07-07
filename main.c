 
  /*******************************************************************************
  *                 
  *                 		       
  --------------------------------------------------------------------------------
  *                           温湿度采集显示系统
  * 连接方式       : 湿度P3_2接555定时器
  * 注    意		 : 
  *******************************************************************************/
  #include"display.h"
  #include"lcd.h"
  #include "adc.h"
  typedef  unsigned char u8;
  typedef  unsigned int u16;
  typedef  unsigned long u32;

  sbit ADCSTART = P3^5;
  sbit ADCEOC = P3^4;
  sbit BEEP = P2^0;
  #define ADCDATAPORT P1;
  uint cnt,period;

  bit flagSecond = 0;

  /*******************************************************************************
  * 函 数 名         : main
  * 函数功能		   : 主函数
  * 输    入         : 无
  * 输    出         : 无
  *******************************************************************************/
  void Delayms(u16 xms)
  {
    u16 i,j;
    for(i=0;i<xms;i++)
    {
      for(j=0;j<120;j++)
    {
    }
    }
  }
  void ADC_init(){
    ADCSTART=0;
  }
  u16 ADConvert(){

    u8 i;
    unsigned long m;
    ADCSTART=1;	
    ADCSTART=0;
    //
    Delayms(1);
    while(ADCEOC==0);
    i=ADCDATAPORT;

    m=5000;
    m*=i;
    m/=256;
    return m;	
  }
  void main(void)
  {
    u16 v;
    u16 humidity;
    u16 temperature;
    u8 beep_second = 0;
    LcdInit();
    //use T0
    TMOD &= 0xf0;
    TMOD |= 0x01;
    TH0 = (65536 - 20000)/256;
    TL0 = (65536 - 20000)%256;
    //外部中断
    IT0 = 1;
    EX0 = 1;
    ET0 = 1;
    TR0 = 1;
    EA  = 1;
  //LcdWriteCom(0xC0); //设置坐标在第二行
  //	LcdWriteCom(0x07); //设置每写一次整屏右移
    ADC_init();
    while(1)
    {
      if(flagSecond)
      {
        v = ADConvert();
        flagSecond = 0;
        temperature = (4.945*(double)v/1000 + 23.87)*10;
        humidity = (-0.0747*(double)period+552.75)*10+12-(double)temperature*0.00005;
        if(humidity >= 1000)
          humidity = 1000;
        display(humidity,temperature);
      }
  //		BEEP = 1;
      if(humidity > 900 || temperature > 320)
      {
        BEEP = 0;
      }
      else
        BEEP = 1;
    }				
  }

  void int0isr(void) interrupt 0
  {
    ++cnt;
  }

  void timer0isr(void) interrupt 1{
    static uint count = 0;
    TR0 = 0;
    TH0 = (65536 - 20000)/256;
    TL0 = (65536 - 20000)%256;
    TR0 = 1;
    ++count;
    if(count >= 50){
      count = 0;
      EA  = 0;
      period = cnt;
      cnt = 0;
      EA  = 1;
      flagSecond = 1;
    }
  }

