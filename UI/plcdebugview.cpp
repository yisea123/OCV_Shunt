#include "plcdebugview.h"
#include "ui_plcdebugview.h"

#include <QMessageBox>
#include <QDebug>

PLCDebugView::PLCDebugView(SiemensPLCDeviceManager * &p_SiemensPLCDeviceManager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PLCDebugView)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QStringLiteral("PLC调试"));
    this->setFixedSize(this->size());

    m_pWarningtButtonGroup = NULL;
    m_pResetButtonGroup = NULL;
    m_pCmdAButtonGroup = NULL;
    m_pCmdBButtonGroup = NULL;
    m_pCmdCButtonGroup = NULL;
    m_pCmdDButtonGroup = NULL;

    InitView();

    m_pSiemensPLCDeviceManager = p_SiemensPLCDeviceManager;

    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_ReadData_1,
            this, &PLCDebugView::slot_ReadData_1);

    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_ReadData_2,
            this, &PLCDebugView::slot_ReadData_2);

    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_ReadData_3,
            this, &PLCDebugView::slot_ReadData_3);

    m_pQTimer = new QTimer;
    connect(m_pQTimer, &QTimer::timeout,
            this, &PLCDebugView::slot_TimeOut);

    m_pQTimer->start(1000);

    PLC_ConnectState();
}

PLCDebugView::~PLCDebugView()
{
    if(m_pQTimer != NULL){
        if(m_pQTimer->isActive())
            m_pQTimer->stop();
        delete m_pQTimer;
        m_pQTimer = NULL;
    }

    delete ui;
}

bool PLCDebugView::SendCmd(int n_Station, PC_CMD cmd)
{
    if(m_pSiemensPLCDeviceManager == NULL)
        return false;

    if(!m_pSiemensPLCDeviceManager->SendCmd(n_Station, cmd)){
        ErrorBox(QStringLiteral("发送命令失败"));
        return false;
    }

    return true;
}

void PLCDebugView::ResetWriteButtonUI()
{
    unsigned char ucLastData[40] = {};
    memset(ucLastData, 0, 40);
    memcpy(ucLastData, &m_LastPCData, 20);

    for(int i = 0; i < 20/2; i++) {
        if(!m_mapPB.contains(PC_CMD(ucLastData[i*2])))
            continue;

        QPushButton *p_QPushButton = m_mapPB.value(PC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QPushButton != NULL){
            p_QPushButton->setStyleSheet(NoClick_Style);
        }
    }
}

void PLCDebugView::ResetReadLabelUI()
{
    unsigned char ucLastData[40] = {};
    memset(ucLastData, 0, 40);
    memcpy(ucLastData, &m_LastPLCData, 20);

    for(int i = 0; i < 20/2; i++) {
        if(!m_mapLB.contains(PLC_CMD(ucLastData[i*2])))
            continue;

        QLabel *p_QLabel = m_mapLB.value(PLC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QLabel != NULL){
            p_QLabel->setStyleSheet(NoClick_Style);
        }
    }
}

