#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
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
    QList<double> floatingPointNumbers;
    QList<char> operations;
    QString inputBuffer;
    struct CastDouble {
        double num;
        bool safe;
        CastDouble(double d, bool b) {
          this->num = d;
          this->safe = b;
        };
    };
private:
    Ui::Widget *ui;
    void clearCache();
    void calcError(QString failureReason="Fatal Error");
    bool DEBUG {true};
    bool floatingPointValuesIncluded {false};
    void updateUI();
    CastDouble qStringToDouble(QString ref);

public slots:
    void numericButtonClicked(char num);
    void calcButtonClicked();
    void clearButtonClicked();
    void operatorButtonClicked(char op);
    void negationButtonClicked();
    void decimalButtonClicked();
    void percentageButtonClicked();

};
#endif // WIDGET_H
