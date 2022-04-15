#include "gbConfig.h"
#include "gbWifiConfig.h"
#include "gbWifi.h"
#include "gbGlobals.h"

#ifdef use_lib_wifi

 HTTPClient * gb_wifi_http;
 WiFiClient * gb_wifi_stream;
 int gb_wifi_len=0;
 int gb_wifi_dsize=0;

 //*************************************************************************************
 void PreparaURL(char *destURL,char *pathType,char *nameType, char *cadFile, char * fileExt)
 {
  strcpy(destURL, gb_wifi_url_base_path);
  strcat(destURL, pathType);  
  strcat(destURL, nameType);
  strcat(destURL, "/");
  strcat(destURL, cadFile);
  strcat(destURL, ".");
  strcat(destURL, fileExt);
 }

 //********************************************************************
 void Asignar_WIFI(HTTPClient * ptr_http, WiFiClient * ptr_stream)
 {
  gb_wifi_http = ptr_http;
  gb_wifi_stream = ptr_stream;
 }


 //*************************************
 bool Check_WIFI()
 {  
  if((WiFi.status() != WL_CONNECTED))
  {
   #ifdef use_lib_wifi_debug
    Serial.printf ("Error WIFI\n");
   #endif
   if (gb_wifi_delay_check != 0)
   {
    delay(gb_wifi_delay_check);
   }
   return false;
  }
  
  if (gb_wifi_delay_check != 0)  
  {
   delay(gb_wifi_delay_check);
  }  
  return true;
 }

 //***************************************
 void FlushStreamWIFI()
 {
  gb_wifi_stream->flush();
 }

 //*************************************
 void Asignar_URL_stream_WIFI(char const * URL)
 {
  #ifdef use_lib_wifi_debug
   unsigned int tiempo_ini,tiempo_fin;
   tiempo_ini = micros();
   Serial.printf ("http\n");
  #endif 
  gb_wifi_http->begin(URL);
  #ifdef use_lib_wifi_debug
   Serial.printf ("url\n");
  #endif 
  int httpCode = gb_wifi_http->GET();
  #ifdef use_lib_wifi_debug
   Serial.printf ("get\n");
  #endif
  gb_wifi_len = 0;
  gb_size_file_wifi=0;
  if (httpCode == HTTP_CODE_OK) 
  {
   gb_wifi_len = gb_wifi_http->getSize();
   #ifdef use_lib_wifi_debug
    Serial.printf("Size:%d\n",gb_wifi_len);
   #endif 
   gb_size_file_wifi = gb_wifi_len;   
   gb_wifi_stream = gb_wifi_http->getStreamPtr();
   #ifdef use_lib_wifi_debug
    Serial.printf ("getStream\n");
   #endif 
  }
  gb_wifi_dsize= 0;    
  #ifdef use_lib_wifi_debug
   tiempo_fin = micros();   
   Serial.printf("Tiempo URL:%d\n",(tiempo_fin-tiempo_ini));
  #endif

  if (gb_wifi_delay_asign_read != 0)
  {
   delay(gb_wifi_delay_asign_read);
  }
 }

 //****************************************
 bool Leer_url_stream_WIFI(int * returnC)
 {  
  bool success= false;
  #ifdef use_lib_wifi_debug
   unsigned int tiempo_ini,tiempo_fin;
   Serial.printf("len:%d dsize:%d\n",gb_wifi_len,gb_wifi_dsize);
   tiempo_ini = micros();
  #endif   
  if (gb_wifi_http->connected() && (gb_wifi_len > 0 || gb_wifi_len == -1)) 
  {      
   //JJ size_t size = gb_wifi_stream->available();
   size_t size= gb_wifi_stream->available();
   while (size == 0)
   {
    size= gb_wifi_stream->available();
    if (gb_wifi_delay_check_available !=0)
    {
     delay(gb_wifi_delay_check_available);
    }
   }

   #ifdef use_lib_wifi_debug
    Serial.printf("available size:%d\n",size);
   #endif 
   if (size) 
   {
    int c = gb_wifi_stream->readBytes(gb_buffer_wifi, 1024);
    //Serial.printf ("Byte leidos %d\n",c);    

    //if (isPtrDest == 1)
    //{
    // memcpy(&ptrDest[auxContWifi],gb_buffer_wifi,c);
    // auxContWifi += c;
    //}

            
    gb_wifi_dsize += c;
    if (gb_wifi_len > 0)
    {
     gb_wifi_len -= c;
    }
    *returnC = c;
   }
   else
   {

   }
   success = (gb_wifi_len == 0 || (gb_wifi_len == -1 && gb_wifi_dsize > 0));
  } //fin wifi
  #ifdef use_lib_wifi_debug
   tiempo_fin = micros();
   Serial.printf("Tiempo stream:%d\n",(tiempo_fin-tiempo_ini));
  #endif

  if (gb_wifi_delay_available != 0)
  {
   delay(gb_wifi_delay_available); //Delay millis
  }
  return success;
 }
#endif

