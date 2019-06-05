#include "siemensplcdevicemanager.h"



SiemensPLCDeviceManager::SiemensPLCDeviceManager(QObject *parent)
    :QObject (parent)
{
    m_pSiemensPLCDevice_1 = NULL;
    m_pSiemensPLCDevice_2 = NULL;
    m_pSiemensPLCDevice_3 = NULL;

    m_pSiemensPLCDevice_1 = new SiemensPLCDevice_1;
    m_pSiemensPLCDevice_2 = new SiemensPLCDevice_2;
    m_pSiemensPLCDevice_3 = new SiemensPLCDevice_3;

    connect(m_pSiemensPLCDevice_1, &SiemensPLCDevice_1::sig_ConnectState,
            this, &SiemensPLCDeviceManager::slot_ConnectState);

    connect(m_pSiemensPLCDevice_1, &SiemensPLCDevice_1::sig_ReadData,
            this, &SiemensPLCDeviceManager::slot_ReadData_1);

    connect(m_pSiemensPLCDevice_1, &SiemensPLCDevice_1::sig_GetCmd,
            this, &SiemensPLCDeviceManager::slot_GetCmd);

    connect(m_pSiemensPLCDevice_2, &SiemensPLCDevice_2::sig_ConnectState,
            this, &SiemensPLCDeviceManager::slot_ConnectState);

    connect(m_pSiemensPLCDevice_2, &SiemensPLCDevice_2::sig_ReadData,
            this, &SiemensPLCDeviceManager::slot_ReadData_2);

    connect(m_pSiemensPLCDevice_2, &SiemensPLCDevice_2::sig_GetCmd,
            this, &SiemensPLCDeviceManager::slot_GetCmd);

    connect(m_pSiemensPLCDevice_3, &SiemensPLCDevice_3::sig_ConnectState,
            this, &SiemensPLCDeviceManager::slot_ConnectState);

    connect(m_pSiemensPLCDevice_3, &SiemensPLCDevice_3::sig_ReadData,
            this, &SiemensPLCDeviceManager::slot_ReadData_3);

    connect(m_pSiemensPLCDevice_3, &SiemensPLCDevice_3::sig_GetCmd,
            this, &SiemensPLCDeviceManager::slot_GetCmd);
}

SiemensPLCDeviceManager::~SiemensPLCDeviceManager()
{
    if(m_pSiemensPLCDevice_1 != NULL){
        delete m_pSiemensPLCDevice_1;
        m_pSiemensPLCDevice_1 = NULL;
    }

    if(m_pSiemensPLCDevice_2 != NULL){
        delete m_pSiemensPLCDevice_2;
        m_pSiemensPLCDevice_2 = NULL;
    }

    if(m_pSiemensPLCDevice_3 != NULL){
        delete m_pSiemensPLCDevice_3;
        m_pSiemensPLCDevice_3 = NULL;
    }
}

void SiemensPLCDeviceManager::SetDeviceID_IP_1(const int &n_ID,
                                               const QString &str_IP)
{
    m_pSiemensPLCDevice_1->SetDeviceID(n_ID);
    m_pSiemensPLCDevice_1->SetDeviceIP(str_IP);
}

void SiemensPLCDeviceManager::SetWrite_ReadParameter_1(const int &n_WriteDBNumber,
                                                       const int &n_WriteStart,
                                                       const int &n_WriteDataSize,
                                                       const int &n_ReadDBNumber,
                                                       const int &n_ReadStart,
                                                       const int &n_ReadDataSize)
{
    m_pSiemensPLCDevice_1->SetWriteParameter(n_WriteDBNumber,
                                             n_WriteStart,
                                             n_WriteDataSize);

    m_pSiemensPLCDevice_1->SetReadParameter(n_ReadDBNumber,
                                            n_ReadStart,
                                            n_ReadDataSize);
}

void SiemensPLCDeviceManager::SetDeviceID_IP_2(const int &n_ID,
                                               const QString &str_IP)
{
    m_pSiemensPLCDevice_2->SetDeviceID(n_ID);
    m_pSiemensPLCDevice_2->SetDeviceIP(str_IP);
}

void SiemensPLCDeviceManager::SetWrite_ReadParameter_2(const int &n_WriteDBNumber,
                                                       const int &n_WriteStart,
                                                       const int &n_WriteDataSize,
                                                       const int &n_ReadDBNumber,
                                                       const int &n_ReadStart,
                                                       const int &n_ReadDataSize)
{
    m_pSiemensPLCDevice_2->SetWriteParameter(n_WriteDBNumber,
                                             n_WriteStart,
                                             n_WriteDataSize);

    m_pSiemensPLCDevice_2->SetReadParameter(n_ReadDBNumber,
                                            n_ReadStart,
                                            n_ReadDataSize);
}

