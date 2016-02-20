#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QTreeWidget>

class MyTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MyTreeWidget(QWidget *parent = 0);
    void setHeader4ColumnTreeWidget(QTreeWidget *treeWidget,
                               QString header0, int headerWidth0,
                               QString header1, int headerWidth1,
                               QString header2, int headerWidth2,
                               QString header3, int headerWidth3);

    void setHeader3ColumnTreeWidget(QTreeWidget *treeWidget,
                                    QString header0, int headerWidth0,
                                    QString header1, int headerWidth1,
                                    QString header2, int headerWidth2);

    void setHeader3ColumnTreeWidget(QTreeWidget *treeWidget,
                                    QString header0, int headerWidth0, bool hideCol0,
                                    QString header1, int headerWidth1, bool hideCol1,
                                    QString header2, int headerWidth2, bool hideCol2);




signals:

public slots:

};

#endif // MYTREEWIDGET_H
