#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // Connect Keypad Buttons
    connect(this->ui->zeroButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('0'); });
    connect(this->ui->oneButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('1'); });
    connect(this->ui->twoButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('2'); });
    connect(this->ui->threeButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('3'); });
    connect(this->ui->fourButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('4'); });
    connect(this->ui->fiveButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('5'); });
    connect(this->ui->sixButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('6'); });
    connect(this->ui->sevenButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('7'); });
    connect(this->ui->eightButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('8'); });
    connect(this->ui->nineButton, &QPushButton::clicked, this, [=]() { numericButtonClicked('9'); });
    // Connect Operation Buttons
    connect(this->ui->plusButton, &QPushButton::clicked, this, [=]() { operatorButtonClicked('+'); });
    connect(this->ui->minusButton, &QPushButton::clicked, this, [=]() { operatorButtonClicked('-'); });
    connect(this->ui->multiplyButton, &QPushButton::clicked, this, [=]() { operatorButtonClicked('*'); });
    connect(this->ui->divisionButton, &QPushButton::clicked, this, [=]() { operatorButtonClicked('/'); });
    // Connect Functor Buttons
    connect(this->ui->calcButton, &QPushButton::clicked, this, &Widget::calcButtonClicked);
    connect(this->ui->clearButton, &QPushButton::clicked, this, &Widget::clearButtonClicked);
    connect(this->ui->negateButton, &QPushButton::clicked, this, &Widget::negationButtonClicked);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::numericButtonClicked(char num) {
    inputBuffer += num;
    this->ui->result->setText(inputBuffer);
}


void Widget::clearButtonClicked() {
    // Need to clear all QLists
    inputBuffer = "";
    this->ui->result->setText("0");
}

void Widget::clearCache() {
    wholeNumbers.clear();
    floatingPointNumbers.clear();
    operations.clear();
}

void Widget::calcError(QString failureReason) {
    this->ui->result->setText("u/d");
    // Clear Input Buffer
    inputBuffer = "";
    // Clear all QLists
    clearCache();
    qDebug() << "Calculation Error: " << failureReason;
}

void Widget::operatorButtonClicked(char op) {
    // Convert Input Buffer to a number
    bool conversionSuccessful {false};
    int curNum {inputBuffer.toInt(&conversionSuccessful)};
    if (conversionSuccessful) {
        // Add new number to QList
        wholeNumbers.append(curNum);
        // Clear Input Buffer and set result text to operator on UI
        inputBuffer = "";
        this->ui->result->setText(QString(op));
        // Add operator to QList
        operations.append(op);
    }
    else {
        calcError("Widget::operatorButtonClicked => Number Conversion Failure");
    }
}

void Widget::negationButtonClicked() {
    // Switch relativity to 0 for current pending entry
    std::string tempInput {inputBuffer.toStdString()};
    if (tempInput.find("-") != std::string::npos) {
        // Number is already negative
        tempInput = tempInput.erase(0, 1);
    }
    else {
        tempInput = "-" + tempInput;
    }
    inputBuffer = QString::fromStdString(tempInput);
    this->ui->result->setText(inputBuffer);
}

