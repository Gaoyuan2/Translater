#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();



signals:

private slots:
    //在使用按键send后发生的一系列处理
    void on_pushButton_send_clicked();
    //更新输出框中的结果
    void updateResult();

private:
    Ui::Widget *ui;
    QString m_transResult;    // 翻译结果

};
#endif // WIDGET_H
