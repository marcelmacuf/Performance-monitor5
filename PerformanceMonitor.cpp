#include "stdafx.h"
#include "PerformanceMonitor.h"
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <pdh.h>
#include <pdhmsg.h>

constexpr int c_unitWidth = 33;
constexpr int c_unitHeight = 21;
constexpr int c_defaultSizeIndex = 2;
constexpr char c_CPU[]{ "CPU" };
constexpr char c_RAM[]{ "RAM" };
constexpr char c_DISK[]{ "DISC" };
constexpr char c_NET[]{ "NET" };
const QColor c_CPUColor(0, 123, 183);
const QColor c_RAMColor(0, 128, 0);
const QColor c_DISKColor(176, 119, 0);
const QColor c_NETColor(149, 149, 37);

namespace
{
	class NetworkBandwithItem
	{
	public:
		NetworkBandwithItem(const QString str, const int magnitude, const int size) : m_str(str), m_magnitude(magnitude), m_size(size)
		{
		}
		const QString m_str;
		const int m_magnitude = 1;
		const double m_size = 1;
	};
	const NetworkBandwithItem c_BandwithList[]
	{
		{ "Auto", 0, 0},
		{ "Dailup (56Kb)", 1, 56 },
		{ "DSL (640Kb)", 1, 640 },
		{ "DSL (1.5Mb)", 1, 1500 },
		{ "DSL (7Mb)", 2, 7 },
		{ "DSL (12Mb)", 2, 12 },
		{ "Wireless g (54Mb)", 2, 54 },
		{ "Ethernet (100Mb)", 2, 100 },
		{ "Wireless n (600Mb)", 2, 600 },
		{ "Ethernet (1Gb)", 3, 1 },
		{ "Ethernet (10Gb)", 3, 10 },
		{ "Ethernet (100Gb)", 3, 100 }
	};

	const QSize c_SizeList[]
	{
		{ 1, 1 },
		{ 2, 2 },
		{ 3, 3 },
		{ 4, 4 },
		{ 3, 2 },
		{ 4, 2 },
		{ 4, 3 }
	};

	QSize ComputeSize(const QSize& input)
	{
		return QSize(input.width() * c_unitWidth, input.height() * c_unitHeight);
	}
	QSize GetDefaultSize()
	{
		static_assert(std::size(c_SizeList) > c_defaultSizeIndex);
		const QSize& defaultSize = c_SizeList[c_defaultSizeIndex];
		return ComputeSize(defaultSize);
	}

	const std::pair<QString, int> c_UpdateIntervals[]
	{
		{ "Fast - 1 sec", 1 },
		{ "Normal - 2 sec", 2 },
		{ "Medium - 3 sec", 3 },
		{ "Slow - 5 sec", 5 },
		{ "Very Slow - 10 sec", 10 },
	};
	const std::pair<QString, ChartGlobalOptions::ProcessPriority> c_Priorities[]
	{
		{ "Below Normal", ChartGlobalOptions::ProcessPriority::eBelowNormal  },
		{ "Normal", ChartGlobalOptions::ProcessPriority::eNormal },
		{ "Above Normal", ChartGlobalOptions::ProcessPriority::eAboveNormal }
	};

	const  std::pair<QString, ChartGlobalOptions::ChartWindowStyle> c_WindowStyles[]
	{
		{ "Flat",		ChartGlobalOptions::ChartWindowStyle::eFlat },
		{ "Style 1",	ChartGlobalOptions::ChartWindowStyle::eStyle2 },
		{ "Style 2",	ChartGlobalOptions::ChartWindowStyle::eStyle3 }
	};
	const std::pair<QString, ChartOptions::ChartStyle> c_GraphStyles[]
	{
		{ "Line", ChartOptions::ChartStyle::eLine },
		{ "Pile", ChartOptions::ChartStyle::ePile },
	};
}

