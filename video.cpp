#include "video.h"
#include "ui_video.h"


/****************************************************************
 *
 * Function:    Video()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
Video::Video(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);

    //ui->topExerciseComboBox->installEventFilter(parent);
    //m_mousePressedtopExerciseComboBox = false;

    //ui->subExcersiceComboBox->installEventFilter(parent);
    //m_mousePressedSubExerciseComboBox = false;



    m_dbIsBusy = false;
    on_pushButtonImportVideoHelp_clicked();
}


#if 0
/*******************************************************************
 *
 * Function:    eventFilter()
 *
 * Description:
 *
 *
 *******************************************************************/
bool Video::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->topExerciseComboBox  && event->type() == QEvent::MouseButtonPress)
    {
        switch( event->type() )
        {
       case QEvent::MouseButtonDblClick:
           qDebug() << "Mouse Button Double Clicked";
           break;
       case QEvent::MouseButtonPress:
           qDebug() << "Mouse Button Pressed";
            m_mousePressedtopExerciseComboBox = true;
           break;
       case QEvent::MouseButtonRelease:
           qDebug() << "Mouse Button Released";
           break;
       default:

           break;
        }
        return Video::eventFilter(object, event);
    }
    else if(object == ui->subExcersiceComboBox  && event->type() == QEvent::MouseButtonPress)
    {
        switch( event->type() )
        {
       case QEvent::MouseButtonDblClick:
           qDebug() << "Mouse Button Double Clicked";
           break;
       case QEvent::MouseButtonPress:
           qDebug() << "Mouse Button Pressed";
            m_mousePressedSubExerciseComboBox = true;
           break;
       case QEvent::MouseButtonRelease:
           qDebug() << "Mouse Button Released";
           break;
       default:

           break;
        }
        return Video::eventFilter(object, event);
    }
    else
    {
        return Video::eventFilter(object, event);
    }

    return Video::eventFilter(object, event);
}
#endif


/****************************************************************
 *
 * Function:    ~Video()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
Video::~Video()
{
    delete ui;
}



/****************************************************************
 *
 * Function:    on_pushButtonImportVideoHelp_clicked()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void Video::on_pushButtonImportVideoHelp_clicked()
{
    QString topExerciseName;
    QString subExerciseName;


    topExerciseName = ui->topExerciseComboBox->currentText();
    subExerciseName = ui->subExcersiceComboBox->currentText();



    if(m_dbIsBusy == false)
    {
        m_dbIsBusy = true;
        ui->exerciseTreeWidget->clear();
        ui->topExerciseComboBox->clear();
        ui->subExcersiceComboBox->clear();

        //m_addSyllableDataToTreeWidget = false;


        // Fill top comboBox
        m_db.addTopExerciseDataToComboBox(ui->topExerciseComboBox);
        ui->topExerciseComboBox->setCurrentIndex(0);
        topExerciseName = ui->topExerciseComboBox->currentText();

        // Fill sub comboBox
        m_db.addSubExerciseDataToComboBox(ui->subExcersiceComboBox, topExerciseName);
        ui->subExcersiceComboBox->setCurrentIndex(0);
        subExerciseName = ui->subExcersiceComboBox->currentText();


        if(ui->topExerciseComboBox->count() < 1)
            return;

        if(ui->subExcersiceComboBox->count() < 1)
            return;

        m_db.addExerciseDataToTreeWidget(ui->exerciseTreeWidget,
                                         m_exerciseDataArr,
                                         topExerciseName,
                                         subExerciseName,
                                         false);

        m_dbIsBusy = false;
    }
    else
    {
        QMessageBox::information(this, QString::fromUtf8("Info"), QString::fromUtf8("Försök igen senare systemet processar data"));
    }


}


/****************************************************************
 *
 * Function:    on_exerciseTreeWidget_clicked()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void Video::on_exerciseTreeWidget_clicked(const QModelIndex &index)
{

}

// räntabilitet_på_eget_kapital_2.mp4


/****************************************************************
 *
 * Function:    on_exerciseTreeWidget_clicked()
 *
 * Description:
 *
 *
 *
 ****************************************************************/
void Video::on_pushButton_2_clicked()
{
    QString topExerciseName;
    QString subExerciseName;

    topExerciseName = ui->topExerciseComboBox->currentText();
    subExerciseName = ui->subExcersiceComboBox->currentText();

    ui->exerciseTreeWidget->clear();


    if( true == m_db.addExerciseDataToTreeWidget(ui->exerciseTreeWidget,
                                  m_exerciseDataArr,
                                  topExerciseName,
                                  subExerciseName,
                                  false))
    {
        ui->exerciseTreeWidget->setColumnWidth(0, 170);
        ui->exerciseTreeWidget->scrollToTop();
        ui->exerciseTreeWidget->setCurrentItem(ui->exerciseTreeWidget->topLevelItem(0));
    }

}

void Video::on_exerciseTreeWidget_doubleClicked(const QModelIndex &index)
{
    int index1;

    index1 = index.row();


    //QMessageBox::information(NULL, "Test", m_exerciseDataArr[index1].exerciseWord);
    //QMessageBox::information(NULL, "Test", m_exerciseDataArr[index1].exercisePronunciation);

    QString path;
    QString str;
     path.clear();
     path =  QCoreApplication::applicationDirPath();
     path += "/database/Video/klipp/";
     path += m_exerciseDataArr[index1].exercisePronunciation;
     if(path.size() > 0)
     {
         // --aspect-ratio 5:4
         str = "vlc --aspect-ratio 5:4 ";
         str += path;
         str += " --play-and-exit";

         //str = "vlc database/theory/video/j-ljudet.mpg";

         QByteArray byteArray = str.toLocal8Bit().constData();
         byteArray.constData();
         system(byteArray);
     }

}

void Video::on_topExerciseComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED( arg1 );


   //if(m_mousePressedtopExerciseComboBox == true)
   {
       QString topExerciseName = ui->topExerciseComboBox->currentText();

       // Fill sub comboBox
       m_db.addSubExerciseDataToComboBox(ui->subExcersiceComboBox, topExerciseName);
       ui->subExcersiceComboBox->setCurrentIndex(0);
       m_mousePressedtopExerciseComboBox = false;

   }

}

void Video::on_subExcersiceComboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED( arg1 );


    //if(m_mousePressedSubExerciseComboBox == true)
    {

        QString topExerciseName;
        QString subExerciseName;


        topExerciseName = ui->topExerciseComboBox->currentText();
        subExerciseName = ui->subExcersiceComboBox->currentText();


        // Show data in tree view
        m_tmpDataArr.clear();
        ui->exerciseTreeWidget->clear();

        m_db.addExerciseDataToTreeWidget(ui->exerciseTreeWidget,
                                         m_tmpDataArr,
                                         topExerciseName,
                                         subExerciseName,
                                         false,
                                         false);

        m_mousePressedSubExerciseComboBox = false;
    }
}
