//Author: ackerman
//Convert dsks .dsk to .h Tiny ESP32 CPC emulator
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define maxObjects 255
#define max_cad_title 12

enum TEnumFileType{
 file_scr = 0,
 file_tape = 10,
 file_sna = 20,
 file_rom = 3,
 file_rom_list = 4
};


unsigned char gb_bufferFile[524288]; //512 KB archivo maximo

void listFilesRecursively(char *basePath, TEnumFileType auxTypeFile, unsigned char tipo, unsigned char doRecursive);
void ShowHelp(void);
void WriteHeadH(char * cadDefine);
void WriteFileSCR(unsigned char num,char *cadPath, char *cadFile, char *cadFileSource);
void WriteFileTape(unsigned char num,char *cadPath, char *cadFile, char *cadFileSource);
void WriteFileSNA(unsigned char num,char *cadPath, char *cadFile, char *cadFileSource);
void WriteFileROM(char *cadPath,char *cadFile, char *cadDir);
void WriteFile4ROMS(char *cadPath, char *cadDir,unsigned char *fileRomsOK);
int GetSizeFile(char *cadFile);
void InitTitles(void);
void RemoveExt(char *cad);
void ProcesaFicherosSCR(void);

FILE *gb_fileWrite = NULL;
char gb_titles[maxObjects][32];
char gb_nameFiles[maxObjects][64];
char gb_nameDir[maxObjects][64];
unsigned char gb_contRom=0;


void RemoveExt(char *cad)
{
 int total= strlen(cad);
 if (total > max_cad_title)
 {
  total= max_cad_title;
  cad[max_cad_title]='\0';
 }
 for (int i=0;i<total;i++)
 {
  if (cad[i]==' ')   
   cad[i] = '_';
  else
  {
   if (cad[i]=='.')
   {
    cad[i]='\0';
    return;
   }
  }
 }
}

//**********************************************
void InitTitles()
{
 for (unsigned char i=0;i<maxObjects;i++)
  gb_titles[i][0]='\0';
}

//**********************************************
void ShowHelp()
{
 printf("Author: ackerman\n\n");
}

//**********************************************
int GetSizeFile(char *cadFile)
{
 long aReturn=0;
 FILE* fp = fopen(cadFile,"rb");
 if(fp) 
 {
  fseek(fp, 0 , SEEK_END);
  aReturn = ftell(fp);
  fseek(fp, 0 , SEEK_SET);// needed for next read from beginning of file
  fclose(fp);
 }
 return aReturn;
}

//**********************************************
void WriteFileSCR(unsigned char num,char *cadPath,char *cadFile, char *cadFileSource)
{
 unsigned char contLine=0;
 FILE *auxWrite = NULL;
 FILE *auxRead = NULL;
 long auxSize=0;
 char cadDest[2048];
 char cadOri[2048];
 sprintf(cadOri,"%s\\%s",cadPath,cadFileSource);
 sprintf(cadDest,"output/dataFlash/scr/scr%s.h",cadFile);
 auxWrite = fopen(cadDest,"w+");
 if (auxWrite!= NULL)
 {
  fprintf(auxWrite,"#ifndef SCR_%s_48K_H\n",cadFile);
  fprintf(auxWrite," #define SCR_%s_48K_H\n",cadFile);   
  fprintf(auxWrite," //screen 48K %s 6912 bytes\n\n",cadFile);

  //WriteHexData(auxWrite,cadFile);
  fprintf(auxWrite,"const unsigned char gb_scr_%s_48k[]={\n",cadFile);

  auxRead = fopen(cadOri,"rb");
  if (auxRead!=NULL)
  {
   auxSize = GetSizeFile(cadOri);
   //printf ("Tam %d",auxSize);
   fread(gb_bufferFile,1,auxSize,auxRead);
   for (long i=0;i<auxSize;i++)
   {
    fprintf(auxWrite,"0x%02X",gb_bufferFile[i]);
    if (i<(auxSize-1))
     fprintf(auxWrite,",");
    contLine++;
    if (contLine>15)
    {
     contLine=0;
     fprintf(auxWrite,"\n");
    }
   }
   fclose(auxRead);
  }
  
  
  fprintf(auxWrite,"\n};\n");
  fprintf(auxWrite,"#endif\n");
  fclose(auxWrite);
 }
}