PerformanceMonitor::PerformanceMonitor(QWidget* parent) : QDialog(parent),
	m_globalOptions("GlobalOptions"),
	m_cpuOptions(m_globalOptions, c_CPU),
	m_ramOptions(m_globalOptions, c_RAM),
	m_diskOptions(m_globalOptions, c_DISK),
	m_netOptions(m_globalOptions, c_NET),
	m_pAppSettings(new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName()))
{
	ui.setupUi(this);
	InitUiElements();
	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &PerformanceMonitor::ButtonBoxClicked);
	ChartOptions* const optionsList[4]{ &m_cpuOptions , &m_ramOptions, &m_diskOptions, &m_netOptions };
	if (m_globalOptions.Load(*m_pAppSettings))
	{
		for (ChartOptions* options : optionsList)
		{
			options->Load(*m_pAppSettings);
		}
	}
	else
	{
		ResetSettings(m_globalOptions, m_cpuOptions, m_ramOptions, m_diskOptions, m_netOptions, true);
	}
	CreateTrayActions();
	m_pTrayIcon->show();
	ValidateData();
	//LoadDataToUi(m_globalOptions, m_cpuOptions,m_ramOptions,m_diskOptions, m_netOptions);
	
	for (ChartOptions* options : optionsList)
	{
		m_pGraphs.emplace_back(new ChartWidget(this, options));
	}

	for (ChartWidget* pChartWidget : m_pGraphs)
	{
		pChartWidget->show();
	}

	CreatePerfCounters();

	QObject::connect(&m_timer, &QTimer::timeout, this, &PerformanceMonitor::HandleTimeout);
	m_timer.setInterval(m_globalOptions.m_updateInterval * 1000);
	m_timer.start();
}

PerformanceMonitor::~PerformanceMonitor()
{
	ReleasePerfCounters();
}

