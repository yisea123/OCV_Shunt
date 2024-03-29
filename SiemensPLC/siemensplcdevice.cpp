#include "siemensplcdevice.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QMetaType>

SiemensPLCDevice::SiemensPLCDevice(QThread *parent)
    : QThread(parent)
{    
    qRegisterMetaType<PLC_DATAAREA>("PLC_DATAAREA");

    InitSiemensPLC();
}

SiemensPLCDevice::~SiemensPLCDevice()
{
    m_bRunSwitch = false;

    this->quit();
    this->wait();

    if(m_pTS7Client != NULL){
        delete m_pTS7Client;
        m_pTS7Client = NULL;
    }
}

void SiemensPLCDevice::InitSiemensPLC()
{
    m_nWriteDBNumber    = 0;
    m_nWriteStart       = 0;
    m_nWriteDataSize    = 0;
    m_nReadDBNumber     = 0;
    m_nReadStart        = 0;
    m_nReadDataSize     = 0 ;

    m_bConnectState     = false;
    m_bRunSwitch        = true;
    m_bUploadCmdSwitch  = false;

    m_strIP             = "";

    m_pTS7Client        = NULL;
    m_pTS7Client        = new TS7Client();

    this->start();
}

void SiemensPLCDevice::SetDeviceIP(const QString &str_IP)
{
    m_strIP = str_IP;
}

void SiemensPLCDevice::SetWriteParameter(const int &n_DBNumber,
                                         const int &n_Start,
                                         const int &n_DataSize)
{
    m_nWriteDBNumber = n_DBNumber;
    m_nWriteStart = n_Start;
    m_nWriteDataSize = n_DataSize;
}

void SiemensPLCDevice::SetReadParameter(const int &n_DBNumber,
                                        const int &n_Start,
                                        const int &n_DataSize)
{
    m_nReadDBNumber = n_DBNumber;
    m_nReadStart = n_Start;
    m_nReadDataSize = n_DataSize;
}

bool SiemensPLCDevice::ConnectDevice()
{
    if(m_pTS7Client == NULL)
        return false;

    if(m_pTS7Client->Connected())
        return false;

    if(m_strIP == "")
        return false;

    if(m_pTS7Client->ConnectTo(m_strIP.toLatin1(), 0 ,1) != 0)
        return false;

    //��ʼ���ڴ���
    if(!WriteData(m_WritePCData))
        return false;

    return true;
}

bool SiemensPLCDevice::DisconnectDevice()
{
    if(m_pTS7Client == NULL)
        return false;

    if(!m_pTS7Client->Connected())
        return false;

    if(m_pTS7Client->Disconnect() != 0)
        return false;

    return true;
}

bool SiemensPLCDevice::GetConnectState()
{
    if(m_pTS7Client == NULL)
        return false;

    return m_pTS7Client->Connected();
}

void SiemensPLCDevice::GetWritePCData(PC_DATAAREA &PCData)
{
    PCData = m_WritePCData;
}

void SiemensPLCDevice::UploadCmdSwitch(const bool &b_Switch)
{
    m_bUploadCmdSwitch = b_Switch;
}

bool SiemensPLCDevice::SendCmd(int n_Station, PC_CMD cmd)
{
    QMutexLocker o_Locker(&m_oQMutex);

    switch (n_Station) {
    case 1:
        m_WritePCData.PC_1_TRIDWAREHOUSEPUSHOUT_CMD = (signed char)cmd;
        break;
    case 2:
        m_WritePCData.PC_2_TRIDWAREHOUSEPUSHOUT_CMD = (signed char)cmd;
        break;
    case 3:
        m_WritePCData.PC_SWAPSTATION_INEMPTYBOX_CMD = (signed char)cmd;
        break;
    case 4:
        m_WritePCData.PC_SWAPSTATION_INEFULLBOX_CMD = (signed char)cmd;
        break;
    case 5:
        m_WritePCData.PC_SWAPCHANNEL_CMD = (signed char)cmd;
        break;
    case 6:
        m_WritePCData.PC_SEVENTHWAREHOUSEPUSHOUT_CMD = (signed char)cmd;
        break;
    case 7:
        m_WritePCData.PC_CHECKQUALITY_CMD = (signed char)cmd;
        break;
    case 8:
        m_WritePCData.PC_1_CACHESTATION_CMD = (signed char)cmd;
        break;
    case 9:
        m_WritePCData.PC_2_CACHESTATION_CMD = (signed char)cmd;
        break;
    case 10:
        m_WritePCData.PC_3_CACHESTATION_CMD = (signed char)cmd;
        break;
    default:
        return false;
    }

    if(!WriteData(m_WritePCData))
        return false;

    return true;
}

