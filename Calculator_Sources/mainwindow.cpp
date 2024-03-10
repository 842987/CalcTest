#include "mainwindow.h"
#include "ui_menuwidget.h"
#include <fstream>
#include <cmath>

MainWindow::MainWindow(QApplication *application, QWidget *parent)
    : QMainWindow(parent)
{
    this->application = application;

    //window
    this->setWindowTitle("Калькулятор 3.4");
    this->setGeometry(500,250,370,480);
    //sizepolicy
    sizePolicy = new QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //fonts
    fontUbuntu = new QFont("Ubuntu", 15);

    //widgets

    //виджет, с помощью которого можно переключаться между меню и центральным виджетом
    stackedWidgetCentral = new QStackedWidget(this);
    this->setCentralWidget(stackedWidgetCentral); //этот виджет центральный

    //меню
    menuWidget = new MenuWidget(this);
    stackedWidgetCentral->addWidget(menuWidget);
    menuWidget->ui->comboBoxStyles->addItems(QStyleFactory::keys());
    //подключение виджетов из MenuWidget к слотам
    //checkBox сигналы
    connect(menuWidget->ui->groupBoxHistoryPanel, &QGroupBox::clicked, this, &MainWindow::on_MenuWidgetCheckBox_changed);
    connect(menuWidget->ui->checkBoxUnpinHistoryPanel, &QCheckBox::clicked, this, &MainWindow::on_MenuWidgetCheckBox_changed);
    connect(menuWidget->ui->groupBoxNotesPanel, &QGroupBox::clicked, this, &MainWindow::on_MenuWidgetCheckBox_changed);
    connect(menuWidget->ui->checkBoxUnpinNotesPanel, &QCheckBox::clicked, this, &MainWindow::on_MenuWidgetCheckBox_changed);
    connect(menuWidget->ui->checkBoxWrapNotesPanel, &QCheckBox::clicked, this, &MainWindow::on_MenuWidgetCheckBox_changed);
    //при клике на кнопку "назад"
    connect(menuWidget->ui->pushButtonGoBack, &QPushButton::clicked, this, &MainWindow::on_MenuWidgetPushButtonGoBack_clicked);
    //при изменении действия кнопки "<-" в comboBox
    connect(menuWidget->ui->comboBoxChooseButtonDeleteAction, &QComboBox::currentTextChanged, this, &MainWindow::on_buttonDeleteAction_changed);
    //при изменении шрифта
    connect(menuWidget->ui->fontComboBoxFonts, &QFontComboBox::currentFontChanged, this, &MainWindow::on_MenuWidgetFontComboBoxFonts_currentFontChanged);
    //при изменении стиля
    connect(menuWidget->ui->comboBoxStyles, &QComboBox::currentTextChanged, this, &MainWindow::on_MenuWidgetComboBoxStyles_currentTextChanged);

    //layouts
    gridLayoutCentral = new QGridLayout();

    /*cental widget, этот виджет не центральный для MainWindow, но на нём распологаются сразу 3 других виджета, поэтому
    / он всё равно центральный */
    widgetCentral = new QWidget(stackedWidgetCentral);
    stackedWidgetCentral->addWidget(widgetCentral); // добавление в stackedWidgetCentral
    stackedWidgetCentral->setCurrentWidget(widgetCentral); // при запуске калькулятора сначала будет виден этот виджет, а не меню
    widgetCentral->setLayout(gridLayoutCentral);
    widgetCentral->setFont(*fontUbuntu);

    //panels

    //HistoryPanel, который как отдельное окно
    listWidgetHistoryPanel = new QListWidget();
    listWidgetHistoryPanel->setWindowTitle("Панель истории вычислений");
    listWidgetHistoryPanel->setVisible(false);
    listWidgetHistoryPanel->setFont(*fontUbuntu);
    connect(listWidgetHistoryPanel, &QListWidget::itemClicked, this, &MainWindow::on_listWidgetHistoryPanelOnMainWindow_clicked);
    //HistoryPanel on MainWindow
    listWidgetHistoryPanelOnMainWindow = new QListWidget(widgetCentral);
    listWidgetHistoryPanelOnMainWindow->setSizePolicy(*sizePolicy);
    gridLayoutCentral->addWidget(listWidgetHistoryPanelOnMainWindow, 0, 0);
    listWidgetHistoryPanelOnMainWindow->setVisible(false);
    connect(listWidgetHistoryPanelOnMainWindow, &QListWidget::itemClicked, this, &MainWindow::on_listWidgetHistoryPanelOnMainWindow_clicked);
    //NotesPanel, который как отдельное окно
    textEditNotesPanel = new QTextEdit();
    textEditNotesPanel->setWindowTitle("Панель заметок");
    textEditNotesPanel->setVisible(false);
    textEditNotesPanel->setWordWrapMode(QTextOption::WrapAnywhere); //изначально переносить строки
    textEditNotesPanel->setFont(*fontUbuntu);
    //NotesPanel on MainWindow
    textEditNotesPanelOnMainWindow = new QTextEdit(widgetCentral);
    textEditNotesPanelOnMainWindow->setSizePolicy(*sizePolicy);
    gridLayoutCentral->addWidget(textEditNotesPanelOnMainWindow, 0, 2);
    textEditNotesPanelOnMainWindow->setVisible(false);
    textEditNotesPanelOnMainWindow->setWordWrapMode(QTextOption::WrapAnywhere); //изначально переносить строки

    //widgetCalculatePanel
    gridLayoutButtons = new QGridLayout();
    gridLayoutButtons->setHorizontalSpacing(3);
    gridLayoutButtons->setVerticalSpacing(3);
    gridLayoutCentral->addLayout(gridLayoutButtons, 0, 1);

    lineEditNumbers = new QLineEdit(widgetCentral);
    gridLayoutButtons->addWidget(lineEditNumbers, 0, 0, 1, 4);
    lineEditNumbers->setSizePolicy(*sizePolicy);
    lineEditNumbers->setReadOnly(true);

    //buttons
    //numbers
    pushButton0 = new QPushButton("0", widgetCentral);
    gridLayoutButtons->addWidget(pushButton0, 5, 1); //добавление в layout, чтобы растягивалась кнопка
    pushButton0->setObjectName("pushButton0"); //имя объекта, чтобы можно было не делать много функций, а проверить это имя в одной функции
    pushButton0->setSizePolicy(*sizePolicy); //чтобы кнопка растягивалась по всему доступному пространству
    connect(pushButton0, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked())); //подключение к функции on_pushButtonNumbers_clicked()
    pushButton0->setShortcut(QKeySequence("0"));

    pushButton1 = new QPushButton("1", widgetCentral);
    gridLayoutButtons->addWidget(pushButton1, 4, 0);
    pushButton1->setObjectName("pushButton1");
    pushButton1->setSizePolicy(*sizePolicy);
    connect(pushButton1, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton1->setShortcut(QKeySequence("1"));

    pushButton2 = new QPushButton("2", widgetCentral);
    gridLayoutButtons->addWidget(pushButton2, 4, 1);
    pushButton2->setObjectName("pushButton2");
    pushButton2->setSizePolicy(*sizePolicy);
    connect(pushButton2, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton2->setShortcut(QKeySequence("2"));

    pushButton3 = new QPushButton("3", widgetCentral);
    gridLayoutButtons->addWidget(pushButton3, 4, 2);
    pushButton3->setObjectName("pushButton3");
    pushButton3->setSizePolicy(*sizePolicy);
    connect(pushButton3, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton3->setShortcut(QKeySequence("3"));

    pushButton4 = new QPushButton("4", widgetCentral);
    gridLayoutButtons->addWidget(pushButton4, 3, 0);
    pushButton4->setObjectName("pushButton4");
    pushButton4->setSizePolicy(*sizePolicy);
    connect(pushButton4, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton4->setShortcut(QKeySequence("4"));

    pushButton5 = new QPushButton("5", widgetCentral);
    gridLayoutButtons->addWidget(pushButton5, 3, 1);
    pushButton5->setObjectName("pushButton5");
    pushButton5->setSizePolicy(*sizePolicy);
    connect(pushButton5, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton5->setShortcut(QKeySequence("5"));

    pushButton6 = new QPushButton("6", widgetCentral);
    gridLayoutButtons->addWidget(pushButton6, 3, 2);
    pushButton6->setObjectName("pushButton6");
    pushButton6->setSizePolicy(*sizePolicy);
    connect(pushButton6, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton6->setShortcut(QKeySequence("6"));

    pushButton7 = new QPushButton("7", widgetCentral);
    gridLayoutButtons->addWidget(pushButton7, 2, 0);
    pushButton7->setObjectName("pushButton7");
    pushButton7->setSizePolicy(*sizePolicy);
    connect(pushButton7, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton7->setShortcut(QKeySequence("7"));

    pushButton8 = new QPushButton("8", widgetCentral);
    gridLayoutButtons->addWidget(pushButton8, 2, 1);
    pushButton8->setObjectName("pushButton8");
    pushButton8->setSizePolicy(*sizePolicy);
    connect(pushButton8, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton8->setShortcut(QKeySequence("8"));

    pushButton9 = new QPushButton("9", widgetCentral);
    gridLayoutButtons->addWidget(pushButton9, 2, 2);
    pushButton9->setObjectName("pushButton9");
    pushButton9->setSizePolicy(*sizePolicy);
    connect(pushButton9, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButton9->setShortcut(QKeySequence("9"));

    //arifmetic operations
    pushButtonMod = new QPushButton("mod", widgetCentral);
    pushButtonMod->setToolTip("Деление без остатка [%]");
    gridLayoutButtons->addWidget(pushButtonMod, 1, 0);
    pushButtonMod->setObjectName("pushButtonMod");
    pushButtonMod->setSizePolicy(*sizePolicy);
    connect(pushButtonMod, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonMod->setShortcut(QKeySequence("%"));

    pushButtonSqrt = new QPushButton("√", widgetCentral);
    pushButtonSqrt->setToolTip("Корень [ctrl+r]");
    gridLayoutButtons->addWidget(pushButtonSqrt, 1, 1);
    pushButtonSqrt->setObjectName("pushButtonSqrt");
    pushButtonSqrt->setSizePolicy(*sizePolicy);
    connect(pushButtonSqrt, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonSqrt->setShortcut(QKeySequence("ctrl+r"));

    pushButtonDivision = new QPushButton("/", widgetCentral);
    gridLayoutButtons->addWidget(pushButtonDivision, 1, 3);
    pushButtonDivision->setObjectName("pushButtonDivision");
    pushButtonDivision->setSizePolicy(*sizePolicy);
    connect(pushButtonDivision, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonDivision->setShortcut(QKeySequence("/"));

    pushButtonMultiplication = new QPushButton("*", widgetCentral);
    gridLayoutButtons->addWidget(pushButtonMultiplication, 2, 3);
    pushButtonMultiplication->setObjectName("pushButtonMultiplication");
    pushButtonMultiplication->setSizePolicy(*sizePolicy);
    connect(pushButtonMultiplication, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonMultiplication->setShortcut(QKeySequence("*"));

    pushButtonPlus = new QPushButton("+", widgetCentral);
    gridLayoutButtons->addWidget(pushButtonPlus, 3, 3);
    pushButtonPlus->setObjectName("pushButtonPlus");
    pushButtonPlus->setSizePolicy(*sizePolicy);
    connect(pushButtonPlus, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonPlus->setShortcut(QKeySequence("+"));

    pushButtonMinus = new QPushButton("-", widgetCentral);
    gridLayoutButtons->addWidget(pushButtonMinus, 4, 3);
    pushButtonMinus->setObjectName("pushButtonMinus");
    pushButtonMinus->setSizePolicy(*sizePolicy);
    connect(pushButtonMinus, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonMinus->setShortcut(QKeySequence("-"));

    //calculator operations
    pushButtonDelete = new QPushButton("<-", widgetCentral);
    gridLayoutButtons->addWidget(pushButtonDelete, 1, 2);
    pushButtonDelete->setObjectName("pushButtonDelete");
    pushButtonDelete->setSizePolicy(*sizePolicy);
    connect(pushButtonDelete, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonDelete->setShortcut(QKeySequence("Backspace"));

    shortCutClear = new QShortcut(QKeySequence("ctrl+Backspace"), widgetCentral);
    connect(shortCutClear, &QShortcut::activated, this, &MainWindow::on_shortcutClear_activated);

    pushButtonCalculate = new QPushButton("=", widgetCentral);
    gridLayoutButtons->addWidget(pushButtonCalculate, 5, 2, 1, 2);
    pushButtonCalculate->setObjectName("pushButtonCalculate");
    pushButtonCalculate->setSizePolicy(*sizePolicy);
    connect(pushButtonCalculate, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonCalculate->setShortcut(QKeySequence("Return"));

    pushButtonMenu = new QPushButton("☰", widgetCentral);
    pushButtonMenu->setToolTip("Настройки [ctrl+m]");
    gridLayoutButtons->addWidget(pushButtonMenu, 5, 0);
    pushButtonMenu->setObjectName("pushButtonMenu");
    pushButtonMenu->setSizePolicy(*sizePolicy);
    connect(pushButtonMenu, SIGNAL(clicked()), this, SLOT(on_pushButtonNumbers_clicked()));
    pushButtonMenu->setShortcut(QKeySequence("ctrl+m"));
}

MainWindow::~MainWindow()
{
}

//при нажатии на элемент в истории вычислений
void MainWindow::on_listWidgetHistoryPanelOnMainWindow_clicked(QListWidgetItem *item) {
    lineEditNumbers->setText(item->text().mid(item->text().indexOf("=")+1));
}

//при изменении действия кнопки "<-" в comboBox в MenuWidget
void MainWindow::on_buttonDeleteAction_changed(const QString &text)
{
    if(text == "Очищает строку ввода") doesButtonDeleteClear = true;
    else if(text == "Стирает один символ") doesButtonDeleteClear = false;
}

//при изменении стиля в MenuWidget
void MainWindow::on_MenuWidgetComboBoxStyles_currentTextChanged(const QString &text)
{
    application->setStyle(QStyleFactory::create(text));
}

//при изменении шрифта в MenuWidget
void MainWindow::on_MenuWidgetFontComboBoxFonts_currentFontChanged(const QFont &font)
{
    application->setFont(font);
}

void MainWindow::on_shortcutClear_activated()
{
    lineEditNumbers->setText("");
}

//при изменении checkBox в MenuWidget
void MainWindow::on_MenuWidgetCheckBox_changed(bool is_activated)
{
    QString objectName = sender()->objectName();
    if(objectName == "groupBoxHistoryPanel") {
        if(is_activated) {
            listWidgetHistoryPanel->setVisible(menuWidget->ui->checkBoxUnpinHistoryPanel->isChecked());
            listWidgetHistoryPanelOnMainWindow->setVisible(!menuWidget->ui->checkBoxUnpinHistoryPanel->isChecked());
            listWidgetHistoryPanel->setGeometry(this->x()-this->width(),this->y(), this->width(), this->height());
        } else {
            listWidgetHistoryPanel->setVisible(false);
            listWidgetHistoryPanelOnMainWindow->setVisible(false);
        }
    } else if(objectName == "checkBoxUnpinHistoryPanel"){
        if(is_activated) {
            listWidgetHistoryPanel->setVisible(true);
            listWidgetHistoryPanel->setGeometry(this->x()-this->width(),this->y(), this->width(), this->height());
        } else {
            listWidgetHistoryPanel->setVisible(false);
        }
        listWidgetHistoryPanelOnMainWindow->setVisible(!is_activated);
    } else if(objectName == "groupBoxNotesPanel") {
        if(is_activated) {
            textEditNotesPanel->setVisible(menuWidget->ui->checkBoxUnpinNotesPanel->isChecked());
            textEditNotesPanelOnMainWindow->setVisible(!menuWidget->ui->checkBoxUnpinNotesPanel->isChecked());
            textEditNotesPanel->setGeometry(this->x()+this->width(),this->y(), this->width(), this->height());
        } else {
            textEditNotesPanel->setVisible(false);
            textEditNotesPanelOnMainWindow->setVisible(false);
        }
    } else if(objectName == "checkBoxUnpinNotesPanel") {
        if(textEditNotesPanel->isVisible() == true) {
            textEditNotesPanelOnMainWindow->setText(textEditNotesPanel->toPlainText());
        } else {
            textEditNotesPanel->setText(textEditNotesPanelOnMainWindow->toPlainText());
        }
        if(is_activated) {
            textEditNotesPanel->setVisible(true);
            textEditNotesPanel->setGeometry(this->x()+this->width(),this->y(), this->width(), this->height());
        } else {
            textEditNotesPanel->setVisible(false);
        }
        textEditNotesPanelOnMainWindow->setVisible(!is_activated);
    } else if(objectName == "checkBoxWrapNotesPanel") {
        if(is_activated) {
            textEditNotesPanel->setWordWrapMode(QTextOption::WrapAnywhere);
            textEditNotesPanelOnMainWindow->setWordWrapMode(QTextOption::WrapAnywhere);
        } else {
            textEditNotesPanel->setWordWrapMode(QTextOption::NoWrap);
            textEditNotesPanelOnMainWindow->setWordWrapMode(QTextOption::NoWrap);
        }
    }
}

//после нажатия на кнопку "назад"
void MainWindow::on_MenuWidgetPushButtonGoBack_clicked()
{
    stackedWidgetCentral->setCurrentWidget(widgetCentral);
}

//после нажатия кнопки калькулятора
void MainWindow::on_pushButtonNumbers_clicked() {
    QString senderObjectName = sender()->objectName(); // имя отправителя сигнала
    auto add_symbol = [this](QString text) -> void {lineEditNumbers->setText(lineEditNumbers->text()+text);};
    if(senderObjectName == "pushButton0") add_symbol("0");
    else if(senderObjectName == "pushButton1") add_symbol("1");
    else if(senderObjectName == "pushButton2") add_symbol("2");
    else if(senderObjectName == "pushButton3") add_symbol("3");
    else if(senderObjectName == "pushButton4") add_symbol("4");
    else if(senderObjectName == "pushButton5") add_symbol("5");
    else if(senderObjectName == "pushButton6") add_symbol("6");
    else if(senderObjectName == "pushButton7") add_symbol("7");
    else if(senderObjectName == "pushButton8") add_symbol("8");
    else if(senderObjectName == "pushButton9") add_symbol("9");
    else if(senderObjectName == "pushButtonMod") add_symbol("%");
    else if(senderObjectName == "pushButtonSqrt") add_symbol("√");
    else if(senderObjectName == "pushButtonDivision") add_symbol("/");
    else if(senderObjectName == "pushButtonMultiplication") add_symbol("*");
    else if(senderObjectName == "pushButtonMinus") add_symbol("-");
    else if(senderObjectName == "pushButtonPlus") add_symbol("+");
    else if(senderObjectName == "pushButtonMod") lineEditNumbers->setText("");
    else if(senderObjectName == "pushButtonDelete" && doesButtonDeleteClear == false) {
        QString lineEditNumbersText = lineEditNumbers->text();
        lineEditNumbersText.chop(1);
        lineEditNumbers->setText(lineEditNumbersText);
    } else if(senderObjectName == "pushButtonDelete" && doesButtonDeleteClear == true) {
        lineEditNumbers->setText("");
    }
    else if (senderObjectName == "pushButtonMenu") {
        stackedWidgetCentral->setCurrentWidget(menuWidget);
    }
    else if(senderObjectName == "pushButtonCalculate") {
        QString text = lineEditNumbers->text();

        QVector<QString> vectorString;
        QVector<int> vectorInt;

        QString buffer;
        int result = 0;
        //выражение из text разделяется на отдельные числа (5*5 - тоже отдельное число)
        for(int i = 0; i < text.size(); i++) {
            //если встречается знак + или -, значит началось новое число, буфер заносится в vectorString и обнуляется
            //если после знака умножения + или -, то это не новое число
            if((text.at(i) == "-" || text.at(i) == "+") && i-1 >= 0) { /* нужно, чтобы сначала проверялось, что i-1 больше
            text.size(), иначе может быть ошибка из-за того, что элемента i-1 не существует (например, в -5*5)*/
                if(text.at(i-1) != "*" && text.at(i-1) != "/" && text.at(i-1) != "%" && text.at(i-1) != "√") {
                    vectorString.append(buffer);
                    buffer.clear();
                }
            }
            buffer += text.at(i); //всё записывается в буфер
        }
        vectorString.append(buffer);

        for(QString i: vectorString) {
            if(i.contains("*") || i.contains("/") || i.contains("%") || i.contains("√")) {
                QString currentOperation = "*"; //первое число нужно умножить
                result = 1;
                buffer.clear();
                for(QString j: i) {
                    if(j == "*" || j == "/" || j == "%" || j == "√") { //все числа перемножаются или делятся друг на друга
                        if(buffer.toInt() != 0 || buffer == "0") {
                            if(currentOperation == "*") result*=buffer.toInt(); // если перед числом "*", его нужно умножить на предыдущее
                            else if(currentOperation == "/") {
                                if(buffer.toInt() == 0) {
                                    lineEditNumbers->setText("Если √-1 ещё можно понять, то это...");
                                    return;
                                } else result/=buffer.toInt();
                            }
                            else if(currentOperation == "%") {
                                if(buffer.toInt() == 0) {
                                    lineEditNumbers->setText("Не делите, пожалуйста, на ноль!");
                                    return;
                                } else result%=buffer.toInt();
                            }
                            else if(currentOperation == "√") {
                                if(buffer.toInt()<0) {
                                    lineEditNumbers->setText("\\_(0_0)_/");
                                    return;
                                } else result*=std::sqrt(buffer.toInt());
                            }
                        }
                        if(j == "*") currentOperation = "*";
                        else if(j == "/") currentOperation = "/";
                        else if(j == "%") currentOperation = "%";
                        else if(j == "√") currentOperation = "√";
                        buffer.clear();
                    } else buffer += j;
                }
                if(currentOperation == "*") result*=buffer.toInt();
                else if(currentOperation == "/") {
                    if(buffer.toInt() == 0) {
                        lineEditNumbers->setText("Не делите, пожалуйста, на ноль!");
                        return;
                    } else result/=buffer.toInt();
                }
                else if(currentOperation == "%") {
                    if(buffer.toInt()==0) {
                        lineEditNumbers->setText("Если √-1 ещё можно понять, то это...");
                        return;
                    } else result%=buffer.toInt();
                }
                else if(currentOperation == "√") {
                    if(buffer.toInt()<0) {
                        lineEditNumbers->setText("Что-то там с i");
                        return;
                    } else result*=std::sqrt(buffer.toInt());
                }
                vectorInt.append(result);
                result = 0;
            } else {
                vectorInt.append(i.toInt());
            }
        }

        //все числа складываются
        for(int i: vectorInt) {
            result += i;
        }

        lineEditNumbers->setText(QString::number(result));
        listWidgetHistoryPanel->addItem(text + "=" + QString::number(result));
        listWidgetHistoryPanelOnMainWindow->addItem(text + "=" + QString::number(result));
    }
}
