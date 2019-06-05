#ifndef DCL_PLC_DEFINE_H
#define DCL_PLC_DEFINE_H

#include <QString>
#include "stdint.h"


enum PC_CMD{
    PC_WARNING              = 0,
    RESTORATION             = 20,

    IN_EMPTYBOX             = 11,
    IN_FULLBOX              = 12,

    AWAY_FULLBOX            = 21,
    AWAY_EMPTYBOX           = 22,

    IN_SWAPCHANNEL          = 31,
    IN_LINE                 = 32,

    IN_ARTIFICIALCHANNEL    = 41,
    IN_LINE_NOBAD           = 42,

    IN_302                  = 51,
    IN_127                  = 52,

    IN_EMPTYBOX_127         = 61,
    IN_EMPTYBOX_302         = 62,
    IN_FULLBOX_127          = 63,
    IN_FULLBOX_302          = 64,
};

enum PLC_CMD{
    PLC_WARNING     = 0,
    NOREQUESTSCAN   = 1,
    REQUESTSCAN     = 2,
};

#pragma pack(push)  //将当前pack设置压栈保存
#pragma pack(1)     //必须在结构体定义之前使用
struct PC_DATAAREA{
    signed char PC_1_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PC_1_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PC_2_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PC_2_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PC_SWAPSTATION_INEMPTYBOX_CMD;
    signed char PC_SWAPSTATION_INEMPTYBOX_Standby;

    signed char PC_SWAPSTATION_INEFULLBOX_CMD;
    signed char PC_SWAPSTATION_INFULLBOX_Standby;

    signed char PC_SWAPCHANNEL_CMD;
    signed char PC_SWAPCHANNEL_Standby;

    signed char PC_SEVENTHWAREHOUSEPUSHOUT_CMD;
    signed char PC_SEVENTHWAREHOUSEPUSHOUT_Standby;

    signed char PC_CHECKQUALITY_CMD;
    signed char PC_CHECKQUALITY_Standby;

    signed char PC_1_CACHESTATION_CMD;
    signed char PC_1_CACHESTATION_Standby;

    signed char PC_2_CACHESTATION_CMD;
    signed char PC_2_CACHESTATION_Standby;

    signed char PC_3_CACHESTATION_CMD;
    signed char PC_3_CACHESTATION_Standby;

    PC_DATAAREA(){
        PC_1_TRIDWAREHOUSEPUSHOUT_CMD       = 20;
        PC_1_TRIDWAREHOUSEPUSHOUT_Standby   = 0x00;
        PC_2_TRIDWAREHOUSEPUSHOUT_CMD       = 20;
        PC_2_TRIDWAREHOUSEPUSHOUT_Standby   = 0x00;

        PC_SWAPSTATION_INEMPTYBOX_CMD           = 20;
        PC_SWAPSTATION_INEMPTYBOX_Standby   = 0x00;
        PC_SWAPSTATION_INEFULLBOX_CMD       = 20;
        PC_SWAPSTATION_INFULLBOX_Standby    = 0x00;
        PC_SWAPCHANNEL_CMD                  = 20;
        PC_SWAPCHANNEL_Standby              = 0x00;

        PC_SEVENTHWAREHOUSEPUSHOUT_CMD      = 20;
        PC_SEVENTHWAREHOUSEPUSHOUT_Standby  = 0x00;
        PC_CHECKQUALITY_CMD                 = 20;
        PC_CHECKQUALITY_Standby             = 0x00;
        PC_1_CACHESTATION_CMD               = 20;
        PC_1_CACHESTATION_Standby           = 0x00;

        PC_2_CACHESTATION_CMD               = 20;
        PC_2_CACHESTATION_Standby           = 0x00;
        PC_3_CACHESTATION_CMD               = 20;
        PC_3_CACHESTATION_Standby           = 0x00;
    }
};

struct PLC_DATAAREA{
    signed char PLC_1_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PLC_1_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PLC_2_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PLC_2_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PLC_SWAPSTATION_INEMPTYBOX_CMD;
    signed char PLC_SWAPSTATION_INEMPTYBOX_Standby;

    signed char PLC_SWAPSTATION_INEFULLBOX_CMD;
    signed char PLC_SWAPSTATION_INFULLBOX_Standby;

    signed char PLC_SWAPCHANNEL_CMD;
    signed char PLC_SWAPCHANNEL_Standby;

