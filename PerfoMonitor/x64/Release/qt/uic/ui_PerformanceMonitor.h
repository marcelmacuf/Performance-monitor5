/********************************************************************************
** Form generated from reading UI file 'PerformanceMonitor.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERFORMANCEMONITOR_H
#define UI_PERFORMANCEMONITOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "ColorButton.h"
#include "MySlider.h"

QT_BEGIN_NAMESPACE

class Ui_PerformanceMonitorClass
{
public:
    QGridLayout *gridLayout_6;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *TabGeneral;
    QGridLayout *gridLayout_3;
    QCheckBox *chAutoStart;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QCheckBox *chPass;
    QLabel *label_5;
    QCheckBox *chAllwaysTop;
    QCheckBox *chShowLabel;
    QComboBox *cbUpdateInterval;
    QLabel *label;
    QComboBox *cbPriority;
    QLabel *label_2;
    QComboBox *cbVisibility;
    QLabel *label_3;
    QComboBox *cbStyle;
    QLabel *label_4;
    QWidget *TabCpu;
    QGridLayout *gridLayout_2;
    QLabel *label_9;
    QCheckBox *showForEachCPU;
    ColorButton *pbLineCPU;
    ColorButton *pbBackCPU;
    QComboBox *lineSizeCPU;
    QLabel *label_14;
    QSpacerItem *verticalSpacer;
    QComboBox *graphCPU;
    QCheckBox *showCPU;
    QLabel *label_8;
    QCheckBox *chCPUManual;
    QComboBox *sizeCPU;
    MySlider *coresCPU;
    QLabel *label_18;
    QWidget *tabMemory;
    QGridLayout *gridLayout_4;
    QCheckBox *showRAM;
    QComboBox *graphRAM;
    QComboBox *sizeRAM;
    QLabel *label_10;
    QCheckBox *chRAMManual;
    QLabel *label_11;
    QSpacerItem *verticalSpacer_5;
    QComboBox *lineSizeRAM;
    QLabel *label_15;
    ColorButton *pbBackRAM;
    ColorButton *pbLineRAM;
    QWidget *tabDisk;
    QGridLayout *gridLayout_5;
    QCheckBox *showDisk;
    QComboBox *graphDisk;
    QComboBox *lineSizeDisk;
    QLabel *label_16;
    QComboBox *sizeDisk;
    ColorButton *pbBackDisk;
    QLabel *label_12;
    QCheckBox *chDiskManual;
    ColorButton *pbLineDisk;
    QLabel *label_13;
    QCheckBox *separateGrpahsDisk;
    QSpacerItem *verticalSpacer_4;
    QWidget *tabNetwork;
    QGridLayout *gridLayout;
    QComboBox *sizeNET;
    QComboBox *graphNET;
    QCheckBox *showNET;
    QCheckBox *separateGrpahsNET;
    ColorButton *pbBackNET;
    QLabel *label_7;
    QComboBox *lineSizeNET;
    QLabel *label_17;
    QLabel *label_27;
    QSpacerItem *verticalSpacer_3;
    QCheckBox *chNETManual;
    QComboBox *maxBandwithNET;
    ColorButton *pbLineNET;
    QComboBox *maxBandwithNET_2;
    QLabel *label_28;
    QLabel *label_6;

    void setupUi(QDialog *PerformanceMonitorClass)
    {
        if (PerformanceMonitorClass->objectName().isEmpty())
            PerformanceMonitorClass->setObjectName("PerformanceMonitorClass");
        PerformanceMonitorClass->resize(336, 406);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PerformanceMonitor/Icon/AppIcon.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        PerformanceMonitorClass->setWindowIcon(icon);
        gridLayout_6 = new QGridLayout(PerformanceMonitorClass);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName("gridLayout_6");
        buttonBox = new QDialogButtonBox(PerformanceMonitorClass);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Apply|QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok|QDialogButtonBox::StandardButton::Reset);

        gridLayout_6->addWidget(buttonBox, 2, 1, 1, 1);

        tabWidget = new QTabWidget(PerformanceMonitorClass);
        tabWidget->setObjectName("tabWidget");
        TabGeneral = new QWidget();
        TabGeneral->setObjectName("TabGeneral");
        gridLayout_3 = new QGridLayout(TabGeneral);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName("gridLayout_3");
        chAutoStart = new QCheckBox(TabGeneral);
        chAutoStart->setObjectName("chAutoStart");

        gridLayout_3->addWidget(chAutoStart, 0, 0, 1, 1);

        groupBox = new QGroupBox(TabGeneral);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(283, 80));
        groupBox->setMaximumSize(QSize(283, 81));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        chPass = new QCheckBox(groupBox);
        chPass->setObjectName("chPass");
        chPass->setAcceptDrops(false);

        horizontalLayout->addWidget(chPass);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setWordWrap(true);

        horizontalLayout->addWidget(label_5);


        gridLayout_3->addWidget(groupBox, 1, 0, 1, 2);

        chAllwaysTop = new QCheckBox(TabGeneral);
        chAllwaysTop->setObjectName("chAllwaysTop");

        gridLayout_3->addWidget(chAllwaysTop, 2, 0, 1, 1);

        chShowLabel = new QCheckBox(TabGeneral);
        chShowLabel->setObjectName("chShowLabel");

        gridLayout_3->addWidget(chShowLabel, 3, 0, 1, 1);

        cbUpdateInterval = new QComboBox(TabGeneral);
        cbUpdateInterval->setObjectName("cbUpdateInterval");

        gridLayout_3->addWidget(cbUpdateInterval, 4, 0, 1, 1);

        label = new QLabel(TabGeneral);
        label->setObjectName("label");

        gridLayout_3->addWidget(label, 4, 1, 1, 1);

        cbPriority = new QComboBox(TabGeneral);
        cbPriority->setObjectName("cbPriority");

        gridLayout_3->addWidget(cbPriority, 5, 0, 1, 1);

        label_2 = new QLabel(TabGeneral);
        label_2->setObjectName("label_2");

        gridLayout_3->addWidget(label_2, 5, 1, 1, 1);

        cbVisibility = new QComboBox(TabGeneral);
        cbVisibility->setObjectName("cbVisibility");

        gridLayout_3->addWidget(cbVisibility, 6, 0, 1, 1);

        label_3 = new QLabel(TabGeneral);
        label_3->setObjectName("label_3");

        gridLayout_3->addWidget(label_3, 6, 1, 1, 1);

        cbStyle = new QComboBox(TabGeneral);
        cbStyle->setObjectName("cbStyle");

        gridLayout_3->addWidget(cbStyle, 7, 0, 1, 1);

        label_4 = new QLabel(TabGeneral);
        label_4->setObjectName("label_4");

        gridLayout_3->addWidget(label_4, 7, 1, 1, 1);

        tabWidget->addTab(TabGeneral, QString());
        TabCpu = new QWidget();
        TabCpu->setObjectName("TabCpu");
        gridLayout_2 = new QGridLayout(TabCpu);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName("gridLayout_2");
        label_9 = new QLabel(TabCpu);
        label_9->setObjectName("label_9");

        gridLayout_2->addWidget(label_9, 6, 1, 1, 1);

        showForEachCPU = new QCheckBox(TabCpu);
        showForEachCPU->setObjectName("showForEachCPU");

        gridLayout_2->addWidget(showForEachCPU, 7, 0, 1, 2);

        pbLineCPU = new ColorButton(TabCpu);
        pbLineCPU->setObjectName("pbLineCPU");
        pbLineCPU->setFlat(true);

        gridLayout_2->addWidget(pbLineCPU, 6, 0, 1, 1);

        pbBackCPU = new ColorButton(TabCpu);
        pbBackCPU->setObjectName("pbBackCPU");
        pbBackCPU->setFlat(true);

        gridLayout_2->addWidget(pbBackCPU, 4, 0, 1, 1);

        lineSizeCPU = new QComboBox(TabCpu);
        lineSizeCPU->setObjectName("lineSizeCPU");

        gridLayout_2->addWidget(lineSizeCPU, 2, 0, 1, 1);

        label_14 = new QLabel(TabCpu);
        label_14->setObjectName("label_14");

        gridLayout_2->addWidget(label_14, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(224, 60, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 9, 0, 1, 2);

        graphCPU = new QComboBox(TabCpu);
        graphCPU->setObjectName("graphCPU");

        gridLayout_2->addWidget(graphCPU, 1, 0, 1, 1);

        showCPU = new QCheckBox(TabCpu);
        showCPU->setObjectName("showCPU");

        gridLayout_2->addWidget(showCPU, 0, 0, 1, 1);

        label_8 = new QLabel(TabCpu);
        label_8->setObjectName("label_8");

        gridLayout_2->addWidget(label_8, 4, 1, 1, 1);

        chCPUManual = new QCheckBox(TabCpu);
        chCPUManual->setObjectName("chCPUManual");

        gridLayout_2->addWidget(chCPUManual, 5, 0, 1, 2);

        sizeCPU = new QComboBox(TabCpu);
        sizeCPU->setObjectName("sizeCPU");

        gridLayout_2->addWidget(sizeCPU, 3, 0, 1, 1);

        coresCPU = new MySlider(TabCpu);
        coresCPU->setObjectName("coresCPU");
        coresCPU->setMinimumSize(QSize(0, 30));
        coresCPU->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(coresCPU, 8, 0, 1, 1);

        label_18 = new QLabel(TabCpu);
        label_18->setObjectName("label_18");

        gridLayout_2->addWidget(label_18, 8, 1, 1, 1);

        tabWidget->addTab(TabCpu, QString());
        tabMemory = new QWidget();
        tabMemory->setObjectName("tabMemory");
        gridLayout_4 = new QGridLayout(tabMemory);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName("gridLayout_4");
        showRAM = new QCheckBox(tabMemory);
        showRAM->setObjectName("showRAM");

        gridLayout_4->addWidget(showRAM, 0, 0, 1, 2);

        graphRAM = new QComboBox(tabMemory);
        graphRAM->setObjectName("graphRAM");

        gridLayout_4->addWidget(graphRAM, 1, 0, 1, 2);

        sizeRAM = new QComboBox(tabMemory);
        sizeRAM->setObjectName("sizeRAM");

        gridLayout_4->addWidget(sizeRAM, 3, 0, 1, 2);

        label_10 = new QLabel(tabMemory);
        label_10->setObjectName("label_10");

        gridLayout_4->addWidget(label_10, 4, 2, 1, 2);

        chRAMManual = new QCheckBox(tabMemory);
        chRAMManual->setObjectName("chRAMManual");

        gridLayout_4->addWidget(chRAMManual, 5, 0, 1, 2);

        label_11 = new QLabel(tabMemory);
        label_11->setObjectName("label_11");

        gridLayout_4->addWidget(label_11, 6, 2, 1, 2);

        verticalSpacer_5 = new QSpacerItem(20, 106, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_4->addItem(verticalSpacer_5, 7, 1, 1, 1);

        lineSizeRAM = new QComboBox(tabMemory);
        lineSizeRAM->setObjectName("lineSizeRAM");

        gridLayout_4->addWidget(lineSizeRAM, 2, 0, 1, 2);

        label_15 = new QLabel(tabMemory);
        label_15->setObjectName("label_15");

        gridLayout_4->addWidget(label_15, 2, 2, 1, 2);

        pbBackRAM = new ColorButton(tabMemory);
        pbBackRAM->setObjectName("pbBackRAM");
        pbBackRAM->setFlat(true);

        gridLayout_4->addWidget(pbBackRAM, 4, 0, 1, 2);

        pbLineRAM = new ColorButton(tabMemory);
        pbLineRAM->setObjectName("pbLineRAM");
        pbLineRAM->setFlat(true);

        gridLayout_4->addWidget(pbLineRAM, 6, 0, 1, 2);

        tabWidget->addTab(tabMemory, QString());
        tabDisk = new QWidget();
        tabDisk->setObjectName("tabDisk");
        gridLayout_5 = new QGridLayout(tabDisk);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName("gridLayout_5");
        showDisk = new QCheckBox(tabDisk);
        showDisk->setObjectName("showDisk");

        gridLayout_5->addWidget(showDisk, 0, 0, 1, 1);

        graphDisk = new QComboBox(tabDisk);
        graphDisk->setObjectName("graphDisk");

        gridLayout_5->addWidget(graphDisk, 1, 0, 1, 1);

        lineSizeDisk = new QComboBox(tabDisk);
        lineSizeDisk->setObjectName("lineSizeDisk");

        gridLayout_5->addWidget(lineSizeDisk, 2, 0, 1, 1);

        label_16 = new QLabel(tabDisk);
        label_16->setObjectName("label_16");

        gridLayout_5->addWidget(label_16, 2, 1, 1, 1);

        sizeDisk = new QComboBox(tabDisk);
        sizeDisk->setObjectName("sizeDisk");

        gridLayout_5->addWidget(sizeDisk, 3, 0, 1, 1);

        pbBackDisk = new ColorButton(tabDisk);
        pbBackDisk->setObjectName("pbBackDisk");
        pbBackDisk->setFlat(true);

        gridLayout_5->addWidget(pbBackDisk, 4, 0, 1, 1);

        label_12 = new QLabel(tabDisk);
        label_12->setObjectName("label_12");

        gridLayout_5->addWidget(label_12, 4, 1, 1, 1);

        chDiskManual = new QCheckBox(tabDisk);
        chDiskManual->setObjectName("chDiskManual");

        gridLayout_5->addWidget(chDiskManual, 5, 0, 1, 1);

        pbLineDisk = new ColorButton(tabDisk);
        pbLineDisk->setObjectName("pbLineDisk");
        pbLineDisk->setFlat(true);

        gridLayout_5->addWidget(pbLineDisk, 6, 0, 1, 1);

        label_13 = new QLabel(tabDisk);
        label_13->setObjectName("label_13");

        gridLayout_5->addWidget(label_13, 6, 1, 1, 1);

        separateGrpahsDisk = new QCheckBox(tabDisk);
        separateGrpahsDisk->setObjectName("separateGrpahsDisk");

        gridLayout_5->addWidget(separateGrpahsDisk, 7, 0, 1, 2);

        verticalSpacer_4 = new QSpacerItem(20, 80, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_5->addItem(verticalSpacer_4, 8, 1, 1, 1);

        tabWidget->addTab(tabDisk, QString());
        tabNetwork = new QWidget();
        tabNetwork->setObjectName("tabNetwork");
        gridLayout = new QGridLayout(tabNetwork);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        sizeNET = new QComboBox(tabNetwork);
        sizeNET->setObjectName("sizeNET");

        gridLayout->addWidget(sizeNET, 3, 0, 1, 1);

        graphNET = new QComboBox(tabNetwork);
        graphNET->setObjectName("graphNET");

        gridLayout->addWidget(graphNET, 1, 0, 1, 1);

        showNET = new QCheckBox(tabNetwork);
        showNET->setObjectName("showNET");

        gridLayout->addWidget(showNET, 0, 0, 1, 1);

        separateGrpahsNET = new QCheckBox(tabNetwork);
        separateGrpahsNET->setObjectName("separateGrpahsNET");

        gridLayout->addWidget(separateGrpahsNET, 7, 0, 1, 2);

        pbBackNET = new ColorButton(tabNetwork);
        pbBackNET->setObjectName("pbBackNET");
        pbBackNET->setFlat(true);

        gridLayout->addWidget(pbBackNET, 4, 0, 1, 1);

        label_7 = new QLabel(tabNetwork);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 9, 0, 1, 1);

        lineSizeNET = new QComboBox(tabNetwork);
        lineSizeNET->setObjectName("lineSizeNET");

        gridLayout->addWidget(lineSizeNET, 2, 0, 1, 1);

        label_17 = new QLabel(tabNetwork);
        label_17->setObjectName("label_17");

        gridLayout->addWidget(label_17, 2, 1, 1, 1);

        label_27 = new QLabel(tabNetwork);
        label_27->setObjectName("label_27");

        gridLayout->addWidget(label_27, 6, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 80, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 11, 1, 1, 1);

        chNETManual = new QCheckBox(tabNetwork);
        chNETManual->setObjectName("chNETManual");

        gridLayout->addWidget(chNETManual, 5, 0, 1, 1);

        maxBandwithNET = new QComboBox(tabNetwork);
        maxBandwithNET->setObjectName("maxBandwithNET");

        gridLayout->addWidget(maxBandwithNET, 8, 0, 1, 1);

        pbLineNET = new ColorButton(tabNetwork);
        pbLineNET->setObjectName("pbLineNET");
        pbLineNET->setFlat(true);

        gridLayout->addWidget(pbLineNET, 6, 0, 1, 1);

        maxBandwithNET_2 = new QComboBox(tabNetwork);
        maxBandwithNET_2->setObjectName("maxBandwithNET_2");

        gridLayout->addWidget(maxBandwithNET_2, 10, 0, 1, 1);

        label_28 = new QLabel(tabNetwork);
        label_28->setObjectName("label_28");

        gridLayout->addWidget(label_28, 4, 1, 1, 1);

        label_6 = new QLabel(tabNetwork);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 8, 1, 1, 1);

        tabWidget->addTab(tabNetwork, QString());

        gridLayout_6->addWidget(tabWidget, 1, 1, 1, 1);


        retranslateUi(PerformanceMonitorClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PerformanceMonitorClass);
    } // setupUi

    void retranslateUi(QDialog *PerformanceMonitorClass)
    {
        PerformanceMonitorClass->setWindowTitle(QCoreApplication::translate("PerformanceMonitorClass", "Performance Monitor 5", nullptr));
        chAutoStart->setText(QCoreApplication::translate("PerformanceMonitorClass", "Autostart with Windows", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PerformanceMonitorClass", "Pass-Through Mode. ", nullptr));
        chPass->setText(QString());
        label_5->setText(QCoreApplication::translate("PerformanceMonitorClass", "Mouse actions will not interfere with panels on desktop, only the tray icon responds to mouse.", nullptr));
        chAllwaysTop->setText(QCoreApplication::translate("PerformanceMonitorClass", "Always on Top", nullptr));
        chShowLabel->setText(QCoreApplication::translate("PerformanceMonitorClass", "Always show labels", nullptr));
        label->setText(QCoreApplication::translate("PerformanceMonitorClass", "Update Interval", nullptr));
        label_2->setText(QCoreApplication::translate("PerformanceMonitorClass", "Priority", nullptr));
        label_3->setText(QCoreApplication::translate("PerformanceMonitorClass", "Visibility", nullptr));
        label_4->setText(QCoreApplication::translate("PerformanceMonitorClass", "Style", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabGeneral), QCoreApplication::translate("PerformanceMonitorClass", "General", nullptr));
        label_9->setText(QCoreApplication::translate("PerformanceMonitorClass", "Foreground Color", nullptr));
        showForEachCPU->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show One Graph for every CPU/Core", nullptr));
        pbLineCPU->setText(QString());
        pbBackCPU->setText(QString());
        label_14->setText(QCoreApplication::translate("PerformanceMonitorClass", "Line width", nullptr));
        showCPU->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show this Panel", nullptr));
        label_8->setText(QCoreApplication::translate("PerformanceMonitorClass", "Background Color", nullptr));
        chCPUManual->setText(QCoreApplication::translate("PerformanceMonitorClass", "Manual Foreground", nullptr));
        label_18->setText(QCoreApplication::translate("PerformanceMonitorClass", "First graph cores", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabCpu), QCoreApplication::translate("PerformanceMonitorClass", "Cpu", nullptr));
        showRAM->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show this Panel", nullptr));
        label_10->setText(QCoreApplication::translate("PerformanceMonitorClass", "Background Color", nullptr));
        chRAMManual->setText(QCoreApplication::translate("PerformanceMonitorClass", "Manual Foreground", nullptr));
        label_11->setText(QCoreApplication::translate("PerformanceMonitorClass", "Foreground Color", nullptr));
        label_15->setText(QCoreApplication::translate("PerformanceMonitorClass", "Line width", nullptr));
        pbBackRAM->setText(QString());
        pbLineRAM->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tabMemory), QCoreApplication::translate("PerformanceMonitorClass", "Memory", nullptr));
        showDisk->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show this Panel", nullptr));
        label_16->setText(QCoreApplication::translate("PerformanceMonitorClass", "Line width", nullptr));
        pbBackDisk->setText(QString());
        label_12->setText(QCoreApplication::translate("PerformanceMonitorClass", "Background Color", nullptr));
        chDiskManual->setText(QCoreApplication::translate("PerformanceMonitorClass", "Manual Foreground", nullptr));
        pbLineDisk->setText(QString());
        label_13->setText(QCoreApplication::translate("PerformanceMonitorClass", "Foreground Color", nullptr));
        separateGrpahsDisk->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show separate graphs for read/write", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabDisk), QCoreApplication::translate("PerformanceMonitorClass", "Disk", nullptr));
        showNET->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show this Panel", nullptr));
        separateGrpahsNET->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show separate graphs for up/dw", nullptr));
        pbBackNET->setText(QString());
        label_7->setText(QCoreApplication::translate("PerformanceMonitorClass", "Net Interface", nullptr));
        label_17->setText(QCoreApplication::translate("PerformanceMonitorClass", "Line width", nullptr));
        label_27->setText(QCoreApplication::translate("PerformanceMonitorClass", "Foreground Color", nullptr));
        chNETManual->setText(QCoreApplication::translate("PerformanceMonitorClass", "Manual Foreground", nullptr));
        pbLineNET->setText(QString());
        label_28->setText(QCoreApplication::translate("PerformanceMonitorClass", "Background Color", nullptr));
        label_6->setText(QCoreApplication::translate("PerformanceMonitorClass", "Max Bandwidth", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabNetwork), QCoreApplication::translate("PerformanceMonitorClass", "Network", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PerformanceMonitorClass: public Ui_PerformanceMonitorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERFORMANCEMONITOR_H
