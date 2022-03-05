#include "gbConfig.h"
#include "PS2Mouse.h"

 #ifdef use_lib_mouse_kempston
 
 #include "gbGlobals.h"
 #include <Arduino.h>

//Hay que realizar mediciones para timeout correcto
#define gb_max_timeout 1000000

//const int m=0x100;

//extern unsigned char gb_mouse_ps2clk;
//extern unsigned char gb_mouse_ps2data;


void PS2Mouse_Init(unsigned char clk, unsigned char data)
{
 gb_mouse_ps2clk=clk;
 gb_mouse_ps2data=data;
 PS2Mouse_gohi(gb_mouse_ps2clk);
 PS2Mouse_gohi(gb_mouse_ps2data);  
}

void PS2Mouse_write(unsigned char data)
{  
  unsigned char parity=1;
  unsigned int timeout=0;
  //Serial.printf("BEGIN write\n");
  PS2Mouse_gohi(gb_mouse_ps2data);
  PS2Mouse_gohi(gb_mouse_ps2clk);
  delayMicroseconds(300);
  PS2Mouse_golo(gb_mouse_ps2clk);
  delayMicroseconds(300);
  PS2Mouse_golo(gb_mouse_ps2data);
  delayMicroseconds(10);
  PS2Mouse_gohi(gb_mouse_ps2clk);

  //Serial.printf("Begin While write\n");
  delayMicroseconds(10); //Para que funcione ponemos espera
  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
   {
    gb_mouse_init_error= 1;
    return;
   }
  }
  //Serial.printf("End While write\n");
  
  for(unsigned char i=0; i<8; i++)
  {
    if(data&0x01) PS2Mouse_gohi(gb_mouse_ps2data);
    else PS2Mouse_golo(gb_mouse_ps2data); 
    timeout=0;
    while(digitalRead(gb_mouse_ps2clk)==LOW)
    {
     timeout++;
     if (timeout>gb_max_timeout)
      return;      
    }
    timeout=0;
    while(digitalRead(gb_mouse_ps2clk)==HIGH)
    {
     timeout++;
     if (timeout>gb_max_timeout)
      return;          
    }
    parity^=(data&0x01);
    data=data>>1;
  }
  
  if(parity){
   PS2Mouse_gohi(gb_mouse_ps2data);
  }
  else{    
   PS2Mouse_golo(gb_mouse_ps2data);
  }

  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  
  PS2Mouse_gohi(gb_mouse_ps2data);
  delayMicroseconds(50);

  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  timeout=0;
  while((digitalRead(gb_mouse_ps2clk)==LOW)||(digitalRead(gb_mouse_ps2data)==LOW))
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  
  PS2Mouse_golo(gb_mouse_ps2clk);
  //Serial.printf("END write\n");
}

unsigned char PS2Mouse_read()
{
  unsigned char data=0, bit=1;
  unsigned int timeout=0;
  PS2Mouse_gohi(gb_mouse_ps2clk);
  PS2Mouse_gohi(gb_mouse_ps2data);
  delayMicroseconds(50);
  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return data;
  }
  
  delayMicroseconds(5);
  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)    
    return data;
  }
  
  for(unsigned char i=0; i<8; i++)
  {
    timeout=0;
    while(digitalRead(gb_mouse_ps2clk)==HIGH)
    {
     timeout++;
     if (timeout>gb_max_timeout)          
      return data;
    }
    bit=digitalRead(gb_mouse_ps2data);
    timeout=0;
    while(digitalRead(gb_mouse_ps2clk)==LOW)
    {
     timeout++;
     if (timeout>gb_max_timeout)          
      return data;      
    }
    bitWrite(data,i,bit);
  }

  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)    
    return data;
  }
  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)          
    return data;      
  }
  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)          
    return data;      
  }
  timeout=0;
  while(digitalRead(gb_mouse_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)          
    return data;      
  }
  
  PS2Mouse_golo(gb_mouse_ps2clk);
  
  return data;
}

void PS2Mouse_begin()
{
  //Serial.printf("BEGIN\n");
  PS2Mouse_write(0xFF);
  for(unsigned char i=0; i<3; i++){
   PS2Mouse_read();
  }
  PS2Mouse_write(0xF0);
  PS2Mouse_read();
  delayMicroseconds(100);
  //Serial.printf("END\n");
}

void PS2Mouse_getPosition(unsigned char &stat, signed short int &x, signed short int &y)
{
  PS2Mouse_write(0xEB);
  PS2Mouse_read();
  stat=PS2Mouse_read();
  unsigned char _x=PS2Mouse_read();
  unsigned char _y=PS2Mouse_read();  

  bool negx=bitRead(stat,4);
  bool negy=bitRead(stat,5);
  //x=PS2Mouse_twos(_x, negx);
  //y=PS2Mouse_twos(_y, negy);  
  x= (negx)?~_x:_x; //delta negativo  
  y= (negy)?~_y:_y;  
}

