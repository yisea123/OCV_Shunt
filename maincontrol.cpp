#include "maincontrol.h"

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QDebug>

#include "ConfigFile/datafile.h"

MainControl::MainControl(QObject *parent)
    :QObject (parent)
{
    m_pScanerManager = NULL;
    m_pSDMesManager = NULL;
    m_pSiemensPLCDeviceManager = NULL;
    m_pSportCardManager = NULL;

    m_WorkState= _WAITE;

    InitMainControl();
}

MainControl::~MainControl()
{
    ExitMainControl();
}

bool MainControl::PrepareAutoWork()
{
    if(m_WorkState != _WAITE)
        return false;

    if(!ConnectAllDevice())
        return false;

    m_WorkState = _READY;
    return true;
}

bool MainControl::StartAutoWork()
{
    if(m_WorkState != _READY)
        return false;

    m_pSiemensPLCDeviceManager->UploadCmdSwitch(true);
    m_pSportCardManager->UploadCmdSwitch(true);

    m_WorkState = _WORKING;
    return true;
}

bool MainControl::PauseAutoWork()
{
    if(m_WorkState != _WORKING)
        return false;

    m_WorkState = _PAUSE;
    return true;
}

bool MainControl::ContinueAutoWork()
{
    if(m_WorkState != _PAUSE)
        return false;

    m_WorkState = _WORKING;
    return true;
}

bool MainControl::StopAutoWork()
{
    if(m_WorkState != _WORKING && m_WorkState != _PAUSE)
        return false;

    m_pSiemensPLCDeviceManager->UploadCmdSwitch(false);
    m_pSportCardManager->UploadCmdSwitch(false);

    m_WorkState = _WAITE;
    return true;
}

WORKSTATE MainControl::GetWorkState()
{
    return m_WorkState;
}

bool MainControl::SetMesType(MESURLTYPE MESUrl_Type)
{
    return m_pSDMesManager->SetMesUrl(MESUrl_Type);
}

ScanerManager *MainControl::GetPoint_ScanerManager()
{
    return m_pScanerManager;
}

SDMesManager *MainControl::GetPoint_SDMesManager()
{
    return m_pSDMesManager;
}

SiemensPLCDeviceManager *MainControl::GetPoint_SiemensPLCDevice()
{
    return m_pSiemensPLCDeviceManager;
}

SportCardManager *MainControl::GetPoint_SportCardManager()
{
    return m_pSportCardManager;
}

void MainControl::SendAnormalWorkInfo(const int &n_STtion, const ANORMALWORK &ERROR)
{
    emit sig_AnormalWork(n_STtion, ERROR);
}

bool MainControl::RequsetCode(const int &n_ID)
{
    if(m_pScanerManager == NULL)
        return false;

    if(!m_pScanerManager->RequsetCode(n_ID)){
        SendAnormalWorkInfo(n_ID, SACNERWARING);
        return false;
    }
    return true;
}

bool MainControl::SendPLCCmd(const int &n_ID, const PC_CMD &cmd)
{
    if(m_pSiemensPLCDeviceManager == NULL)
        return false;

    if(!m_pSiemensPLCDeviceManager->SendCmd(n_ID, cmd)){
        SendAnormalWorkInfo(n_ID, PLCWARING);
        return false;
    }
    return true;
}

bool MainControl::SportCardCmd_GetRemoveFullBox(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_GetRemoveFullBox(n_ID)){
        SendAnormalWorkInfo(n_ID, SPORTSCARDWARING);
        return false;
    }
    return true;
}

bool MainControl::SportCardCmd_Cmd_GetRemoveEmptyBox(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_GetRemoveEmptyBox(n_ID)){
        SendAnormalWorkInfo(n_ID, SPORTSCARDWARING);
        return false;
    }
    return true;
}

bool MainControl::SportCardCmd_Cmd_SendFullReady(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_SendFullReady(n_ID)){
        SendAnormalWorkInfo(n_ID, SPORTSCARDWARING);
        return false;
    }
    return true;
}

bool MainControl::SportCardCmd_Cmd_SendEmptyReady(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_SendEmptyReady(n_ID)){
        SendAnormalWorkInfo(n_ID, SPORTSCARDWARING);
        return false;
    }
    return true;
}

