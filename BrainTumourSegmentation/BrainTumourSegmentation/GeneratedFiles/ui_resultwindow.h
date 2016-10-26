/********************************************************************************
** Form generated from reading UI file 'resultwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTWINDOW_H
#define UI_RESULTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ResultWindow
{
public:
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_5;
    QFormLayout *formLayout_8;
    QLabel *selectPatientLabel;
    QComboBox *comboBoxPatient;
    QFormLayout *formLayout_10;
    QLabel *selectEvaluatedDataLabel;
    QComboBox *comboBoxEvaluatedData;
    QTableWidget *tableWidget;

    void setupUi(QDialog *ResultWindow)
    {
        if (ResultWindow->objectName().isEmpty())
            ResultWindow->setObjectName(QStringLiteral("ResultWindow"));
        ResultWindow->resize(731, 430);
        gridLayout_6 = new QGridLayout(ResultWindow);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        formLayout_8 = new QFormLayout();
        formLayout_8->setObjectName(QStringLiteral("formLayout_8"));
        selectPatientLabel = new QLabel(ResultWindow);
        selectPatientLabel->setObjectName(QStringLiteral("selectPatientLabel"));

        formLayout_8->setWidget(0, QFormLayout::LabelRole, selectPatientLabel);

        comboBoxPatient = new QComboBox(ResultWindow);
        comboBoxPatient->setObjectName(QStringLiteral("comboBoxPatient"));

        formLayout_8->setWidget(0, QFormLayout::FieldRole, comboBoxPatient);


        gridLayout_5->addLayout(formLayout_8, 0, 0, 1, 1);

        formLayout_10 = new QFormLayout();
        formLayout_10->setObjectName(QStringLiteral("formLayout_10"));
        selectEvaluatedDataLabel = new QLabel(ResultWindow);
        selectEvaluatedDataLabel->setObjectName(QStringLiteral("selectEvaluatedDataLabel"));

        formLayout_10->setWidget(0, QFormLayout::LabelRole, selectEvaluatedDataLabel);

        comboBoxEvaluatedData = new QComboBox(ResultWindow);
        comboBoxEvaluatedData->setObjectName(QStringLiteral("comboBoxEvaluatedData"));

        formLayout_10->setWidget(0, QFormLayout::FieldRole, comboBoxEvaluatedData);


        gridLayout_5->addLayout(formLayout_10, 0, 1, 1, 1);


        gridLayout->addLayout(gridLayout_5, 0, 0, 1, 1);

        tableWidget = new QTableWidget(ResultWindow);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        gridLayout->addWidget(tableWidget, 1, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(ResultWindow);

        QMetaObject::connectSlotsByName(ResultWindow);
    } // setupUi

    void retranslateUi(QDialog *ResultWindow)
    {
        ResultWindow->setWindowTitle(QApplication::translate("ResultWindow", "Results window", 0));
        selectPatientLabel->setText(QApplication::translate("ResultWindow", "Select patient:", 0));
        selectEvaluatedDataLabel->setText(QApplication::translate("ResultWindow", "Select evaluated data:", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ResultWindow", "#Slice", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ResultWindow", "Dice", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ResultWindow", "Jaccard", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ResultWindow", "TP", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("ResultWindow", "FP", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("ResultWindow", "FN", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("ResultWindow", "TN", 0));
    } // retranslateUi

};

namespace Ui {
    class ResultWindow: public Ui_ResultWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTWINDOW_H
