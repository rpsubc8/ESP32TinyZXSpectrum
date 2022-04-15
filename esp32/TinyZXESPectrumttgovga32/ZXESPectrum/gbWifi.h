#ifndef GB_WIFI_H
 #define GB_WIFI_H

 #include "gbConfig.h"

 #ifdef use_lib_wifi
  #include <WiFi.h>
  #include <HTTPClient.h>

  void Asignar_WIFI(HTTPClient * ptr_http, WiFiClient * ptr_stream);
  bool Check_WIFI(void);
  void Asignar_URL_stream_WIFI(char const * URL);
  bool Leer_url_stream_WIFI(int * returnC);
  void PreparaURL(char *destURL,char *pathType,char *nameType, char *cadFile, char * fileExt);
  void FlushStreamWIFI(void);

 #endif 

#endif