bool MainControl::SportCardCmd_WriteProductInfo(const int &n_ID, QList<PRODUCTINFO_> &list_ProductInfo)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->WriteProductInfo(n_ID, list_ProductInfo)){
        SendAnormalWorkInfo(n_ID, SPORTSCARDWARING);
        return false;
    }
    return true;
}

bool MainControl::SportCardCmd_GetProductInfo(const int &n_ID, QList<PRODUCTINFO_> &list_ProductInfo)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->GetProductInfo(n_ID, list_ProductInfo)){
        SendAnormalWorkInfo(n_ID, SPORTSCARDWARING);
        return false;
    }
    return true;
}

bool MainControl::MES_UploadBadProductList(const int &n_ScanerId,
                                           const QString &str_Code,
                                           const QList<PRODUCTINFO> &list_Product)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->UploadBadProductList(n_ScanerId, str_Code, list_Product)){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return false;
    }

    emit sig_MesWork(n_ScanerId, true);
    return true;
}

bool MainControl::MES_RequestExportDirection(const int &n_ScanerId,
                                             const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->RequestExportDirection(n_ScanerId, str_Code)){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return false;
    }

    emit sig_MesWork(n_ScanerId, true);
    return true;
}

bool MainControl::MES_CompleteSortingBadProduct(const int &n_ScanerId,
                                                const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->CompleteSortingBadProduct(n_ScanerId, str_Code)){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return false;
    }

    emit sig_MesWork(n_ScanerId, true);
    return true;
}

bool MainControl::MES_RequestProductListInfo(const int &n_ScanerId,
                                             const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->RequestProductListInfo(n_ScanerId, str_Code)){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return false;
    }

    emit sig_MesWork(n_ScanerId, true);
    return true;
}

bool MainControl::MES_RequestBoxIsEmpty(const int &n_ScanerId,
                                        const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->RequestBoxIsEmpty(n_ScanerId, str_Code)){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return false;
    }

    emit sig_MesWork(n_ScanerId, true);
    return true;
}

void MainControl::InitMainControl()
{
    m_pScanerManager = new ScanerManager;
    m_pSDMesManager = new SDMesManager;
    m_pSiemensPLCDeviceManager = new SiemensPLCDeviceManager;
    m_pSportCardManager = new SportCardManager;

    connect(m_pScanerManager, &ScanerManager::sig_CodeData,
            this, &MainControl::slot_ScanCodeData);
    connect(m_pScanerManager, &ScanerManager::sig_ConnectState,
            this, &MainControl::slot_ScanerConnectState);

    connect(m_pSDMesManager, &SDMesManager::sig_Result_Heartbeat,
            this, &MainControl::slot_Result_Heartbeat);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_SendBadProductList,
            this, &MainControl::slot_Result_SendBadProductList);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_RequestProductListInfo,
            this, &MainControl::slot_Result_RequestProductListInfo);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_RequestBoxIsEmpty,
            this, &MainControl::slot_Result_RequestBoxIsEmpty);

    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_ConnectState,
            this, &MainControl::slot_PLCConnectState);
    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_ReadData_1,
            this, &MainControl::slot_ReadData_1);
    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_ReadData_2,
            this, &MainControl::slot_ReadData_2);
    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_ReadData_3,
            this, &MainControl::slot_ReadData_3);
    connect(m_pSiemensPLCDeviceManager, &SiemensPLCDeviceManager::sig_GetCmd,
            this, &MainControl::slot_GetCmd);

    connect(m_pSportCardManager, &SportCardManager::sig_ConnectState,
            this, &MainControl::slot_SportCardConnectState);
    connect(m_pSportCardManager, &SportCardManager::sig_CmdData,
            this, &MainControl::slot_SportCardCmdData);
    connect(m_pSportCardManager, &SportCardManager::sig_SportCardCmd,
            this, &MainControl::slot_SportCardCmd);
}

void MainControl::ExitMainControl()
{
    if(m_pScanerManager != NULL){
        delete  m_pScanerManager;
        m_pScanerManager = NULL;
    }

    if(m_pSDMesManager != NULL){
        delete  m_pSDMesManager;
        m_pSDMesManager = NULL;
    }

    if(m_pSiemensPLCDeviceManager != NULL){
        delete  m_pSiemensPLCDeviceManager;
        m_pSiemensPLCDeviceManager = NULL;
    }

    if(m_pSportCardManager != NULL){
        delete  m_pSportCardManager;
        m_pSportCardManager = NULL;
    }
}