void PLCDebugView::InitView()
{
    QMap<int, QPushButton * > map_pb_Error;
    map_pb_Error.insert(1, ui->pb_Error_1);
    map_pb_Error.insert(2, ui->pb_Error_2);
    map_pb_Error.insert(3, ui->pb_Error_3);
    map_pb_Error.insert(4, ui->pb_Error_4);
    map_pb_Error.insert(5, ui->pb_Error_5);
    map_pb_Error.insert(6, ui->pb_Error_6);
    map_pb_Error.insert(7, ui->pb_Error_7);
    map_pb_Error.insert(8, ui->pb_Error_8);
    map_pb_Error.insert(9, ui->pb_Error_9);
    map_pb_Error.insert(10, ui->pb_Error_10);

    QMap<int, QPushButton * > map_pb_Reset;
    map_pb_Reset.insert(1, ui->pb_Reset_1);
    map_pb_Reset.insert(2, ui->pb_Reset_2);
    map_pb_Reset.insert(3, ui->pb_Reset_3);
    map_pb_Reset.insert(4, ui->pb_Reset_4);
    map_pb_Reset.insert(5, ui->pb_Reset_5);
    map_pb_Reset.insert(6, ui->pb_Reset_6);
    map_pb_Reset.insert(7, ui->pb_Reset_7);
    map_pb_Reset.insert(8, ui->pb_Reset_8);
    map_pb_Reset.insert(9, ui->pb_Reset_9);
    map_pb_Reset.insert(10, ui->pb_Reset_10);

    QMap<int, QPushButton * > map_pb_EMPTYBOX;
    map_pb_EMPTYBOX.insert(1, ui->pb_CmdA_1);
    map_pb_EMPTYBOX.insert(2, ui->pb_CmdA_2);
    map_pb_EMPTYBOX.insert(3, NULL);
    map_pb_EMPTYBOX.insert(4, NULL);
    map_pb_EMPTYBOX.insert(5, NULL);
    map_pb_EMPTYBOX.insert(6, NULL);
    map_pb_EMPTYBOX.insert(7, NULL);
    map_pb_EMPTYBOX.insert(8, NULL);
    map_pb_EMPTYBOX.insert(9, NULL);
    map_pb_EMPTYBOX.insert(10, NULL);

    QMap<int, QPushButton * > map_pb_FULLBOX;
    map_pb_FULLBOX.insert(1, ui->pb_CmdB_1);
    map_pb_FULLBOX.insert(2, ui->pb_CmdB_2);
    map_pb_FULLBOX.insert(3, NULL);
    map_pb_FULLBOX.insert(4, NULL);
    map_pb_FULLBOX.insert(5, NULL);
    map_pb_FULLBOX.insert(6, NULL);
    map_pb_FULLBOX.insert(7, NULL);
    map_pb_FULLBOX.insert(8, NULL);
    map_pb_FULLBOX.insert(9, NULL);
    map_pb_FULLBOX.insert(10, NULL);

    QMap<int, QPushButton * > map_pb_AWAY_FULLBOX;
    map_pb_AWAY_FULLBOX.insert(1, NULL);
    map_pb_AWAY_FULLBOX.insert(2, NULL);
    map_pb_AWAY_FULLBOX.insert(3, ui->pb_CmdA_3);
    map_pb_AWAY_FULLBOX.insert(4, NULL);
    map_pb_AWAY_FULLBOX.insert(5, NULL);
    map_pb_AWAY_FULLBOX.insert(6, NULL);
    map_pb_AWAY_FULLBOX.insert(7, NULL);
    map_pb_AWAY_FULLBOX.insert(8, NULL);
    map_pb_AWAY_FULLBOX.insert(9, NULL);
    map_pb_AWAY_FULLBOX.insert(10, NULL);

    QMap<int, QPushButton * > map_pb_AWAY_EMPTYBOX;
    map_pb_AWAY_EMPTYBOX.insert(1, NULL);
    map_pb_AWAY_EMPTYBOX.insert(2, NULL);
    map_pb_AWAY_EMPTYBOX.insert(3, NULL);
    map_pb_AWAY_EMPTYBOX.insert(4, ui->pb_CmdA_4);
    map_pb_AWAY_EMPTYBOX.insert(5, NULL);
    map_pb_AWAY_EMPTYBOX.insert(6, NULL);
    map_pb_AWAY_EMPTYBOX.insert(7, NULL);
    map_pb_AWAY_EMPTYBOX.insert(8, NULL);
    map_pb_AWAY_EMPTYBOX.insert(9, NULL);
    map_pb_AWAY_EMPTYBOX.insert(10, NULL);


    QMap<int, QPushButton * > map_pb_IN_SWAPCHANNEL;
    map_pb_IN_SWAPCHANNEL.insert(1, NULL);
    map_pb_IN_SWAPCHANNEL.insert(2, NULL);
    map_pb_IN_SWAPCHANNEL.insert(3, NULL);
    map_pb_IN_SWAPCHANNEL.insert(4, NULL);
    map_pb_IN_SWAPCHANNEL.insert(5, ui->pb_CmdA_5);
    map_pb_IN_SWAPCHANNEL.insert(6, NULL);
    map_pb_IN_SWAPCHANNEL.insert(7, NULL);
    map_pb_IN_SWAPCHANNEL.insert(8, NULL);
    map_pb_IN_SWAPCHANNEL.insert(9, NULL);
    map_pb_IN_SWAPCHANNEL.insert(10, NULL);


    QMap<int, QPushButton * > map_pb_IN_LINE;
    map_pb_IN_LINE.insert(1, NULL);
    map_pb_IN_LINE.insert(2, NULL);
    map_pb_IN_LINE.insert(3, NULL);
    map_pb_IN_LINE.insert(4, NULL);
    map_pb_IN_LINE.insert(5, ui->pb_CmdB_5);
    map_pb_IN_LINE.insert(6, NULL);
    map_pb_IN_LINE.insert(7, NULL);
    map_pb_IN_LINE.insert(8, NULL);
    map_pb_IN_LINE.insert(9, NULL);
    map_pb_IN_LINE.insert(10, NULL);

    QMap<int, QPushButton * > map_pb_IN_ARTIFICIALCHANNEL;
    map_pb_IN_ARTIFICIALCHANNEL.insert(1, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(2, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(3, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(4, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(5, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(6, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(7, ui->pb_CmdA_7);
    map_pb_IN_ARTIFICIALCHANNEL.insert(8, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(9, NULL);
    map_pb_IN_ARTIFICIALCHANNEL.insert(10, NULL);


    QMap<int, QPushButton * > map_pb_IN_LINE_NOBAD;
    map_pb_IN_LINE_NOBAD.insert(1, NULL);
    map_pb_IN_LINE_NOBAD.insert(2, NULL);
    map_pb_IN_LINE_NOBAD.insert(3, NULL);
    map_pb_IN_LINE_NOBAD.insert(4, NULL);
    map_pb_IN_LINE_NOBAD.insert(5, NULL);
    map_pb_IN_LINE_NOBAD.insert(6, NULL);
    map_pb_IN_LINE_NOBAD.insert(7, ui->pb_CmdB_7);
    map_pb_IN_LINE_NOBAD.insert(8, NULL);
    map_pb_IN_LINE_NOBAD.insert(9, NULL);
    map_pb_IN_LINE_NOBAD.insert(10, NULL);

    QMap<int, QPushButton * > map_pb_IN_302;
    map_pb_IN_LINE_NOBAD.insert(1, NULL);
    map_pb_IN_LINE_NOBAD.insert(2, NULL);
    map_pb_IN_LINE_NOBAD.insert(3, NULL);
    map_pb_IN_LINE_NOBAD.insert(4, NULL);
    map_pb_IN_LINE_NOBAD.insert(5, NULL);
    map_pb_IN_LINE_NOBAD.insert(6, NULL);
    map_pb_IN_LINE_NOBAD.insert(7, NULL);
    map_pb_IN_LINE_NOBAD.insert(8, ui->pb_CmdA_8);
    map_pb_IN_LINE_NOBAD.insert(9, ui->pb_CmdA_9);
    map_pb_IN_LINE_NOBAD.insert(10, ui->pb_CmdA_10);

    QMap<int, QPushButton * > map_pb_IN_127;
    map_pb_IN_127.insert(1, NULL);
    map_pb_IN_127.insert(2, NULL);
    map_pb_IN_127.insert(3, NULL);
    map_pb_IN_127.insert(4, NULL);
    map_pb_IN_127.insert(5, NULL);
    map_pb_IN_127.insert(6, NULL);
    map_pb_IN_127.insert(7, NULL);
    map_pb_IN_127.insert(8, ui->pb_CmdB_8);
    map_pb_IN_127.insert(9, ui->pb_CmdB_9);
    map_pb_IN_127.insert(10, ui->pb_CmdB_10);

    QMap<int, QPushButton * > map_pb_IN_EMPTYBOX_127;
    map_pb_IN_EMPTYBOX_127.insert(1, NULL);
    map_pb_IN_EMPTYBOX_127.insert(2, NULL);
    map_pb_IN_EMPTYBOX_127.insert(3, NULL);
    map_pb_IN_EMPTYBOX_127.insert(4, NULL);
    map_pb_IN_EMPTYBOX_127.insert(5, NULL);
    map_pb_IN_EMPTYBOX_127.insert(6, ui->pb_CmdA_6);
    map_pb_IN_EMPTYBOX_127.insert(7, NULL);
    map_pb_IN_EMPTYBOX_127.insert(8, NULL);
    map_pb_IN_EMPTYBOX_127.insert(9, NULL);
    map_pb_IN_EMPTYBOX_127.insert(10, NULL);

    QMap<int, QPushButton * > map_pb_IN_EMPTYBOX_302;
    map_pb_IN_EMPTYBOX_302.insert(1, NULL);
    map_pb_IN_EMPTYBOX_302.insert(2, NULL);
    map_pb_IN_EMPTYBOX_302.insert(3, NULL);
    map_pb_IN_EMPTYBOX_302.insert(4, NULL);
    map_pb_IN_EMPTYBOX_302.insert(5, NULL);
    map_pb_IN_EMPTYBOX_302.insert(6, ui->pb_CmdB_6);
    map_pb_IN_EMPTYBOX_302.insert(7, NULL);
    map_pb_IN_EMPTYBOX_302.insert(8, NULL);
    map_pb_IN_EMPTYBOX_302.insert(9, NULL);
    map_pb_IN_EMPTYBOX_302.insert(10, NULL);


    QMap<int, QPushButton * > map_pb_IN_FULLBOX_127;
    map_pb_IN_FULLBOX_127.insert(1, NULL);
    map_pb_IN_FULLBOX_127.insert(2, NULL);
    map_pb_IN_FULLBOX_127.insert(3, NULL);
    map_pb_IN_FULLBOX_127.insert(4, NULL);
    map_pb_IN_FULLBOX_127.insert(5, NULL);
    map_pb_IN_FULLBOX_127.insert(6, ui->pb_CmdC_6);
    map_pb_IN_FULLBOX_127.insert(7, NULL);
    map_pb_IN_FULLBOX_127.insert(8, NULL);
    map_pb_IN_FULLBOX_127.insert(9, NULL);
    map_pb_IN_FULLBOX_127.insert(10, NULL);

    QMap<int, QPushButton * > map_pb_IN_FULLBOX_302;
    map_pb_IN_FULLBOX_302.insert(1, NULL);
    map_pb_IN_FULLBOX_302.insert(2, NULL);
    map_pb_IN_FULLBOX_302.insert(3, NULL);
    map_pb_IN_FULLBOX_302.insert(4, NULL);
    map_pb_IN_FULLBOX_302.insert(5, NULL);
    map_pb_IN_FULLBOX_302.insert(6, ui->pb_CmdD_6);
    map_pb_IN_FULLBOX_302.insert(7, NULL);
    map_pb_IN_FULLBOX_302.insert(8, NULL);
    map_pb_IN_FULLBOX_302.insert(9, NULL);
    map_pb_IN_FULLBOX_302.insert(10, NULL);


    m_mapPB.insert(PC_WARNING, map_pb_Error);
    m_mapPB.insert(RESTORATION, map_pb_Reset);
    m_mapPB.insert(IN_EMPTYBOX, map_pb_EMPTYBOX);
    m_mapPB.insert(IN_FULLBOX, map_pb_FULLBOX);
    m_mapPB.insert(AWAY_FULLBOX, map_pb_AWAY_FULLBOX);
    m_mapPB.insert(AWAY_EMPTYBOX, map_pb_AWAY_EMPTYBOX);
    m_mapPB.insert(IN_SWAPCHANNEL, map_pb_IN_LINE_NOBAD);
    m_mapPB.insert(IN_LINE, map_pb_IN_LINE);
    m_mapPB.insert(IN_ARTIFICIALCHANNEL, map_pb_IN_ARTIFICIALCHANNEL);
    m_mapPB.insert(IN_LINE_NOBAD, map_pb_IN_LINE_NOBAD);
    m_mapPB.insert(IN_302, map_pb_IN_302);
    m_mapPB.insert(IN_127, map_pb_IN_127);
    m_mapPB.insert(IN_EMPTYBOX_127, map_pb_IN_EMPTYBOX_127);
    m_mapPB.insert(IN_EMPTYBOX_302, map_pb_IN_EMPTYBOX_302);
    m_mapPB.insert(IN_FULLBOX_127, map_pb_IN_FULLBOX_127);
    m_mapPB.insert(IN_FULLBOX_302, map_pb_IN_FULLBOX_302);

    QMap<int, QLabel * > map_lb_Error;
    map_lb_Error.insert(1, ui->lb_Error_1);
    map_lb_Error.insert(2, ui->lb_Error_2);
    map_lb_Error.insert(3, ui->lb_Error_3);
    map_lb_Error.insert(4, ui->lb_Error_4);
    map_lb_Error.insert(5, ui->lb_Error_5);
    map_lb_Error.insert(6, ui->lb_Error_6);
    map_lb_Error.insert(7, ui->lb_Error_7);
    map_lb_Error.insert(8, ui->lb_Error_8);
    map_lb_Error.insert(9, ui->lb_Error_9);
    map_lb_Error.insert(10, ui->lb_Error_10);


    QMap<int, QLabel * > map_lb_Wait;
    map_lb_Wait.insert(1, ui->lb_Wait_1);
    map_lb_Wait.insert(2, ui->lb_Wait_2);
    map_lb_Wait.insert(3, ui->lb_Wait_3);
    map_lb_Wait.insert(4, ui->lb_Wait_4);
    map_lb_Wait.insert(5, ui->lb_Wait_5);
    map_lb_Wait.insert(6, ui->lb_Wait_6);
    map_lb_Wait.insert(7, ui->lb_Wait_7);
    map_lb_Wait.insert(8, ui->lb_Wait_8);
    map_lb_Wait.insert(9, ui->lb_Wait_9);
    map_lb_Wait.insert(10, ui->lb_Wait_10);


    QMap<int, QLabel * > map_lb_RequestCode;
    map_lb_RequestCode.insert(1, ui->lb_RequestCode_1);
    map_lb_RequestCode.insert(2, ui->lb_RequestCode_2);
    map_lb_RequestCode.insert(3, ui->lb_RequestCode_3);
    map_lb_RequestCode.insert(4, ui->lb_RequestCode_4);
    map_lb_RequestCode.insert(5, ui->lb_RequestCode_5);
    map_lb_RequestCode.insert(6, ui->lb_RequestCode_6);
    map_lb_RequestCode.insert(7, ui->lb_RequestCode_7);
    map_lb_RequestCode.insert(8, ui->lb_RequestCode_8);
    map_lb_RequestCode.insert(9, ui->lb_RequestCode_9);
    map_lb_RequestCode.insert(10, ui->lb_RequestCode_10);


    m_mapLB.insert(PLC_WARNING, map_lb_Error);
    m_mapLB.insert(NOREQUESTSCAN, map_lb_Wait);
    m_mapLB.insert(REQUESTSCAN, map_lb_RequestCode);

    m_pWarningtButtonGroup = new QButtonGroup(this);
    m_pResetButtonGroup = new QButtonGroup(this);
    m_pCmdAButtonGroup = new QButtonGroup(this);
    m_pCmdBButtonGroup = new QButtonGroup(this);
    m_pCmdCButtonGroup = new QButtonGroup(this);
    m_pCmdDButtonGroup = new QButtonGroup(this);

    m_pWarningtButtonGroup->addButton(ui->pb_Error_1, 1);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_2, 2);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_3, 3);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_4, 4);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_5, 5);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_6, 6);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_7, 7);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_8, 8);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_9, 9);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_10, 10);

    m_pResetButtonGroup->addButton(ui->pb_Reset_1, 1);
    m_pResetButtonGroup->addButton(ui->pb_Reset_2, 2);
    m_pResetButtonGroup->addButton(ui->pb_Reset_3, 3);
    m_pResetButtonGroup->addButton(ui->pb_Reset_4, 4);
    m_pResetButtonGroup->addButton(ui->pb_Reset_5, 5);
    m_pResetButtonGroup->addButton(ui->pb_Reset_6, 6);
    m_pResetButtonGroup->addButton(ui->pb_Reset_7, 7);
    m_pResetButtonGroup->addButton(ui->pb_Reset_8, 8);
    m_pResetButtonGroup->addButton(ui->pb_Reset_9, 9);
    m_pResetButtonGroup->addButton(ui->pb_Reset_10, 10);

    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_1, 1);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_2, 2);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_3, 3);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_4, 4);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_5, 5);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_6, 6);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_7, 7);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_8, 8);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_9, 9);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_10, 10);


    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_1 ,1);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_2 ,2);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_3 ,3);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_4 ,4);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_5 ,5);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_6 ,6);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_7 ,7);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_8 ,8);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_9 ,9);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_10 ,10);


    m_pCmdCButtonGroup->addButton(ui->pb_CmdC_6 ,6);


    m_pCmdDButtonGroup->addButton(ui->pb_CmdD_6 ,6);

    connect(m_pWarningtButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_WarningtButtonClick(int)));

    connect(m_pResetButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_ResetButtonClick(int)));

    connect(m_pCmdAButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdAButtonClick(int)));

    connect(m_pCmdBButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdBButtonClick(int)));

    connect(m_pCmdCButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdCButtonClick(int)));

    connect(m_pCmdDButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdDButtonClick(int)));
}

