#ifndef GUIFINANCECTRLS_H
#define GUIFINANCECTRLS_H

#include <QWidget>
#include <QtGui>

class GuiFinanceCtrls : public QWidget
{
    Q_OBJECT
public:
    explicit GuiFinanceCtrls(QWidget *parent = 0);

    void fa5AddData(QString inEarningsDivDividend,  QLineEdit *earningsDivDividend,
                    QString inBavDivStockPrice,     QLineEdit *navDivStockPrice,
                    QString inPe,                   QLineEdit *pe,
                    QString inPs,                   QLineEdit *ps,
                    QString inYield,                QLineEdit *yield,
                    QString assetType = "");

    void fa5Reset(QLineEdit *earningsDivDividend,
                  QLineEdit *navDivStockPrice,
                  QLineEdit *pe,
                  QLineEdit *ps,
                  QLineEdit *yield)
    ;


signals:

public slots:

};

#endif // GUIFINANCECTRLS_H
