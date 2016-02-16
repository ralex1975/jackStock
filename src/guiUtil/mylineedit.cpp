#include "../../inc/guiUtil/mylineedit.h"

/*******************************************************************
 *
 * Function:    MyLineEdit()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
MyLineEdit::MyLineEdit(QWidget *parent) :
                    QLineEdit(parent)
{

}


MyLineEdit::MyLineEdit(QString const& contents, QWidget *parent) :
                                                QLineEdit(contents, parent)
{

}

/*******************************************************************
 *
 * Function:    setTxtColor()
 *
 * Description: Set text color
 *
 * Example: color = Qt::red
 *
 *
 *******************************************************************/
void MyLineEdit::setTxtColor(QPalette *palette, const QColor &color)
{
    palette->setColor(QPalette::Text, color);
    // lineEdit->setPalette(*palette);
    this->setPalette(*palette);
}



/*******************************************************************
 *
 * Function:    setTxtColor()
 *
 * Description: Set text color
 *
 * Example: color = Qt::red
 *
 *
 *******************************************************************/
void MyLineEdit::setTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color)
{
    palette->setColor(QPalette::Text, color);
    lineEdit->setPalette(*palette);
}