void SiemensPLCDeviceManager::SetDeviceID_IP_3(const int &n_ID,
                                               const QString &str_IP)
{
    m_pSiemensPLCDevice_3->SetDeviceID(n_ID);
    m_pSiemensPLCDevice_3->SetDeviceIP(str_IP);
}

void SiemensPLCDeviceManager::SetWrite_ReadParameter_3(const int &n_WriteDBNumber,
                                                       const int &n_WriteStart,
                                                       const int &n_WriteDataSize,
                                                       const int &n_ReadDBNumber,
                                                       const int &n_ReadStart,
                                                       const int &n_ReadDataSize)
{
    m_pSiemensPLCDevice_3->SetWriteParameter(n_WriteDBNumber,
                                             n_WriteStart,
                                             n_WriteDataSize);

    m_pSiemensPLCDevice_3->SetReadParameter(n_ReadDBNumber,
                                            n_ReadStart,
                                            n_ReadDataSize);
}

bool SiemensPLCDeviceManager::ConnectDevice()
{
    return m_pSiemensPLCDevice_1->ConnectDevice() && m_pSiemensPLCDevice_2->ConnectDevice() && m_pSiemensPLCDevice_3->ConnectDevice();
}

bool SiemensPLCDeviceManager::DisconnectDevice()
{
    return m_pSiemensPLCDevice_1->DisconnectDevice() && m_pSiemensPLCDevice_2->DisconnectDevice() && m_pSiemensPLCDevice_3->DisconnectDevice();
}

bool SiemensPLCDeviceManager::GetConnectState()
{
    return m_pSiemensPLCDevice_1->GetConnectState() && m_pSiemensPLCDevice_2->GetConnectState() && m_pSiemensPLCDevice_3->GetConnectState();
}

bool SiemensPLCDeviceManager::GetConnectState_1()
{
    return m_pSiemensPLCDevice_1->GetConnectState();
}

bool SiemensPLCDeviceManager::GetConnectState_2()
{
    return m_pSiemensPLCDevice_2->GetConnectState();
}

bool SiemensPLCDeviceManager::GetConnectState_3()
{
    return m_pSiemensPLCDevice_3->GetConnectState();
}

void SiemensPLCDeviceManager::GetWritePCData_1(PC_DATAAREA_1 &PCData)
{
    m_pSiemensPLCDevice_1->GetWritePCData(PCData);
}

void SiemensPLCDeviceManager::GetWritePCData_2(PC_DATAAREA_2 &PCData)
{
    m_pSiemensPLCDevice_2->GetWritePCData(PCData);
}

void SiemensPLCDeviceManager::GetWritePCData_3(PC_DATAAREA_3 &PCData)
{
    m_pSiemensPLCDevice_3->GetWritePCData(PCData);
}

void SiemensPLCDeviceManager::UploadCmdSwitch(const bool &b_Switch)
{
    m_pSiemensPLCDevice_1->UploadCmdSwitch(b_Switch);
    m_pSiemensPLCDevice_2->UploadCmdSwitch(b_Switch);
    m_pSiemensPLCDevice_3->UploadCmdSwitch(b_Switch);
}

bool SiemensPLCDeviceManager::SendCmd(int n_Station, PC_CMD cmd)
{
    bool b_Ret = false;

    if(n_Station > 0 && n_Station < 6){
        b_Ret = m_pSiemensPLCDevice_1->SendCmd(n_Station, cmd);
    }
    else if(n_Station == 6){
        b_Ret = m_pSiemensPLCDevice_2->SendCmd(n_Station, cmd);
    }
    else if(n_Station > 6 && n_Station < 11){
        b_Ret = m_pSiemensPLCDevice_1->SendCmd(n_Station, cmd);
    }

    return b_Ret;
}

void SiemensPLCDeviceManager::slot_ConnectState(int n_Id, bool b_Connect)
{
    emit sig_ConnectState(n_Id, b_Connect);
}

void SiemensPLCDeviceManager::slot_ReadData_1(PLC_DATAAREA_1 PLCData)
{
    emit sig_ReadData_1(PLCData);
}

void SiemensPLCDeviceManager::slot_ReadData_2(PLC_DATAAREA_2 PLCData)
{
    emit sig_ReadData_2(PLCData);
}

void SiemensPLCDeviceManager::slot_ReadData_3(PLC_DATAAREA_3 PLCData)
{
    emit sig_ReadData_3(PLCData);
}

void SiemensPLCDeviceManager::slot_GetCmd(int n_Station, PLC_CMD cmd)
{
    emit sig_GetCmd(n_Station, cmd);
}
