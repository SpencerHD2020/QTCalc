#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
class Calculator: public QWidget
{
    Q_OBJECT
    public:
        /*
        struct UiCompSize {
            int hPos;
            int vPos;
            int hSize;
            int vSize;
        };
        enum UiCompSizeEnum {
            H_POS, V_POS, H_SIZE, V_SIZE
        };
        */
        explicit Calculator(QWidget *parent = nullptr);
        /*
        // ******************************************** Results Frame **********************************************************
        QLabel *resultsFrame = new QLabel(this);
        UiCompSize resFrameSize;
        void resizeResultsFrame(int size, int param);
        // *********************************************************************************************************************
        int buttonVSize {60};
        // ******************************************** AC Button **************************************************************
        QPushButton *clearButton = new QPushButton(this);
        UiCompSize clearBFrameSize;
        void resizeClearButton(int size, int param);
        // *********************************************************************************************************************
        */
    private:
        //QSize sizeHint() const;
        //void resizeEvent(QResizeEvent *e);
};

#endif // CALCULATOR_H
