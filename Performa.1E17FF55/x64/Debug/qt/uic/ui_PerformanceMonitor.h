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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PerformanceMonitorClass
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QPushButton *pushButton;
    QTabWidget *tabWidget;
    QWidget *TabGeneral;
    QGridLayout *gridLayout_3;
    QCheckBox *checkBoxAutoStart;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBoxPass;
    QLabel *label_5;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QComboBox *comboBox;
    QLabel *label;
    QComboBox *comboBox_2;
    QLabel *label_2;
    QComboBox *comboBox_3;
    QLabel *label_3;
    QComboBox *comboBox_4;
    QLabel *label_4;
    QWidget *TabCpu;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QCheckBox *showCPU;
    QCheckBox *checkBox_3;
    QComboBox *graphCPU;
    QCheckBox *doubleLineCPU;
    QCheckBox *backCPU;
    QCheckBox *showForEachCPU;
    QSpacerItem *horizontalSpacer;
    QComboBox *sizeCPU;
    QCheckBox *checkBox_4;
    QWidget *tabMemory;
    QWidget *tabDisk;
    QWidget *tabNetwork;

    void setupUi(QDialog *PerformanceMonitorClass)
    {
        if (PerformanceMonitorClass->objectName().isEmpty())
            PerformanceMonitorClass->setObjectName("PerformanceMonitorClass");
        PerformanceMonitorClass->resize(325, 374);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PerformanceMonitor/Icon/AppIcon.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        PerformanceMonitorClass->setWindowIcon(icon);
        gridLayout = new QGridLayout(PerformanceMonitorClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        buttonBox = new QDialogButtonBox(PerformanceMonitorClass);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 1, 1, 1, 1);

        pushButton = new QPushButton(PerformanceMonitorClass);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);

        tabWidget = new QTabWidget(PerformanceMonitorClass);
        tabWidget->setObjectName("tabWidget");
        TabGeneral = new QWidget();
        TabGeneral->setObjectName("TabGeneral");
        gridLayout_3 = new QGridLayout(TabGeneral);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName("gridLayout_3");
        checkBoxAutoStart = new QCheckBox(TabGeneral);
        checkBoxAutoStart->setObjectName("checkBoxAutoStart");

        gridLayout_3->addWidget(checkBoxAutoStart, 0, 0, 1, 1);

        groupBox = new QGroupBox(TabGeneral);
        groupBox->setObjectName("groupBox");
        groupBox->setMinimumSize(QSize(283, 80));
        groupBox->setMaximumSize(QSize(283, 81));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        checkBoxPass = new QCheckBox(groupBox);
        checkBoxPass->setObjectName("checkBoxPass");
        checkBoxPass->setAcceptDrops(false);

        horizontalLayout->addWidget(checkBoxPass);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setWordWrap(true);

        horizontalLayout->addWidget(label_5);


        gridLayout_3->addWidget(groupBox, 1, 0, 1, 2);

        checkBox = new QCheckBox(TabGeneral);
        checkBox->setObjectName("checkBox");

        gridLayout_3->addWidget(checkBox, 2, 0, 1, 1);

        checkBox_2 = new QCheckBox(TabGeneral);
        checkBox_2->setObjectName("checkBox_2");

        gridLayout_3->addWidget(checkBox_2, 3, 0, 1, 1);

        comboBox = new QComboBox(TabGeneral);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");

        gridLayout_3->addWidget(comboBox, 4, 0, 1, 1);

        label = new QLabel(TabGeneral);
        label->setObjectName("label");

        gridLayout_3->addWidget(label, 4, 1, 1, 1);

        comboBox_2 = new QComboBox(TabGeneral);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");

        gridLayout_3->addWidget(comboBox_2, 5, 0, 1, 1);

        label_2 = new QLabel(TabGeneral);
        label_2->setObjectName("label_2");

        gridLayout_3->addWidget(label_2, 5, 1, 1, 1);

        comboBox_3 = new QComboBox(TabGeneral);
        comboBox_3->setObjectName("comboBox_3");

        gridLayout_3->addWidget(comboBox_3, 6, 0, 1, 1);

        label_3 = new QLabel(TabGeneral);
        label_3->setObjectName("label_3");

        gridLayout_3->addWidget(label_3, 6, 1, 1, 1);

        comboBox_4 = new QComboBox(TabGeneral);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");

        gridLayout_3->addWidget(comboBox_4, 7, 0, 1, 1);

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
        verticalSpacer = new QSpacerItem(224, 60, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 9, 0, 1, 2);

        showCPU = new QCheckBox(TabCpu);
        showCPU->setObjectName("showCPU");

        gridLayout_2->addWidget(showCPU, 0, 0, 1, 1);

        checkBox_3 = new QCheckBox(TabCpu);
        checkBox_3->setObjectName("checkBox_3");

        gridLayout_2->addWidget(checkBox_3, 7, 0, 1, 1);

        graphCPU = new QComboBox(TabCpu);
        graphCPU->addItem(QString());
        graphCPU->addItem(QString());
        graphCPU->setObjectName("graphCPU");

        gridLayout_2->addWidget(graphCPU, 1, 0, 1, 1);

        doubleLineCPU = new QCheckBox(TabCpu);
        doubleLineCPU->setObjectName("doubleLineCPU");

        gridLayout_2->addWidget(doubleLineCPU, 2, 0, 1, 1);

        backCPU = new QCheckBox(TabCpu);
        backCPU->setObjectName("backCPU");

        gridLayout_2->addWidget(backCPU, 5, 0, 1, 1);

        showForEachCPU = new QCheckBox(TabCpu);
        showForEachCPU->setObjectName("showForEachCPU");

        gridLayout_2->addWidget(showForEachCPU, 4, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(47, 129, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 5, 1);

        sizeCPU = new QComboBox(TabCpu);
        sizeCPU->setObjectName("sizeCPU");

        gridLayout_2->addWidget(sizeCPU, 3, 0, 1, 1);

        checkBox_4 = new QCheckBox(TabCpu);
        checkBox_4->setObjectName("checkBox_4");

        gridLayout_2->addWidget(checkBox_4, 6, 0, 1, 1);

        tabWidget->addTab(TabCpu, QString());
        tabMemory = new QWidget();
        tabMemory->setObjectName("tabMemory");
        tabWidget->addTab(tabMemory, QString());
        tabDisk = new QWidget();
        tabDisk->setObjectName("tabDisk");
        tabWidget->addTab(tabDisk, QString());
        tabNetwork = new QWidget();
        tabNetwork->setObjectName("tabNetwork");
        tabWidget->addTab(tabNetwork, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 2);


        retranslateUi(PerformanceMonitorClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PerformanceMonitorClass);
    } // setupUi

    void retranslateUi(QDialog *PerformanceMonitorClass)
    {
        PerformanceMonitorClass->setWindowTitle(QCoreApplication::translate("PerformanceMonitorClass", "Performance Monitor 5", nullptr));
        pushButton->setText(QCoreApplication::translate("PerformanceMonitorClass", "Reset", nullptr));
        checkBoxAutoStart->setText(QCoreApplication::translate("PerformanceMonitorClass", "Autostart with Windows", nullptr));
        groupBox->setTitle(QCoreApplication::translate("PerformanceMonitorClass", "Pass-Through Mode. ", nullptr));
        checkBoxPass->setText(QString());
        label_5->setText(QCoreApplication::translate("PerformanceMonitorClass", "Mouse actions will not interfere with panels on desktop, only the tray icon responds to mouse.", nullptr));
        checkBox->setText(QCoreApplication::translate("PerformanceMonitorClass", "Always on Top", nullptr));
        checkBox_2->setText(QCoreApplication::translate("PerformanceMonitorClass", "Always show labels", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("PerformanceMonitorClass", "Ultra Fast -1 sec", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("PerformanceMonitorClass", "Fast - 1.5 sec", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("PerformanceMonitorClass", "Normal - 2 sec", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("PerformanceMonitorClass", "Medium - 2.5 sec", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("PerformanceMonitorClass", "Slow - 5 sec", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("PerformanceMonitorClass", "Very Slow -10 sec", nullptr));

        label->setText(QCoreApplication::translate("PerformanceMonitorClass", "Update Interval", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("PerformanceMonitorClass", "Below Normal", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("PerformanceMonitorClass", "Normal", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("PerformanceMonitorClass", "Above Normal", nullptr));

        label_2->setText(QCoreApplication::translate("PerformanceMonitorClass", "Priority", nullptr));
        label_3->setText(QCoreApplication::translate("PerformanceMonitorClass", "Visibility", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("PerformanceMonitorClass", "Glass 1", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("PerformanceMonitorClass", "Glass 2", nullptr));
        comboBox_4->setItemText(2, QCoreApplication::translate("PerformanceMonitorClass", "Flat", nullptr));

        label_4->setText(QCoreApplication::translate("PerformanceMonitorClass", "Style", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabGeneral), QCoreApplication::translate("PerformanceMonitorClass", "General", nullptr));
        showCPU->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show this Panel", nullptr));
        checkBox_3->setText(QCoreApplication::translate("PerformanceMonitorClass", "Foreground Color", nullptr));
        graphCPU->setItemText(0, QCoreApplication::translate("PerformanceMonitorClass", "Line", nullptr));
        graphCPU->setItemText(1, QCoreApplication::translate("PerformanceMonitorClass", "Pile", nullptr));

        doubleLineCPU->setText(QCoreApplication::translate("PerformanceMonitorClass", "Double Line", nullptr));
        backCPU->setText(QCoreApplication::translate("PerformanceMonitorClass", "Background Color", nullptr));
        showForEachCPU->setText(QCoreApplication::translate("PerformanceMonitorClass", "Show One Graph for every CPU/Core", nullptr));
        checkBox_4->setText(QCoreApplication::translate("PerformanceMonitorClass", "Manual Foreground", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(TabCpu), QCoreApplication::translate("PerformanceMonitorClass", "Cpu", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMemory), QCoreApplication::translate("PerformanceMonitorClass", "Memory", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabDisk), QCoreApplication::translate("PerformanceMonitorClass", "Disk", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabNetwork), QCoreApplication::translate("PerformanceMonitorClass", "Network", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PerformanceMonitorClass: public Ui_PerformanceMonitorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERFORMANCEMONITOR_H
