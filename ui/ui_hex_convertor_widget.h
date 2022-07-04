/********************************************************************************
** Form generated from reading UI file 'hex_convertor_widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEX_CONVERTOR_WIDGET_H
#define UI_HEX_CONVERTOR_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_hex_convertor_widget
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *hex_lbl_;
    QLineEdit *hex_txt_;
    QComboBox *type_cb_;
    QPushButton *to_hex_btn_;
    QLabel *number_lbl_;
    QLineEdit *number_txt_;
    QPushButton *to_number_btn_;

    void setupUi(QWidget *hex_convertor_widget)
    {
        if (hex_convertor_widget->objectName().isEmpty())
            hex_convertor_widget->setObjectName(QString::fromUtf8("hex_convertor_widget"));
        hex_convertor_widget->resize(494, 85);
        hex_convertor_widget->setMinimumSize(QSize(494, 85));
        layoutWidget = new QWidget(hex_convertor_widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 471, 61));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        hex_lbl_ = new QLabel(layoutWidget);
        hex_lbl_->setObjectName(QString::fromUtf8("hex_lbl_"));

        gridLayout->addWidget(hex_lbl_, 0, 0, 1, 1);

        hex_txt_ = new QLineEdit(layoutWidget);
        hex_txt_->setObjectName(QString::fromUtf8("hex_txt_"));

        gridLayout->addWidget(hex_txt_, 0, 1, 1, 1);

        type_cb_ = new QComboBox(layoutWidget);
        type_cb_->setObjectName(QString::fromUtf8("type_cb_"));
        type_cb_->setEditable(false);

        gridLayout->addWidget(type_cb_, 0, 2, 1, 1);

        to_hex_btn_ = new QPushButton(layoutWidget);
        to_hex_btn_->setObjectName(QString::fromUtf8("to_hex_btn_"));

        gridLayout->addWidget(to_hex_btn_, 0, 3, 1, 1);

        number_lbl_ = new QLabel(layoutWidget);
        number_lbl_->setObjectName(QString::fromUtf8("number_lbl_"));

        gridLayout->addWidget(number_lbl_, 1, 0, 1, 1);

        number_txt_ = new QLineEdit(layoutWidget);
        number_txt_->setObjectName(QString::fromUtf8("number_txt_"));

        gridLayout->addWidget(number_txt_, 1, 1, 1, 1);

        to_number_btn_ = new QPushButton(layoutWidget);
        to_number_btn_->setObjectName(QString::fromUtf8("to_number_btn_"));

        gridLayout->addWidget(to_number_btn_, 1, 3, 1, 1);


        retranslateUi(hex_convertor_widget);

        QMetaObject::connectSlotsByName(hex_convertor_widget);
    } // setupUi

    void retranslateUi(QWidget *hex_convertor_widget)
    {
        hex_convertor_widget->setWindowTitle(QApplication::translate("hex_convertor_widget", "HEX Convertor", 0, QApplication::UnicodeUTF8));
        hex_lbl_->setText(QApplication::translate("hex_convertor_widget", "HEX", 0, QApplication::UnicodeUTF8));
        type_cb_->clear();
        type_cb_->insertItems(0, QStringList()
         << QApplication::translate("hex_convertor_widget", "UINT32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("hex_convertor_widget", "INT32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("hex_convertor_widget", "FLOAT32", 0, QApplication::UnicodeUTF8)
        );
        to_hex_btn_->setText(QApplication::translate("hex_convertor_widget", "To HEX", 0, QApplication::UnicodeUTF8));
        number_lbl_->setText(QApplication::translate("hex_convertor_widget", "Number", 0, QApplication::UnicodeUTF8));
        to_number_btn_->setText(QApplication::translate("hex_convertor_widget", "To Number", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class hex_convertor_widget: public Ui_hex_convertor_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEX_CONVERTOR_WIDGET_H
