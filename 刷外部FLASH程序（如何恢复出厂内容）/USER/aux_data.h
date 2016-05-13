#ifndef __AUX_DATA_H
#define	__AUX_DATA_H

#include "stm32f4xx.h"

typedef enum 
{
    UPDATE,
    DO_NOT_UPDATE
}Burn_Option_Typedef;

typedef struct
{
  const char*               filename;      //��SD����¼����ʱ���ļ�·��
  const char*               description;   //����
  uint32_t                  start_addr;    //��ʼ��ַ
  uint32_t                  length;        //����
  Burn_Option_Typedef     burn_option;   
  
}Aux_Data_Typedef;

typedef enum 
{
    AUX_DATA_ERROR = -1,
  
    AUX_XBF_XINSONGTI_25, 
    AUX_XBF_XINSONGTI_19, 
    AUX_UNIGBK, 
    AUX_WIFI_FW,
    AUX_WIFI_PARA1,
    AUX_WIFI_PARA2,
    AUX_GB2312_H2424,  
    AUX_FILE_SYSTEM,      //FLASH�ļ�ϵͳ
  
    AUX_MAX_NUM,
} aux_data_t; 

extern  Aux_Data_Typedef  burn_data[AUX_MAX_NUM];


/*��Ϣ���*/
#define BURN_DEBUG_ON         1
#define BURN_DEBUG_FUNC_ON    0

#define BURN_INFO(fmt,arg...)           printf("<<-BURN-INFO->> "fmt"\n",##arg)
#define BURN_ERROR(fmt,arg...)          printf("<<-BURN-ERROR->> "fmt"\n",##arg)
#define BURN_DEBUG(fmt,arg...)          do{\
                                          if(BURN_DEBUG_ON)\
                                          printf("<<-BURN-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define BURN_DEBUG_FUNC()               do{\
                                         if(BURN_DEBUG_FUNC_ON)\
                                         printf("<<-BURN-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)

int burn_file_sd2flash(Aux_Data_Typedef *dat,uint8_t file_num);


#endif /* __BURN_DATA_H */
