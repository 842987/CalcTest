#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <menuwidget.h>
class MainWindow: public QMainWindow
{
    Q_OBJECT
public: //widgets

    //HistoryPanel
    QListWidget *listWidgetHistoryPanel;
    //HistoryPanel on MainWindow
    QListWidget *listWidgetHistoryPanelOnMainWindow;

    //NotesPanel
    QTextEdit *textEditNotesPanel;
    //NotesPanel on MainWindow
    QTextEdit *textEditNotesPanelOnMainWindow;

    QStackedWidget *stackedWidgetCentral;

    QLineEdit *lineEditNumbers;

    //layouts
    QGridLayout *gridLayoutCentral;
    QGridLayout *gridLayoutButtons;

    //fonts
    QFont *fontUbuntu;

    //cental widget
    QWidget *widgetCentral;

    //горячая клавиша для pushButtonDelete ("<-")
    QShortcut *shortCutClear;

    //buttons
    //numbers
    QPushButton *pushButton0;
    QPushButton *pushButton1;
    QPushButton *pushButton2;
    QPushButton *pushButton3;
    QPushButton *pushButton4;
    QPushButton *pushButton5;
    QPushButton *pushButton6;
    QPushButton *pushButton7;
    QPushButton *pushButton8;
    QPushButton *pushButton9;

    //arifmetic operations
    QPushButton *pushButtonMod;
    QPushButton *pushButtonSqrt;
    QPushButton *pushButtonDivision;
    QPushButton *pushButtonMultiplication;
    QPushButton *pushButtonPlus;
    QPushButton *pushButtonMinus;

    //calculator operations
    QPushButton *pushButtonDelete;
    QPushButton *pushButtonCalculate;
    QPushButton *pushButtonMenu;

    MainWindow(QApplication *applciation, QWidget *parent = nullptr);
    ~MainWindow();

private slots: //слоты (функции, которые можно вызвать после сигнала)
    void on_pushButtonNumbers_clicked(); //при клике на одну из кнопок калькулятора
    void on_MenuWidgetCheckBox_changed(bool is_activated); // при изменении QCheckBox в MenuWidget
    void on_MenuWidgetPushButtonGoBack_clicked(); // при нажатии кнопки "назад" в MenuWidget
    void on_listWidgetHistoryPanelOnMainWindow_clicked(QListWidgetItem *item); //при нажатии на историю поиска
    void on_buttonDeleteAction_changed(const QString &text); // при изменении comboBoxChooseButtonDeleteAction в MenuWidget
    void on_MenuWidgetComboBoxStyles_currentTextChanged(const QString &text = nullptr); // при изменении шрифта в MenuWidget
    void on_MenuWidgetFontComboBoxFonts_currentFontChanged(const QFont &font); //при изменении стиля в MenuWidget
    void on_shortcutClear_activated();
public:
    QSizePolicy *sizePolicy;
    MenuWidget* menuWidget; //меню
    QApplication *application;
    bool doesButtonDeleteClear = false; //кнопка "<-" стирает полностью или только один символ
};
#endif // MAINWINDOW_H
