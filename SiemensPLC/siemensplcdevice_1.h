#ifndef SIEMENSPLCDEVICE_1_H
#define SIEMENSPLCDEVICE_1_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "snap7.h"
#include "ocv_plc_define.h"

class SiemensPLCDevice_1 : public QThread
{
    Q_OBJECT
public:
    explicit SiemensPLCDevice_1(QThread *parent = 0);
    ~SiemensPLCDevice_1();

private:
    void InitSiemensPLC();

public:
    void SetDeviceIP(const QString &str_IP);

    void SetDeviceID(int n_ID);

    void SetWriteParameter(const int &n_DBNumber,
                           const int &n_Start,
                           const int &n_DataSize);

    void SetReadParameter(const int &n_DBNumber,
                          const int &n_Start,
                          const int &n_DataSize);

    bool ConnectDevice();

    bool DisconnectDevice();

    bool GetConnectState();

    void GetWritePCData(PC_DATAAREA_1 &PCData);

    void UploadCmdSwitch(const bool &b_Switch);

public:
    bool SendCmd(int n_Station, PC_CMD cmd);

private:
    bool GetReadData(PLC_DATAAREA_1 &PLCData);

    bool GetWriteData(PC_DATAAREA_1 &PCData);

    bool WriteData(PC_DATAAREA_1 &PCData);

    void GetPLCCmd();

private:
    void WorkSleep(int n_Msec);

protected:
    void run();

signals:
    void sig_ConnectState(int n_Id, bool b_Connect);

    void sig_ReadData(PLC_DATAAREA_1 PLCData);

    void sig_GetCmd(int n_Station, PLC_CMD cmd);

private:
    TS7Client *m_pReadTS7Client;
    TS7Client *m_pWriteS7Client;

    QString m_strIP;

    int m_nID;

    int m_nWriteDBNumber;
    int m_nWriteStart;
    int m_nWriteDataSize;
    int m_nReadDBNumber;
    int m_nReadStart;
    int m_nReadDataSize;

    bool m_bConnectState;

    bool m_bRunSwitch;

    PC_DATAAREA_1 m_WritePCData;

    PLC_DATAAREA_1 m_LastReadPLCData;
    PLC_DATAAREA_1 m_CurrentReadPLCData;

    QMutex m_oQMutex;

    bool m_bUploadCmdSwitch;
};

#endif // SIEMENSPLCDEVICE_1_H