//**********************************************
void WriteFileTape(unsigned char num,char *cadPath,char *cadFile, char *cadFileSource, int tipo)
{
 unsigned char contLine=0;
 FILE *auxWrite = NULL;
 FILE *auxRead = NULL;
 long auxSize=0;
 char cadDest[2048];
 char cadOri[2048];
 sprintf(cadOri,"%s\\%s",cadPath,cadFileSource);
 sprintf(cadDest,"output/dataFlash/tape/tape%s.h",cadFile);
 auxWrite = fopen(cadDest,"w+");
 if (auxWrite!= NULL)
 {
  if (tipo == 48){
   fprintf(auxWrite,"#ifndef TAPE_%s_48K_H\n",cadFile);
   fprintf(auxWrite," #define TAPE_%s_48K_H\n",cadFile);   
  }
  else{
   fprintf(auxWrite,"#ifndef TAPE_%s_128K_H\n",cadFile);
   fprintf(auxWrite," #define TAPE_%s_128K_H\n",cadFile);   
  }
  //WriteHexData(auxWrite,cadFile);  

  auxRead = fopen(cadOri,"rb");
  if (auxRead!=NULL)
  {
   auxSize = GetSizeFile(cadOri);
   
   if (tipo == 48){   
    fprintf(auxWrite," //tape 48K %s %d bytes\n\n",cadFile,auxSize);
    fprintf(auxWrite,"const unsigned char gb_tape_%s_48k[]={\n",cadFile);
   }
   else{
    fprintf(auxWrite," //tape 128K %s %d bytes\n\n",cadFile,auxSize);
    fprintf(auxWrite,"const unsigned char gb_tape_%s_128k[]={\n",cadFile);
   }
      
   //printf ("Tam %d",auxSize);
   fread(gb_bufferFile,1,auxSize,auxRead);
   for (long i=0;i<auxSize;i++)
   {
    fprintf(auxWrite,"0x%02X",gb_bufferFile[i]);
    if (i<(auxSize-1))
     fprintf(auxWrite,",");
    contLine++;
    if (contLine>15)
    {
     contLine=0;
     fprintf(auxWrite,"\n");
    }
   }
   fclose(auxRead);
  }
  
  
  fprintf(auxWrite,"\n};\n");
  fprintf(auxWrite,"#endif\n");
  fclose(auxWrite);
 }
}



//**********************************************
void WriteFileSNA(unsigned char num,char *cadPath,char *cadFile, char *cadFileSource, int tipo)
{
 unsigned char contLine=0;
 FILE *auxWrite = NULL;
 FILE *auxRead = NULL;
 long auxSize=0;
 char cadDest[2048];
 char cadOri[2048];
 sprintf(cadOri,"%s\\%s",cadPath,cadFileSource);
 sprintf(cadDest,"output/dataFlash/sna/sna%s.h",cadFile);
 auxWrite = fopen(cadDest,"w+");
 if (auxWrite!= NULL)
 {
  if (tipo == 48){
   fprintf(auxWrite,"#ifndef SNA_%s_48K_H\n",cadFile);
   fprintf(auxWrite," #define SNA_%s_48K_H\n",cadFile);   
  }
  else{
   fprintf(auxWrite,"#ifndef SNA_%s_128K_H\n",cadFile);
   fprintf(auxWrite," #define SNA_%s_128K_H\n",cadFile);   
  }
  //WriteHexData(auxWrite,cadFile);  

  auxRead = fopen(cadOri,"rb");
  if (auxRead!=NULL)
  {
   auxSize = GetSizeFile(cadOri);
   
   if (tipo == 48){   
    fprintf(auxWrite," //sna 48K %s %d bytes\n\n",cadFile,auxSize);
    fprintf(auxWrite,"const unsigned char gb_sna_%s_48k[]={\n",cadFile);
   }
   else{
    fprintf(auxWrite," //sna 128K %s %d bytes\n\n",cadFile,auxSize);
    fprintf(auxWrite,"const unsigned char gb_sna_%s_128k[]={\n",cadFile);
   }
      
   //printf ("Tam %d",auxSize);
   fread(gb_bufferFile,1,auxSize,auxRead);
   for (long i=0;i<auxSize;i++)
   {
    fprintf(auxWrite,"0x%02X",gb_bufferFile[i]);
    if (i<(auxSize-1))
     fprintf(auxWrite,",");
    contLine++;
    if (contLine>15)
    {
     contLine=0;
     fprintf(auxWrite,"\n");
    }
   }
   fclose(auxRead);
  }
  
  
  fprintf(auxWrite,"\n};\n");
  fprintf(auxWrite,"#endif\n");
  fclose(auxWrite);
 }
}


