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



/**********************************************************************************
 *
 * Function:        fa5CtrlReset()
 *
 *
 * Description: Remove data from 5 QEditLine controlls
 *
 *
 *
 ********************************************************************************/
void MyLineEdit::reset5Ctrl(QLineEdit *ctrl0,
                            QLineEdit *ctrl1,
                            QLineEdit *ctrl2,
                            QLineEdit *ctrl3,
                            QLineEdit *ctrl4)
{
    ctrl0->clear();
    ctrl1->clear();
    ctrl2->clear();
    ctrl3->clear();
    ctrl4->clear();
}