    signed char PLC_SEVENTHWAREHOUSEPUSHOUT_CMD;
    signed char PLC_SEVENTHWAREHOUSEPUSHOUT_Standby;

    signed char PLC_CHECKQUALITY_CMD;
    signed char PLC_CHECKQUALITY_Standby;

    signed char PLC_1_CACHESTATION_CMD;
    signed char PLC_1_CACHESTATION_Standby;

    signed char PLC_2_CACHESTATION_CMD;
    signed char PLC_2_CACHESTATION_Standby;

    signed char PLC_3_CACHESTATION_CMD;
    signed char PLC_3_CACHESTATION_Standby;

    PLC_DATAAREA(){
        PLC_1_TRIDWAREHOUSEPUSHOUT_CMD      = 0x01;
        PLC_1_TRIDWAREHOUSEPUSHOUT_Standby  = 0x00;
        PLC_2_TRIDWAREHOUSEPUSHOUT_CMD      = 0x01;
        PLC_2_TRIDWAREHOUSEPUSHOUT_Standby  = 0x00;

        PLC_SWAPSTATION_INEMPTYBOX_CMD      = 0x01;
        PLC_SWAPSTATION_INEMPTYBOX_Standby  = 0x00;
        PLC_SWAPSTATION_INEFULLBOX_CMD      = 0x01;
        PLC_SWAPSTATION_INFULLBOX_Standby   = 0x00;
        PLC_SWAPCHANNEL_CMD                 = 0x01;
        PLC_SWAPCHANNEL_Standby             = 0x00;

        PLC_SEVENTHWAREHOUSEPUSHOUT_CMD     = 0x01;
        PLC_SEVENTHWAREHOUSEPUSHOUT_Standby = 0x00;
        PLC_CHECKQUALITY_CMD                = 0x01;
        PLC_CHECKQUALITY_Standby            = 0x00;
        PLC_1_CACHESTATION_CMD              = 0x01;
        PLC_1_CACHESTATION_Standby          = 0x00;

        PLC_2_CACHESTATION_CMD              = 0x01;
        PLC_2_CACHESTATION_Standby          = 0x00;
        PLC_3_CACHESTATION_CMD              = 0x01;
        PLC_3_CACHESTATION_Standby          = 0x00;
    }
};

struct PC_DATAAREA_1{
    signed char PC_1_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PC_1_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PC_2_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PC_2_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PC_SWAPSTATION_INEMPTYBOX_CMD;
    signed char PC_SWAPSTATION_INEMPTYBOX_Standby;

    signed char PC_SWAPSTATION_INEFULLBOX_CMD;
    signed char PC_SWAPSTATION_INFULLBOX_Standby;

    signed char PC_SWAPCHANNEL_CMD;
    signed char PC_SWAPCHANNEL_Standby;


    PC_DATAAREA_1(){
        PC_1_TRIDWAREHOUSEPUSHOUT_CMD       = 20;
        PC_1_TRIDWAREHOUSEPUSHOUT_Standby   = 0x00;
        PC_2_TRIDWAREHOUSEPUSHOUT_CMD       = 20;
        PC_2_TRIDWAREHOUSEPUSHOUT_Standby   = 0x00;

        PC_SWAPSTATION_INEMPTYBOX_CMD           = 20;
        PC_SWAPSTATION_INEMPTYBOX_Standby   = 0x00;
        PC_SWAPSTATION_INEFULLBOX_CMD       = 20;
        PC_SWAPSTATION_INFULLBOX_Standby    = 0x00;
        PC_SWAPCHANNEL_CMD                  = 20;
        PC_SWAPCHANNEL_Standby              = 0x00;
    }
};

struct PLC_DATAAREA_1{
    signed char PLC_1_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PLC_1_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PLC_2_TRIDWAREHOUSEPUSHOUT_CMD;
    signed char PLC_2_TRIDWAREHOUSEPUSHOUT_Standby;

    signed char PLC_SWAPSTATION_INEMPTYBOX_CMD;
    signed char PLC_SWAPSTATION_INEMPTYBOX_Standby;

    signed char PLC_SWAPSTATION_INEFULLBOX_CMD;
    signed char PLC_SWAPSTATION_INFULLBOX_Standby;