void PerformanceMonitor::CreateTrayActions()
{
	QStyle* pStyle = style();
	QAction* pSettingsAction = new QAction(tr("&Settings..."), this);
	pSettingsAction->setIcon(pStyle->standardIcon(QStyle::SP_FileDialogContentsView));
	connect(pSettingsAction, &QAction::triggered, this, [this]() {if (!isVisible()) { showNormal(); }});

	m_pPassThroughAction = new QAction(tr("&Pass-Through Mode"), this);
	m_pPassThroughAction->setCheckable(true);
	m_pPassThroughAction->setChecked(m_globalOptions.m_bPassThroughtMode);
	connect(m_pPassThroughAction, &QAction::triggered, this, &PerformanceMonitor::PassThroughMode);

	QAction* pBackUpAction = new QAction(tr("&Backup Positions"), this);
	pBackUpAction->setIcon(pStyle->standardIcon(QStyle::SP_DialogSaveButton));
	connect(pBackUpAction, &QAction::triggered, this, &PerformanceMonitor::BackUpPositions);

	QAction* pRestoreAction = new QAction(tr("&Restore Positions"), this);
	pRestoreAction->setIcon(pStyle->standardIcon(QStyle::SP_BrowserReload));
	connect(pRestoreAction, &QAction::triggered, this, &PerformanceMonitor::RestorePositions);

	QAction* pHelpAction = new QAction(tr("&Help"), this);
	pHelpAction->setIcon(pStyle->standardIcon(QStyle::SP_TitleBarShadeButton));
	connect(pHelpAction, &QAction::triggered, this, &QWidget::showNormal);

	QAction* pAboutAction = new QAction(tr("&About"), this);
	pAboutAction->setIcon(pStyle->standardIcon(QStyle::SP_TitleBarContextHelpButton));
	connect(pAboutAction, &QAction::triggered, this, &QWidget::showNormal);

	QAction* pExitAction = new QAction(tr("&Exit"), this);
	pExitAction->setIcon(pStyle->standardIcon(QStyle::SP_TabCloseButton));
	connect(pExitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

	QMenu* pTrayIconMenu = new QMenu(this);
	pTrayIconMenu->addAction(pSettingsAction);
	pTrayIconMenu->addSeparator();
	pTrayIconMenu->addAction(m_pPassThroughAction);
	pTrayIconMenu->addSeparator();
	pTrayIconMenu->addAction(pBackUpAction);
	pTrayIconMenu->addAction(pRestoreAction);
	pTrayIconMenu->addSeparator();
	pTrayIconMenu->addAction(pHelpAction);
	pTrayIconMenu->addAction(pAboutAction);
	pTrayIconMenu->addSeparator();
	pTrayIconMenu->addAction(pExitAction);

	m_pTrayIcon = new QSystemTrayIcon(this);
	m_pTrayIcon->setIcon(windowIcon());
	m_pTrayIcon->setToolTip("Performance Monitor 5");
	m_pTrayIcon->setContextMenu(pTrayIconMenu);
}

void PerformanceMonitor::InitUiElements()
{
	for (const std::pair<QString, int>& updateIntervnal : c_UpdateIntervals)
	{
		ui.cbUpdateInterval->addItem(updateIntervnal.first);
	}
	for (const auto&[priority, dummy] : c_Priorities)
	{
		ui.cbPriority->addItem(priority);
	}
	for (const auto&[style, dummy] : c_WindowStyles)
	{
		ui.cbStyle->addItem(style);
	}
	for (int i = 100 ;i > 10; i -= 10)
	{
		ui.cbVisibility->addItem(QString::number(i) + "%", i);
	}

	for (QComboBox* pComboBox : { ui.sizeCPU, ui.sizeRAM, ui.sizeDisk, ui.sizeNET })
	{
		for (const QSize& size : c_SizeList)
		{
			QString sizeStr("Size ");
			sizeStr += QString::number(size.width()) + "x" + QString::number(size.height());
			const QSize windowSize(size.width() * c_unitWidth, size.height() * c_unitHeight);
			pComboBox->addItem(sizeStr, windowSize);
		}
	}
	for (QComboBox* pComboBox : { ui.graphCPU, ui.graphRAM, ui.graphDisk, ui.graphNET })
	{
		for (const auto&[graphStyle, dummy] : c_GraphStyles)
		{
			pComboBox->addItem(graphStyle);
		}
	}
	for (const NetworkBandwithItem& item : c_BandwithList)
	{
		ui.maxBandwithNET->addItem(item.m_str);
	}
}

void PerformanceMonitor::ValidateData()
{
	const auto foundInternal = std::ranges::find_if(c_UpdateIntervals, [this](const auto& item) { return m_globalOptions.m_updateInterval == item.second;});
	if (foundInternal == std::end(c_UpdateIntervals))
	{
		m_globalOptions.m_updateInterval = c_DefaultUpdateInterval;
	}
	auto foundPriority = std::ranges::find_if(c_Priorities, [this](const auto& item) { return m_globalOptions.m_processPriority == item.second;});
	if (foundPriority == std::end(c_Priorities))
	{
		m_globalOptions.m_processPriority = ChartGlobalOptions::ProcessPriority::eDefaultPriority;
	}
	auto foundStyle = std::ranges::find_if(c_WindowStyles, [this](const auto& item) { return m_globalOptions.m_chartWindowStyle == item.second;});
	if (foundStyle == std::end(c_WindowStyles))
	{
		m_globalOptions.m_chartWindowStyle = ChartGlobalOptions::ChartWindowStyle::eDefaultStyle;
	}
	bool bFoundTransparency = false;
	for (int i = 100;i > 10; i -= 10)
	{
		if (i == m_globalOptions.m_transparency)
		{
			bFoundTransparency = true;
			break;
		}
	}
	if (!bFoundTransparency)
		m_globalOptions.m_transparency = c_DefaultTransparency;

	const auto ValidateChartOptions = [](ChartOptions& options, const QColor defaultColor)
		{
			const auto foundchartType = std::ranges::find_if(c_GraphStyles, [&options](const auto& item) { return options.m_ChartType == item.second;});
			if (foundchartType == std::end(c_GraphStyles))
			{
				options.m_ChartType = ChartOptions::eDefaultCharStyle;
			}
			const auto foundSize = std::ranges::find_if(c_SizeList, [&options](const auto& item) { return options.m_size == ComputeSize(item);});
			if (foundSize == std::end(c_SizeList))
			{
				options.m_size = GetDefaultSize();
			}
			if (!options.m_backgroundColor.isValid())
				options.m_backgroundColor = defaultColor;
			if (!options.m_lineColor.isValid())
				options.m_lineColor = ChartOptions::GenerateLineColor(defaultColor);
		};
	ValidateChartOptions(m_cpuOptions, c_CPUColor);
	ValidateChartOptions(m_ramOptions, c_RAMColor);
	ValidateChartOptions(m_diskOptions, c_DISKColor);
	ValidateChartOptions(m_netOptions, c_NETColor);

	if (m_netOptions.m_maxNetworkBandwidthIndex < 0 || m_netOptions.m_maxNetworkBandwidthIndex >= std::size(c_BandwithList))
		m_netOptions.m_maxNetworkBandwidthIndex = c_DefaultMaxBandwithIndex;
}

void PerformanceMonitor::LoadDataToUi(const ChartGlobalOptions& globalOptions, const ChartCpuOptions& cpuOptions, const ChartOptions& ramOptions,
									  const ChartDoubleOptions& diskOptions, const ChartNetOptions& netOptions)
{
	ui.chAutoStart->setChecked(globalOptions.m_bAutostartWithWindows);
	ui.chPass->setChecked(globalOptions.m_bPassThroughtMode);
	ui.chAllwaysTop->setChecked(globalOptions.m_bAllwaysOnTheTop);
	ui.chShowLabel->setChecked(globalOptions.m_bAllwaysShowLabel);
	const auto foundInterval = std::ranges::find_if(c_UpdateIntervals, [globalOptions](const auto& item) { return globalOptions.m_updateInterval == item.second;});
	ui.cbUpdateInterval->setCurrentIndex(std::distance(std::begin(c_UpdateIntervals), foundInterval));

	const auto foundPriority = std::ranges::find_if(c_Priorities, [globalOptions](const auto& item) { return globalOptions.m_processPriority == item.second;});
	ui.cbPriority->setCurrentIndex(std::distance(std::begin(c_Priorities), foundPriority));

	const int foundTransparency = ui.cbVisibility->findData(globalOptions.m_transparency);
	if (foundTransparency != -1)
	{
		ui.cbVisibility->setCurrentIndex(foundTransparency);
	}

	const auto foundStyle = std::ranges::find_if(c_WindowStyles, [globalOptions](const auto& item) { return globalOptions.m_chartWindowStyle == item.second;});
	ui.cbStyle->setCurrentIndex(std::distance(std::begin(c_WindowStyles), foundStyle));
	LoadDataToChart(cpuOptions, ui.showCPU, ui.graphCPU, ui.doubleLineCPU, ui.sizeCPU, ui.pbBackCPU, ui.chCPUManual, ui.pbLineCPU);
	LoadDataToChart(ramOptions, ui.showRAM, ui.graphRAM, ui.doubleLineRAM, ui.sizeRAM, ui.pbBackRAM, ui.chRAMManual, ui.pbLineRAM);
	LoadDataToChart(diskOptions, ui.showDisk, ui.graphDisk, ui.doubleLineDisk, ui.sizeDisk, ui.pbBackDisk, ui.chDiskManual, ui.pbLineDisk);
	LoadDataToChart(netOptions, ui.showNET, ui.graphNET, ui.doubleLineNET, ui.sizeNET, ui.pbBackNET, ui.chNETManual, ui.pbLineNET);

	ui.showForEachCPU->setChecked(cpuOptions.m_bOneGraphForEachCore);
	ui.separateGrpahsDisk->setChecked(diskOptions.m_bShowSeparateGraphs);
	ui.separateGrpahsNET->setChecked(netOptions.m_bShowSeparateGraphs);
	ui.maxBandwithNET->setCurrentIndex(netOptions.m_maxNetworkBandwidthIndex);
}

void PerformanceMonitor::LoadDataToChart(const ChartOptions& options, QCheckBox* pVisible, QComboBox* pGraph, QCheckBox* pDoubleLine, QComboBox* pSize, 
										 ColorButton* pBack, QCheckBox* pManualForeground, ColorButton* pForeground)
{
	pVisible->setChecked(options.m_bShowGraph);
	pDoubleLine->setChecked(options.m_bDoubleLine);
	pManualForeground->setChecked(options.m_bManualForeground);
	pBack->SetColor(options.m_backgroundColor);
	pForeground->SetColor(options.m_lineColor);

	const auto foundStyle = std::ranges::find_if(c_GraphStyles, [&options](const auto& item) { return options.m_ChartType == item.second;});
	pGraph->setCurrentIndex(std::distance(std::begin(c_GraphStyles), foundStyle));

	const int foundSize = pSize->findData(options.m_size);
	if (foundSize != -1)
	{
		pSize->setCurrentIndex(foundSize);
	}
}

void PerformanceMonitor::SaveDataFromUi()
{
	m_globalOptions.m_bAutostartWithWindows = ui.chAutoStart->isChecked();
	m_globalOptions.m_bPassThroughtMode = ui.chPass->isChecked();
	m_pPassThroughAction->setChecked(m_globalOptions.m_bPassThroughtMode);
	m_globalOptions.m_bAllwaysOnTheTop = ui.chAllwaysTop->isChecked();
	m_globalOptions.m_bAllwaysShowLabel = ui.chShowLabel->isChecked();
	m_globalOptions.m_updateInterval = c_UpdateIntervals[ui.cbUpdateInterval->currentIndex()].second;
	m_globalOptions.m_processPriority = c_Priorities[ui.cbPriority->currentIndex()].second;
	m_globalOptions.m_transparency = ui.cbVisibility->currentData().toInt();
	m_globalOptions.m_chartWindowStyle = c_WindowStyles[ui.cbStyle->currentIndex()].second;

	SaveDataFromChart(m_cpuOptions, ui.showCPU, ui.graphCPU, ui.doubleLineCPU, ui.sizeCPU, ui.pbBackCPU, ui.chCPUManual, ui.pbLineCPU);
	SaveDataFromChart(m_ramOptions, ui.showRAM, ui.graphRAM, ui.doubleLineRAM, ui.sizeRAM, ui.pbBackRAM, ui.chRAMManual, ui.pbLineRAM);
	SaveDataFromChart(m_diskOptions, ui.showDisk, ui.graphDisk, ui.doubleLineDisk, ui.sizeDisk, ui.pbBackDisk, ui.chDiskManual, ui.pbLineDisk);
	SaveDataFromChart(m_netOptions, ui.showNET, ui.graphNET, ui.doubleLineNET, ui.sizeNET, ui.pbBackNET, ui.chNETManual, ui.pbLineNET);

	m_cpuOptions.m_bOneGraphForEachCore = ui.showForEachCPU->isChecked();
	m_diskOptions.m_bShowSeparateGraphs = ui.separateGrpahsDisk->isChecked();
	m_netOptions.m_bShowSeparateGraphs = ui.separateGrpahsNET->isChecked();
	m_netOptions.m_maxNetworkBandwidthIndex = ui.maxBandwithNET->currentIndex();

	m_globalOptions.Save(*m_pAppSettings);
	ChartOptions* const optionsList[4]{ &m_cpuOptions , &m_ramOptions, &m_diskOptions, &m_netOptions };
	for (ChartOptions* pOptions : optionsList)
	{
		if (ChartWidget* pWidget = FindChart(pOptions->m_optionName))
		{
			pWidget->LoadSettings(pOptions);
		}
		pOptions->Save(*m_pAppSettings);
	}
	m_pAppSettings->sync();
	m_timer.setInterval(m_globalOptions.m_updateInterval * 1000);
}

void PerformanceMonitor::SaveDataFromChart(ChartOptions& options, QCheckBox* pVisible, QComboBox* pGraph, QCheckBox* pDoubleLine, QComboBox* pSize, 
										   ColorButton* pBack, QCheckBox* pManualForeground, ColorButton* pForeground)
{
	options.m_bShowGraph = pVisible->isChecked();
	options.m_bDoubleLine = pDoubleLine->isChecked();
	options.m_bManualForeground = pManualForeground->isChecked();
	options.m_backgroundColor = pBack->GetColor();
	options.m_lineColor = pForeground->GetColor();

	options.m_ChartType = c_GraphStyles[pGraph->currentIndex()].second;
	options.m_size = pSize->currentData().toSize();
}

void PerformanceMonitor::ResetUi()
{
	ChartGlobalOptions globalOptions("GlobalOptions");
	ChartCpuOptions cpuOptions(globalOptions, c_CPU);
	ChartOptions ramOptions(globalOptions, c_RAM);
	ChartDoubleOptions diskOptions(globalOptions, c_DISK);
	ChartNetOptions netOptions(globalOptions, c_NET);
	cpuOptions.m_position = m_cpuOptions.m_position;
	ramOptions.m_position = m_ramOptions.m_position;
	diskOptions.m_position = m_diskOptions.m_position;
	netOptions.m_position = m_netOptions.m_position;

	ResetSettings(globalOptions, cpuOptions, ramOptions, diskOptions, netOptions, false);
	LoadDataToUi(globalOptions, cpuOptions, ramOptions, diskOptions, netOptions);
}

void PerformanceMonitor::ButtonBoxClicked(QAbstractButton* pButton)
{
	if (ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok) == pButton)
	{
		SaveDataFromUi();
		hide();
	}
	else if (ui.buttonBox->button(QDialogButtonBox::StandardButton::Apply) == pButton)
	{
		SaveDataFromUi();
	}
	else if (ui.buttonBox->button(QDialogButtonBox::StandardButton::Reset) == pButton)
	{
		ResetUi();
	}
	else if (ui.buttonBox->button(QDialogButtonBox::StandardButton::Cancel) == pButton)
	{
		hide();
	}
}

