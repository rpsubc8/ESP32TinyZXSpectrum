//Author: ackerman
//Genera 255 entradas para WIFI
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define maxObjects 255
#define max_cad_title 8

//enum TEnumFileType{
// file_scr = 0,
// file_tape = 10,
// file_sna = 20,
// file_rom = 3,
// file_rom_list = 4
//};


//unsigned char gb_bufferFile[524288]; //512 KB archivo maximo

void SaveListFile(char *cadFichDestino);
void listFilesRecursively(char *basePath, unsigned char doRecursive);
void ShowHelp(void);
//void WriteFileSCR(unsigned char num,char *cadPath, char *cadFile, char *cadFileSource);
int GetSizeFile(char *cadFile);
void InitTitles(void);
void RemoveExt(char *cad);
void ProcesaFicherosSCR(char *cadPathOrigen, char cadFichDestino);

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

/*
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
*/



//**********************************************
/*
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

*/






//**********************************************
void listFilesRecursively(char *basePath, unsigned char doRecursive)
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
                        
            gb_contRom++;
            if (gb_contRom > (maxObjects-1))
             return;
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if (doRecursive == 1){
             listFilesRecursively(path, doRecursive );
            }
        }
    }
    closedir(dir);
}


//*************************************************
void SaveListFile(char *cadFichDestino)
{
 char cadAux[256];     
 char cadTemp[4096]; //Buffer temporal
 cadTemp[0]='\0';
 gb_fileWrite = fopen(cadFichDestino,"w+");
 if (gb_fileWrite != NULL)
 {
  for (int i=0;i<gb_contRom;i++)
  {
   int longitud= strlen(gb_titles[i]);
   //printf("%d\n",longitud);
   if (longitud < max_cad_title) //8
   {
    for (int j=0;j<longitud;j++)
    {
     cadAux[j]= gb_titles[i][j];
    }
    for (int j=longitud;j<max_cad_title;j++) //8
    {
     cadAux[j]=' ';
    }
    cadAux[max_cad_title]='\0'; //8 caracteres
   }
   else
   {
    strcpy(cadAux,gb_titles[i]);
   }
   //printf("%s",cadAux);
   //fprintf(gb_fileWrite,"%s",cadAux);
   strcat(cadTemp,cadAux);
  } //fin for
  
  fprintf(gb_fileWrite,"%s",cadTemp);
  
  fclose(gb_fileWrite);
 }     
}

//*************************************************
void ProcesaFicherosSCR(char *cadPathOrigen, char *cadFichDestino)
{
 gb_contRom=0; 
 listFilesRecursively(cadPathOrigen , 1);
 
 SaveListFile(cadFichDestino);
 
 
 //gb_fileWrite = fopen("output/dataFlash/gbscr.h","w+");
 //if (gb_fileWrite!= NULL)
 //{
  //listFilesRecursively("input/scr" , file_scr, 48,1);
//  WriteHeadSCR_H("_GB_SCR_H");
//  fclose(gb_fileWrite);
 //}     
}









//********
//* MAIN *
//********
int main(int argc, char**argv)
{
 char path[100];    //Directory path to list files 

 //printf("Argc:%d\n",argc);
 if (argc<=2)
 {
  ShowHelp();
  printf("Pocos argumentos\n");
  return 0;
 }
 InitTitles();
 
 ProcesaFicherosSCR(argv[1],argv[2]);
  
 return 0;
}
