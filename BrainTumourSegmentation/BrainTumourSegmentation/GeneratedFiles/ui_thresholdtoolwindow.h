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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ThresholdToolWindow
{
public:
    QWidget *formLayoutWidget_4;
    QFormLayout *formLayout_4;
    QPushButton *pushButtonOK;
    QPushButton *pushButtonCancel;
    QGroupBox *groupBox;
    QWidget *formLayoutWidget_3;
    QFormLayout *formLayout_3;
    QLabel *labelPatient;
    QLabel *labelSlices;
    QComboBox *comboBoxPatient;
    QComboBox *comboBoxSlices;
    QGroupBox *groupBox_2;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *labelTreshold;
    QSpinBox *spinBoxThreshold;
    QCheckBox *checkBoxFindOptimal;
    QLabel *label;
    QComboBox *comboBoxCorrection;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *labelAngle;
    QSpinBox *spinBoxAngle;
    QLabel *labelDistance;
    QLabel *labelFromTop;
    QSpinBox *spinBoxFromTop;
    QDoubleSpinBox *doubleSpinBoxDistance;
    QCheckBox *checkBoxMinAverage;

    void setupUi(QDialog *ThresholdToolWindow)
    {
        if (ThresholdToolWindow->objectName().isEmpty())
            ThresholdToolWindow->setObjectName(QStringLiteral("ThresholdToolWindow"));
        ThresholdToolWindow->resize(349, 280);
        formLayoutWidget_4 = new QWidget(ThresholdToolWindow);
        formLayoutWidget_4->setObjectName(QStringLiteral("formLayoutWidget_4"));
        formLayoutWidget_4->setGeometry(QRect(160, 243, 181, 31));
        formLayout_4 = new QFormLayout(formLayoutWidget_4);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setContentsMargins(0, 0, 0, 0);
        pushButtonOK = new QPushButton(formLayoutWidget_4);
        pushButtonOK->setObjectName(QStringLiteral("pushButtonOK"));
        pushButtonOK->setEnabled(false);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, pushButtonOK);

        pushButtonCancel = new QPushButton(formLayoutWidget_4);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, pushButtonCancel);

        groupBox = new QGroupBox(ThresholdToolWindow);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 331, 81));
        formLayoutWidget_3 = new QWidget(groupBox);
        formLayoutWidget_3->setObjectName(QStringLiteral("formLayoutWidget_3"));
        formLayoutWidget_3->setGeometry(QRect(10, 20, 311, 51));
        formLayout_3 = new QFormLayout(formLayoutWidget_3);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setContentsMargins(0, 0, 0, 0);
        labelPatient = new QLabel(formLayoutWidget_3);
        labelPatient->setObjectName(QStringLiteral("labelPatient"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, labelPatient);

        labelSlices = new QLabel(formLayoutWidget_3);
        labelSlices->setObjectName(QStringLiteral("labelSlices"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, labelSlices);

        comboBoxPatient = new QComboBox(formLayoutWidget_3);
        comboBoxPatient->setObjectName(QStringLiteral("comboBoxPatient"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, comboBoxPatient);

        comboBoxSlices = new QComboBox(formLayoutWidget_3);
        comboBoxSlices->setObjectName(QStringLiteral("comboBoxSlices"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, comboBoxSlices);

        groupBox_2 = new QGroupBox(ThresholdToolWindow);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 90, 331, 141));
        formLayoutWidget = new QWidget(groupBox_2);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(7, 20, 151, 111));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        labelTreshold = new QLabel(formLayoutWidget);
        labelTreshold->setObjectName(QStringLiteral("labelTreshold"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelTreshold);

        spinBoxThreshold = new QSpinBox(formLayoutWidget);
        spinBoxThreshold->setObjectName(QStringLiteral("spinBoxThreshold"));
        spinBoxThreshold->setMaximum(100);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinBoxThreshold);

        checkBoxFindOptimal = new QCheckBox(formLayoutWidget);
        checkBoxFindOptimal->setObjectName(QStringLiteral("checkBoxFindOptimal"));

        formLayout->setWidget(1, QFormLayout::LabelRole, checkBoxFindOptimal);

        label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label);

        comboBoxCorrection = new QComboBox(formLayoutWidget);
        comboBoxCorrection->setObjectName(QStringLiteral("comboBoxCorrection"));

        formLayout->setWidget(2, QFormLayout::FieldRole, comboBoxCorrection);

        formLayoutWidget_2 = new QWidget(groupBox_2);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(170, 20, 159, 111));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        labelAngle = new QLabel(formLayoutWidget_2);
        labelAngle->setObjectName(QStringLiteral("labelAngle"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, labelAngle);

        spinBoxAngle = new QSpinBox(formLayoutWidget_2);
        spinBoxAngle->setObjectName(QStringLiteral("spinBoxAngle"));
        spinBoxAngle->setEnabled(false);
        spinBoxAngle->setMaximum(90);
        spinBoxAngle->setValue(10);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, spinBoxAngle);

        labelDistance = new QLabel(formLayoutWidget_2);
        labelDistance->setObjectName(QStringLiteral("labelDistance"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, labelDistance);

        labelFromTop = new QLabel(formLayoutWidget_2);
        labelFromTop->setObjectName(QStringLiteral("labelFromTop"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, labelFromTop);

        spinBoxFromTop = new QSpinBox(formLayoutWidget_2);
        spinBoxFromTop->setObjectName(QStringLiteral("spinBoxFromTop"));
        spinBoxFromTop->setEnabled(false);
        spinBoxFromTop->setMaximum(100);
        spinBoxFromTop->setValue(25);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, spinBoxFromTop);

        doubleSpinBoxDistance = new QDoubleSpinBox(formLayoutWidget_2);
        doubleSpinBoxDistance->setObjectName(QStringLiteral("doubleSpinBoxDistance"));
        doubleSpinBoxDistance->setEnabled(false);
        doubleSpinBoxDistance->setMaximum(100);
        doubleSpinBoxDistance->setSingleStep(0.5);
        doubleSpinBoxDistance->setValue(1);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, doubleSpinBoxDistance);

        checkBoxMinAverage = new QCheckBox(formLayoutWidget_2);
        checkBoxMinAverage->setObjectName(QStringLiteral("checkBoxMinAverage"));
        checkBoxMinAverage->setEnabled(false);

        formLayout_2->setWidget(3, QFormLayout::LabelRole, checkBoxMinAverage);


        retranslateUi(ThresholdToolWindow);

        QMetaObject::connectSlotsByName(ThresholdToolWindow);
    } // setupUi

    void retranslateUi(QDialog *ThresholdToolWindow)
    {
        ThresholdToolWindow->setWindowTitle(QApplication::translate("ThresholdToolWindow", "Thresholding tool", 0));
        pushButtonOK->setText(QApplication::translate("ThresholdToolWindow", "OK", 0));
        pushButtonCancel->setText(QApplication::translate("ThresholdToolWindow", "Cancel", 0));
        groupBox->setTitle(QApplication::translate("ThresholdToolWindow", "Select input data", 0));
        labelPatient->setText(QApplication::translate("ThresholdToolWindow", "Patient: ", 0));
        labelSlices->setText(QApplication::translate("ThresholdToolWindow", "Slices: ", 0));
        groupBox_2->setTitle(QApplication::translate("ThresholdToolWindow", "Algorithm parameters", 0));
        labelTreshold->setText(QApplication::translate("ThresholdToolWindow", "Threshold[%]:", 0));
        checkBoxFindOptimal->setText(QApplication::translate("ThresholdToolWindow", "Find optimal", 0));
        label->setText(QApplication::translate("ThresholdToolWindow", "Correction: ", 0));
        comboBoxCorrection->clear();
        comboBoxCorrection->insertItems(0, QStringList()
         << QApplication::translate("ThresholdToolWindow", "None", 0)
        );
        labelAngle->setText(QApplication::translate("ThresholdToolWindow", "Angle[\302\260]: ", 0));
        labelDistance->setText(QApplication::translate("ThresholdToolWindow", "Distance[%]: ", 0));
        labelFromTop->setText(QApplication::translate("ThresholdToolWindow", "From top[%]: ", 0));
        checkBoxMinAverage->setText(QApplication::translate("ThresholdToolWindow", "Min average", 0));
    } // retranslateUi

};

namespace Ui {
    class ThresholdToolWindow: public Ui_ThresholdToolWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THRESHOLDTOOLWINDOW_H
