#include "siemensplcdevice_1.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QMetaType>

SiemensPLCDevice_1::SiemensPLCDevice_1(QThread *parent)
    : QThread(parent)
{
    qRegisterMetaType<PLC_DATAAREA_1>("PLC_DATAAREA_1");

    InitSiemensPLC();
}

SiemensPLCDevice_1::~SiemensPLCDevice_1()
{
    m_bRunSwitch = false;

    this->quit();
    this->wait();

    if(m_pReadTS7Client != NULL){
        delete m_pReadTS7Client;
        m_pReadTS7Client = NULL;
    }

    if(m_pWriteS7Client != NULL){
        delete m_pWriteS7Client;
        m_pWriteS7Client = NULL;
    }
}

void SiemensPLCDevice_1::InitSiemensPLC()
{
    m_nID               = 1;

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

    m_pReadTS7Client    = NULL;
    m_pReadTS7Client    = new TS7Client();

    m_pWriteS7Client    = NULL;
    m_pWriteS7Client    = new TS7Client();

    this->start();
}

void SiemensPLCDevice_1::SetDeviceIP(const QString &str_IP)
{
    m_strIP = str_IP;
}

void SiemensPLCDevice_1::SetDeviceID(int n_ID)
{
    m_nID = n_ID;
}

void SiemensPLCDevice_1::SetWriteParameter(const int &n_DBNumber, const int &n_Start, const int &n_DataSize)
{
    m_nWriteDBNumber = n_DBNumber;
    m_nWriteStart = n_Start;
    m_nWriteDataSize = n_DataSize;
}

void SiemensPLCDevice_1::SetReadParameter(const int &n_DBNumber, const int &n_Start, const int &n_DataSize)
{
    m_nReadDBNumber = n_DBNumber;
    m_nReadStart = n_Start;
    m_nReadDataSize = n_DataSize;
}

bool SiemensPLCDevice_1::ConnectDevice()
{
    if(m_pReadTS7Client == NULL || m_pWriteS7Client == NULL)
        return false;

    if(m_pReadTS7Client->Connected() || m_pWriteS7Client->Connected())
        return false;

    if(m_strIP == "")
        return false;

    if(m_pReadTS7Client->ConnectTo(m_strIP.toLatin1(), 0 ,1) != 0 ||
            m_pWriteS7Client->ConnectTo(m_strIP.toLatin1(), 0 ,1) != 0)
        return false;

    //初始化内存区
    if(!WriteData(m_WritePCData))
        return false;

    return true;
}

bool SiemensPLCDevice_1::DisconnectDevice()
{
    if(m_pReadTS7Client == NULL ||m_pWriteS7Client == NULL)
        return false;

    if(!m_pReadTS7Client->Connected() || !m_pWriteS7Client->Connected())
        return false;

    if(m_pReadTS7Client->Disconnect() != 0 || m_pWriteS7Client->Disconnect() != 0)
        return false;

    return true;
}

bool SiemensPLCDevice_1::GetConnectState()
{
    if(m_pReadTS7Client == NULL || m_pWriteS7Client == NULL)
        return false;

    return (m_pReadTS7Client->Connected() && m_pWriteS7Client->Connected());
}

void SiemensPLCDevice_1::GetWritePCData(PC_DATAAREA_1 &PCData)
{
    PCData = m_WritePCData;
}

void SiemensPLCDevice_1::UploadCmdSwitch(const bool &b_Switch)
{
    m_bUploadCmdSwitch = b_Switch;
}

bool SiemensPLCDevice_1::SendCmd(int n_Station, PC_CMD cmd)
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
    default:
        return false;
    }

    if(!WriteData(m_WritePCData))
        return false;

    return true;
}

bool SiemensPLCDevice_1::GetReadData(PLC_DATAAREA_1 &PLCData)
{
    if(m_pReadTS7Client == NULL)
        return false;

    if(!m_pReadTS7Client->Connected())
        return false;

    if(m_pReadTS7Client->DBRead(m_nReadDBNumber, m_nReadStart, m_nReadDataSize, &PLCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice_1::GetWriteData(PC_DATAAREA_1 &PCData)
{
    if(m_pWriteS7Client == NULL)
        return false;

    if(!m_pWriteS7Client->Connected())
        return false;

    if(m_pWriteS7Client->DBRead(m_nWriteDBNumber, m_nWriteStart, m_nWriteDataSize, &PCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice_1::WriteData(PC_DATAAREA_1 &PCData)
{
    if(m_pWriteS7Client == NULL)
        return false;

    if(!m_pWriteS7Client->Connected())
        return false;

    if(m_pWriteS7Client->DBWrite(m_nWriteDBNumber, m_nWriteStart, m_nWriteDataSize, &PCData) != 0)
        return false;

    return true;
}

void SiemensPLCDevice_1::GetPLCCmd()
{
    if(!m_bUploadCmdSwitch)
        return;

    unsigned char ucCurrentData[20] = {};
    unsigned char ucLastData[20] = {};

    memset(ucCurrentData, 0, 20);
    memset(ucLastData, 0, 20);

    memcpy(ucCurrentData, &m_CurrentReadPLCData, sizeof(ucCurrentData)/sizeof(unsigned char));
    memcpy(ucLastData, &m_LastReadPLCData, sizeof(ucCurrentData)/sizeof(unsigned char));

    for (int i = 0; i < 10/2; i++) {
        if(ucCurrentData[i*2] != ucLastData[i*2]){
            emit sig_GetCmd(i + 1, (PLC_CMD)ucCurrentData[i*2]);
        }
    }

    m_LastReadPLCData = m_CurrentReadPLCData;
}

void SiemensPLCDevice_1::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime && m_bRunSwitch){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void SiemensPLCDevice_1::run()
{
    while(m_bRunSwitch){
        WorkSleep(500);

        if(m_pReadTS7Client == NULL)
            continue;

        if(m_pReadTS7Client->Connected()/* && m_pReadTS7Client->Connected()*/)
            emit sig_ConnectState(m_nID, true);
        else{
            emit sig_ConnectState(m_nID, false);
            ConnectDevice();
            continue;
        }

        if(m_pReadTS7Client->PlcStatus() != par_stopped){
            if(GetReadData(m_CurrentReadPLCData)){
                emit sig_ReadData(m_CurrentReadPLCData);

                GetPLCCmd();
            }
        }
    }
}
