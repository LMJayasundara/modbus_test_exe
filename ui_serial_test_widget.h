/********************************************************************************
** Form generated from reading UI file 'serial_test_widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIAL_TEST_WIDGET_H
#define UI_SERIAL_TEST_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Serial_Test_Widget
{
public:
    QGroupBox *send_serial_info_gb_;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *send_serial_port_lbl_;
    QComboBox *send_serial_port_cb_;
    QLabel *send_baud_lbl_;
    QComboBox *send_baud_cb_;
    QLabel *send_data_bits_lbl_;
    QComboBox *send_data_bits_cb_;
    QLabel *send_parity_lbl_;
    QComboBox *send_parity_cb_;
    QLabel *send_stop_bits_lbl_;
    QComboBox *send_stop_bits_cb_;
    QLabel *send_flow_ctrl_lbl_;
    QComboBox *send_flow_ctrl_cb_;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *conn_lbl_;
    QComboBox *conn_cb_;
    QGroupBox *send_data_gb_;
    QTextEdit *send_data_txt_;
    QGroupBox *recv_data_gb_;
    QTextEdit *recv_data_txt_;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *send_data_type_lbl_;
    QComboBox *send_data_type_cb_;
    QLabel *send_interval_lbl_;
    QComboBox *send_interval_cb_;
    QCheckBox *send_repeatedly_cb_;
    QPushButton *send_btn_;
    QGroupBox *recv_serial_info_gb_;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *recv_serial_port_lbl_;
    QComboBox *recv_serial_port_cb_;
    QLabel *recv_baud_lbl_;
    QComboBox *recv_baud_cb_;
    QLabel *recv_data_bits_lbl_;
    QComboBox *recv_data_bits_cb_;
    QLabel *recv_parity_lbl_;
    QComboBox *recv_parity_cb_;
    QLabel *recv_stop_bits_lbl_;
    QComboBox *recv_stop_bits_cb_;
    QLabel *recv_flow_ctrl_lbl_;
    QComboBox *recv_flow_ctrl_cb_;
    QWidget *layoutWidget_4;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *recv_btn_;
    QPushButton *stop_recv_btn_;

    void setupUi(QWidget *Serial_Test_Widget)
    {
        if (Serial_Test_Widget->objectName().isEmpty())
            Serial_Test_Widget->setObjectName(QString::fromUtf8("Serial_Test_Widget"));
        Serial_Test_Widget->resize(800, 650);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Serial_Test_Widget->sizePolicy().hasHeightForWidth());
        Serial_Test_Widget->setSizePolicy(sizePolicy);
        Serial_Test_Widget->setMinimumSize(QSize(800, 650));
        send_serial_info_gb_ = new QGroupBox(Serial_Test_Widget);
        send_serial_info_gb_->setObjectName(QString::fromUtf8("send_serial_info_gb_"));
        send_serial_info_gb_->setGeometry(QRect(10, 40, 781, 51));
        layoutWidget = new QWidget(send_serial_info_gb_);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 21, 751, 22));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        send_serial_port_lbl_ = new QLabel(layoutWidget);
        send_serial_port_lbl_->setObjectName(QString::fromUtf8("send_serial_port_lbl_"));

        horizontalLayout_3->addWidget(send_serial_port_lbl_);

        send_serial_port_cb_ = new QComboBox(layoutWidget);
        send_serial_port_cb_->setObjectName(QString::fromUtf8("send_serial_port_cb_"));
        send_serial_port_cb_->setEditable(true);

        horizontalLayout_3->addWidget(send_serial_port_cb_);

        send_baud_lbl_ = new QLabel(layoutWidget);
        send_baud_lbl_->setObjectName(QString::fromUtf8("send_baud_lbl_"));

        horizontalLayout_3->addWidget(send_baud_lbl_);

        send_baud_cb_ = new QComboBox(layoutWidget);
        send_baud_cb_->setObjectName(QString::fromUtf8("send_baud_cb_"));
        send_baud_cb_->setEditable(true);

        horizontalLayout_3->addWidget(send_baud_cb_);

        send_data_bits_lbl_ = new QLabel(layoutWidget);
        send_data_bits_lbl_->setObjectName(QString::fromUtf8("send_data_bits_lbl_"));

        horizontalLayout_3->addWidget(send_data_bits_lbl_);

        send_data_bits_cb_ = new QComboBox(layoutWidget);
        send_data_bits_cb_->setObjectName(QString::fromUtf8("send_data_bits_cb_"));

        horizontalLayout_3->addWidget(send_data_bits_cb_);

        send_parity_lbl_ = new QLabel(layoutWidget);
        send_parity_lbl_->setObjectName(QString::fromUtf8("send_parity_lbl_"));

        horizontalLayout_3->addWidget(send_parity_lbl_);

        send_parity_cb_ = new QComboBox(layoutWidget);
        send_parity_cb_->setObjectName(QString::fromUtf8("send_parity_cb_"));

        horizontalLayout_3->addWidget(send_parity_cb_);

        send_stop_bits_lbl_ = new QLabel(layoutWidget);
        send_stop_bits_lbl_->setObjectName(QString::fromUtf8("send_stop_bits_lbl_"));

        horizontalLayout_3->addWidget(send_stop_bits_lbl_);

        send_stop_bits_cb_ = new QComboBox(layoutWidget);
        send_stop_bits_cb_->setObjectName(QString::fromUtf8("send_stop_bits_cb_"));

        horizontalLayout_3->addWidget(send_stop_bits_cb_);

        send_flow_ctrl_lbl_ = new QLabel(layoutWidget);
        send_flow_ctrl_lbl_->setObjectName(QString::fromUtf8("send_flow_ctrl_lbl_"));

        horizontalLayout_3->addWidget(send_flow_ctrl_lbl_);

        send_flow_ctrl_cb_ = new QComboBox(layoutWidget);
        send_flow_ctrl_cb_->setObjectName(QString::fromUtf8("send_flow_ctrl_cb_"));
        send_flow_ctrl_cb_->setEditable(true);

        horizontalLayout_3->addWidget(send_flow_ctrl_cb_);

        layoutWidget1 = new QWidget(Serial_Test_Widget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 10, 781, 25));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        conn_lbl_ = new QLabel(layoutWidget1);
        conn_lbl_->setObjectName(QString::fromUtf8("conn_lbl_"));

        horizontalLayout->addWidget(conn_lbl_);

        conn_cb_ = new QComboBox(layoutWidget1);
        conn_cb_->setObjectName(QString::fromUtf8("conn_cb_"));
        conn_cb_->setEnabled(false);

        horizontalLayout->addWidget(conn_cb_);


        horizontalLayout_2->addLayout(horizontalLayout);

        send_data_gb_ = new QGroupBox(Serial_Test_Widget);
        send_data_gb_->setObjectName(QString::fromUtf8("send_data_gb_"));
        send_data_gb_->setGeometry(QRect(10, 140, 781, 141));
        send_data_txt_ = new QTextEdit(send_data_gb_);
        send_data_txt_->setObjectName(QString::fromUtf8("send_data_txt_"));
        send_data_txt_->setGeometry(QRect(10, 20, 761, 111));
        recv_data_gb_ = new QGroupBox(Serial_Test_Widget);
        recv_data_gb_->setObjectName(QString::fromUtf8("recv_data_gb_"));
        recv_data_gb_->setGeometry(QRect(10, 370, 781, 271));
        recv_data_txt_ = new QTextEdit(recv_data_gb_);
        recv_data_txt_->setObjectName(QString::fromUtf8("recv_data_txt_"));
        recv_data_txt_->setGeometry(QRect(10, 20, 761, 241));
        layoutWidget2 = new QWidget(Serial_Test_Widget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 110, 781, 25));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        send_data_type_lbl_ = new QLabel(layoutWidget2);
        send_data_type_lbl_->setObjectName(QString::fromUtf8("send_data_type_lbl_"));

        horizontalLayout_4->addWidget(send_data_type_lbl_);

        send_data_type_cb_ = new QComboBox(layoutWidget2);
        send_data_type_cb_->setObjectName(QString::fromUtf8("send_data_type_cb_"));
        send_data_type_cb_->setEditable(false);

        horizontalLayout_4->addWidget(send_data_type_cb_);

        send_interval_lbl_ = new QLabel(layoutWidget2);
        send_interval_lbl_->setObjectName(QString::fromUtf8("send_interval_lbl_"));

        horizontalLayout_4->addWidget(send_interval_lbl_);

        send_interval_cb_ = new QComboBox(layoutWidget2);
        send_interval_cb_->setObjectName(QString::fromUtf8("send_interval_cb_"));
        send_interval_cb_->setEditable(true);
        send_interval_cb_->setProperty("currentText", QVariant(QString::fromUtf8("100")));

        horizontalLayout_4->addWidget(send_interval_cb_);

        send_repeatedly_cb_ = new QCheckBox(layoutWidget2);
        send_repeatedly_cb_->setObjectName(QString::fromUtf8("send_repeatedly_cb_"));

        horizontalLayout_4->addWidget(send_repeatedly_cb_);

        send_btn_ = new QPushButton(layoutWidget2);
        send_btn_->setObjectName(QString::fromUtf8("send_btn_"));

        horizontalLayout_4->addWidget(send_btn_);

        recv_serial_info_gb_ = new QGroupBox(Serial_Test_Widget);
        recv_serial_info_gb_->setObjectName(QString::fromUtf8("recv_serial_info_gb_"));
        recv_serial_info_gb_->setGeometry(QRect(10, 280, 781, 51));
        layoutWidget_3 = new QWidget(recv_serial_info_gb_);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(11, 21, 751, 22));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        recv_serial_port_lbl_ = new QLabel(layoutWidget_3);
        recv_serial_port_lbl_->setObjectName(QString::fromUtf8("recv_serial_port_lbl_"));

        horizontalLayout_6->addWidget(recv_serial_port_lbl_);

        recv_serial_port_cb_ = new QComboBox(layoutWidget_3);
        recv_serial_port_cb_->setObjectName(QString::fromUtf8("recv_serial_port_cb_"));
        recv_serial_port_cb_->setEditable(true);

        horizontalLayout_6->addWidget(recv_serial_port_cb_);

        recv_baud_lbl_ = new QLabel(layoutWidget_3);
        recv_baud_lbl_->setObjectName(QString::fromUtf8("recv_baud_lbl_"));

        horizontalLayout_6->addWidget(recv_baud_lbl_);

        recv_baud_cb_ = new QComboBox(layoutWidget_3);
        recv_baud_cb_->setObjectName(QString::fromUtf8("recv_baud_cb_"));
        recv_baud_cb_->setEditable(true);

        horizontalLayout_6->addWidget(recv_baud_cb_);

        recv_data_bits_lbl_ = new QLabel(layoutWidget_3);
        recv_data_bits_lbl_->setObjectName(QString::fromUtf8("recv_data_bits_lbl_"));

        horizontalLayout_6->addWidget(recv_data_bits_lbl_);

        recv_data_bits_cb_ = new QComboBox(layoutWidget_3);
        recv_data_bits_cb_->setObjectName(QString::fromUtf8("recv_data_bits_cb_"));

        horizontalLayout_6->addWidget(recv_data_bits_cb_);

        recv_parity_lbl_ = new QLabel(layoutWidget_3);
        recv_parity_lbl_->setObjectName(QString::fromUtf8("recv_parity_lbl_"));

        horizontalLayout_6->addWidget(recv_parity_lbl_);

        recv_parity_cb_ = new QComboBox(layoutWidget_3);
        recv_parity_cb_->setObjectName(QString::fromUtf8("recv_parity_cb_"));

        horizontalLayout_6->addWidget(recv_parity_cb_);

        recv_stop_bits_lbl_ = new QLabel(layoutWidget_3);
        recv_stop_bits_lbl_->setObjectName(QString::fromUtf8("recv_stop_bits_lbl_"));

        horizontalLayout_6->addWidget(recv_stop_bits_lbl_);

        recv_stop_bits_cb_ = new QComboBox(layoutWidget_3);
        recv_stop_bits_cb_->setObjectName(QString::fromUtf8("recv_stop_bits_cb_"));

        horizontalLayout_6->addWidget(recv_stop_bits_cb_);

        recv_flow_ctrl_lbl_ = new QLabel(layoutWidget_3);
        recv_flow_ctrl_lbl_->setObjectName(QString::fromUtf8("recv_flow_ctrl_lbl_"));

        horizontalLayout_6->addWidget(recv_flow_ctrl_lbl_);

        recv_flow_ctrl_cb_ = new QComboBox(layoutWidget_3);
        recv_flow_ctrl_cb_->setObjectName(QString::fromUtf8("recv_flow_ctrl_cb_"));
        recv_flow_ctrl_cb_->setEditable(true);

        horizontalLayout_6->addWidget(recv_flow_ctrl_cb_);

        layoutWidget_4 = new QWidget(Serial_Test_Widget);
        layoutWidget_4->setObjectName(QString::fromUtf8("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(10, 340, 781, 25));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget_4);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        recv_btn_ = new QPushButton(layoutWidget_4);
        recv_btn_->setObjectName(QString::fromUtf8("recv_btn_"));

        horizontalLayout_7->addWidget(recv_btn_);

        stop_recv_btn_ = new QPushButton(layoutWidget_4);
        stop_recv_btn_->setObjectName(QString::fromUtf8("stop_recv_btn_"));

        horizontalLayout_7->addWidget(stop_recv_btn_);


        retranslateUi(Serial_Test_Widget);

        QMetaObject::connectSlotsByName(Serial_Test_Widget);
    } // setupUi

    void retranslateUi(QWidget *Serial_Test_Widget)
    {
        Serial_Test_Widget->setWindowTitle(QApplication::translate("Serial_Test_Widget", "Serial Test Tool", 0, QApplication::UnicodeUTF8));
        send_serial_info_gb_->setTitle(QApplication::translate("Serial_Test_Widget", "Send Data Serial Port Info", 0, QApplication::UnicodeUTF8));
        send_serial_port_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Port", 0, QApplication::UnicodeUTF8));
        send_serial_port_cb_->clear();
        send_serial_port_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "COM1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "/dev/ttyAMA0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "/dev/ttya", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "/dev/ttyb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM10", 0, QApplication::UnicodeUTF8)
        );
        send_baud_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Baud", 0, QApplication::UnicodeUTF8));
        send_baud_cb_->clear();
        send_baud_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "19200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "38400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "57600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "115200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "921600", 0, QApplication::UnicodeUTF8)
        );
        send_data_bits_lbl_->setText(QApplication::translate("Serial_Test_Widget", "DataBits", 0, QApplication::UnicodeUTF8));
        send_data_bits_cb_->clear();
        send_data_bits_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "8 bits", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "7 bits", 0, QApplication::UnicodeUTF8)
        );
        send_parity_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Parity", 0, QApplication::UnicodeUTF8));
        send_parity_cb_->clear();
        send_parity_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "None", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "Even", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "Odd", 0, QApplication::UnicodeUTF8)
        );
        send_stop_bits_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Stop Bits", 0, QApplication::UnicodeUTF8));
        send_stop_bits_cb_->clear();
        send_stop_bits_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "1 bit", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "1.5 bits", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "2 bits", 0, QApplication::UnicodeUTF8)
        );
        send_flow_ctrl_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Flow Control", 0, QApplication::UnicodeUTF8));
        send_flow_ctrl_cb_->clear();
        send_flow_ctrl_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "none", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "hardware", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "software", 0, QApplication::UnicodeUTF8)
        );
        conn_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Connection", 0, QApplication::UnicodeUTF8));
        conn_cb_->clear();
        conn_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "Serial Port", 0, QApplication::UnicodeUTF8)
        );
        send_data_gb_->setTitle(QApplication::translate("Serial_Test_Widget", "Send Data", 0, QApplication::UnicodeUTF8));
        recv_data_gb_->setTitle(QApplication::translate("Serial_Test_Widget", "Receive Data", 0, QApplication::UnicodeUTF8));
        send_data_type_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Send Data Type", 0, QApplication::UnicodeUTF8));
        send_data_type_cb_->clear();
        send_data_type_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "HEX", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "ASCII", 0, QApplication::UnicodeUTF8)
        );
        send_interval_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Send Interval(ms)", 0, QApplication::UnicodeUTF8));
        send_interval_cb_->clear();
        send_interval_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "100", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "500", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "1000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "2000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "5000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "10000", 0, QApplication::UnicodeUTF8)
        );
        send_repeatedly_cb_->setText(QApplication::translate("Serial_Test_Widget", "Send Repeatedly", 0, QApplication::UnicodeUTF8));
        send_btn_->setText(QApplication::translate("Serial_Test_Widget", "Send", 0, QApplication::UnicodeUTF8));
        recv_serial_info_gb_->setTitle(QApplication::translate("Serial_Test_Widget", "Receive Data Serial Port Info", 0, QApplication::UnicodeUTF8));
        recv_serial_port_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Port", 0, QApplication::UnicodeUTF8));
        recv_serial_port_cb_->clear();
        recv_serial_port_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "COM1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "/dev/ttyAMA0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "/dev/ttya", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "/dev/ttyb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "COM10", 0, QApplication::UnicodeUTF8)
        );
        recv_baud_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Baud", 0, QApplication::UnicodeUTF8));
        recv_baud_cb_->clear();
        recv_baud_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "19200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "38400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "57600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "115200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "921600", 0, QApplication::UnicodeUTF8)
        );
        recv_data_bits_lbl_->setText(QApplication::translate("Serial_Test_Widget", "DataBits", 0, QApplication::UnicodeUTF8));
        recv_data_bits_cb_->clear();
        recv_data_bits_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "8 bits", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "7 bits", 0, QApplication::UnicodeUTF8)
        );
        recv_parity_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Parity", 0, QApplication::UnicodeUTF8));
        recv_parity_cb_->clear();
        recv_parity_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "None", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "Even", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "Odd", 0, QApplication::UnicodeUTF8)
        );
        recv_stop_bits_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Stop Bits", 0, QApplication::UnicodeUTF8));
        recv_stop_bits_cb_->clear();
        recv_stop_bits_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "1 bit", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "1.5 bits", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "2 bits", 0, QApplication::UnicodeUTF8)
        );
        recv_flow_ctrl_lbl_->setText(QApplication::translate("Serial_Test_Widget", "Flow Control", 0, QApplication::UnicodeUTF8));
        recv_flow_ctrl_cb_->clear();
        recv_flow_ctrl_cb_->insertItems(0, QStringList()
         << QApplication::translate("Serial_Test_Widget", "none", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "hardware", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Serial_Test_Widget", "software", 0, QApplication::UnicodeUTF8)
        );
        recv_btn_->setText(QApplication::translate("Serial_Test_Widget", "Receive", 0, QApplication::UnicodeUTF8));
        stop_recv_btn_->setText(QApplication::translate("Serial_Test_Widget", "Stop", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Serial_Test_Widget: public Ui_Serial_Test_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_TEST_WIDGET_H
