#include "SiemensPLCDevice_2.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QMetaType>

SiemensPLCDevice_2::SiemensPLCDevice_2(QThread *parent)
    : QThread(parent)
{
    qRegisterMetaType<PLC_DATAAREA_2>("PLC_DATAAREA_2");

    InitSiemensPLC();
}

SiemensPLCDevice_2::~SiemensPLCDevice_2()
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

void SiemensPLCDevice_2::InitSiemensPLC()
{
    m_nID               = 2;

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

void SiemensPLCDevice_2::SetDeviceIP(const QString &str_IP)
{
    m_strIP = str_IP;
}

void SiemensPLCDevice_2::SetDeviceID(int n_ID)
{
    m_nID = n_ID;
}

void SiemensPLCDevice_2::SetWriteParameter(const int &n_DBNumber, const int &n_Start, const int &n_DataSize)
{
    m_nWriteDBNumber = n_DBNumber;
    m_nWriteStart = n_Start;
    m_nWriteDataSize = n_DataSize;
}

void SiemensPLCDevice_2::SetReadParameter(const int &n_DBNumber, const int &n_Start, const int &n_DataSize)
{
    m_nReadDBNumber = n_DBNumber;
    m_nReadStart = n_Start;
    m_nReadDataSize = n_DataSize;
}

bool SiemensPLCDevice_2::ConnectDevice()
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

bool SiemensPLCDevice_2::DisconnectDevice()
{
    if(m_pReadTS7Client == NULL ||m_pWriteS7Client == NULL)
        return false;

    if(!m_pReadTS7Client->Connected() || !m_pWriteS7Client->Connected())
        return false;

    if(m_pReadTS7Client->Disconnect() != 0 || m_pWriteS7Client->Disconnect() != 0)
        return false;

    return true;
}

bool SiemensPLCDevice_2::GetConnectState()
{
    if(m_pReadTS7Client == NULL || m_pWriteS7Client == NULL)
        return false;

    return (m_pReadTS7Client->Connected() && m_pWriteS7Client->Connected());
}

void SiemensPLCDevice_2::GetWritePCData(PC_DATAAREA_2 &PCData)
{
    PCData = m_WritePCData;
}

void SiemensPLCDevice_2::UploadCmdSwitch(const bool &b_Switch)
{
    m_bUploadCmdSwitch = b_Switch;
}

bool SiemensPLCDevice_2::SendCmd(int n_Station, PC_CMD cmd)
{
    QMutexLocker o_Locker(&m_oQMutex);

    if(n_Station == 6){
        m_WritePCData.PC_SEVENTHWAREHOUSEPUSHOUT_CMD = (signed char)cmd;
    }
    else{
        return false;
    }

    if(!WriteData(m_WritePCData))
        return false;

    return true;
}

bool SiemensPLCDevice_2::GetReadData(PLC_DATAAREA_2 &PLCData)
{
    if(m_pReadTS7Client == NULL)
        return false;

    if(!m_pReadTS7Client->Connected())
        return false;

    if(m_pReadTS7Client->DBRead(m_nReadDBNumber, m_nReadStart, m_nReadDataSize, &PLCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice_2::GetWriteData(PC_DATAAREA_2 &PCData)
{
    if(m_pWriteS7Client == NULL)
        return false;

    if(!m_pWriteS7Client->Connected())
        return false;

    if(m_pWriteS7Client->DBRead(m_nWriteDBNumber, m_nWriteStart, m_nWriteDataSize, &PCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice_2::WriteData(PC_DATAAREA_2 &PCData)
{
    if(m_pWriteS7Client == NULL)
        return false;

    if(!m_pWriteS7Client->Connected())
        return false;

    if(m_pWriteS7Client->DBWrite(m_nWriteDBNumber, m_nWriteStart, m_nWriteDataSize, &PCData) != 0)
        return false;

    return true;
}

void SiemensPLCDevice_2::GetPLCCmd()
{
    if(!m_bUploadCmdSwitch)
        return;

    unsigned char ucCurrentData[4] = {};
    unsigned char ucLastData[4] = {};

    memset(ucCurrentData, 0, 4);
    memset(ucLastData, 0, 4);

    memcpy(ucCurrentData, &m_CurrentReadPLCData, 2);
    memcpy(ucLastData, &m_LastReadPLCData, 2);

    for (int i = 0; i < 2/2; i++) {
        if(ucCurrentData[i*2] != ucLastData[i*2]){
            emit sig_GetCmd(i + 6, (PLC_CMD)ucCurrentData[i*2]);
        }
    }

    m_LastReadPLCData = m_CurrentReadPLCData;
}

void SiemensPLCDevice_2::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime && m_bRunSwitch){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void SiemensPLCDevice_2::run()
{
    while(m_bRunSwitch){
        WorkSleep(500);

        if(m_pReadTS7Client == NULL)
            continue;

        if(m_pReadTS7Client->Connected() && m_pReadTS7Client->Connected())
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
