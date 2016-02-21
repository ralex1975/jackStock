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
    void reset5Ctrl(QLineEdit *ctrl0,
                    QLineEdit *ctrl1,
                    QLineEdit *ctrl2,
                    QLineEdit *ctrl3,
                    QLineEdit *ctrl4);




    static void setTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color);



signals:

public slots:

};

#endif // MYLINEEDIT_H