void PerformanceMonitor::ResetSettings(ChartGlobalOptions& globalOptions, ChartCpuOptions& cpuOptions, ChartOptions& ramOptions,
									   ChartDoubleOptions& diskOptions, ChartNetOptions& netOptions, const bool bPositions)
{
	globalOptions.Reset();
	cpuOptions.Reset();
	ramOptions.Reset();
	diskOptions.Reset();
	netOptions.Reset();
	
	cpuOptions.m_backgroundColor = c_CPUColor;
	ramOptions.m_backgroundColor = c_RAMColor;
	diskOptions.m_backgroundColor = c_DISKColor;
	netOptions.m_backgroundColor = c_NETColor;
	ChartOptions* const optionsList[4]{ &netOptions, &diskOptions, &ramOptions, &cpuOptions };
	for (ChartOptions* options : optionsList)
	{
		options->m_lineColor = options->GenerateLineColor(options->m_backgroundColor);
	}
	
	if (bPositions)
	{
		const QSize size(GetDefaultSize());
		cpuOptions.m_size = size;
		ramOptions.m_size = size;
		diskOptions.m_size = size;
		netOptions.m_size = size;

		const QScreen* screen = QGuiApplication::primaryScreen();
		const QRect screenGeometry = screen->geometry();
		const int screenheight = screenGeometry.height();
		const int screenWidth = screenGeometry.width();
		QPoint addPoint(0, size.height());
		QPoint startPosition(screenWidth - (size.width()*2/*1.2*/), 80);
		
		for (ChartOptions* options : optionsList)
		{
			options->m_position = startPosition;
			startPosition += addPoint;
		}
	}
	if(&globalOptions == &m_globalOptions)
		m_timer.setInterval(globalOptions.m_updateInterval * 1000);
}

QSize PerformanceMonitor::GetMinimumSize() const
{
	return{c_unitWidth, c_unitHeight};
}

void PerformanceMonitor::BackUpPositions()
{
	m_globalOptions.Save(*m_pAppSettings);
	ChartOptions* const optionsList[4]{ &m_cpuOptions , &m_ramOptions, &m_diskOptions, &m_netOptions };
	for (ChartOptions* pOptions : optionsList)
	{
		if (ChartWidget* pWidget = FindChart(pOptions->m_optionName))
		{
			pOptions->m_position = pWidget->pos();
		}
		pOptions->Save(*m_pAppSettings);
	}
	m_pAppSettings->sync();
}

void PerformanceMonitor::ShowMenu(const QPoint& pos)
{
	QMenu* pMenu = m_pTrayIcon->contextMenu();
	pMenu->popup(pos);
}

void PerformanceMonitor::closeEvent(QCloseEvent* event)
{
	if (!event->spontaneous() || !isVisible())
		return;
	hide();
	event->ignore();
}

void PerformanceMonitor::showEvent(QShowEvent* pEvent)
{
	LoadDataToUi(m_globalOptions, m_cpuOptions, m_ramOptions, m_diskOptions, m_netOptions);
}

void PerformanceMonitor::RestorePositions()
{
	ChartOptions* const optionsList[4]{ &m_cpuOptions , &m_ramOptions, &m_diskOptions, &m_netOptions };
	for (const ChartOptions* pOptions : optionsList)
	{
		if (ChartWidget* pWidget = FindChart(pOptions->m_optionName))
		{
			pWidget->move(pOptions->m_position);
		}
	}
}

