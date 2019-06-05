#ifndef PLCDEBUGVIEW_H
#define PLCDEBUGVIEW_H

#include <QWidget>
#include <QTimer>

#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QButtonGroup>

#include "ui_define.h"
#include <SiemensPLC/siemensplcdevicemanager.h>


namespace Ui {
class PLCDebugView;
}

class PLCDebugView : public QWidget
{
    Q_OBJECT
public:
    explicit PLCDebugView(SiemensPLCDeviceManager * &p_SiemensPLCDeviceManager, QWidget *parent = nullptr);
    ~PLCDebugView();

    bool SendCmd(int n_Station, PC_CMD cmd);

    void ResetWriteButtonUI();

    void ResetReadLabelUI();

private:
    void InitView();

    void ErrorBox(QString str_Info);

    void PLC_ConnectState();

private slots:
    void slot_WarningtButtonClick(int n_ID);

    void slot_ResetButtonClick(int n_ID);

    void slot_CmdAButtonClick(int n_ID);

    void slot_CmdBButtonClick(int n_ID);

    void slot_CmdCButtonClick(int n_ID);

    void slot_CmdDButtonClick(int n_ID);

private slots:
    void slot_TimeOut();

private slots:
    void slot_ReadData_1(PLC_DATAAREA_1 PLCData);

    void slot_ReadData_2(PLC_DATAAREA_2 PLCData);

    void slot_ReadData_3(PLC_DATAAREA_3 PLCData);

    void slot_PLCConnectState(int n_Station, bool b_Connect);

private:
    Ui::PLCDebugView *ui;
    SiemensPLCDeviceManager * m_pSiemensPLCDeviceManager;
    QTimer *m_pQTimer;

    QMap<PC_CMD, QMap<int, QPushButton * >> m_mapPB;
    QMap<PLC_CMD, QMap<int, QLabel * >> m_mapLB;

    QButtonGroup *m_pWarningtButtonGroup;
    QButtonGroup *m_pResetButtonGroup;
    QButtonGroup *m_pCmdAButtonGroup;
    QButtonGroup *m_pCmdBButtonGroup;
    QButtonGroup *m_pCmdCButtonGroup;
    QButtonGroup *m_pCmdDButtonGroup;

    PC_DATAAREA m_LastPCData;
    PLC_DATAAREA m_LastPLCData;
};

#endif // PLCDEBUGVIEW_H
