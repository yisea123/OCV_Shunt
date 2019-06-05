#ifndef SIEMENSPLCDEVICEMANAGER_H
#define SIEMENSPLCDEVICEMANAGER_H

#include <QObject>

#include <siemensplcdevice_1.h>
#include <siemensplcdevice_2.h>
#include <siemensplcdevice_3.h>


class SiemensPLCDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit SiemensPLCDeviceManager(QObject *parent = 0);
    ~SiemensPLCDeviceManager();

public:
    void SetDeviceID_IP_1(const int &n_ID,
                          const QString &str_IP);

    void SetWrite_ReadParameter_1(const int &n_WriteDBNumber,
                                  const int &n_WriteStart,
                                  const int &n_WriteDataSize,
                                  const int &n_ReadDBNumber,
                                  const int &n_ReadStart,
                                  const int &n_ReadDataSize);

    void SetDeviceID_IP_2(const int &n_ID,
                          const QString &str_IP);

    void SetWrite_ReadParameter_2(const int &n_WriteDBNumber,
                                  const int &n_WriteStart,
                                  const int &n_WriteDataSize,
                                  const int &n_ReadDBNumber,
                                  const int &n_ReadStart,
                                  const int &n_ReadDataSize);

    void SetDeviceID_IP_3(const int &n_ID,
                          const QString &str_IP);

    void SetWrite_ReadParameter_3(const int &n_WriteDBNumber,
                                  const int &n_WriteStart,
                                  const int &n_WriteDataSize,
                                  const int &n_ReadDBNumber,
                                  const int &n_ReadStart,
                                  const int &n_ReadDataSize);

    bool ConnectDevice();

    bool DisconnectDevice();

    bool GetConnectState();

    bool GetConnectState_1();

    bool GetConnectState_2();

    bool GetConnectState_3();

    void GetWritePCData_1(PC_DATAAREA_1 &PCData);

    void GetWritePCData_2(PC_DATAAREA_2 &PCData);

    void GetWritePCData_3(PC_DATAAREA_3 &PCData);

    void UploadCmdSwitch(const bool &b_Switch);

    bool SendCmd(int n_Station, PC_CMD cmd);

signals:
    void sig_ConnectState(int n_Id, bool b_Connect);

    void sig_ReadData_1(PLC_DATAAREA_1 PLCData);

    void sig_ReadData_2(PLC_DATAAREA_2 PLCData);

    void sig_ReadData_3(PLC_DATAAREA_3 PLCData);

    void sig_GetCmd(int n_Station, PLC_CMD cmd);

private slots:
    void slot_ConnectState(int n_Id, bool b_Connect);

    void slot_ReadData_1(PLC_DATAAREA_1 PLCData);

    void slot_ReadData_2(PLC_DATAAREA_2 PLCData);

    void slot_ReadData_3(PLC_DATAAREA_3 PLCData);

    void slot_GetCmd(int n_Station, PLC_CMD cmd);

private:
    SiemensPLCDevice_1 *m_pSiemensPLCDevice_1;
    SiemensPLCDevice_2 *m_pSiemensPLCDevice_2;
    SiemensPLCDevice_3 *m_pSiemensPLCDevice_3;

};

#endif // SIEMENSPLCDEVICEMANAGER_H
