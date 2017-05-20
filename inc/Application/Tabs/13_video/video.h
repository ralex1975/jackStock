#ifndef VIDEO_H
#define VIDEO_H

#include <QDialog>
#include "dbHndl.h"

namespace Ui {
class Video;
}

class Video : public QDialog
{
    Q_OBJECT


    bool m_dbIsBusy;
    CDbHndl m_db;
    QVector<CDbHndl::exerciseData_ST> m_exerciseDataArr;

    bool m_mousePressedtopExerciseComboBox;
    bool m_mousePressedSubExerciseComboBox;
    QVector<CDbHndl::exerciseData_ST> m_tmpDataArr;


public:
    explicit Video(QWidget *parent = 0);
    ~Video();
   // bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_pushButtonImportVideoHelp_clicked();

    void on_exerciseTreeWidget_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_exerciseTreeWidget_doubleClicked(const QModelIndex &index);

    void on_topExerciseComboBox_currentIndexChanged(const QString &arg1);

    void on_subExcersiceComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::Video *ui;
};

#endif // VIDEO_H