//**********************************************
void WriteFileROM(char *cadPath,char *cadFile, char *cadDir)
{//path completo, file con path completo, cadDir nombre del directorio
 unsigned char contLine=0;
 FILE *auxWrite = NULL;
 FILE *auxRead = NULL;
 long auxSize=0;
 char cadDest[2048];
 //char cadOri[2048];
 //sprintf(cadOri,"%s\\%s",cadPath,cadFileSource);
 sprintf(cadDest,"output/dataFlash/roms/rom%s48k.h",cadDir);
 auxWrite = fopen(cadDest,"w+");
 if (auxWrite!= NULL)
 {
  //fprintf(auxWrite,"Fichero %s\n",cadFile);                
  fprintf(auxWrite,"#ifndef ROM_%s_48K_H\n",cadDir);
  fprintf(auxWrite," #define ROM_%s_48K_H\n",cadDir);     
  
  //WriteHexData(auxWrite,cadFile);  

  auxRead = fopen(cadFile,"rb");
  if (auxRead!=NULL)
  {
   auxSize = GetSizeFile(cadFile);
   
   fprintf(auxWrite," //rom 0 48K %s \n\n",cadDir);
   fprintf(auxWrite,"const unsigned char gb_rom_0_%s_48k[]={\n",cadDir);   
      
   //printf ("Tam %d",auxSize);
   fread(gb_bufferFile,1,auxSize,auxRead);
   for (long i=0;i<auxSize;i++)
   {
    fprintf(auxWrite,"0x%02X",gb_bufferFile[i]);
    if (i<(auxSize-1))
     fprintf(auxWrite,",");
    contLine++;
    if (contLine>15)
    {
     contLine=0;
     fprintf(auxWrite,"\n");
    }
   }
   fclose(auxRead);
  }
  
  
  fprintf(auxWrite,"\n};\n");
  fprintf(auxWrite,"#endif\n");
  fclose(auxWrite);
 }
}


//**********************************************
void WriteFile4ROMS(char *cadPath,char *cadDir,unsigned char *fileRomsOK)
{//path completo, cadDir nombre dir, fileRomsOK lista 4 roms
 unsigned char contLine=0;
 FILE *auxWrite = NULL;
 FILE *auxRead = NULL;
 long auxSize=0;
 char cadDest[2048];
 char cadFile[2048];
 //char cadOri[2048];
 //sprintf(cadOri,"%s\\%s",cadPath,cadFileSource);
 sprintf(cadDest,"output/dataFlash/roms/rom%s128k.h",cadDir);
 auxWrite = fopen(cadDest,"w+");
 if (auxWrite!= NULL)
 {  
  fprintf(auxWrite,"#ifndef ROM_%s_128K_H\n",cadDir);
  fprintf(auxWrite," #define ROM_%s_128K_H\n",cadDir);     

  // fprintf(auxWrite,"Array %d%d%d%d\n",fileRomsOK[0],fileRomsOK[1],fileRomsOK[2],fileRomsOK[3]);
  for (int i=0;i<4;i++)
  {
   if (fileRomsOK[i] == 1)
   {
    sprintf(cadFile, "%s/%d.rom",cadPath,i);
    //fprintf(auxWrite,"Fichero:%s\n",cadFile);

    auxRead = fopen(cadFile,"rb");
    if (auxRead!=NULL)
    {
     auxSize = GetSizeFile(cadFile);    
     fprintf(auxWrite," //rom 128K %s \n\n",cadDir);
     fprintf(auxWrite,"const unsigned char gb_rom_%d_%s_128k[]={\n",i,cadDir);
      
     //printf ("Tam %d",auxSize);
     fread(gb_bufferFile,1,auxSize,auxRead);
     for (long i=0;i<auxSize;i++)
     {
      fprintf(auxWrite,"0x%02X",gb_bufferFile[i]);
      if (i<(auxSize-1))
       fprintf(auxWrite,",");
      contLine++;
      if (contLine>15)
      {
       contLine=0;
       fprintf(auxWrite,"\n");
      }
     }
     fclose(auxRead);
     fprintf(auxWrite,"\n};\n");    
    }//fin if auxRead
   }//fin if fileRomsOK
  }//fin for 
  

  fprintf(auxWrite,"#endif\n");
  fclose(auxWrite);
 }
}



