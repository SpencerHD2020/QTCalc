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
    connect(this->ui->decimalButton, &QPushButton::clicked, this, &Widget::decimalButtonClicked);
    connect(this->ui->percentageButton, &QPushButton::clicked, this, &Widget::percentageButtonClicked);

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
    clearCache();
}

void Widget::clearCache() {
    floatingPointNumbers.clear();
    operations.clear();
    inputBuffer = "";
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
    Widget::CastDouble castNum {qStringToDouble(inputBuffer)};
    if (castNum.safe) {
        floatingPointNumbers.append(castNum.num);
        inputBuffer = "";
        // Add operator to QList
        operations.append(op);
        this->ui->result->setText(QString(op));
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
    updateUI();
}

void Widget::decimalButtonClicked() {
    inputBuffer += ".";
    updateUI();

}

void Widget::percentageButtonClicked() {
    // Just need to divide the input by 100
    Widget::CastDouble castNum {qStringToDouble(inputBuffer)};
    if (castNum.safe) {
        double newNum {castNum.num / 100};
        inputBuffer = QString::number(newNum);
        updateUI();
    }
    else {
        calcError("Widget::percentageButtonClicked => Number Conversion Failure");
    }
}

Widget::CastDouble Widget::qStringToDouble(QString ref) {
    bool converted {false};
    double finalNum {ref.toDouble(&converted)};
    return Widget::CastDouble(finalNum, converted);
}

void Widget::updateUI() {
    // Make look like an int if whole number
    // Will always have a decimal, need to cast to double to see, also ensure not operator
    // If operator button was just clicked there will be just as many numbers as operators
    if (floatingPointNumbers.size() == operations.size()) {
        // Update is showing operator
        this->ui->result->setText(inputBuffer);
        return;
    }
    else {
        // Update is a number
        bool isInteger {true};
        std::string tempString {inputBuffer.toStdString()};
        if (tempString.find(".") != std::string::npos) {
            tempString.erase(0, tempString.find("."));
            for (int i {0}; i < tempString.length(); ++i) {
                char curChar {tempString[i]};
                if (curChar != '0') {
                    isInteger = false;
                    break;
                }
            }
            if (isInteger) {
                std::string intString {inputBuffer.toStdString()};
                intString.erase(intString.find("."));
                this->ui->result->setText(QString::fromStdString(intString));
                return;
            }
        }
        this->ui->result->setText(inputBuffer);
    }

}

void Widget::calcButtonClicked() {
    // Convert Input Buffer to a number
    Widget::CastDouble castNum {qStringToDouble(inputBuffer)};
    if (castNum.safe) {
        floatingPointNumbers.append(castNum.num);
        inputBuffer = "";
    }
    else {
        calcError("Widget::operatorButtonClicked => Number Conversion Failure");
    }
    // Validate all input
    if (floatingPointNumbers.size() == (operations.size() + 1)) {
        // Process equation
        double finalAnswer {0.0};
        QList<int> priorityOperations;
        QList<double> priorityResults;
        if (operations.indexOf('/') != -1 || operations.indexOf('*') != -1) {
            for (int i {0}; i < operations.length(); ++i) {
                if ((operations[i] == '/' || operations[i] == '*')) {
                    priorityOperations.append(i);
                }
            }
        }
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
            double firstNum {floatingPointNumbers[opIndex]};
            double secNum {floatingPointNumbers[(opIndex + 1)]};
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
                    finalAnswer += floatingPointNumbers[0];
                }
                // Next Operation is priority
                char operation {operations[i]};
                double secondNum {priorityResults[priorityOperations.indexOf(i + 1)]};
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
                        qDebug() << "This is first operation so finalAnswer will be: " << floatingPointNumbers[0];
                    }
                    finalAnswer += floatingPointNumbers[0];
                }
                // Process Operation normally
                char operation {operations[i]};
                // 1 + 2 - (3 * 5)
                // [1 2 3 5]
                // [+ - *]
                // 0+ == (1)
                // 1- == (2)
                // 2* == (3)
                double secondNum {floatingPointNumbers[(i + 1)]};
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
        // Update result on UI
        inputBuffer = QString::number(finalAnswer);
        updateUI();
    }
    else {
        qDebug() << "Floating Point Numbers Received: " << floatingPointNumbers.length();
        qDebug() << "Operators Received: " << operations.length();
        calcError("Widget::calcButtonClicked => Floating Point Number to operator ratio mismatch");
    }
    // Clear all QLists
    clearCache();
}
