#ifndef _GB_PS2MOUSE_H
 #define _GB_PS2MOUSE_H

 #include "gbConfig.h"

 #ifdef use_lib_mouse_kempston

  //#include <Arduino.h>

  inline void PS2Mouse_golo(unsigned char pin);
  inline void PS2Mouse_gohi(unsigned char pin);
  //int PS2Mouse_twos(unsigned char value, bool sign);

  void PS2Mouse_Init(unsigned char clk, unsigned char data);
  void PS2Mouse_write(unsigned char data);
  unsigned char PS2Mouse_read(void);

  void PS2Mouse_begin(void);
  void PS2Mouse_getPosition(unsigned char &stat, signed short int &x, signed short int &y); 

#endif


//class PS2Mouse 
//{
//  public:
//    PS2Mouse(int data, int clk);
//    void write(uint8_t data);
//    uint8_t read(void);
//
//    void begin(void);
//    void getPosition(uint8_t &stat, int &x, int &y);
//
//  private:
//    int _ps2clk;
//    int _ps2data;
//    void golo(int pin);
//    void gohi(int pin);
//    int twos(uint8_t value, bool sign);
//};


#endif // _GB_PS2MOUSE_H