void PLCDebugView::ErrorBox(QString str_Info)
{
    qWarning()<<"PLCDebugView "<<str_Info;

    QMessageBox::warning(
                NULL,
                QStringLiteral("警告"),
                str_Info
                );
}

void PLCDebugView::PLC_ConnectState()
{
    if(m_pSiemensPLCDeviceManager->GetConnectState_1()){
        ui->lb_State_1->setText(QStringLiteral("连接"));
        ui->lb_State_1->setStyleSheet(Connect_Style);
    }
    else{
        ui->lb_State_1->setText(QStringLiteral("断开"));
        ui->lb_State_1->setStyleSheet(Disconnect_Style);
    }

    if(m_pSiemensPLCDeviceManager->GetConnectState_2()){
        ui->lb_State_2->setText(QStringLiteral("连接"));
        ui->lb_State_2->setStyleSheet(Connect_Style);
    }
    else{
        ui->lb_State_2->setText(QStringLiteral("断开"));
        ui->lb_State_2->setStyleSheet(Disconnect_Style);
    }

    if(m_pSiemensPLCDeviceManager->GetConnectState_3()){
        ui->lb_State_3->setText(QStringLiteral("连接"));
        ui->lb_State_3->setStyleSheet(Connect_Style);
    }
    else{
        ui->lb_State_3->setText(QStringLiteral("断开"));
        ui->lb_State_3->setStyleSheet(Disconnect_Style);
    }
}

