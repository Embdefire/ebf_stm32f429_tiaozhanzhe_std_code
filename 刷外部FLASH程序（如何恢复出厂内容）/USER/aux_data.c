/**
  ******************************************************************************
  * @file    aux_data.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ڴ�SD����¼FLASH��������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */  


#include "aux_data.h"
#include "ff.h"
#include "./fatfs/drivers/fatfs_flash_spi.h"
#include "./Bsp/led/bsp_led.h" 


/***************************************************************************************************************/
/*
�� �����ⲿFlashʹ�����˵����W25Q128��	

�������������к󣬻��FLASHоƬˢ���������ݡ�
//Tab = 4���ո�

|-------------------------------------------------------------------------------------------------------------------|												
|���	|�ļ���/����					|����										|��ʼ��ַ		|����				|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|1		|�ⲿflash��д����			|Ԥ�������Flash����							|0				|4096 (BYTE)		|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|2		|Ԥ��						|Ԥ��										|1*4096			|59*4096 (BYTE)		|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|3		|app.c						|XBF�ֿ��ļ���emWinʹ��,������25.xbf��		|60*4096		|649*4096(1.23MB)	|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|4		|app.c						|XBF�ֿ��ļ�(emWinʹ��,������19.xbf)			|710*4096		|529*4096(172KB)	|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|4		|firecc936.c				|�ļ�ϵͳ����֧���ֿ�(emWinʹ��,UNIGBK.BIN)	|1240*4096		|43*4096(172KB)		|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|5		|EMW1062ģ��				|WIFIģ��̼�(BCM43362-5.90.230.12.bin)		|1284*4096		|62*4096(248KB)		|
|5.1	|EMW1062ģ��				|WIFIģ�����1(Ԥ��������Ҫд�ļ�)		    |1347*4096		|1*4096(4KB)		|
|5.2	|EMW1062ģ��				|WIFIģ�����2(Ԥ��������Ҫд�ļ�)		    |1348*4096		|1*4096(4KB)		|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|6		|���������ʾ����			|��������ֿ⣨GB2312_H2424.FON��			|1360*4096		|144*4096(576KB)	|
|-------|---------------------------|-------------------------------------------|---------------|-------------------|
|7		|diskio.c					|FATFS�ļ�ϵͳ��emWinʹ�ã�					|1536*4096		|2560*4096(10MB)	|
|-------------------------------------------------------------------------------------------------------------------|
*/
/*******************************************************************************************************************/



Aux_Data_Typedef  burn_data[] =  
{
  [AUX_XBF_XINSONGTI_25] =
  {
    .filename           =   "0:������25.xbf",
    .description        =   "XBF�ֿ��ļ���emWinʹ��,������25.xbf��",
    .start_addr         =   60*4096 ,
    .length             =   649*4096, 
    .burn_option      =  UPDATE,
  },
  
  [AUX_XBF_XINSONGTI_19] =
  {
    .filename           =   "0:������19.xbf",
    .description        =   "XBF�ֿ��ļ�(emWinʹ��,������19.xbf)",
    .start_addr         =   710*4096 ,
    .length             =   529*4096, 
    .burn_option      =  UPDATE,
  },
  
  [AUX_UNIGBK] =
  {
    .filename           =   "0:UNIGBK.BIN",
    .description        =   "�ļ�ϵͳ����֧���ֿ�(emWinʹ��,UNIGBK.BIN)",
    .start_addr         =   1240*4096 ,
    .length             =   43*4096,
    .burn_option      =  UPDATE,      
  },
  
  [AUX_WIFI_FW] =
  {
    .filename           =   "0:BCM43362-5.90.230.12.bin",
    .description        =   "WIFIģ��̼�",
    .start_addr         =   1284*4096 ,
    .length             =   62*4096, 
    .burn_option      =  UPDATE,
  },
  
  [AUX_WIFI_PARA1] =
  {
    .filename           =   "",
    .description        =   "WIFIģ�����1",
    .start_addr         =   1347*4096 ,
    .length             =   1*4096, 
    .burn_option      =  DO_NOT_UPDATE,
  },
  
  [AUX_WIFI_PARA2] =
  {
    .filename           =   "",
    .description        =   "WIFIģ�����2",
    .start_addr         =   1348*4096 ,
    .length             =   1*4096, 
    .burn_option      =  DO_NOT_UPDATE,
  },
  
  [AUX_GB2312_H2424] =
  {
    .filename           =   "0:GB2312_H2424.FON",
    .description        =   "��������ֿ�",
    .start_addr         =   1360*4096 ,
    .length             =   144*4096,
    .burn_option      =  UPDATE,      
  },
  
  [AUX_FILE_SYSTEM] =
  {
    .filename           =   "",
    .description        =   "FATFS�ļ�ϵͳ",
    .start_addr         =   1536*4096 ,
    .length             =   2560*4096, 
    .burn_option      =  DO_NOT_UPDATE,
  }

};