inline void PS2Mouse_golo(unsigned char pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

inline void PS2Mouse_gohi(unsigned char pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}


//int PS2Mouse_twos(unsigned char value, bool sign)
//{
//  int v=(int)value;
//  if(sign) v|=0xFF00;
//  return v;
//}

#endif







/*
PS2Mouse::PS2Mouse(int clk, int data)
{
  _ps2clk=clk;
  _ps2data=data;
  gohi(_ps2clk);
  gohi(_ps2data);  
}

void PS2Mouse::write(uint8_t data){  
  uint8_t parity=1;
  unsigned int timeout=0;
  //Serial.printf("BEGIN write\n");
  gohi(_ps2data);
  gohi(_ps2clk);
  delayMicroseconds(300);
  golo(_ps2clk);
  delayMicroseconds(300);
  golo(_ps2data);
  delayMicroseconds(10);
  gohi(_ps2clk);

  //Serial.printf("Begin While write\n");
  delayMicroseconds(10); //Para que funcione ponemos espera
  timeout=0;
  while(digitalRead(_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;
  }
  //Serial.printf("End While write\n");
  
  for(int i=0; i<8; i++){
    if(data&0x01) gohi(_ps2data);
    else golo(_ps2data); 
    timeout=0;
    while(digitalRead(_ps2clk)==LOW)
    {
     timeout++;
     if (timeout>gb_max_timeout)
      return;      
    }
    timeout=0;
    while(digitalRead(_ps2clk)==HIGH)
    {
     timeout++;
     if (timeout>gb_max_timeout)
      return;          
    }
    parity^=(data&0x01);
    data=data>>1;
  }
  
  if(parity) gohi(_ps2data);
  else golo(_ps2data);

  timeout=0;
  while(digitalRead(_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  timeout=0;
  while(digitalRead(_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  
  gohi(_ps2data);
  delayMicroseconds(50);

  timeout=0;
  while(digitalRead(_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  timeout=0;
  while((digitalRead(_ps2clk)==LOW)||(digitalRead(_ps2data)==LOW))
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return;    
  }
  
  golo(_ps2clk);
  //Serial.printf("END write\n");
}

uint8_t PS2Mouse::read(void){
  uint8_t data=0, bit=1;
  unsigned int timeout=0;
  gohi(_ps2clk);
  gohi(_ps2data);
  delayMicroseconds(50);
  timeout=0;
  while(digitalRead(_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)
    return data;
  }
  
  delayMicroseconds(5);
  timeout=0;
  while(digitalRead(_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)    
    return data;
  }
  
  for(int i=0; i<8; i++)
  {
    timeout=0;
    while(digitalRead(_ps2clk)==HIGH)
    {
     timeout++;
     if (timeout>gb_max_timeout)          
      return data;
    }
    bit=digitalRead(_ps2data);
    timeout=0;
    while(digitalRead(_ps2clk)==LOW)
    {
     timeout++;
     if (timeout>gb_max_timeout)          
      return data;      
    }
    bitWrite(data,i,bit);
  }

  timeout=0;
  while(digitalRead(_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)    
    return data;
  }
  timeout=0;
  while(digitalRead(_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)          
    return data;      
  }
  timeout=0;
  while(digitalRead(_ps2clk)==HIGH)
  {
   timeout++;
   if (timeout>gb_max_timeout)          
    return data;      
  }
  timeout=0;
  while(digitalRead(_ps2clk)==LOW)
  {
   timeout++;
   if (timeout>gb_max_timeout)          
    return data;      
  }
  
  golo(_ps2clk);
  
  return data;
}

void PS2Mouse::begin(void){
  Serial.printf("BEGIN\n");
  write(0xFF);
  for(int i=0; i<3; i++) read();
  write(0xF0);
  read();
  delayMicroseconds(100);
  Serial.printf("END\n");
}

void PS2Mouse::getPosition(uint8_t &stat, int &x, int &y){
  write(0xEB);
  read();
  stat=read();
  uint8_t _x=read();
  uint8_t _y=read();  

  bool negx=bitRead(stat,4);
  bool negy=bitRead(stat,5);
  x=twos(_x, negx);
  y=twos(_y, negy);
}

void PS2Mouse::golo(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void PS2Mouse::gohi(int pin){
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

const int m=0x100;
int PS2Mouse::twos(uint8_t value, bool sign){
  int v=(int)value;
  if(sign) v|=0xFF00;
  return v;
}
*/