/********************************************************************************
** Form generated from reading UI file 'thresholdtoolwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THRESHOLDTOOLWINDOW_H
#define UI_THRESHOLDTOOLWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_ThresholdToolWindow
{
public:

    void setupUi(QDialog *ThresholdToolWindow)
    {
        if (ThresholdToolWindow->objectName().isEmpty())
            ThresholdToolWindow->setObjectName(QStringLiteral("ThresholdToolWindow"));
        ThresholdToolWindow->resize(400, 300);

        retranslateUi(ThresholdToolWindow);

        QMetaObject::connectSlotsByName(ThresholdToolWindow);
    } // setupUi

    void retranslateUi(QDialog *ThresholdToolWindow)
    {
        ThresholdToolWindow->setWindowTitle(QApplication::translate("ThresholdToolWindow", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class ThresholdToolWindow: public Ui_ThresholdToolWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THRESHOLDTOOLWINDOW_H