FIL fnew;													/* file objects */
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT result; 
UINT  bw;            					    /* File R/W count */
 
/**
  * @brief  ��FLASHд���ļ�
  * @param  dat��Ҫд����ļ����ݵ���Ϣ
  * @param  file_num��Ҫд����ļ�����
  * @retval ��������FR_OK
  */
int burn_file_sd2flash(Aux_Data_Typedef *dat,uint8_t file_num) 
{
    uint8_t i;

    uint32_t write_addr=0,j=0;
    uint8_t tempbuf[256],flash_buf[256];
    
    result = f_mount(&fs,"0:",1);
    
    //����ļ�ϵͳ����ʧ�ܾ��˳�
    if(result != FR_OK)
    {
      BURN_ERROR("f_mount ERROR!");
      LED_RED;
      return result;
    }
    
    for(i=0;i<file_num;i++)
    {
       if (dat[i].burn_option == DO_NOT_UPDATE)
          continue;
       
       BURN_INFO("-------------------------------------"); 
       BURN_INFO("׼����¼���ݣ�%s",dat[i].description);
       LED_BLUE;
       
       result = f_open(&fnew,dat[i].filename,FA_OPEN_EXISTING | FA_READ);
        if(result != FR_OK)
        {
            BURN_ERROR("���ļ�ʧ�ܣ�");
            LED_RED;
            return result;
        }
        
      BURN_INFO("���ڲ���Ҫʹ�õ�FLASH�ռ�...");
  
      write_addr = dat[i].start_addr;
        
      for(j=0;j < dat[i].length/4096 ;j++)//������������ʼλ��710*4096��2116KB
      {
        SPI_FLASH_SectorErase(write_addr+j*4096);
      }
      
      BURN_INFO("������FLASHд������...");
      
      write_addr = dat[i].start_addr;
      while(result == FR_OK) 
      {
        result = f_read( &fnew, tempbuf, 256, &bw);//��ȡ����	 
        if(result!=FR_OK)			 //ִ�д���
        {
          BURN_ERROR("��ȡ�ļ�ʧ�ܣ�");
          LED_RED;
          return result;
        }      
        SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //�������ݵ��ⲿflash��    
        write_addr+=256;				
        if(bw !=256)break;
      }

        BURN_INFO("����д�����,��ʼУ������...");

        //У������
      write_addr = dat[i].start_addr;
     
      f_lseek(&fnew,0);
      
      while(result == FR_OK) 
      {
        result = f_read( &fnew, tempbuf, 256, &bw);//��ȡ����	 
        if(result!=FR_OK)			 //ִ�д���
        {
          BURN_ERROR("��ȡ�ļ�ʧ�ܣ�");
          LED_RED;
          return result;
        }      
        SPI_FLASH_BufferRead(flash_buf,write_addr,bw);  //��FLASH�ж�ȡ����
        write_addr+=bw;		
        
        for(j=0;j<bw;j++)
        {
          if(tempbuf[i] != flash_buf[i])
          {
            BURN_ERROR("����У��ʧ�ܣ�");
            LED_RED;
            return -1;
          }
         }  
     
        if(bw !=256)break;//�����ļ�β
      }
      

      BURN_INFO("����У��ɹ���");
      BURN_INFO("�ļ���%s д�����",dat[i].filename);
      BURN_INFO("-------------------------------------");
      LED_GREEN;

      f_close(&fnew); 
    }
    
    
    BURN_INFO("************************************");
    BURN_INFO("�����ļ�������¼�ɹ���");
    return FR_OK;


}

