//**********************************************
void WriteHeadSCR_H(char *cadDefine)
{//Cabecera las 10 roms
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite,"#ifndef %s\n",cadDefine);
 fprintf(gb_fileWrite," #define %s\n",cadDefine); 
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #include <stddef.h>\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) != 0)
   fprintf(gb_fileWrite," #include \"scr/scr%s.h\"\n",gb_titles[i]);
 }
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #define max_list_scr_48 %d\n\n\n",gb_contRom);
 fprintf(gb_fileWrite," //screen 48K\n //Titulos\n");
 fprintf(gb_fileWrite," static const char * gb_list_scr_48k_title[max_list_scr_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else 
   fprintf(gb_fileWrite,"  \"%s\"",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," //Datos 48K\n");
 fprintf(gb_fileWrite," static const unsigned char * gb_list_scr_48k_data[max_list_scr_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  NULL");
  else 
   fprintf(gb_fileWrite,"  gb_scr_%s_48k",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");   
 }  
 fprintf(gb_fileWrite,"\n };\n");
 
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite,"#endif\n");
}


//**********************************************
void WriteHeadTAPE48k_H(char *cadDefine)
{//Los 48k
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite,"#ifndef %s\n",cadDefine);
 fprintf(gb_fileWrite," #define %s\n",cadDefine); 
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #include <stddef.h>\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) != 0)
   fprintf(gb_fileWrite," #include \"tape/tape%s.h\"\n",gb_titles[i]);
 }
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #define max_list_tape_48 %d\n\n\n",gb_contRom);
 fprintf(gb_fileWrite," //tapes 48K\n //Titulos\n");
 fprintf(gb_fileWrite," static const char * gb_list_tapes_48k_title[max_list_tape_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else 
   fprintf(gb_fileWrite,"  \"%s\"",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," //Datos 48K\n");
 fprintf(gb_fileWrite," static const unsigned char * gb_list_tapes_48k_data[max_list_tape_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  NULL");
  else 
   fprintf(gb_fileWrite,"  gb_tape_%s_48k",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");   
 }  
 fprintf(gb_fileWrite,"\n };\n");
 
 fprintf(gb_fileWrite,"\n");
 //fprintf(gb_fileWrite,"#endif\n");
}


//**********************************************
void WriteHeadTAPE128k_H()
{//Los 128k
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite,"\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) != 0)
   fprintf(gb_fileWrite," #include \"tape/tape%s.h\"\n",gb_titles[i]);
 }
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #define max_list_tape_128 %d\n\n\n",gb_contRom);
 fprintf(gb_fileWrite," //tapes 128K\n //Titulos\n");
 fprintf(gb_fileWrite," static const char * gb_list_tapes_128k_title[max_list_tape_128]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else 
   fprintf(gb_fileWrite,"  \"%s\"",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," //Datos 128K\n");
 fprintf(gb_fileWrite," static const unsigned char * gb_list_tapes_128k_data[max_list_tape_128]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  NULL");
  else 
   fprintf(gb_fileWrite,"  gb_tape_%s_128k",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");   
 }  
 fprintf(gb_fileWrite,"\n };\n\n");
}


