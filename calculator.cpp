#include "calculator.h"

Calculator::Calculator(QWidget* parent) : QWidget{parent}
{
    // First Arg seems to be Horizontal axis
    // Second Arg seems to be vertical axis
    // Third arg seems relative to size horizontally
    // I imagine that the last is vertical size
    /*
    // ******************************************** Results Frame **********************************************************
    resFrameSize.hPos = 0;
    resFrameSize.vPos = 0;
    resFrameSize.hSize = 500;
    resFrameSize.vSize = 40;
    resultsFrame->setGeometry(resFrameSize.hPos, resFrameSize.vPos, resFrameSize.hSize, resFrameSize.vSize);
    QPalette ResultsFramePallete;
    ResultsFramePallete.setColor(QPalette::Window, Qt::gray);
    resultsFrame->setAutoFillBackground(true);
    resultsFrame->setPalette(ResultsFramePallete);
    resultsFrame->show();
    // *********************************************************************************************************************
    // First Row of buttons - AC  +/-  %  /
    // ******************************************** AC Button **************************************************************
    clearBFrameSize.hPos = 0;
    clearBFrameSize.vPos = (resFrameSize.vPos + 1);
    clearBFrameSize.hSize = (resFrameSize.hSize / 4);
    clearBFrameSize.vSize = buttonVSize;
    //clearButton->setGeometry(clearBFrameSize.hPos)
    // *********************************************************************************************************************
    */

}
/*
void Calculator::resizeResultsFrame(int size, int param) {
    switch (param) {
        case H_POS:
            resFrameSize.hPos = size;
            break;
        case V_POS:
            resFrameSize.vPos = size;
            break;
        case H_SIZE:
            resFrameSize.hSize = size;
            break;
        case V_SIZE:
            resFrameSize.vSize = size;
            break;
    }
    resultsFrame->setGeometry(resFrameSize.hPos, resFrameSize.vPos, resFrameSize.hSize, resFrameSize.vSize);
};

// Handle Window Resizing
void Calculator::resizeEvent(QResizeEvent *e) {
    resizeResultsFrame(this->size().rwidth(), H_SIZE);
};

// Specify starting window size for Widget
QSize Calculator::sizeHint() const
{
    return QSize(500, 500); // Width, Height
}
*/
