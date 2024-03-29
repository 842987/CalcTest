#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QtWidgets>

namespace Ui {
class MenuWidget;
}


class MenuWidget : public QWidget
{
    Q_OBJECT

public:

    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

public:
    Ui::MenuWidget *ui;
};

#endif // MENUWIDGET_H