//**********************************************
void WriteHeadSNA48k_H(char *cadDefine)
{//Los 48k
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite,"#ifndef %s\n",cadDefine);
 fprintf(gb_fileWrite," #define %s\n",cadDefine); 
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #include <stddef.h>\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) != 0)
   fprintf(gb_fileWrite," #include \"sna/sna%s.h\"\n",gb_titles[i]);
 }
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #define max_list_sna_48 %d\n\n\n",gb_contRom);
 fprintf(gb_fileWrite," //sna 48K\n //Titulos\n");
 fprintf(gb_fileWrite," static const char * gb_list_sna_48k_title[max_list_sna_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else 
   fprintf(gb_fileWrite,"  \"%s\"",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," //Datos 48K\n");
 fprintf(gb_fileWrite," static const unsigned char * gb_list_sna_48k_data[max_list_sna_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  NULL");
  else 
   fprintf(gb_fileWrite,"  gb_sna_%s_48k",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");   
 }  
 fprintf(gb_fileWrite,"\n };\n");
 
 fprintf(gb_fileWrite,"\n");
 //fprintf(gb_fileWrite,"#endif\n");
}


//**********************************************
void WriteHeadSNA128k_H()
{//Los 128k
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite,"\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) != 0)
   fprintf(gb_fileWrite," #include \"sna/sna%s.h\"\n",gb_titles[i]);
 }
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #define max_list_sna_128 %d\n\n\n",gb_contRom);
 fprintf(gb_fileWrite," //sna 128K\n //Titulos\n");
 fprintf(gb_fileWrite," static const char * gb_list_sna_128k_title[max_list_sna_128]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else 
   fprintf(gb_fileWrite,"  \"%s\"",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," //Datos 128K\n");
 fprintf(gb_fileWrite," static const unsigned char * gb_list_sna_128k_data[max_list_sna_128]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  NULL");
  else 
   fprintf(gb_fileWrite,"  gb_sna_%s_128k",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");   
 }  
 fprintf(gb_fileWrite,"\n };\n\n");
}

//**********************************************
void WriteHeadROM48k_H(char *cadDefine)
{//Los 48k
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite,"#ifndef %s\n",cadDefine);
 fprintf(gb_fileWrite," #define %s\n",cadDefine); 
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #include <stddef.h>\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) != 0)
   fprintf(gb_fileWrite," #include \"roms/rom%s48k.h\"\n",gb_titles[i]);
 }
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #define max_list_rom_48 %d\n\n\n",gb_contRom);
 fprintf(gb_fileWrite," //roms 48K\n //Titulos\n");
 fprintf(gb_fileWrite," static const char * gb_list_roms_48k_title[max_list_rom_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else 
   fprintf(gb_fileWrite,"  \"%s\"",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," //Datos 48K 1 ROM en slot 0\n");
 fprintf(gb_fileWrite," static const unsigned char * gb_list_roms_48k_data[max_list_rom_48]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  NULL");
  else 
   fprintf(gb_fileWrite,"  gb_rom_0_%s_48k",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");   
 }  
 fprintf(gb_fileWrite,"\n };\n");
 
 fprintf(gb_fileWrite,"\n");
 //fprintf(gb_fileWrite,"#endif\n");
}

//**********************************************
void WriteHeadROM128k_H()
{//Los 128k
 unsigned char fileRomsOK[4]={0,0,0,0};
 char auxCadFile[1024];
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite,"\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) != 0)
   fprintf(gb_fileWrite," #include \"roms/rom%s128k.h\"\n",gb_titles[i]);
 }
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," #define max_list_rom_128 %d\n\n\n",gb_contRom);
 fprintf(gb_fileWrite," //rom 128K\n //Titulos\n");
 fprintf(gb_fileWrite," static const char * gb_list_roms_128k_title[max_list_rom_128]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {   
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else 
   fprintf(gb_fileWrite,"  \"%s\"",gb_titles[i]);
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");
 fprintf(gb_fileWrite," //Datos 128K 4 roms en 4 slots\n");
 fprintf(gb_fileWrite," static const unsigned char * gb_list_roms_128k_data[max_list_rom_128][4]={\n");
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  memset(fileRomsOK,0,4);
  for (int j=0;j<4;j++)
  {
   sprintf(auxCadFile,"input/roms/128k/%s/%d.rom",gb_titles[i],j);
   fileRomsOK[j]= (access(auxCadFile, F_OK ) == 0 )?1:0;
  }  
  fprintf(gb_fileWrite,"  ");
  for (int j=0;j<4;j++)
  {
   if (strlen(gb_titles[i]) == 0)
    fprintf(gb_fileWrite,"  NULL");
   else
   {
    if (fileRomsOK[j] == 1)
     fprintf(gb_fileWrite,"gb_rom_%d_%s_128k",j,gb_titles[i]);
    else 
     fprintf(gb_fileWrite,"gb_rom_0_%s_128k",gb_titles[i]);
    if (j<3) 
     fprintf(gb_fileWrite,",");
   }
  }
  if (i<(gb_contRom-1))
    fprintf(gb_fileWrite,",\n");   
 }  
 fprintf(gb_fileWrite,"\n };\n\n");
}