void PerformanceMonitor::PassThroughMode()
{
	m_globalOptions.m_bPassThroughtMode = !m_globalOptions.m_bPassThroughtMode;
	m_pPassThroughAction->setChecked(m_globalOptions.m_bPassThroughtMode);
	m_globalOptions.Save(*m_pAppSettings);
	for (ChartWidget* pChartWidget : m_pGraphs)
	{
		pChartWidget->SetPassThroughMode(m_globalOptions.m_bPassThroughtMode);
	}
	ChartOptions* const optionsList[4]{ &m_cpuOptions , &m_ramOptions, &m_diskOptions, &m_netOptions };
	for (ChartOptions* pOptions : optionsList)
	{
		pOptions->Save(*m_pAppSettings);
	}
	m_pAppSettings->sync();
}

void PerformanceMonitor::CreatePerfCounters()
{
	PDH_STATUS pdhStatus = PdhOpenQuery(NULL, 0, &m_hQuery);
	if (pdhStatus != ERROR_SUCCESS)
	{
		ShowError("Cannot initialize query.");
		return;
	}
	if (m_cpuOptions.m_bOneGraphForEachCore)
	{

	}
	else
	{
		if (ChartWidget* pCpuWidget = FindChart(m_cpuOptions.m_optionName))
		{
			void* pTotalSystemCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\Processor(_Total)\\% Privileged Time", 0, &pTotalSystemCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalSystemCounter)
			{
				ShowError("Cannot initialize CPU system counter.");
				return;
			}
			void* pTotalUserCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\Processor(_Total)\\% User Time", 0, &pTotalUserCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalUserCounter)
			{
				ShowError("Cannot initialize CPU user counter.");
				return;
			}
			pCpuWidget->SetCounters({ pTotalSystemCounter, pTotalUserCounter });
		}
	}
	if (m_diskOptions.m_bShowSeparateGraphs)
	{

	}
	else
	{
		if (ChartWidget* pDiscWidget = FindChart(m_diskOptions.m_optionName))
		{
			void* pTotalWriteCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\PhysicalDisk(_Total)\\% Disk Write Time", 0, &pTotalWriteCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalWriteCounter)
			{
				ShowError("Cannot initialize disc write counter.");
				return;
			}
			void* pTotalReadCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\PhysicalDisk(_Total)\\% Disk Read Time", 0, &pTotalReadCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalReadCounter)
			{
				ShowError("Cannot initialize disc read counter.");
				return;
			}
			pDiscWidget->SetCounters({ pTotalWriteCounter, pTotalReadCounter });
		}
	}

	if (m_netOptions.m_bShowSeparateGraphs)
	{

	}
	else
	{
		if (ChartWidget* pNetWidget = FindChart(m_netOptions.m_optionName))
		{
			void* pTotalSentCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\Network Interface(*)\\Bytes Received/sec", 0, &pTotalSentCounter);	//Realtek Gaming 2.5Gbe Family Controller
			if (pdhStatus != ERROR_SUCCESS || !pTotalSentCounter)
			{
				ShowError("Cannot initialize disc write counter.");
				return;
			}
			void* pTotalReceivedCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\Network Interface(*)\\Bytes Sent/sec", 0, &pTotalReceivedCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalReceivedCounter)
			{
				ShowError("Cannot initialize disc read counter.");
				return;
			}
			pNetWidget->SetCounters({ pTotalSentCounter, pTotalReceivedCounter });
		}
	}
}

