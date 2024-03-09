/********************************************************************************
** Form generated from reading UI file 'QtWidgetsApplication2.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWIDGETSAPPLICATION2_H
#define UI_QTWIDGETSAPPLICATION2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsApplication2Class
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtWidgetsApplication2Class)
    {
        if (QtWidgetsApplication2Class->objectName().isEmpty())
            QtWidgetsApplication2Class->setObjectName("QtWidgetsApplication2Class");
        QtWidgetsApplication2Class->resize(1046, 722);
        centralWidget = new QWidget(QtWidgetsApplication2Class);
        centralWidget->setObjectName("centralWidget");
        label = new QLabel(centralWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(340, 135, 321, 181));
        QtWidgetsApplication2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtWidgetsApplication2Class);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1046, 26));
        QtWidgetsApplication2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtWidgetsApplication2Class);
        mainToolBar->setObjectName("mainToolBar");
        QtWidgetsApplication2Class->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtWidgetsApplication2Class);
        statusBar->setObjectName("statusBar");
        QtWidgetsApplication2Class->setStatusBar(statusBar);

        retranslateUi(QtWidgetsApplication2Class);

        QMetaObject::connectSlotsByName(QtWidgetsApplication2Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtWidgetsApplication2Class)
    {
        QtWidgetsApplication2Class->setWindowTitle(QCoreApplication::translate("QtWidgetsApplication2Class", "QtWidgetsApplication2", nullptr));
        label->setText(QCoreApplication::translate("QtWidgetsApplication2Class", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsApplication2Class: public Ui_QtWidgetsApplication2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWIDGETSAPPLICATION2_H
