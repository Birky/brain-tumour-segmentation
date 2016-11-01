/********************************************************************************
** Form generated from reading UI file 'imagearithmwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEARITHMWINDOW_H
#define UI_IMAGEARITHMWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageArithmWindow
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QRadioButton *substractionRadioButton;
    QGroupBox *groupBox_2;
    QGroupBox *groupBox_3;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *patientALabel;
    QComboBox *patientAComboBox;
    QLabel *slicesALabel;
    QComboBox *slicesAComboBox;
    QGroupBox *groupBox_4;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *patientBLabel;
    QComboBox *patientBComboBox;
    QLabel *slicesBLabel;
    QComboBox *slicesBComboBox;

    void setupUi(QDialog *ImageArithmWindow)
    {
        if (ImageArithmWindow->objectName().isEmpty())
            ImageArithmWindow->setObjectName(QStringLiteral("ImageArithmWindow"));
        ImageArithmWindow->resize(363, 406);
        buttonBox = new QDialogButtonBox(ImageArithmWindow);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setEnabled(false);
        buttonBox->setGeometry(QRect(10, 360, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(ImageArithmWindow);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 210, 341, 141));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(gridLayoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        substractionRadioButton = new QRadioButton(groupBox);
        substractionRadioButton->setObjectName(QStringLiteral("substractionRadioButton"));
        substractionRadioButton->setGeometry(QRect(10, 30, 151, 17));
        substractionRadioButton->setChecked(true);

        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(gridLayoutWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));

        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(ImageArithmWindow);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 10, 341, 81));
        formLayoutWidget = new QWidget(groupBox_3);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 20, 321, 48));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        patientALabel = new QLabel(formLayoutWidget);
        patientALabel->setObjectName(QStringLiteral("patientALabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, patientALabel);

        patientAComboBox = new QComboBox(formLayoutWidget);
        patientAComboBox->setObjectName(QStringLiteral("patientAComboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, patientAComboBox);

        slicesALabel = new QLabel(formLayoutWidget);
        slicesALabel->setObjectName(QStringLiteral("slicesALabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, slicesALabel);

        slicesAComboBox = new QComboBox(formLayoutWidget);
        slicesAComboBox->setObjectName(QStringLiteral("slicesAComboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, slicesAComboBox);

        groupBox_4 = new QGroupBox(ImageArithmWindow);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 100, 341, 81));
        formLayoutWidget_2 = new QWidget(groupBox_4);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(10, 20, 321, 51));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        patientBLabel = new QLabel(formLayoutWidget_2);
        patientBLabel->setObjectName(QStringLiteral("patientBLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, patientBLabel);

        patientBComboBox = new QComboBox(formLayoutWidget_2);
        patientBComboBox->setObjectName(QStringLiteral("patientBComboBox"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, patientBComboBox);

        slicesBLabel = new QLabel(formLayoutWidget_2);
        slicesBLabel->setObjectName(QStringLiteral("slicesBLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, slicesBLabel);

        slicesBComboBox = new QComboBox(formLayoutWidget_2);
        slicesBComboBox->setObjectName(QStringLiteral("slicesBComboBox"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, slicesBComboBox);


        retranslateUi(ImageArithmWindow);
        QObject::connect(buttonBox, SIGNAL(accepted()), ImageArithmWindow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ImageArithmWindow, SLOT(reject()));

        QMetaObject::connectSlotsByName(ImageArithmWindow);
    } // setupUi

    void retranslateUi(QDialog *ImageArithmWindow)
    {
        ImageArithmWindow->setWindowTitle(QApplication::translate("ImageArithmWindow", "Image arithm", 0));
        groupBox->setTitle(QApplication::translate("ImageArithmWindow", "Operations for two images", 0));
        substractionRadioButton->setText(QApplication::translate("ImageArithmWindow", "Substraction A - B", 0));
        groupBox_2->setTitle(QApplication::translate("ImageArithmWindow", "Operations for one image", 0));
        groupBox_3->setTitle(QApplication::translate("ImageArithmWindow", "Select first image (A):", 0));
        patientALabel->setText(QApplication::translate("ImageArithmWindow", "Patient:", 0));
        slicesALabel->setText(QApplication::translate("ImageArithmWindow", "Slices", 0));
        groupBox_4->setTitle(QApplication::translate("ImageArithmWindow", "Select second image (B):", 0));
        patientBLabel->setText(QApplication::translate("ImageArithmWindow", "Patient:", 0));
        slicesBLabel->setText(QApplication::translate("ImageArithmWindow", "SlicesB", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageArithmWindow: public Ui_ImageArithmWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEARITHMWINDOW_H