void PLCDebugView::slot_WarningtButtonClick(int n_ID)
{
    SendCmd(n_ID, PC_WARNING);
}

void PLCDebugView::slot_ResetButtonClick(int n_ID)
{
    SendCmd(n_ID, RESTORATION);
}

void PLCDebugView::slot_CmdAButtonClick(int n_ID)
{
    switch(n_ID){
    case 1:
        SendCmd(n_ID, IN_EMPTYBOX);
        break;
    case 2:
        SendCmd(n_ID, IN_EMPTYBOX);
        break;
    case 3:
        SendCmd(n_ID, AWAY_FULLBOX);
        break;
    case 4:
        SendCmd(n_ID, AWAY_EMPTYBOX);
        break;
    case 5:
        SendCmd(n_ID, IN_SWAPCHANNEL);
        break;
    case 6:
        SendCmd(n_ID, IN_EMPTYBOX_127);
        break;
    case 7:
        SendCmd(n_ID, IN_ARTIFICIALCHANNEL);
        break;
    case 8:
        SendCmd(n_ID, IN_302);
        break;
    case 9:
        SendCmd(n_ID, IN_302);
        break;
    case 10:
        SendCmd(n_ID, IN_302);
        break;
    default:
        break;
    }
}

void PLCDebugView::slot_CmdBButtonClick(int n_ID)
{
    switch(n_ID){
    case 1:
        SendCmd(n_ID, IN_FULLBOX);
        break;
    case 2:
        SendCmd(n_ID, IN_FULLBOX);
        break;
    case 5:
        SendCmd(n_ID, IN_LINE);
        break;
    case 6:
        SendCmd(n_ID, IN_EMPTYBOX_302);
        break;
    case 7:
        SendCmd(n_ID, IN_LINE_NOBAD);
        break;
    case 8:
        SendCmd(n_ID, IN_127);
        break;
    case 9:
        SendCmd(n_ID, IN_127);
        break;
    case 10:
        SendCmd(n_ID, IN_127);
        break;
    default:
        break;
    }
}