bool MainControl::ConnectAllDevice()
{
    bool b_Result = true;

    if(!m_pScanerManager->ClearScanerDevice())
        SendAnormalWorkInfo(0, SACNERCONNECTFAILE);

    DataFile o_DataFile;

    QList<QString> list_ScanerIP;
    QList<QString> list_PLCIP;
    QList<QString> list_SportCardIP;

    o_DataFile.GetScanerIPList(list_ScanerIP);
    o_DataFile.GetPLCIPList(list_PLCIP);
    o_DataFile.GetSportCardIPList(list_SportCardIP);

    for(int i = 1; i <= list_ScanerIP.size(); i++){
        if(!m_pScanerManager->AddScanerDevice(i, list_ScanerIP.at(i-1), 4001)){
            SendAnormalWorkInfo(i, SACNERCONNECTFAILE);
            b_Result = false;
        }
    }

    if(list_PLCIP.size() == 3){
        m_pSiemensPLCDeviceManager->SetDeviceID_IP_1(1, list_PLCIP.value(0));
        m_pSiemensPLCDeviceManager->SetWrite_ReadParameter_1(1,1,26,1,1,26);

        m_pSiemensPLCDeviceManager->SetDeviceID_IP_2(2, list_PLCIP.value(1));
        m_pSiemensPLCDeviceManager->SetWrite_ReadParameter_1(1,1,26,1,1,26);

        m_pSiemensPLCDeviceManager->SetDeviceID_IP_3(3, list_PLCIP.value(2));
        m_pSiemensPLCDeviceManager->SetWrite_ReadParameter_1(1,1,26,1,1,26);
    }

    if(!m_pSiemensPLCDeviceManager->ConnectDevice()){
        b_Result = false;
        SendAnormalWorkInfo(0, PLCCONNECTFAILE);
    }

    if(!m_pSportCardManager->ClearSportCard())
        SendAnormalWorkInfo(0, SPORTSCARDCONNECTFAILE);

    for(int i = 1; i <= list_SportCardIP.size(); i++){
        if(!m_pSportCardManager->AddSportCard(i, list_ScanerIP.at(i-1))){
            SendAnormalWorkInfo(i, SPORTSCARDCONNECTFAILE);
            b_Result = false;
        }
    }

    return b_Result;
}

void MainControl::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(50);
    }
}

