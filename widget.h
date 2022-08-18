#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QDebug>
#include <QList>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QList<int> wholeNumbers;
    QList<double> floatingPointNumbers;
    QList<char> operations;
    QString inputBuffer;
private:
    Ui::Widget *ui;
    void clearCache();
    void calcError(QString failureReason="Fatal Error");
    bool DEBUG {true};
public slots:
    void numericButtonClicked(char num);
    void calcButtonClicked();
    void clearButtonClicked();
    void operatorButtonClicked(char op);
    void negationButtonClicked();
};
#endif // WIDGET_H
