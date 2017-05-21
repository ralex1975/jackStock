/****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #ifndef FINDFILEDIALOG_H
 #define FINDFILEDIALOG_H

 #include <QtGui/QDialog>

 class QComboBox;
 class QDialogButtonBox;
 class QLabel;
 class QToolButton;
 class QTreeWidget;
 class QTreeWidgetItem;

 class Assistant;
 class TextEdit;

 class FindFileDialog : public QDialog
 {
     Q_OBJECT

 public:
     FindFileDialog(TextEdit *editor, Assistant *assistant);

 private slots:
     void browse();
     void help();
     void openFile(QTreeWidgetItem *item = 0);
     void update();

 private:
     void findFiles();
     void showFiles(const QStringList &files);

     void createButtons();
     void createComboBoxes();
     void createFilesTree();
     void createLabels();
     void createLayout();

     Assistant *currentAssistant;
     TextEdit *currentEditor;
     QTreeWidget *foundFilesTree;

     QComboBox *directoryComboBox;
     QComboBox *fileNameComboBox;

     QLabel *directoryLabel;
     QLabel *fileNameLabel;

     QDialogButtonBox *buttonBox;

     QToolButton *browseButton;
 };

 #endif