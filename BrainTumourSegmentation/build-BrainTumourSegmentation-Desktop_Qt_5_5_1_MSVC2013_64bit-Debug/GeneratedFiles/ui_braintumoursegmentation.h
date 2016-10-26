/********************************************************************************
** Form generated from reading UI file 'braintumoursegmentation.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRAINTUMOURSEGMENTATION_H
#define UI_BRAINTUMOURSEGMENTATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BrainTumourSegmentationClass
{
public:
    QAction *actionOpen_image_s;
    QAction *actionAuthor;
    QAction *actionThreshold;
    QAction *actionSuperpixels;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QTreeWidget *treeWidget;
    QGridLayout *gridLayoutView;
    QHBoxLayout *horizontalLayoutTL;
    QLabel *labelTL;
    QSlider *verticalSliderTL;
    QHBoxLayout *horizontalLayoutBL;
    QLabel *labelBL;
    QSlider *verticalSliderBL;
    QHBoxLayout *horizontalLayoutBR;
    QLabel *labelBR;
    QSlider *verticalSliderBR;
    QHBoxLayout *horizontalLayoutTR;
    QLabel *labelTR;
    QSlider *verticalSliderTR;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *BrainTumourSegmentationClass)
    {
        if (BrainTumourSegmentationClass->objectName().isEmpty())
            BrainTumourSegmentationClass->setObjectName(QStringLiteral("BrainTumourSegmentationClass"));
        BrainTumourSegmentationClass->resize(1023, 696);
        actionOpen_image_s = new QAction(BrainTumourSegmentationClass);
        actionOpen_image_s->setObjectName(QStringLiteral("actionOpen_image_s"));
        actionAuthor = new QAction(BrainTumourSegmentationClass);
        actionAuthor->setObjectName(QStringLiteral("actionAuthor"));
        actionThreshold = new QAction(BrainTumourSegmentationClass);
        actionThreshold->setObjectName(QStringLiteral("actionThreshold"));
        actionSuperpixels = new QAction(BrainTumourSegmentationClass);
        actionSuperpixels->setObjectName(QStringLiteral("actionSuperpixels"));
        centralWidget = new QWidget(BrainTumourSegmentationClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        treeWidget = new QTreeWidget(centralWidget);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::NoBrush);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setForeground(0, brush);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setMaximumSize(QSize(250, 100000));
        treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        gridLayout_4->addWidget(treeWidget, 0, 0, 1, 1);

        gridLayoutView = new QGridLayout();
        gridLayoutView->setSpacing(6);
        gridLayoutView->setObjectName(QStringLiteral("gridLayoutView"));
        horizontalLayoutTL = new QHBoxLayout();
        horizontalLayoutTL->setSpacing(6);
        horizontalLayoutTL->setObjectName(QStringLiteral("horizontalLayoutTL"));
        labelTL = new QLabel(centralWidget);
        labelTL->setObjectName(QStringLiteral("labelTL"));

        horizontalLayoutTL->addWidget(labelTL);

        verticalSliderTL = new QSlider(centralWidget);
        verticalSliderTL->setObjectName(QStringLiteral("verticalSliderTL"));
        verticalSliderTL->setOrientation(Qt::Vertical);

        horizontalLayoutTL->addWidget(verticalSliderTL);


        gridLayoutView->addLayout(horizontalLayoutTL, 1, 1, 1, 1);

        horizontalLayoutBL = new QHBoxLayout();
        horizontalLayoutBL->setSpacing(6);
        horizontalLayoutBL->setObjectName(QStringLiteral("horizontalLayoutBL"));
        labelBL = new QLabel(centralWidget);
        labelBL->setObjectName(QStringLiteral("labelBL"));

        horizontalLayoutBL->addWidget(labelBL);

        verticalSliderBL = new QSlider(centralWidget);
        verticalSliderBL->setObjectName(QStringLiteral("verticalSliderBL"));
        verticalSliderBL->setOrientation(Qt::Vertical);

        horizontalLayoutBL->addWidget(verticalSliderBL);


        gridLayoutView->addLayout(horizontalLayoutBL, 2, 1, 1, 1);

        horizontalLayoutBR = new QHBoxLayout();
        horizontalLayoutBR->setSpacing(6);
        horizontalLayoutBR->setObjectName(QStringLiteral("horizontalLayoutBR"));
        labelBR = new QLabel(centralWidget);
        labelBR->setObjectName(QStringLiteral("labelBR"));

        horizontalLayoutBR->addWidget(labelBR);

        verticalSliderBR = new QSlider(centralWidget);
        verticalSliderBR->setObjectName(QStringLiteral("verticalSliderBR"));
        verticalSliderBR->setOrientation(Qt::Vertical);

        horizontalLayoutBR->addWidget(verticalSliderBR);


        gridLayoutView->addLayout(horizontalLayoutBR, 2, 2, 1, 1);

        horizontalLayoutTR = new QHBoxLayout();
        horizontalLayoutTR->setSpacing(6);
        horizontalLayoutTR->setObjectName(QStringLiteral("horizontalLayoutTR"));
        labelTR = new QLabel(centralWidget);
        labelTR->setObjectName(QStringLiteral("labelTR"));

        horizontalLayoutTR->addWidget(labelTR);

        verticalSliderTR = new QSlider(centralWidget);
        verticalSliderTR->setObjectName(QStringLiteral("verticalSliderTR"));
        verticalSliderTR->setOrientation(Qt::Vertical);

        horizontalLayoutTR->addWidget(verticalSliderTR);


        gridLayoutView->addLayout(horizontalLayoutTR, 1, 2, 1, 1);


        gridLayout_4->addLayout(gridLayoutView, 0, 1, 2, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_4->addWidget(pushButton, 1, 0, 1, 1);

        BrainTumourSegmentationClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(BrainTumourSegmentationClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1023, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        BrainTumourSegmentationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(BrainTumourSegmentationClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        BrainTumourSegmentationClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(BrainTumourSegmentationClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        BrainTumourSegmentationClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen_image_s);
        menuTools->addAction(actionThreshold);
        menuTools->addAction(actionSuperpixels);
        menuAbout->addAction(actionAuthor);

        retranslateUi(BrainTumourSegmentationClass);

        QMetaObject::connectSlotsByName(BrainTumourSegmentationClass);
    } // setupUi

    void retranslateUi(QMainWindow *BrainTumourSegmentationClass)
    {
        BrainTumourSegmentationClass->setWindowTitle(QApplication::translate("BrainTumourSegmentationClass", "Brain Tumour Segmentation", 0));
        actionOpen_image_s->setText(QApplication::translate("BrainTumourSegmentationClass", "Open image(s)", 0));
        actionAuthor->setText(QApplication::translate("BrainTumourSegmentationClass", "Author", 0));
        actionThreshold->setText(QApplication::translate("BrainTumourSegmentationClass", "Threshold", 0));
        actionSuperpixels->setText(QApplication::translate("BrainTumourSegmentationClass", "Superpixels", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("BrainTumourSegmentationClass", "Image(s)", 0));
        labelTL->setText(QString());
        labelBL->setText(QString());
        labelBR->setText(QString());
        labelTR->setText(QString());
        pushButton->setText(QApplication::translate("BrainTumourSegmentationClass", "PushButton", 0));
        menuFile->setTitle(QApplication::translate("BrainTumourSegmentationClass", "File", 0));
        menuTools->setTitle(QApplication::translate("BrainTumourSegmentationClass", "Tools", 0));
        menuAbout->setTitle(QApplication::translate("BrainTumourSegmentationClass", "About", 0));
    } // retranslateUi

};

namespace Ui {
    class BrainTumourSegmentationClass: public Ui_BrainTumourSegmentationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRAINTUMOURSEGMENTATION_H
