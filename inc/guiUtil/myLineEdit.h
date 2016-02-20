#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QPalette>
#include <QColor>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = 0);
    MyLineEdit(QString const& contents, QWidget *parent = 0);

    void setTxtColor(QPalette *palette, const QColor &color);
    static void setTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color);



signals:

public slots:

};

#endif // MYLINEEDIT_H
