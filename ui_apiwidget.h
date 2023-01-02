/********************************************************************************
** Form generated from reading UI file 'apiwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APIWIDGET_H
#define UI_APIWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_APIWidget
{
public:
    QTextEdit *textEdit_appid;
    QTextEdit *textEdit_key;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QLabel *label_3;

    void setupUi(QWidget *APIWidget)
    {
        if (APIWidget->objectName().isEmpty())
            APIWidget->setObjectName(QString::fromUtf8("APIWidget"));
        APIWidget->resize(461, 348);
        textEdit_appid = new QTextEdit(APIWidget);
        textEdit_appid->setObjectName(QString::fromUtf8("textEdit_appid"));
        textEdit_appid->setGeometry(QRect(130, 140, 231, 31));
        textEdit_key = new QTextEdit(APIWidget);
        textEdit_key->setObjectName(QString::fromUtf8("textEdit_key"));
        textEdit_key->setGeometry(QRect(130, 210, 231, 31));
        label = new QLabel(APIWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(80, 130, 51, 51));
        label_2 = new QLabel(APIWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(90, 210, 31, 31));
        pushButton = new QPushButton(APIWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(190, 290, 80, 20));
        label_3 = new QLabel(APIWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(130, 30, 221, 71));

        retranslateUi(APIWidget);

        QMetaObject::connectSlotsByName(APIWidget);
    } // setupUi

    void retranslateUi(QWidget *APIWidget)
    {
        APIWidget->setWindowTitle(QCoreApplication::translate("APIWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("APIWidget", "APPID:", nullptr));
        label_2->setText(QCoreApplication::translate("APIWidget", "KEY:", nullptr));
        pushButton->setText(QCoreApplication::translate("APIWidget", "\347\241\256\345\256\232", nullptr));
        label_3->setText(QCoreApplication::translate("APIWidget", "\347\231\276\345\272\246\351\200\232\347\224\250\347\277\273\350\257\221API\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class APIWidget: public Ui_APIWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APIWIDGET_H