void MainControl::PauseWork()
{
    while(m_WorkState == _PAUSE){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

bool MainControl::GetProductbQuality(QList<PRODUCTINFO> &list_Product)
{
    bool b_Quality = true;

    for(int i = 0; i < list_Product.size(); i++){
        if(list_Product.at(i).str_Quality == "1")
            b_Quality = false;
    }

    return b_Quality;
}

void MainControl::TranferData1(QList<PRODUCTINFO> &list_Product,
                               QList<PRODUCTINFO_> &list_ProductInfo_)
{
    PRODUCTINFO_ ProductInfo;

    for(int i = 0; i < list_Product.size(); i++) {
        ProductInfo.n_SequencnNumber = list_Product.at(i).n_SequencnNumber;
        ProductInfo.str_ID = list_Product.at(i).str_ID;
        ProductInfo.str_Quality = list_Product.at(i).str_Quality;

        list_ProductInfo_.append(ProductInfo);
    }
}

void MainControl::TranferData2(QList<PRODUCTINFO_> &list_ProductInfo_,
                               QList<PRODUCTINFO> &list_Product)
{
    PRODUCTINFO ProductInfo;

    for(int i = 0; i < list_ProductInfo_.size(); i++) {
        ProductInfo.n_SequencnNumber = list_ProductInfo_.at(i).n_SequencnNumber;
        ProductInfo.str_ID = list_ProductInfo_.at(i).str_ID;
        ProductInfo.str_Quality = list_ProductInfo_.at(i).str_Quality;

        list_Product.append(ProductInfo);
    }
}

void MainControl::slot_ScanCodeData(int n_ID, bool b_ScanState, QString str_Code)
{
    PauseWork();

    if(m_WorkState == _WAITE || m_WorkState == _READY)
        return;

    if(!b_ScanState){
        SendAnormalWorkInfo(n_ID, SACNERWARING);
        return;
    }

    emit sig_ScanCodeData(n_ID, str_Code);

    m_mapScanCode.insert(n_ID, str_Code);

    switch(n_ID){
    case 1:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 2:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 3:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 4:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 5:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 6:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 7:{
        MES_RequestProductListInfo(n_ID, str_Code);
        break;
    }
    case 8:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 9:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    case 10:{
        MES_RequestBoxIsEmpty(n_ID, str_Code);
        break;
    }
    default:
        break;
    }
}

void MainControl::slot_ScanerConnectState(int n_ID, bool b_ConnectState)
{
    emit sig_ScanerConnectState(n_ID, b_ConnectState);
}

void MainControl::slot_Result_SendBadProductList(int n_ScanerId, QString str_Code, MESRESPOND RespondState)
{
    PauseWork();

    if(m_WorkState == _WAITE || m_WorkState == _READY)
        return;

    if(RespondState != RESPONDSUCCEED){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return;
    }

    emit sig_MesWork(n_ScanerId, false);

    switch (n_ScanerId) {
    case 5:{
        
        break;
    }
    case 8:{

        break;
    }
    case 12:{

        break;
    }
    default:
        SendAnormalWorkInfo(n_ScanerId, MESDATAERROR);
        break;
    }
}

void MainControl::slot_Result_Heartbeat(MESRESPOND RespondState)
{
    emit sig_Result_Heartbeat(RespondState);
}

void MainControl::slot_Result_RequestProductListInfo(int n_ScanerId, QString str_Code, QList<PRODUCTINFO> list_Product, MESRESPOND RespondState)
{
    PauseWork();

    if(m_WorkState == _WAITE || m_WorkState == _READY)
        return;

    if(RespondState != RESPONDSUCCEED){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return;
    }

    emit sig_MesWork(n_ScanerId, false);

    if(n_ScanerId == 7){
        if(GetProductbQuality(list_Product))
            SendPLCCmd(n_ScanerId,IN_LINE_NOBAD);
        else
            SendPLCCmd(n_ScanerId,IN_ARTIFICIALCHANNEL);
    }
}

void MainControl::slot_Result_RequestBoxIsEmpty(int n_ScanerId, QString str_Code, BOXSTATE BoxState, MESRESPOND RespondState)
{
    PauseWork();

    if(m_WorkState == _WAITE || m_WorkState == _READY)
        return;

    if(RespondState != RESPONDSUCCEED){
        SendAnormalWorkInfo(n_ScanerId, MESWARING);
        return;
    }

    emit sig_MesWork(n_ScanerId, false);

    switch (n_ScanerId) {
    case 1:{
        if(BoxState == EMPTYBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, PC_WARNING);
                SendAnormalWorkInfo(n_ScanerId, BOXERROR);
            }
            else{
                SendPLCCmd(n_ScanerId, IN_EMPTYBOX);
            }
        }
        else if(BoxState == FULLBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, PC_WARNING);
                SendAnormalWorkInfo(n_ScanerId, BOXERROR);
            }
            else{
                SendPLCCmd(n_ScanerId, IN_FULLBOX);
            }
        }
        else if(BoxState == INEXISTENCE){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        break;
    }
    case 3:{
        if(BoxState == EMPTYBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, PC_WARNING);
                SendAnormalWorkInfo(n_ScanerId, BOXERROR);
            }
            else{
                SportCardCmd_Cmd_SendEmptyReady(1);
            }
        }
        else if(BoxState == FULLBOX){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        else if(BoxState == INEXISTENCE){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        break;
    }
    case 4:{
        if(BoxState == EMPTYBOX){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        else if(BoxState == FULLBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, PC_WARNING);
                SendAnormalWorkInfo(n_ScanerId, BOXERROR);
            }
            else{
                SportCardCmd_Cmd_SendFullReady(1);
            }
        }
        else if(BoxState == INEXISTENCE){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        break;
    }
    case 5:{
        if(str_Code.contains("A217217") || str_Code.contains("C217217")){
            SendPLCCmd(n_ScanerId, IN_LINE);
        }
        else{
            if(BoxState == EMPTYBOX){
                SendPLCCmd(n_ScanerId, IN_LINE);
            }
            else if(BoxState == FULLBOX){
                SendPLCCmd(n_ScanerId, IN_SWAPCHANNEL);
            }
            else if(BoxState == INEXISTENCE){
                SendPLCCmd(n_ScanerId, PC_WARNING);
                SendAnormalWorkInfo(n_ScanerId, BOXERROR);
            }
        }
        break;
    }
    case 6:{
        if(BoxState == EMPTYBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, IN_EMPTYBOX_127);
            }
            else{
                SendPLCCmd(n_ScanerId, IN_EMPTYBOX_302);
            }
        }
        else if(BoxState == FULLBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, IN_FULLBOX_127);
            }
            else{
                SendPLCCmd(n_ScanerId, IN_FULLBOX_302);
            }
        }
        else if(BoxState == INEXISTENCE){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        break;
    }
    case 8:{
        if(BoxState == EMPTYBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, IN_127);
            }
            else{
                SendPLCCmd(n_ScanerId, IN_302);
            }
        }
        else if(BoxState == FULLBOX){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        else if(BoxState == INEXISTENCE){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        break;
    }
    case 9:{
        if(BoxState == EMPTYBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, IN_127);
            }
            else{
                SendPLCCmd(n_ScanerId, IN_302);
            }
        }
        else if(BoxState == FULLBOX){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        else if(BoxState == INEXISTENCE){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        break;
    }
    case 10:{
        if(BoxState == EMPTYBOX){
            if(str_Code.contains("A217217") || str_Code.contains("C217217")){
                SendPLCCmd(n_ScanerId, IN_127);
            }
            else{
                SendPLCCmd(n_ScanerId, IN_302);
            }
        }
        else if(BoxState == FULLBOX){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        else if(BoxState == INEXISTENCE){
            SendPLCCmd(n_ScanerId, PC_WARNING);
            SendAnormalWorkInfo(n_ScanerId, BOXERROR);
        }
        break;
    }
    default:
        SendAnormalWorkInfo(n_ScanerId, MESDATAERROR);
        break;
    }
}