void PLCDebugView::slot_CmdCButtonClick(int n_ID)
{
    SendCmd(n_ID, IN_FULLBOX_127);
}

void PLCDebugView::slot_CmdDButtonClick(int n_ID)
{
    SendCmd(n_ID, IN_FULLBOX_302);
}

void PLCDebugView::slot_TimeOut()
{
    PC_DATAAREA_1 PCData_1;
    PC_DATAAREA_2 PCData_2;
    PC_DATAAREA_3 PCData_3;

    m_pSiemensPLCDeviceManager->GetWritePCData_1(PCData_1);
    m_pSiemensPLCDeviceManager->GetWritePCData_2(PCData_2);
    m_pSiemensPLCDeviceManager->GetWritePCData_3(PCData_3);

    unsigned char ucLastData[40] = {};
    memset(ucLastData, 0, 40);
    memcpy(ucLastData, &PCData_1, 10);
    memcpy(ucLastData + 10, &PCData_1, 2);
    memcpy(ucLastData + 12, &PCData_1, 8);

    for(int i = 0; i < 20/2; i++) {
        if(!m_mapPB.contains(PC_CMD(ucLastData[i*2])))
            continue;

        QPushButton *p_QPushButton = m_mapPB.value(PC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QPushButton != NULL){
            p_QPushButton->setStyleSheet(NoClick_Style);
        }
    }

    memcpy(&m_LastPCData, ucLastData, 20);
}

