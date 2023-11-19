#ifndef CAN_DISPATCHER_TABLE_H
#define CAN_DISPATCHER_TABLE_H
#include <QObject>
//#include "can_dispatcher.h"
//#include "canmessagebase.h"
//#include "ui_projectwindow.h"
struct Cit_Candata_Struct {

    int id = 0;
    uint8_t data[8] = {0};
};


const struct Cit_Candata_Struct can_cit_cmd_t[]=
{
    //ID     data
    {0x6E9,{0x0A ,0x01 ,0x00 ,0x00 ,0x00 ,0x00, 0x00 ,0x00}},
    {0x6E9,{0x0A ,0x02 ,0x00 ,0x00 ,0x00 ,0x00, 0x00 ,0x00}},
    //{0x6E9,{}}
};

const uint  CAN_ID_RECEIVE_TABLE_SUM[2]={
    0x6EA,
    0x6EB
};

typedef enum
{
    label_test,//
    Candata_singal_list_MAX
}Candata_singal_enum;
#endif // CAN_DISPATCHER_TABLE_H
