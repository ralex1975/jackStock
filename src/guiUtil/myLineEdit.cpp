#include "../../inc/guiUtil/myLineEdit.h"

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


/*******************************************************************
 *
 * Function:    MyLineEdit()
 *
 * Description:
 *
 *
 *
 *******************************************************************/
MyLineEdit::
MyLineEdit(QString const& contents, QWidget *parent) :
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

