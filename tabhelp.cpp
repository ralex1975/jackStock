#include "tabhelp.h"
#include "ui_tabhelp.h"
#include "dbHndl.h"


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
TabHelp::TabHelp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TabHelp)
{
    CDbHndl db;
    QString  mainHelpSectionName;
    QString  subHelpSectionName;
    int mainHelpSectionId;

    ui->setupUi(this);

    // connect(ui->comboBoxMainSection, SIGNAL(currentIndexChanged(int)), this, SLOT(slotComboBoxMainSectionChange(int)));


    if(true == db.getAllMainHelpSectionNames(ui->comboBoxMainSection))
    {
       ui->comboBoxMainSection->setCurrentIndex(0);
       mainHelpSectionName = ui->comboBoxMainSection->currentText();
       if(mainHelpSectionName.size() > 0)
       {

           if(true == db.findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId))
           {
               if(true == db.getAllSubHelpSectionNames(ui->comboBoxSubSection, mainHelpSectionId))
               {
                   ui->comboBoxSubSection->setCurrentIndex(0);
                   ui->lineEditSelectedMainSection->clear();
                   ui->lineEditSelectedMainSection->insert(mainHelpSectionName);

                   subHelpSectionName = ui->comboBoxSubSection->currentText();
                   if(subHelpSectionName.size() > 0)
                   {
                        ui->lineEditSelectedSubItem->clear();
                        ui->lineEditSelectedSubItem->insert(subHelpSectionName);
                        on_pushButSelectHelpTxt_clicked();
                   }
               }
           }
       }
    }
}