void Widget::calcButtonClicked() {
    // Convert Input Buffer to a number
    bool conversionSuccessful {false};
    int curNum {inputBuffer.toInt(&conversionSuccessful)};
    if (conversionSuccessful) {
        // Add new number to QList
        wholeNumbers.append(curNum);
        // Clear Input Buffer and set result text to operator on UI
        inputBuffer = "";
    }
    else {
        calcError("Widget::operatorButtonClicked => Number Conversion Failure");
    }
    // Validate all input
    if (wholeNumbers.size() == (operations.size() + 1)) {
        // Process Equation
        int finalAnswer {0};
        // Need to consider OOP Here
        // Check if any Multiplication or Division occurs
        QList<int> priorityOperations;
        QList<int> priorityResults;
        if (operations.indexOf('/') != -1 || operations.indexOf('*') != -1) {
            for (int i {0}; i < operations.length(); ++i) {
                if ((operations[i] == '/' || operations[i] == '*')) {
                    priorityOperations.append(i);
                }
            }
        }

        // 8 + 6 * 2 == 20 but we printed 24


        // Process Priority Calculations
        for (int i {0}; i < priorityOperations.length(); ++i) {
            int opIndex {priorityOperations[i]};
            char operation {operations[opIndex]};
            // 1 + 2 - 3 * 5
            // [1 2 3 5]
            // [+ - *]
            // 0+ == (0, 1)
            // 1- == (1, 2)
            // 2* == (2, 3)
            // FirstNum == (opIndex * 2)
            // SecondNum == (FirstNum + 1)
            int firstNum {wholeNumbers[opIndex]};
            int secNum {wholeNumbers[(opIndex + 1)]};
            if (DEBUG) {
                qDebug() << "Processing Priority: " << firstNum << " " << operation << " " << secNum;
            }
            switch (operation) {
                case '*':
                    priorityResults.append(firstNum * secNum);
                    if (DEBUG) {
                        qDebug() << "Evaluated to: " << (firstNum * secNum);
                    }
                    break;
                case '/':
                    priorityResults.append(firstNum / secNum);
                    if (DEBUG) {
                        qDebug() << "Evaluated to: " << (firstNum / secNum);
                    }
                    break;
            }
        }
        if (DEBUG) {
            qDebug() << "Identified the following priority operations by index!";
            for (int i {0}; i < priorityOperations.length(); ++i) {
                qDebug() << priorityOperations[i];
            }
        }
        // Process Remaining Calculations checking for priority operators and subbing in the result as secNum
        for (int i {0}; i < operations.length(); ++i) {
            if (DEBUG) {
                qDebug() << "****************************** Op " << i << " *******************************";
            }
            if (priorityOperations.indexOf(i) != -1 && i == 0) {
                if (DEBUG) {
                    qDebug() << "This is Priority! Prior to evaluation, finalAns == " << finalAnswer << " Going to add " << priorityResults[priorityOperations.indexOf(i)];
                }
                // This is a priority operation
                // Only addition or subtraction so process equation (pos or neg) and add to entire answer
                finalAnswer += priorityResults[priorityOperations.indexOf(i)];
                if (DEBUG) {
                    qDebug() << "PRIORITY FIRST: Post-Eval, finalAns == " << finalAnswer;
                }
            }
            else if (priorityOperations.indexOf((i + 1)) != -1) {
                if (i == 0) {
                    finalAnswer += wholeNumbers[0];
                }
                // Next Operation is priority
                char operation {operations[i]};
                int secondNum {priorityResults[priorityOperations.indexOf(i + 1)]};
                switch (operation) {
                    case '+':
                        finalAnswer += secondNum;
                        break;
                    case '-':
                        finalAnswer -= secondNum;
                        break;
                }
            }
            else if (priorityOperations.indexOf(i) == -1) {
                if (i == 0) {
                    if (DEBUG) {
                        qDebug() << "This is first operation so finalAnswer will be: " << wholeNumbers[0];
                    }
                    finalAnswer += wholeNumbers[0];
                }
                // Process Operation normally
                char operation {operations[i]};
                // 1 + 2 - (3 * 5)
                // [1 2 3 5]
                // [+ - *]
                // 0+ == (1)
                // 1- == (2)
                // 2* == (3)
                int secondNum {wholeNumbers[(i + 1)]};
                if (DEBUG) {
                    qDebug() << "NO UPCOMING PRIORITY!: Going to process as follows: " << finalAnswer << " " << operation << " " << secondNum;
                }
                switch (operation) {
                    case '+':
                        finalAnswer += secondNum;
                        break;
                    case '-':
                        finalAnswer -= secondNum;
                        break;
                }
            }
        }
        // Clear all QLists
        clearCache();
        // Update result on UI
        this->ui->result->setText(QString::number(finalAnswer));
    }
    else {
        qDebug() << "Whole Numbers Received: " << wholeNumbers.length();
        qDebug() << "Operators Received: " << operations.length();
        calcError("Widget::calcButtonClicked => Whole Number to operator ratio mismatch");
    }
}
