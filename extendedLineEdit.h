#ifndef EXTENDEDLINEEDIT_H
#define EXTENDEDLINEEDIT_H

#include <QLineEdit>

class ExtendedLineEdit : public QLineEdit
{
public:
    ExtendedLineEdit();
    void setTxtColor(QLineEdit *lineEdit, QPalette *palette, const QColor &color);

};

#endif // EXTENDEDLINEEDIT_H
