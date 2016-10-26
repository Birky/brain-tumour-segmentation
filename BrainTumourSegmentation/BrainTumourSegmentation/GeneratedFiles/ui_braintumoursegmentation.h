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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
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
    QAction *actionConnect_sliders;
    QAction *actionGlobal_local_normalization;
    QAction *actionSegmented_Evaluated_data;
    QAction *actionShow_results;
    QAction *actionImage_arithm;
    QAction *actionDo_the_segmentation;
    QAction *actionComplex_segmentation;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QTreeWidget *treeWidget;
    QGridLayout *gridLayoutView;
    QFrame *line;
    QGridLayout *gridLayout;
    QLabel *labelTL;
    QSlider *verticalSliderTL;
    QLabel *labelTitleTL;
    QLabel *labelResultsTL;
    QGridLayout *gridLayout_5;
    QLabel *labelBR;
    QSlider *verticalSliderBR;
    QLabel *labelTitleBR;
    QLabel *labelResultsBR;
    QGridLayout *gridLayout_3;
    QLabel *labelBL;
    QSlider *verticalSliderBL;
    QLabel *labelTitleBL;
    QLabel *labelResultsBL;
    QGridLayout *gridLayout_2;
    QSlider *verticalSliderTR;
    QLabel *labelTR;
    QLabel *labelTitleTR;
    QLabel *labelResultsTR;
    QFrame *line_2;
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
        BrainTumourSegmentationClass->resize(1023, 779);
        actionOpen_image_s = new QAction(BrainTumourSegmentationClass);
        actionOpen_image_s->setObjectName(QStringLiteral("actionOpen_image_s"));
        actionAuthor = new QAction(BrainTumourSegmentationClass);
        actionAuthor->setObjectName(QStringLiteral("actionAuthor"));
        actionThreshold = new QAction(BrainTumourSegmentationClass);
        actionThreshold->setObjectName(QStringLiteral("actionThreshold"));
        actionSuperpixels = new QAction(BrainTumourSegmentationClass);
        actionSuperpixels->setObjectName(QStringLiteral("actionSuperpixels"));
        actionConnect_sliders = new QAction(BrainTumourSegmentationClass);
        actionConnect_sliders->setObjectName(QStringLiteral("actionConnect_sliders"));
        actionConnect_sliders->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral("../icons/Disconnect.ico"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QStringLiteral("../icons/connect.ico"), QSize(), QIcon::Normal, QIcon::On);
        actionConnect_sliders->setIcon(icon);
        actionGlobal_local_normalization = new QAction(BrainTumourSegmentationClass);
        actionGlobal_local_normalization->setObjectName(QStringLiteral("actionGlobal_local_normalization"));
        actionGlobal_local_normalization->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral("../icons/webapp.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon1.addFile(QStringLiteral("../icons/local_seo-512.png"), QSize(), QIcon::Normal, QIcon::On);
        actionGlobal_local_normalization->setIcon(icon1);
        actionSegmented_Evaluated_data = new QAction(BrainTumourSegmentationClass);
        actionSegmented_Evaluated_data->setObjectName(QStringLiteral("actionSegmented_Evaluated_data"));
        actionSegmented_Evaluated_data->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral("../icons/Alecive-Flatwoken-Apps-Workspace-Switcher-Top-Left.ico"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QStringLiteral("../icons/Alecive-Flatwoken-Apps-Workspace-Switcher-Right-Top.ico"), QSize(), QIcon::Normal, QIcon::On);
        actionSegmented_Evaluated_data->setIcon(icon2);
        actionShow_results = new QAction(BrainTumourSegmentationClass);
        actionShow_results->setObjectName(QStringLiteral("actionShow_results"));
        actionImage_arithm = new QAction(BrainTumourSegmentationClass);
        actionImage_arithm->setObjectName(QStringLiteral("actionImage_arithm"));
        actionDo_the_segmentation = new QAction(BrainTumourSegmentationClass);
        actionDo_the_segmentation->setObjectName(QStringLiteral("actionDo_the_segmentation"));
        actionComplex_segmentation = new QAction(BrainTumourSegmentationClass);
        actionComplex_segmentation->setObjectName(QStringLiteral("actionComplex_segmentation"));
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
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayoutView->addWidget(line, 2, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelTL = new QLabel(centralWidget);
        labelTL->setObjectName(QStringLiteral("labelTL"));

        gridLayout->addWidget(labelTL, 1, 0, 1, 1);

        verticalSliderTL = new QSlider(centralWidget);
        verticalSliderTL->setObjectName(QStringLiteral("verticalSliderTL"));
        verticalSliderTL->setOrientation(Qt::Vertical);

        gridLayout->addWidget(verticalSliderTL, 1, 1, 1, 1);

        labelTitleTL = new QLabel(centralWidget);
        labelTitleTL->setObjectName(QStringLiteral("labelTitleTL"));

        gridLayout->addWidget(labelTitleTL, 0, 0, 1, 1);

        labelResultsTL = new QLabel(centralWidget);
        labelResultsTL->setObjectName(QStringLiteral("labelResultsTL"));

        gridLayout->addWidget(labelResultsTL, 2, 0, 1, 1);


        gridLayoutView->addLayout(gridLayout, 1, 1, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        labelBR = new QLabel(centralWidget);
        labelBR->setObjectName(QStringLiteral("labelBR"));

        gridLayout_5->addWidget(labelBR, 1, 0, 1, 1);

        verticalSliderBR = new QSlider(centralWidget);
        verticalSliderBR->setObjectName(QStringLiteral("verticalSliderBR"));
        verticalSliderBR->setOrientation(Qt::Vertical);

        gridLayout_5->addWidget(verticalSliderBR, 1, 1, 1, 1);

        labelTitleBR = new QLabel(centralWidget);
        labelTitleBR->setObjectName(QStringLiteral("labelTitleBR"));

        gridLayout_5->addWidget(labelTitleBR, 0, 0, 1, 1);

        labelResultsBR = new QLabel(centralWidget);
        labelResultsBR->setObjectName(QStringLiteral("labelResultsBR"));

        gridLayout_5->addWidget(labelResultsBR, 2, 0, 1, 1);


        gridLayoutView->addLayout(gridLayout_5, 3, 2, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        labelBL = new QLabel(centralWidget);
        labelBL->setObjectName(QStringLiteral("labelBL"));

        gridLayout_3->addWidget(labelBL, 1, 0, 1, 1);

        verticalSliderBL = new QSlider(centralWidget);
        verticalSliderBL->setObjectName(QStringLiteral("verticalSliderBL"));
        verticalSliderBL->setOrientation(Qt::Vertical);

        gridLayout_3->addWidget(verticalSliderBL, 1, 1, 1, 1);

        labelTitleBL = new QLabel(centralWidget);
        labelTitleBL->setObjectName(QStringLiteral("labelTitleBL"));

        gridLayout_3->addWidget(labelTitleBL, 0, 0, 1, 1);

        labelResultsBL = new QLabel(centralWidget);
        labelResultsBL->setObjectName(QStringLiteral("labelResultsBL"));

        gridLayout_3->addWidget(labelResultsBL, 2, 0, 1, 1);


        gridLayoutView->addLayout(gridLayout_3, 3, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSliderTR = new QSlider(centralWidget);
        verticalSliderTR->setObjectName(QStringLiteral("verticalSliderTR"));
        verticalSliderTR->setOrientation(Qt::Vertical);

        gridLayout_2->addWidget(verticalSliderTR, 1, 1, 1, 1);

        labelTR = new QLabel(centralWidget);
        labelTR->setObjectName(QStringLiteral("labelTR"));

        gridLayout_2->addWidget(labelTR, 1, 0, 1, 1);

        labelTitleTR = new QLabel(centralWidget);
        labelTitleTR->setObjectName(QStringLiteral("labelTitleTR"));

        gridLayout_2->addWidget(labelTitleTR, 0, 0, 1, 1);

        labelResultsTR = new QLabel(centralWidget);
        labelResultsTR->setObjectName(QStringLiteral("labelResultsTR"));

        gridLayout_2->addWidget(labelResultsTR, 2, 0, 1, 1);


        gridLayoutView->addLayout(gridLayout_2, 1, 2, 1, 1);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayoutView->addWidget(line_2, 2, 2, 1, 1);


        gridLayout_4->addLayout(gridLayoutView, 0, 1, 2, 1);

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
        BrainTumourSegmentationClass->addToolBar(Qt::RightToolBarArea, mainToolBar);
        statusBar = new QStatusBar(BrainTumourSegmentationClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        BrainTumourSegmentationClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionOpen_image_s);
        menuTools->addAction(actionThreshold);
        menuTools->addAction(actionImage_arithm);
        menuTools->addAction(actionSuperpixels);
        menuTools->addAction(actionShow_results);
        menuTools->addAction(actionDo_the_segmentation);
        menuTools->addAction(actionComplex_segmentation);
        menuAbout->addAction(actionAuthor);
        mainToolBar->addAction(actionConnect_sliders);
        mainToolBar->addAction(actionGlobal_local_normalization);
        mainToolBar->addAction(actionSegmented_Evaluated_data);

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
        actionConnect_sliders->setText(QApplication::translate("BrainTumourSegmentationClass", "Connect/disconnect sliders", 0));
#ifndef QT_NO_TOOLTIP
        actionConnect_sliders->setToolTip(QApplication::translate("BrainTumourSegmentationClass", "Connect/disconnect sliders", 0));
#endif // QT_NO_TOOLTIP
        actionGlobal_local_normalization->setText(QApplication::translate("BrainTumourSegmentationClass", "Global/local normalization", 0));
#ifndef QT_NO_TOOLTIP
        actionGlobal_local_normalization->setToolTip(QApplication::translate("BrainTumourSegmentationClass", "Global/local normalization", 0));
#endif // QT_NO_TOOLTIP
        actionSegmented_Evaluated_data->setText(QApplication::translate("BrainTumourSegmentationClass", "Segmented/Evaluated data", 0));
#ifndef QT_NO_TOOLTIP
        actionSegmented_Evaluated_data->setToolTip(QApplication::translate("BrainTumourSegmentationClass", "<html><head/><body><p>Segmented/Evaluated processed data</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        actionShow_results->setText(QApplication::translate("BrainTumourSegmentationClass", "Show results", 0));
        actionImage_arithm->setText(QApplication::translate("BrainTumourSegmentationClass", "Image arithm", 0));
        actionDo_the_segmentation->setText(QApplication::translate("BrainTumourSegmentationClass", "Do the segmentation", 0));
        actionComplex_segmentation->setText(QApplication::translate("BrainTumourSegmentationClass", "Complex segmentation", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("BrainTumourSegmentationClass", "Image(s)", 0));
        labelTL->setText(QString());
        labelTitleTL->setText(QString());
        labelResultsTL->setText(QString());
        labelBR->setText(QString());
        labelTitleBR->setText(QString());
        labelResultsBR->setText(QString());
        labelBL->setText(QString());
        labelTitleBL->setText(QString());
        labelResultsBL->setText(QString());
        labelTR->setText(QString());
        labelTitleTR->setText(QString());
        labelResultsTR->setText(QString());
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
