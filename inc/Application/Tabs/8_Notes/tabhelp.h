#ifndef TABHELP_H
#define TABHELP_H

#include <QDialog>

namespace Ui {
class TabHelp;
}

class TabHelp : public QDialog
{
    Q_OBJECT
    
public:
    explicit TabHelp(QWidget *parent = 0);
    ~TabHelp();
    
private slots:
    void on_pushButAddMainSection_clicked();

    void on_pushButRemoveMainSection_clicked();

    void on_pushButAddSubSection_clicked();

    void on_pushButRemoveSubSection_clicked();

    void on_pushButAddMsg_clicked();

    void on_pushButAddMainSection_2_clicked();

    void on_pushButSelectHelpTxt_clicked();

    void on_pushButSelMainSectionData_clicked();

    void on_pushButModifySubSectionNameInDb_clicked();

    void on_pushButRemoveMsg_clicked();

    void on_pushButRemoveMainSectionFromDb_clicked();

private:
    Ui::TabHelp *ui;
};

#endif // TABHELP_H