bool SiemensPLCDevice::GetReadData(PLC_DATAAREA &PLCData)
{
    if(m_pTS7Client == NULL)
        return false;

    if(!m_pTS7Client->Connected())
        return false;

    if(m_pTS7Client->DBRead(m_nReadDBNumber, m_nReadStart, m_nReadDataSize, &PLCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice::GetWriteData(PC_DATAAREA &PCData)
{
    if(m_pTS7Client == NULL)
        return false;

    if(!m_pTS7Client->Connected())
        return false;

    if(m_pTS7Client->DBRead(m_nReadDBNumber, m_nReadStart, m_nReadDataSize, &PCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice::WriteData(PC_DATAAREA &PCData)
{
    if(m_pTS7Client == NULL)
        return false;

    if(!m_pTS7Client->Connected())
        return false;

    if(m_pTS7Client->DBWrite(m_nWriteDBNumber, m_nWriteStart, m_nWriteDataSize, &PCData) != 0)
        return false;

    return true;
}

void SiemensPLCDevice::GetPLCCmd()
{
    if(!m_bUploadCmdSwitch)
        return;

    unsigned char ucCurrentData[40] = {};
    unsigned char ucLastData[40] = {};

    memset(ucCurrentData, 0, 40);
    memset(ucLastData, 0, 40);

    memcpy(ucCurrentData, &m_CurrentReadPLCData, 20);
    memcpy(ucLastData, &m_LastReadPLCData, 20);

    for (int i = 0; i < 20/2; i++) {
        if(ucCurrentData[i*2] != ucLastData[i*2]){
            emit sig_GetCmd(i, (PLC_CMD)ucCurrentData[i*2]);
        }
    }

    /*
    if(m_CurrentReadPLCData.PLC_Entryance_CMD != m_LastReadPLCData.PLC_Entryance_CMD){
        emit sig_GetCmd(1, (PLC_CMD)m_CurrentReadPLCData.PLC_Entryance_CMD);
    }

    if(m_CurrentReadPLCData.PLC_Exit_CMD != m_LastReadPLCData.PLC_Exit_CMD){
        emit sig_GetCmd(2, (PLC_CMD)m_CurrentReadPLCData.PLC_Exit_CMD);
    }

    if(m_CurrentReadPLCData.PLC_1_QualityCheck_CMD != m_LastReadPLCData.PLC_1_QualityCheck_CMD){
        emit sig_GetCmd(3, (PLC_CMD)m_CurrentReadPLCData.PLC_1_QualityCheck_CMD);
    }

    if(m_CurrentReadPLCData.PLC_1_SortingStation_CMD != m_LastReadPLCData.PLC_1_SortingStation_CMD){
        emit sig_GetCmd(4, (PLC_CMD)m_CurrentReadPLCData.PLC_1_SortingStation_CMD);
    }

    if(m_CurrentReadPLCData.PLC_1_BadStore_CMD != m_LastReadPLCData.PLC_1_BadStore_CMD){
        emit sig_GetCmd(5, (PLC_CMD)m_CurrentReadPLCData.PLC_1_BadStore_CMD);
    }

    if(m_CurrentReadPLCData.PLC_2_QualityCheck_CMD != m_LastReadPLCData.PLC_2_QualityCheck_CMD){
        emit sig_GetCmd(6, (PLC_CMD)m_CurrentReadPLCData.PLC_2_QualityCheck_CMD);
    }

    if(m_CurrentReadPLCData.PLC_2_SortingStation_CMD != m_LastReadPLCData.PLC_2_SortingStation_CMD){
        emit sig_GetCmd(7, (PLC_CMD)m_CurrentReadPLCData.PLC_2_SortingStation_CMD);
    }

    if(m_CurrentReadPLCData.PLC_2_BadStore_CMD != m_LastReadPLCData.PLC_2_BadStore_CMD){
        emit sig_GetCmd(8, (PLC_CMD)m_CurrentReadPLCData.PLC_2_BadStore_CMD);
    }

    if(m_CurrentReadPLCData.PLC_3L_QualityCheck_CMD != m_LastReadPLCData.PLC_3L_QualityCheck_CMD){
        emit sig_GetCmd(9, (PLC_CMD)m_CurrentReadPLCData.PLC_3L_QualityCheck_CMD);
    }

    if(m_CurrentReadPLCData.PLC_3R_QualityCheck_CMD != m_LastReadPLCData.PLC_3R_QualityCheck_CMD){
        emit sig_GetCmd(10, (PLC_CMD)m_CurrentReadPLCData.PLC_3R_QualityCheck_CMD);
    }

    if(m_CurrentReadPLCData.PLC_3_SortingStation_CMD != m_LastReadPLCData.PLC_3_SortingStation_CMD){
        emit sig_GetCmd(11, (PLC_CMD)m_CurrentReadPLCData.PLC_3_SortingStation_CMD);
    }

    if(m_CurrentReadPLCData.PLC_3_BadStore_CMD != m_LastReadPLCData.PLC_3_BadStore_CMD){
        emit sig_GetCmd(12, (PLC_CMD)m_CurrentReadPLCData.PLC_3_BadStore_CMD);
    }

    if(m_CurrentReadPLCData.PLC_HeadDCL_CMD != m_LastReadPLCData.PLC_HeadDCL_CMD){
        emit sig_GetCmd(13, (PLC_CMD)m_CurrentReadPLCData.PLC_HeadDCL_CMD);
    }

    */

    m_LastReadPLCData = m_CurrentReadPLCData;

}

void SiemensPLCDevice::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime && m_bRunSwitch){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void SiemensPLCDevice::run()
{
    while(m_bRunSwitch){
        WorkSleep(500);

        if(m_pTS7Client == NULL)
            continue;

        if(m_pTS7Client->Connected())
            emit sig_ConnectState(true);
        else{
            emit sig_ConnectState(false);
//            ConnectDevice();
            continue;
        }

        if(m_pTS7Client->PlcStatus() != par_stopped){
            if(GetReadData(m_CurrentReadPLCData)){
                emit sig_ReadData(m_CurrentReadPLCData);

                GetPLCCmd();
            }
        }
    }
}