void PerformanceMonitor::ShowError(const QString& errorMessage)
{
	QMessageBox::information(this, QString("Perf counters error."), errorMessage, QMessageBox::StandardButton::Yes);
}

void PerformanceMonitor::HandleTimeout()
{
	if (m_hQuery)
	{
		PDH_STATUS status = PdhCollectQueryData(m_hQuery);
		if (status == ERROR_SUCCESS)
		{
			DWORD ret = 0;

			if (ChartWidget* pCpuWidget = FindChart(m_cpuOptions.m_optionName))
			{
				const QVector<void*>& counters = pCpuWidget->GetCounters();
				double values[2]{};
				for (size_t i = 0, size = counters.size();i < size; i++)
				{
					PDH_FMT_COUNTERVALUE value;
					status = PdhGetFormattedCounterValue(counters[i], PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &ret, &value);
					values[i] = value.doubleValue;
				}
				// Second value is sum of both values.
				values[1] += values[0];
				pCpuWidget->AddData(values);
				SYSTEM_INFO sysinfo;
				GetSystemInfo(&sysinfo);
				const double numCPU = sysinfo.dwNumberOfProcessors;
				constexpr double visibleCPUS = 4;
				pCpuWidget->SetFirstLineScale(numCPU / visibleCPUS);
			}
			if (ChartWidget* pRAMWidget = FindChart(m_ramOptions.m_optionName))
			{
				MEMORYSTATUSEX statex;
				statex.dwLength = sizeof(statex);
				GlobalMemoryStatusEx(&statex);
				//constexpr DWORDLONG k = 1024 * 1024;	// From bytes to GB.
				const double usedPhysical = statex.ullTotalPhys - statex.ullAvailPhys;
				const double usedPhysicalPercentage = (usedPhysical / statex.ullTotalPhys) * 100.0;

				const double usedPage = statex.ullTotalPageFile - statex.ullAvailPageFile;
				const double usedPagePercentage = (usedPage / statex.ullTotalPageFile) * 100.0;

				double values[2]{ usedPhysicalPercentage , usedPagePercentage };
				pRAMWidget->AddData(values);
			}
			if (ChartWidget* pDiscWidget = FindChart(m_diskOptions.m_optionName))
			{
				const QVector<void*>& counters = pDiscWidget->GetCounters();
				double values[2]{};
				for (size_t i = 0, size = counters.size();i < size; i++)
				{
					PDH_FMT_COUNTERVALUE value;
					status = PdhGetFormattedCounterValue(counters[i], PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &ret, &value);
					values[i] = value.doubleValue;
				}
				pDiscWidget->AddData(values);
			}
			if (ChartWidget* pNetWidget = FindChart(m_netOptions.m_optionName))
			{
				const QVector<void*>& counters = pNetWidget->GetCounters();
				double values[2]{};
				for (size_t i = 0, size = counters.size();i < size; i++)
				{
					PDH_FMT_COUNTERVALUE value;
					status = PdhGetFormattedCounterValue(counters[i], PDH_FMT_DOUBLE, &ret, &value);
					if (m_netOptions.m_bIgnoreSmallTrafic)
					{
						if (value.doubleValue < 1024)	// Ignore trafic smaller than 1KB.
							value.doubleValue = 0;
						else
						{
							// Our value is in bytes.
							double bytes = value.doubleValue;
							const NetworkBandwithItem& bandItem = c_BandwithList[m_netOptions.m_maxNetworkBandwidthIndex];
							for (int i = 0; i < bandItem.m_magnitude; i++)
							{
								bytes /= 1024;
							}
							bytes *= 8;
							value.doubleValue = (bytes*10 / bandItem.m_size) * 100;
						}
					}
					values[i] = value.doubleValue;
				}
				pNetWidget->AddData(values); /// in bytes
			}
		}
	}
}

void PerformanceMonitor::ReleasePerfCounters()
{
	if (m_hQuery)
	{
		PdhCloseQuery(m_hQuery);
		m_hQuery = nullptr;
	}
}

ChartWidget* PerformanceMonitor::FindChart(const QString& accesibleName) const
{
	for (ChartWidget* pChartWidget : m_pGraphs)
	{
		if (pChartWidget->accessibleName() == accesibleName)
			return pChartWidget;
	}
	return nullptr;
}