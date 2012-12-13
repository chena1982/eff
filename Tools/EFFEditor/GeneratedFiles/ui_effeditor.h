/********************************************************************************
** Form generated from reading UI file 'effeditor.ui'
**
** Created: Thu Dec 13 22:24:24 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EFFEDITOR_H
#define UI_EFFEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EFFEditorClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EFFEditorClass)
    {
        if (EFFEditorClass->objectName().isEmpty())
            EFFEditorClass->setObjectName(QString::fromUtf8("EFFEditorClass"));
        EFFEditorClass->resize(684, 585);
        EFFEditorClass->setStyleSheet(QString::fromUtf8(""));
        centralWidget = new QWidget(EFFEditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        EFFEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EFFEditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 684, 23));
        EFFEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EFFEditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setEnabled(true);
        EFFEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EFFEditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        EFFEditorClass->setStatusBar(statusBar);

        retranslateUi(EFFEditorClass);

        QMetaObject::connectSlotsByName(EFFEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *EFFEditorClass)
    {
        EFFEditorClass->setWindowTitle(QApplication::translate("EFFEditorClass", "EFFEditor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EFFEditorClass: public Ui_EFFEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EFFEDITOR_H
