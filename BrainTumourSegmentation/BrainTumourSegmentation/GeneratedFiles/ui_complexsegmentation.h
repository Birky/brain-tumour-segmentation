/********************************************************************************
** Form generated from reading UI file 'complexsegmentation.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPLEXSEGMENTATION_H
#define UI_COMPLEXSEGMENTATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ComplexSegmentation
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *patientLabel;
    QComboBox *patientComboBox;

    void setupUi(QDialog *ComplexSegmentation)
    {
        if (ComplexSegmentation->objectName().isEmpty())
            ComplexSegmentation->setObjectName(QStringLiteral("ComplexSegmentation"));
        ComplexSegmentation->resize(400, 300);
        buttonBox = new QDialogButtonBox(ComplexSegmentation);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(ComplexSegmentation);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 160, 41));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        patientLabel = new QLabel(formLayoutWidget);
        patientLabel->setObjectName(QStringLiteral("patientLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, patientLabel);

        patientComboBox = new QComboBox(formLayoutWidget);
        patientComboBox->setObjectName(QStringLiteral("patientComboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, patientComboBox);


        retranslateUi(ComplexSegmentation);
        QObject::connect(buttonBox, SIGNAL(accepted()), ComplexSegmentation, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ComplexSegmentation, SLOT(reject()));

        QMetaObject::connectSlotsByName(ComplexSegmentation);
    } // setupUi

    void retranslateUi(QDialog *ComplexSegmentation)
    {
        ComplexSegmentation->setWindowTitle(QApplication::translate("ComplexSegmentation", "Dialog", 0));
        patientLabel->setText(QApplication::translate("ComplexSegmentation", "Patient", 0));
    } // retranslateUi

};

namespace Ui {
    class ComplexSegmentation: public Ui_ComplexSegmentation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPLEXSEGMENTATION_H