void TabHelp::slotComboBoxMainSectionChange(int index)
{
    //on_pushButSelMainSectionData_clicked();
     //QMessageBox::information(this, tr("Error"), tr("Error fail to retrive data"));
}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
TabHelp::~TabHelp()
{
    delete ui;
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButAddMainSection_clicked()
{
    QString  str;
    str = ui->lineEditMainSection->text().trimmed();
    if(str.size() > 0)
    {
        ui->comboBoxMainSection->addItem(str);

        int index = ui->comboBoxMainSection->findText(str);
        if(index > 0)
        {
            ui->comboBoxMainSection->setCurrentIndex(index);
            ui->comboBoxSubSection->clear();
            on_pushButSelMainSectionData_clicked();
        }
        else
        {
            ui->comboBoxMainSection->setCurrentIndex(0);
        }
    }

}

/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButRemoveMainSection_clicked()
{
    QString str;
    int mainHelpSectionId;
    CDbHndl db;

    str = ui->comboBoxMainSection->currentText();

    if(true == db.findMainHelpSectionName(str, mainHelpSectionId))
    {
        QMessageBox::information(this, tr("Error"), tr("The mainSectionName is already in database"));
        return;
    }

     ui->lineEditMainSection->clear();


     if(str.size() > 0)
     {
        ui->lineEditMainSection->insert(str);
        int index = ui->comboBoxMainSection->findText(str);
        if(index > -1)
        {
            ui->comboBoxMainSection->removeItem(index);
            ui->comboBoxSubSection->clear();
            on_pushButSelMainSectionData_clicked();
        }
     }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButAddSubSection_clicked()
{
    QString  str;
    str = ui->lineEditSubSection->text().trimmed();
    if(str.size() > 0)
    {
        ui->textEditMsg->clear();
        ui->lineEditSelectedSubItem->clear();
        ui->comboBoxSubSection->addItem(str);

        int index = ui->comboBoxSubSection->findText(str);
        if(index > 0)
            ui->comboBoxSubSection->setCurrentIndex(index);
        else
            ui->comboBoxSubSection->setCurrentIndex(0);
    }
}



/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButRemoveSubSection_clicked()
{
    QString str;
    CDbHndl db;
    int subHelpSectionId;

    str = ui->comboBoxSubSection->currentText();

    if(true == db.findSubHelpSectionName(str, subHelpSectionId))
    {
        QMessageBox::information(this, tr("Error"), tr("The SubSectionName is already in database"));
        return;
    }

     ui->lineEditSubSection->clear();


     if(str.size() > 0)
     {
        ui->lineEditSubSection->insert(str);
        int index = ui->comboBoxSubSection->findText(str);
        if(index > -1)
        {
            ui->comboBoxSubSection->removeItem(index);
        }
     }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButAddMsg_clicked()
{
    QString  main;
    QString  sub;
    QString  txt;
    CDbHndl db;

    main = ui->comboBoxMainSection->currentText();
    sub = ui->comboBoxSubSection->currentText();
    txt = ui->textEditMsg->toPlainText();
    qDebug() << main;
    qDebug() << sub;
    qDebug() << txt;
    if(main.size() > 0 && sub.size() > 0 && txt.size() > 0)
    {
        if(false == db.insertHelpSectionTxt(main, sub, txt))
        {
            QMessageBox::information(this, tr("Error"), tr("Error fail to add HelpSectionTxt"));
        }
        on_pushButSelectHelpTxt_clicked();
    }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButAddMainSection_2_clicked()
{
    QString  str;
    CDbHndl db;

    str = ui->lineEditMainSection->text().trimmed();
    if(str.size() > 0)
    {
        if(false == db.insertMainHelpSection(str))
        {
            QMessageBox::information(this, tr("Error"), tr("Error fail to add MainHelpSectionName"));
        }
    }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButSelectHelpTxt_clicked()
{
    CDbHndl db;
    QString main;
    QString sub;
    QString txt;
    int mainId;
    int subId;
    bool res1 = false;
    bool res2 = false;

    main = ui->comboBoxMainSection->currentText();
    sub = ui->comboBoxSubSection->currentText();
    qDebug() << main;
    qDebug() << sub;

    if(main.size() > 0 && sub.size() > 0 )
    {
        res1 = db.findMainHelpSectionName(main, mainId);
        res2 = db.findSubHelpSectionName(sub, subId);

        if(res1 == true && res2 == true)
        {
            qDebug() << mainId;
            qDebug() << subId;


           if(false == db.getHelpSectionTxt(mainId, subId, txt))
           {
               QMessageBox::information(this, tr("Error"), tr("Error fail to retrive data"));
           }
           ui->textEditMsg->clear();
           ui->textEditMsg->insertPlainText(txt);
           txt = ui->comboBoxSubSection->currentText();
           if(txt.size() > 0)
           {
               ui->lineEditSelectedSubItem->clear();
               ui->lineEditSelectedSubItem->insert(txt);
           }
        }
    }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description:
 *
 *
 *******************************************************************/
void TabHelp::on_pushButSelMainSectionData_clicked()
{
    CDbHndl db;
    QString mainHelpSectionName;
    int mainHelpSectionId;

    mainHelpSectionName = ui->comboBoxMainSection->currentText();
    if(mainHelpSectionName.size() > 0)
    {
        if(true == db.findMainHelpSectionName(mainHelpSectionName, mainHelpSectionId))
        {
            if(true == db.getAllSubHelpSectionNames(ui->comboBoxSubSection, mainHelpSectionId))
            {
                ui->comboBoxSubSection->setCurrentIndex(0);
                ui->lineEditSelectedMainSection->clear();
                ui->lineEditSelectedSubItem->clear();
                ui->textEditMsg->clear();
                ui->lineEditSelectedMainSection->insert(mainHelpSectionName);

                on_pushButSelectHelpTxt_clicked();
            }
        }
    }
}


/*******************************************************************
 *
 * Function:    ()
 *
 * Description: This function change the name of the SubSectionName
 *              it update the database and the combobox.
 *
 *
 *******************************************************************/
void TabHelp::on_pushButModifySubSectionNameInDb_clicked()
{
    CDbHndl db;
    QString oldSubHelpSectionName;
    QString newSubHelpSectionName;

    oldSubHelpSectionName = ui->comboBoxSubSection->currentText();


    if(oldSubHelpSectionName.size() > 0)
    {
        newSubHelpSectionName = ui->lineEditSubSection->text().trimmed();
        if(newSubHelpSectionName.size() > 0)
        {
            if(false == db.modifySubHelpSectionName(oldSubHelpSectionName, newSubHelpSectionName))
            {
                QMessageBox::information(this, tr("Error"), tr("Error to update SubHelpSecionName"));
            }

            // Remove old name from combobox
            if(oldSubHelpSectionName.size() > 0)
            {
               int index = ui->comboBoxSubSection->findText(oldSubHelpSectionName);
               if(index > -1)
               {
                   ui->comboBoxSubSection->removeItem(index);
               }
            }


            // Insert new name in combobox
            if(newSubHelpSectionName.size() > 0)
            {
                ui->comboBoxSubSection->addItem(newSubHelpSectionName);

                int index = ui->comboBoxSubSection->findText(newSubHelpSectionName);
                if(index > 0)
                    ui->comboBoxSubSection->setCurrentIndex(index);
                else
                    ui->comboBoxSubSection->setCurrentIndex(0);
            }
        }
   }
}

void TabHelp::on_pushButRemoveMsg_clicked()
{
    CDbHndl db;
    QString main;
    QString sub;

    if (QMessageBox::No == QMessageBox::question(NULL,
                                                 QString::fromUtf8("Ta bort"),
                                                 QString::fromUtf8("Vill du ta bort inlägget?"),
                                                 QMessageBox::Yes | QMessageBox::No))
    {
        return;
    }


    main = ui->lineEditSelectedMainSection->text();
    sub = ui->lineEditSelectedSubItem->text();

    qDebug() << main;
    qDebug() << sub;

    if(main.size() > 0 && sub.size() > 0)
    {

        if( true == db.deleteSubHelpSectionTxt(main, sub))
        {
            if( true == db.deleteSubHelpSectionName(sub))
            {
                int index = ui->comboBoxSubSection->findText(sub);
                if(index > -1)
                {
                    ui->comboBoxSubSection->removeItem(index);
                    ui->textEditMsg->clear();
                    ui->lineEditSelectedSubItem->clear();
                }
                return;
            }
        }
    }
}

void TabHelp::on_pushButRemoveMainSectionFromDb_clicked()
{
    CDbHndl db;
    QString  mainHelpSectionName;
    int mainHelpSectionId;

    if (QMessageBox::No == QMessageBox::question(NULL,
                                                 QString::fromUtf8("Ta bort"),
                                                 QString::fromUtf8("Vill du ta Huvudkategorin?"),
                                                 QMessageBox::Yes | QMessageBox::No))
    {
        return;
    }

    mainHelpSectionName = ui->comboBoxMainSection->currentText();

    qDebug() << mainHelpSectionName;

    if(mainHelpSectionName.size() > 0)
    {
        // Check that there is no subsection that is connected to this main section
        if(false == db.findMainHelpSectionInTblHelpSectionTxt(mainHelpSectionName, mainHelpSectionId))
        {
            db.deleteMainHelpSectionName(mainHelpSectionName);

            int index = ui->comboBoxMainSection->findText(mainHelpSectionName);
            if(index > -1)
            {
                ui->comboBoxMainSection->removeItem(index);
                ui->comboBoxMainSection->setCurrentIndex(0);
                ui->lineEditSelectedMainSection->clear();

            }
        }
    }
}
