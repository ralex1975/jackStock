#include "extendedLineEdit.h"


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/

ExtendedLineEdit::ExtendedLineEdit()
{
}


/*******************************************************************
 *
 * Function:    setFaEditControlTxtColor()
 *
 * Description:
 *
 *
 *
 *
 *******************************************************************/
void ExtendedLineEdit::setTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color)
{
    palette->setColor(QPalette::Text, color); //Qt::red
    lineEdit->setPalette(*palette);
}