void PLCDebugView::slot_ReadData_1(PLC_DATAAREA_1 PLCData)
{
    unsigned char ucLastData[20] = {};
    memset(ucLastData, 0, 20);
    memcpy(ucLastData, &PLCData, 10);

    for(int i = 0; i < 10/2; i++) {
        if(!m_mapLB.contains(PLC_CMD(ucLastData[i*2])))
            continue;

        QLabel *p_QLabel = m_mapLB.value(PLC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QLabel != NULL){
            p_QLabel->setStyleSheet(NoClick_Style);
        }
    }

    memcpy(&m_LastPLCData, ucLastData, 10);
}

void PLCDebugView::slot_ReadData_2(PLC_DATAAREA_2 PLCData)
{
    unsigned char ucLastData[4] = {};
    memset(ucLastData, 0, 4);
    memcpy(ucLastData, &PLCData, 2);

    for(int i = 5; i < 2/2; i++) {
        if(!m_mapLB.contains(PLC_CMD(ucLastData[i*2])))
            continue;

        QLabel *p_QLabel = m_mapLB.value(PLC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QLabel != NULL){
            p_QLabel->setStyleSheet(NoClick_Style);
        }
    }

    memcpy(&m_LastPCData + 10, ucLastData, 2);
}

void PLCDebugView::slot_ReadData_3(PLC_DATAAREA_3 PLCData)
{
    unsigned char ucLastData[16] = {};
    memset(ucLastData, 0, 16);
    memcpy(ucLastData, &PLCData, 8);

    for(int i = 6; i < 8/2; i++) {
        if(!m_mapLB.contains(PLC_CMD(ucLastData[i*2])))
            continue;

        QLabel *p_QLabel = m_mapLB.value(PLC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QLabel != NULL){
            p_QLabel->setStyleSheet(NoClick_Style);
        }
    }

    memcpy(&m_LastPLCData + 12, ucLastData, 8);
}

void PLCDebugView::slot_PLCConnectState(int n_Station, bool b_Connect)
{
    if(n_Station == 1){
        if(b_Connect){
            ui->lb_State_1->setText(QStringLiteral("连接"));
            ui->lb_State_1->setStyleSheet(Connect_Style);
        }
        else{
            ui->lb_State_1->setText(QStringLiteral("断开"));
            ui->lb_State_1->setStyleSheet(Disconnect_Style);
        }
    }
    else if(n_Station == 2){
        if(b_Connect){
            ui->lb_State_2->setText(QStringLiteral("连接"));
            ui->lb_State_2->setStyleSheet(Connect_Style);
        }
        else{
            ui->lb_State_2->setText(QStringLiteral("断开"));
            ui->lb_State_2->setStyleSheet(Disconnect_Style);
        }
    }
    else if(n_Station == 3){
        if(b_Connect){
            ui->lb_State_3->setText(QStringLiteral("连接"));
            ui->lb_State_3->setStyleSheet(Connect_Style);
        }
        else{
            ui->lb_State_3->setText(QStringLiteral("断开"));
            ui->lb_State_3->setStyleSheet(Disconnect_Style);
        }
    }

}
