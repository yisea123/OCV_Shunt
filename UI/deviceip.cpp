#include "deviceip.h"
#include "ui_deviceip.h"

#include <QPushButton>

DeviceIP::DeviceIP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceIP)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QStringLiteral("Éè±¸IP"));
    this->setFixedSize(this->size());

    InitView();

    ReadConfig();
}

DeviceIP::~DeviceIP()
{
    delete ui;
}

void DeviceIP::Write_pb()
{
    ui->pb_Read->setStyleSheet(NoClick_Style);
    ui->pb_Write->setStyleSheet(Click_Style);

    WriteConfig();
}

void DeviceIP::Read_pb()
{
    ui->pb_Read->setStyleSheet(Click_Style);
    ui->pb_Write->setStyleSheet(NoClick_Style);

    ReadConfig();
}

void DeviceIP::InitView()
{
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_1);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_2);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_3);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_4);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_5);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_6);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_7);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_8);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_9);
    m_listScanerIPLineEdit.append(ui->le_ScanerIP_10);

    m_listPLCIPLineEdit.append(ui->le_PLCIP_1);
    m_listPLCIPLineEdit.append(ui->le_PLCIP_2);
    m_listPLCIPLineEdit.append(ui->le_PLCIP_3);

    m_listSportCardIPLineEdit.append(ui->le_SportCardIP_1);

    connect(ui->pb_Write, &QPushButton::clicked,
            this, &DeviceIP::Write_pb);

    connect(ui->pb_Read, &QPushButton::clicked,
            this, &DeviceIP::Read_pb);
}

void DeviceIP::ReadConfig()
{
    DataFile o_DataFile;

    QList<QString> list_ScanerIP;
    QList<QString> list_PLCIP;
    QList<QString> list_SportCardIP;

    o_DataFile.GetScanerIPList(list_ScanerIP);
    o_DataFile.GetPLCIPList(list_PLCIP);
    o_DataFile.GetSportCardIPList(list_SportCardIP);

    if(m_listScanerIPLineEdit.size() != list_ScanerIP.size() ||
            m_listSportCardIPLineEdit.size() != list_SportCardIP.size()){
        return;
    }

    for(int i = 0; i < list_ScanerIP.size(); i++){
        m_listScanerIPLineEdit.at(i)->setText(list_ScanerIP.at(i));
    }

    for(int i = 0; i < list_SportCardIP.size(); i++){
        m_listSportCardIPLineEdit.at(i)->setText(list_SportCardIP.at(i));
    }

    for(int i = 0; i < list_PLCIP.size(); i++){
        m_listPLCIPLineEdit.at(i)->setText(list_PLCIP.at(i));
    }

}

void DeviceIP::WriteConfig()
{
    DataFile o_DataFile;

    QList<QString> list_ScanerIP;
    QList<QString> list_PLCIP;
    QList<QString> list_SportCardIP;

    for(int i = 0; i < m_listScanerIPLineEdit.size(); i++){
        list_ScanerIP.append(m_listScanerIPLineEdit.at(i)->text());
    }

    for(int i = 0; i < m_listSportCardIPLineEdit.size(); i++){
        list_SportCardIP.append(m_listSportCardIPLineEdit.at(i)->text());
    }

    for(int i = 0; i < m_listPLCIPLineEdit.size(); i++){
        list_PLCIP.append(m_listPLCIPLineEdit.at(i)->text());
    }

    o_DataFile.SaveScanerIPList(list_ScanerIP);
    o_DataFile.SavePLCIPList(list_PLCIP);
    o_DataFile.SaveSportCardIPList(list_SportCardIP);
}
