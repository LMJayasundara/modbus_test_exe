/********************************************************************************
** Form generated from reading UI file 'mbs_widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MBS_WIDGET_H
#define UI_MBS_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Mbs_Widget
{
public:
    QTabWidget *mbs_tab_widget_;
    QWidget *widget;
    QGroupBox *serial_info_gb_;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *serial_port_lbl_;
    QComboBox *serial_port_cb_;
    QLabel *baud_lbl_;
    QComboBox *baud_cb_;
    QLabel *data_bits_lbl_;
    QComboBox *data_bits_cb_;
    QLabel *parity_lbl_;
    QComboBox *parity_cb_;
    QLabel *stop_bits_lbl_;
    QComboBox *stop_bits_cb_;
    QLabel *flow_ctrl_lbl_;
    QComboBox *flow_ctrl_cb_;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *conn_lbl_;
    QComboBox *conn_cb_;
    QLabel *type_lbl_;
    QComboBox *type_cb_;
    QLabel *mode_lbl_;
    QComboBox *mode_cb_;
    QPushButton *start_btn_;
    QPushButton *stop_btn_;
    QGroupBox *tcp_info_gb_;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *slave_ip_lbl_;
    QComboBox *slave_ip_cb_;
    QLabel *ip_port_lbl_;
    QComboBox *ip_port_cb_;
    QLabel *timeout_lbl_;
    QComboBox *timeout_cb_;
    QGroupBox *tcp_info_gb_1;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *slave_ip_lbl_1;
    QComboBox *slave_ip_cb_1;
    QLabel *ip_port_lbl_1;
    QComboBox *ip_port_cb_1;
    QLabel *timeout_lbl_1;
    QComboBox *timeout_cb_1;

    void setupUi(QWidget *Mbs_Widget)
    {
        if (Mbs_Widget->objectName().isEmpty())
            Mbs_Widget->setObjectName(QString::fromUtf8("Mbs_Widget"));
        Mbs_Widget->resize(800, 650);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Mbs_Widget->sizePolicy().hasHeightForWidth());
        Mbs_Widget->setSizePolicy(sizePolicy);
        Mbs_Widget->setMinimumSize(QSize(800, 650));
        mbs_tab_widget_ = new QTabWidget(Mbs_Widget);
        mbs_tab_widget_->setObjectName(QString::fromUtf8("mbs_tab_widget_"));
        mbs_tab_widget_->setGeometry(QRect(0, 150, 801, 501));
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        mbs_tab_widget_->addTab(widget, QString());
        serial_info_gb_ = new QGroupBox(Mbs_Widget);
        serial_info_gb_->setObjectName(QString::fromUtf8("serial_info_gb_"));
        serial_info_gb_->setGeometry(QRect(10, 40, 781, 51));
        layoutWidget = new QWidget(serial_info_gb_);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 21, 751, 22));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        serial_port_lbl_ = new QLabel(layoutWidget);
        serial_port_lbl_->setObjectName(QString::fromUtf8("serial_port_lbl_"));

        horizontalLayout_3->addWidget(serial_port_lbl_);

        serial_port_cb_ = new QComboBox(layoutWidget);
        serial_port_cb_->setObjectName(QString::fromUtf8("serial_port_cb_"));
        serial_port_cb_->setEditable(true);

        horizontalLayout_3->addWidget(serial_port_cb_);

        baud_lbl_ = new QLabel(layoutWidget);
        baud_lbl_->setObjectName(QString::fromUtf8("baud_lbl_"));

        horizontalLayout_3->addWidget(baud_lbl_);

        baud_cb_ = new QComboBox(layoutWidget);
        baud_cb_->setObjectName(QString::fromUtf8("baud_cb_"));
        baud_cb_->setEditable(true);

        horizontalLayout_3->addWidget(baud_cb_);

        data_bits_lbl_ = new QLabel(layoutWidget);
        data_bits_lbl_->setObjectName(QString::fromUtf8("data_bits_lbl_"));

        horizontalLayout_3->addWidget(data_bits_lbl_);

        data_bits_cb_ = new QComboBox(layoutWidget);
        data_bits_cb_->setObjectName(QString::fromUtf8("data_bits_cb_"));

        horizontalLayout_3->addWidget(data_bits_cb_);

        parity_lbl_ = new QLabel(layoutWidget);
        parity_lbl_->setObjectName(QString::fromUtf8("parity_lbl_"));

        horizontalLayout_3->addWidget(parity_lbl_);

        parity_cb_ = new QComboBox(layoutWidget);
        parity_cb_->setObjectName(QString::fromUtf8("parity_cb_"));

        horizontalLayout_3->addWidget(parity_cb_);

        stop_bits_lbl_ = new QLabel(layoutWidget);
        stop_bits_lbl_->setObjectName(QString::fromUtf8("stop_bits_lbl_"));

        horizontalLayout_3->addWidget(stop_bits_lbl_);

        stop_bits_cb_ = new QComboBox(layoutWidget);
        stop_bits_cb_->setObjectName(QString::fromUtf8("stop_bits_cb_"));

        horizontalLayout_3->addWidget(stop_bits_cb_);

        flow_ctrl_lbl_ = new QLabel(layoutWidget);
        flow_ctrl_lbl_->setObjectName(QString::fromUtf8("flow_ctrl_lbl_"));

        horizontalLayout_3->addWidget(flow_ctrl_lbl_);

        flow_ctrl_cb_ = new QComboBox(layoutWidget);
        flow_ctrl_cb_->setObjectName(QString::fromUtf8("flow_ctrl_cb_"));
        flow_ctrl_cb_->setEditable(true);

        horizontalLayout_3->addWidget(flow_ctrl_cb_);

        layoutWidget1 = new QWidget(Mbs_Widget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 10, 781, 27));
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

        horizontalLayout->addWidget(conn_cb_);

        type_lbl_ = new QLabel(layoutWidget1);
        type_lbl_->setObjectName(QString::fromUtf8("type_lbl_"));

        horizontalLayout->addWidget(type_lbl_);

        type_cb_ = new QComboBox(layoutWidget1);
        type_cb_->setObjectName(QString::fromUtf8("type_cb_"));

        horizontalLayout->addWidget(type_cb_);

        mode_lbl_ = new QLabel(layoutWidget1);
        mode_lbl_->setObjectName(QString::fromUtf8("mode_lbl_"));

        horizontalLayout->addWidget(mode_lbl_);

        mode_cb_ = new QComboBox(layoutWidget1);
        mode_cb_->setObjectName(QString::fromUtf8("mode_cb_"));

        horizontalLayout->addWidget(mode_cb_);


        horizontalLayout_2->addLayout(horizontalLayout);

        start_btn_ = new QPushButton(layoutWidget1);
        start_btn_->setObjectName(QString::fromUtf8("start_btn_"));

        horizontalLayout_2->addWidget(start_btn_);

        stop_btn_ = new QPushButton(layoutWidget1);
        stop_btn_->setObjectName(QString::fromUtf8("stop_btn_"));

        horizontalLayout_2->addWidget(stop_btn_);

        tcp_info_gb_ = new QGroupBox(Mbs_Widget);
        tcp_info_gb_->setObjectName(QString::fromUtf8("tcp_info_gb_"));
        tcp_info_gb_->setGeometry(QRect(10, 90, 781, 51));
        layoutWidget_2 = new QWidget(tcp_info_gb_);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(11, 21, 751, 22));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        slave_ip_lbl_ = new QLabel(layoutWidget_2);
        slave_ip_lbl_->setObjectName(QString::fromUtf8("slave_ip_lbl_"));

        horizontalLayout_4->addWidget(slave_ip_lbl_);

        slave_ip_cb_ = new QComboBox(layoutWidget_2);
        slave_ip_cb_->setObjectName(QString::fromUtf8("slave_ip_cb_"));
        slave_ip_cb_->setEditable(true);

        horizontalLayout_4->addWidget(slave_ip_cb_);

        ip_port_lbl_ = new QLabel(layoutWidget_2);
        ip_port_lbl_->setObjectName(QString::fromUtf8("ip_port_lbl_"));

        horizontalLayout_4->addWidget(ip_port_lbl_);

        ip_port_cb_ = new QComboBox(layoutWidget_2);
        ip_port_cb_->setObjectName(QString::fromUtf8("ip_port_cb_"));
        ip_port_cb_->setEditable(true);

        horizontalLayout_4->addWidget(ip_port_cb_);

        timeout_lbl_ = new QLabel(layoutWidget_2);
        timeout_lbl_->setObjectName(QString::fromUtf8("timeout_lbl_"));

        horizontalLayout_4->addWidget(timeout_lbl_);

        timeout_cb_ = new QComboBox(layoutWidget_2);
        timeout_cb_->setObjectName(QString::fromUtf8("timeout_cb_"));
        timeout_cb_->setEditable(true);

        horizontalLayout_4->addWidget(timeout_cb_);

        tcp_info_gb_1 = new QGroupBox(tcp_info_gb_);
        tcp_info_gb_1->setObjectName(QString::fromUtf8("tcp_info_gb_1"));
        tcp_info_gb_1->setGeometry(QRect(0, 0, 781, 51));
        layoutWidget_3 = new QWidget(tcp_info_gb_1);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(11, 21, 751, 22));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        slave_ip_lbl_1 = new QLabel(layoutWidget_3);
        slave_ip_lbl_1->setObjectName(QString::fromUtf8("slave_ip_lbl_1"));

        horizontalLayout_5->addWidget(slave_ip_lbl_1);

        slave_ip_cb_1 = new QComboBox(layoutWidget_3);
        slave_ip_cb_1->setObjectName(QString::fromUtf8("slave_ip_cb_1"));
        slave_ip_cb_1->setEditable(true);

        horizontalLayout_5->addWidget(slave_ip_cb_1);

        ip_port_lbl_1 = new QLabel(layoutWidget_3);
        ip_port_lbl_1->setObjectName(QString::fromUtf8("ip_port_lbl_1"));

        horizontalLayout_5->addWidget(ip_port_lbl_1);

        ip_port_cb_1 = new QComboBox(layoutWidget_3);
        ip_port_cb_1->setObjectName(QString::fromUtf8("ip_port_cb_1"));
        ip_port_cb_1->setEditable(true);

        horizontalLayout_5->addWidget(ip_port_cb_1);

        timeout_lbl_1 = new QLabel(layoutWidget_3);
        timeout_lbl_1->setObjectName(QString::fromUtf8("timeout_lbl_1"));

        horizontalLayout_5->addWidget(timeout_lbl_1);

        timeout_cb_1 = new QComboBox(layoutWidget_3);
        timeout_cb_1->setObjectName(QString::fromUtf8("timeout_cb_1"));
        timeout_cb_1->setEditable(true);

        horizontalLayout_5->addWidget(timeout_cb_1);


        retranslateUi(Mbs_Widget);

        mbs_tab_widget_->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Mbs_Widget);
    } // setupUi

    void retranslateUi(QWidget *Mbs_Widget)
    {
        Mbs_Widget->setWindowTitle(QApplication::translate("Mbs_Widget", "Modbus Station", 0, QApplication::UnicodeUTF8));
        mbs_tab_widget_->setTabText(mbs_tab_widget_->indexOf(widget), QString());
        serial_info_gb_->setTitle(QApplication::translate("Mbs_Widget", "Serial Port Info", 0, QApplication::UnicodeUTF8));
        serial_port_lbl_->setText(QApplication::translate("Mbs_Widget", "Port", 0, QApplication::UnicodeUTF8));
        serial_port_cb_->clear();
        serial_port_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "COM1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "/dev/ttyAMA0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "/dev/ttya", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "/dev/ttyb", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "COM10", 0, QApplication::UnicodeUTF8)
        );
        baud_lbl_->setText(QApplication::translate("Mbs_Widget", "Baud", 0, QApplication::UnicodeUTF8));
        baud_cb_->clear();
        baud_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "19200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "38400", 0, QApplication::UnicodeUTF8)
        );
        data_bits_lbl_->setText(QApplication::translate("Mbs_Widget", "DataBits", 0, QApplication::UnicodeUTF8));
        data_bits_cb_->clear();
        data_bits_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "8 bits", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "7 bits", 0, QApplication::UnicodeUTF8)
        );
        parity_lbl_->setText(QApplication::translate("Mbs_Widget", "Parity", 0, QApplication::UnicodeUTF8));
        parity_cb_->clear();
        parity_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "None", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "Even", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "Odd", 0, QApplication::UnicodeUTF8)
        );
        stop_bits_lbl_->setText(QApplication::translate("Mbs_Widget", "Stop Bits", 0, QApplication::UnicodeUTF8));
        stop_bits_cb_->clear();
        stop_bits_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "1 bit", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "1.5 bits", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "2 bits", 0, QApplication::UnicodeUTF8)
        );
        flow_ctrl_lbl_->setText(QApplication::translate("Mbs_Widget", "Flow Control", 0, QApplication::UnicodeUTF8));
        flow_ctrl_cb_->clear();
        flow_ctrl_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "none", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "hardware", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "software", 0, QApplication::UnicodeUTF8)
        );
        conn_lbl_->setText(QApplication::translate("Mbs_Widget", "Connection", 0, QApplication::UnicodeUTF8));
        conn_cb_->clear();
        conn_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "Serial Port", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "TCP", 0, QApplication::UnicodeUTF8)
        );
        type_lbl_->setText(QApplication::translate("Mbs_Widget", "Type", 0, QApplication::UnicodeUTF8));
        type_cb_->clear();
        type_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "Master", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "Slave", 0, QApplication::UnicodeUTF8)
        );
        mode_lbl_->setText(QApplication::translate("Mbs_Widget", "Mode", 0, QApplication::UnicodeUTF8));
        mode_cb_->clear();
        mode_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "RTU", 0, QApplication::UnicodeUTF8)
        );
        start_btn_->setText(QApplication::translate("Mbs_Widget", "Start", 0, QApplication::UnicodeUTF8));
        stop_btn_->setText(QApplication::translate("Mbs_Widget", "Stop", 0, QApplication::UnicodeUTF8));
        tcp_info_gb_->setTitle(QApplication::translate("Mbs_Widget", "TCP Port Info", 0, QApplication::UnicodeUTF8));
        slave_ip_lbl_->setText(QApplication::translate("Mbs_Widget", "Slave IP", 0, QApplication::UnicodeUTF8));
        slave_ip_cb_->clear();
        slave_ip_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "127.0.0.1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "192.168.100.105", 0, QApplication::UnicodeUTF8)
         << QString()
        );
        ip_port_lbl_->setText(QApplication::translate("Mbs_Widget", "IP Port", 0, QApplication::UnicodeUTF8));
        ip_port_cb_->clear();
        ip_port_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "502", 0, QApplication::UnicodeUTF8)
        );
        timeout_lbl_->setText(QApplication::translate("Mbs_Widget", "Timeout", 0, QApplication::UnicodeUTF8));
        timeout_cb_->clear();
        timeout_cb_->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "5000 ms", 0, QApplication::UnicodeUTF8)
        );
        tcp_info_gb_1->setTitle(QApplication::translate("Mbs_Widget", "TCP Port Info", 0, QApplication::UnicodeUTF8));
        slave_ip_lbl_1->setText(QApplication::translate("Mbs_Widget", "Slave IP", 0, QApplication::UnicodeUTF8));
        slave_ip_cb_1->clear();
        slave_ip_cb_1->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "127.0.0.1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Mbs_Widget", "192.168.100.105", 0, QApplication::UnicodeUTF8)
         << QString()
        );
        ip_port_lbl_1->setText(QApplication::translate("Mbs_Widget", "IP Port", 0, QApplication::UnicodeUTF8));
        ip_port_cb_1->clear();
        ip_port_cb_1->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "502", 0, QApplication::UnicodeUTF8)
        );
        timeout_lbl_1->setText(QApplication::translate("Mbs_Widget", "Timeout", 0, QApplication::UnicodeUTF8));
        timeout_cb_1->clear();
        timeout_cb_1->insertItems(0, QStringList()
         << QApplication::translate("Mbs_Widget", "5000 ms", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class Mbs_Widget: public Ui_Mbs_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MBS_WIDGET_H