void MainControl::slot_PLCConnectState(int n_Station, bool b_Connect)
{
    emit sig_PLCConnectState(n_Station, b_Connect);
}

void MainControl::slot_ReadData_1(PLC_DATAAREA_1 PLCData)
{
    if(m_WorkState == _WAITE)
        return;

    emit sig_ReadPLCData_1(PLCData);
}

void MainControl::slot_ReadData_2(PLC_DATAAREA_2 PLCData)
{
    if(m_WorkState == _WAITE)
        return;

    emit sig_ReadPLCData_2(PLCData);
}

void MainControl::slot_ReadData_3(PLC_DATAAREA_3 PLCData)
{
    if(m_WorkState == _WAITE)
        return;

    emit sig_ReadPLCData_3(PLCData);
}


void MainControl::slot_GetCmd(int n_Station, PLC_CMD cmd)
{
    PauseWork();

    if(m_WorkState == _WAITE || m_WorkState == _READY)
        return;

    if(cmd == PLC_WARNING){
        SendAnormalWorkInfo(n_Station, PLCWARING);
    }
    else if(cmd == NOREQUESTSCAN){
        SendPLCCmd(n_Station, RESTORATION);
    }
    else if(cmd == REQUESTSCAN){
        RequsetCode(n_Station);
    }
}

void MainControl::slot_SportCardConnectState(int n_ID, bool b_State)
{
    emit sig_SportCardConnectState(n_ID, b_State);
}

void MainControl::slot_SportCardCmdData(int n_ID, SPC_SWAP_CMD Cmd)
{
    if(m_WorkState == _WAITE)
        return;

    emit sig_SportCardCmdData(n_ID, Cmd);
}

void MainControl::slot_SportCardCmd(int n_ID, SWAPCMD Cmd)
{  
    PauseWork();

    if(m_WorkState == _WAITE || m_WorkState == _READY)
        return;

    if(Cmd == REMOVEFULLBOX_COMPLETETAKE){
        MES_CompleteSortingBadProduct(n_ID, m_mapScanCode.value(n_ID));
        SportCardCmd_GetRemoveFullBox(n_ID);
    }
    else if(Cmd == REMOVEEMPTYBOX_COMPLETEPUT){
        QList<PRODUCTINFO_> list_ProductInfo_;
        SportCardCmd_GetProductInfo(n_ID, list_ProductInfo_);

        QList<PRODUCTINFO> list_ProductInfo;
        TranferData2(list_ProductInfo_, list_ProductInfo);

        MES_UploadBadProductList(n_ID, m_mapScanCode.value(n_ID), list_ProductInfo);
    }
    else{
        SendAnormalWorkInfo(n_ID, MESDATAERROR);
    }
}