//********************************************
void WriteSizeTape(int tipo)
{
 char cadDestino[1024];
 int auxSize;
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite," //Tamanio en bytes\n");
 if (tipo == 48)
  fprintf(gb_fileWrite," static const int gb_list_tapes_48k_size[max_list_tape_48]={\n");
 else 
  fprintf(gb_fileWrite," static const int gb_list_tapes_128k_size[max_list_tape_128]={\n"); 
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else
  {
   if (tipo==48)   
    sprintf(cadDestino,"input\\tape\\48k\\%s",gb_nameFiles[i]);
   else
    sprintf(cadDestino,"input\\tape\\128k\\%s",gb_nameFiles[i]);
   auxSize = GetSizeFile(cadDestino);
   fprintf(gb_fileWrite,"  %d",auxSize);
  }
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");  
}


//********************************************
/*
void WriteSizeSNA(int tipo)
{
 char cadDestino[1024];
 int auxSize;
 if (gb_fileWrite == NULL)
  return;
 fprintf(gb_fileWrite," //Tamanio en bytes\n");
 if (tipo == 48)
  fprintf(gb_fileWrite," static const char * gb_list_sna_48k_size[max_list_sna_48]={\n");
 else 
  fprintf(gb_fileWrite," static const char * gb_list_sna_128k_size[max_list_sna_128]={\n"); 
 for (unsigned char i=0;i<gb_contRom;i++)
 {
  if (strlen(gb_titles[i]) == 0)
   fprintf(gb_fileWrite,"  \"\"");
  else
  {
   if (tipo==48)   
    sprintf(cadDestino,"input\\sna\\48k\\%s",gb_nameFiles[i]);
   else
    sprintf(cadDestino,"input\\sna\\128k\\%s",gb_nameFiles[i]);
   auxSize = GetSizeFile(cadDestino);
   fprintf(gb_fileWrite,"  %d",auxSize);
  }
  if (i<(gb_contRom-1))
   fprintf(gb_fileWrite,",\n");
 }
 fprintf(gb_fileWrite,"\n };\n");
 fprintf(gb_fileWrite,"\n");  
}
*/

//**********************************************
void listFilesRecursively(char *basePath, TEnumFileType auxTypeFile, unsigned char tipo, unsigned char doRecursive)
{     
    char cadFileSource[1000];
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir)
        return; // Unable to open directory stream
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("%s\n", dp->d_name);
            
            strcpy(cadFileSource,dp->d_name);
            strcpy(gb_nameFiles[gb_contRom],dp->d_name); //Nombre completo
            RemoveExt(dp->d_name);
            strcpy(gb_titles[gb_contRom],dp->d_name);
            switch (auxTypeFile)
            {
             case file_scr: WriteFileSCR(gb_contRom,basePath,dp->d_name,cadFileSource); break;
             case file_tape: WriteFileTape(gb_contRom,basePath,dp->d_name,cadFileSource,tipo); break;
             case file_sna: WriteFileSNA(gb_contRom,basePath,dp->d_name,cadFileSource,tipo); break;             
             case file_rom_list: break;
            }
                        
            gb_contRom++;
            if (gb_contRom > (maxObjects-1))
             return;
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if (doRecursive == 1){
             listFilesRecursively(path, auxTypeFile, tipo, doRecursive );
            }
        }
    }
    closedir(dir);
}

//*************************************************
void ProcesaFicherosSCR()
{
 gb_contRom=0;
 gb_fileWrite = fopen("output/dataFlash/gbscr.h","w+");
 if (gb_fileWrite!= NULL)
 {
  listFilesRecursively("input/scr" , file_scr, 48,1);
  WriteHeadSCR_H("_GB_SCR_H");
  fclose(gb_fileWrite);
 }     
}