    signed char PLC_SWAPCHANNEL_CMD;
    signed char PLC_SWAPCHANNEL_Standby;
    PLC_DATAAREA_1(){
        PLC_1_TRIDWAREHOUSEPUSHOUT_CMD      = 0x01;
        PLC_1_TRIDWAREHOUSEPUSHOUT_Standby  = 0x00;
        PLC_2_TRIDWAREHOUSEPUSHOUT_CMD      = 0x01;
        PLC_2_TRIDWAREHOUSEPUSHOUT_Standby  = 0x00;

        PLC_SWAPSTATION_INEMPTYBOX_CMD      = 0x01;
        PLC_SWAPSTATION_INEMPTYBOX_Standby  = 0x00;
        PLC_SWAPSTATION_INEFULLBOX_CMD      = 0x01;
        PLC_SWAPSTATION_INFULLBOX_Standby   = 0x00;
        PLC_SWAPCHANNEL_CMD                 = 0x01;
        PLC_SWAPCHANNEL_Standby             = 0x00;
    }
};

struct PC_DATAAREA_2{
    signed char PC_SEVENTHWAREHOUSEPUSHOUT_CMD;
    signed char PC_SEVENTHWAREHOUSEPUSHOUT_Standby;

    PC_DATAAREA_2(){
        PC_SEVENTHWAREHOUSEPUSHOUT_CMD      = 20;
        PC_SEVENTHWAREHOUSEPUSHOUT_Standby  = 0x00;
    }
};

struct PLC_DATAAREA_2{
    signed char PLC_CHECKQUALITY_CMD;
    signed char PLC_CHECKQUALITY_Standby;

    signed char PLC_1_CACHESTATION_CMD;
    signed char PLC_1_CACHESTATION_Standby;

    signed char PLC_2_CACHESTATION_CMD;
    signed char PLC_2_CACHESTATION_Standby;

    signed char PLC_3_CACHESTATION_CMD;
    signed char PLC_3_CACHESTATION_Standby;

    PLC_DATAAREA_2(){
        PLC_CHECKQUALITY_CMD                = 0x01;
        PLC_CHECKQUALITY_Standby            = 0x00;
        PLC_1_CACHESTATION_CMD              = 0x01;
        PLC_1_CACHESTATION_Standby          = 0x00;

        PLC_2_CACHESTATION_CMD              = 0x01;
        PLC_2_CACHESTATION_Standby          = 0x00;
        PLC_3_CACHESTATION_CMD              = 0x01;
        PLC_3_CACHESTATION_Standby          = 0x00;
    }
};

struct PC_DATAAREA_3{
    signed char PC_CHECKQUALITY_CMD;
    signed char PC_CHECKQUALITY_Standby;

    signed char PC_1_CACHESTATION_CMD;
    signed char PC_1_CACHESTATION_Standby;

    signed char PC_2_CACHESTATION_CMD;
    signed char PC_2_CACHESTATION_Standby;

    signed char PC_3_CACHESTATION_CMD;
    signed char PC_3_CACHESTATION_Standby;

    PC_DATAAREA_3(){
        PC_CHECKQUALITY_CMD                 = 20;
        PC_CHECKQUALITY_Standby             = 0x00;
        PC_1_CACHESTATION_CMD               = 20;
        PC_1_CACHESTATION_Standby           = 0x00;

        PC_2_CACHESTATION_CMD               = 20;
        PC_2_CACHESTATION_Standby           = 0x00;
        PC_3_CACHESTATION_CMD               = 20;
        PC_3_CACHESTATION_Standby           = 0x00;
    }
};

struct PLC_DATAAREA_3{
    signed char PLC_CHECKQUALITY_CMD;
    signed char PLC_CHECKQUALITY_Standby;

    signed char PLC_1_CACHESTATION_CMD;
    signed char PLC_1_CACHESTATION_Standby;

    signed char PLC_2_CACHESTATION_CMD;
    signed char PLC_2_CACHESTATION_Standby;

    signed char PLC_3_CACHESTATION_CMD;
    signed char PLC_3_CACHESTATION_Standby;

    PLC_DATAAREA_3(){
        PLC_CHECKQUALITY_CMD                = 0x01;
        PLC_CHECKQUALITY_Standby            = 0x00;
        PLC_1_CACHESTATION_CMD              = 0x01;
        PLC_1_CACHESTATION_Standby          = 0x00;

        PLC_2_CACHESTATION_CMD              = 0x01;
        PLC_2_CACHESTATION_Standby          = 0x00;
        PLC_3_CACHESTATION_CMD              = 0x01;
        PLC_3_CACHESTATION_Standby          = 0x00;
    }
};

#pragma pack(pop) // 恢复先前的pack设置
#endif // DCL_PLC_DEFINE_H
