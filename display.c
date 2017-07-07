  #include "display.h"

  void display(uint temperature,uint humidity)
  {
    //Ambient Temperature
    LcdWriteCom(0x01);
    LcdWriteData('A');
    LcdWriteData('T');
    LcdWriteData(':');
    LcdWriteData(temperature/1000+0x30);
    LcdWriteData(temperature/100%10+0x30);
    LcdWriteData(temperature/10%10+0x30);
    LcdWriteData('.');
    LcdWriteData(temperature%10+0x30);
    LcdWriteData('%');
    LcdWriteCom(0xC0); //设置坐标在第二行
    LcdWriteData('A');
    LcdWriteData('H');
    LcdWriteData(':');
    LcdWriteData(humidity/1000+0x30);
    LcdWriteData(humidity/100%10+0x30);
    LcdWriteData(humidity/10%10+0x30);
    LcdWriteData('.');
    LcdWriteData(humidity%10+0x30);
    LcdWriteData('C');
  }