//*************************************************
void ProcesaFicherosTape()
{
 gb_contRom=0;
 gb_fileWrite = fopen("output/dataFlash/gbtape.h","w+");
 if (gb_fileWrite!= NULL)
 {
  listFilesRecursively("input/tape/48k", file_tape, 48,1);
  WriteHeadTAPE48k_H("_GB_TAPE_H");
  WriteSizeTape(48);
  gb_contRom=0;  
  listFilesRecursively("input/tape/128k", file_tape, 128,1);
  WriteHeadTAPE128k_H();  
  WriteSizeTape(128);
  fprintf(gb_fileWrite,"#endif\n");
  fclose(gb_fileWrite);
 }     
}

//*************************************************
void ProcesaFicherosSNA()
{
 gb_contRom=0;
 gb_fileWrite = fopen("output/dataFlash/gbsna.h","w+");
 if (gb_fileWrite!= NULL)
 {
  listFilesRecursively("input/sna/48k", file_sna, 48,1);
  WriteHeadSNA48k_H("_GB_SNA_H");
  //WriteSizeSNA(48);
  gb_contRom=0;  
  listFilesRecursively("input/sna/128k", file_sna, 128,1);
  WriteHeadSNA128k_H();  
  //WriteSizeSNA(128);
  fprintf(gb_fileWrite,"#endif\n");
  fclose(gb_fileWrite);
 }     
}

//********************************************************
void ProcesaROM48k()
{//Grabamos el fichero 0.rom
 int i;
 char cadFile[1024];
 char cadPath[1024];
 //memcpy(gb_nameDir,gb_nameFiles,gb_contRom);//Entramos en directorio 
 for (i=0;i<gb_contRom;i++)
 {
  sprintf(cadFile,"input/roms/48k/%s/0.rom",gb_nameFiles[i]);
  sprintf(cadPath,"input/roms/48k/%s",gb_nameFiles[i]);
  if (access(cadFile, F_OK ) == 0 ){
   WriteFileROM(cadPath,cadFile,gb_nameFiles[i]);
  }
 } 
}

//********************************************************
void ProcesaROM128k()
{//Grabamos de 0..4 rom
 int i,j;
 char cadFile[1024];
 char cadPath[1024];
 unsigned char fileRomsOK[4]={0,0,0,0};
 for (i=0;i<gb_contRom;i++)
 {  
  //sprintf(cadPath,"input/roms/128k/%s",gb_nameFiles[i]);
  //WriteFileROM(cadPath,cadFile,gb_nameFiles[i],48);
  memset(fileRomsOK,0,4);
  for (j=0;j<4;j++)
  {
   sprintf(cadFile,"input/roms/128k/%s/%d.rom",gb_nameFiles[i],j);
   fileRomsOK[j]= (access(cadFile, F_OK ) == 0 )?1:0;
  }
  sprintf(cadPath,"input/roms/128k/%s",gb_nameFiles[i]);
  WriteFile4ROMS(cadPath,gb_nameFiles[i],fileRomsOK); 
 } 
}

//*************************************************
void ProcesaFicherosROM()
{
 gb_contRom=0;
 gb_fileWrite = fopen("output/dataFlash/gbrom.h","w+");
 if (gb_fileWrite!= NULL)
 {
  listFilesRecursively("input/roms/48k", file_rom, 48,0);  
  WriteHeadROM48k_H("_GB_ROM_H");
  ProcesaROM48k();//Procesa solo archivo 0.rom
  
  gb_contRom=0;    
  listFilesRecursively("input/roms/128k", file_rom, 128,0);
  WriteHeadROM128k_H();
  ProcesaROM128k();//Procesa 0 .. 4 rom
  
  fprintf(gb_fileWrite,"#endif\n");
  fclose(gb_fileWrite);   
 }     
}


//********
//* MAIN *
//********
int main(int argc, char**argv)
{
 char path[100];    //Directory path to list files
 ShowHelp();

 InitTitles();
 
 ProcesaFicherosSCR();
 ProcesaFicherosTape();
 ProcesaFicherosSNA();
 ProcesaFicherosROM();
 //printf("Enter path to list files: ");    // Input path from user
 //scanf("%s", path);
 //listFilesRecursively(path);
 
 return 0;
}
