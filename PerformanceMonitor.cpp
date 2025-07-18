#include "stdafx.h"
#include "PerformanceMonitor.h"
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QScatterSeries>
#include <QValueAxis>
#include <pdh.h>
#include <pdhmsg.h>

constexpr int c_unitWidth = 41;
constexpr int c_unitHeight = 26;
constexpr int c_defaultWidthMul = 3;
constexpr int c_defaultHeightMul = 3;
constexpr char c_CPU[]{ "CPU" };
constexpr char c_RAM[]{ "RAM" };
constexpr char c_DISK[]{ "DISC" };
constexpr char c_NET[]{ "NET" };

PerformanceMonitor::PerformanceMonitor(QWidget* parent) : QDialog(parent),
	m_globalOptions("GlobalOptions"),
	m_cpuOptions(m_globalOptions, c_CPU),
	m_ramOptions(m_globalOptions, c_RAM),
	m_diskOptions(m_globalOptions, c_DISK),
	m_netOptions(m_globalOptions, c_NET),
	m_pAppSettings(new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName()))
{
	ui.setupUi(this);
	CreateTrayActions();
	m_pTrayIcon->show();
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
		ResetSettings(true);
	}
	
	for (ChartOptions* options : optionsList)
	{
		m_pGraphs.emplace_back(new ChartWidget(options));
	}

	for (ChartWidget* pChartWidget : m_pGraphs)
	{
		pChartWidget->show();
	}

	CreatePerfCounters();

	QObject::connect(&m_timer, &QTimer::timeout, this, &PerformanceMonitor::HandleTimeout);
	m_timer.setInterval(m_globalOptions.m_updateInterval * 1000);
	m_timer.start();


	//	QChart* pChart = new QChart;
	//	QMargins margins;
	//	pChart->setMargins(margins);
	//	this->setContentsMargins(margins);
	//	//this->setWindowOpacity(0.9);
	//
	//	QLineSeries* series = new QLineSeries;
	//	series->append(0, 6);
	//	series->append(2, 4);
	//	series->append(3, 8);
	//	series->append(7, 4);
	//	series->append(10, 5);
	//	*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
	//	series->setColor(QColor(119, 242, 255));
	//	pChart->addSeries(series);
	//
	//	QScatterSeries* series2 = new QScatterSeries();
	//	series2->append(0, 60);
	//	series2->append(20, 40);
	//	pChart->addSeries(series);
	//
	//	pChart->legend()->hide();
	//	pChart->layout()->setContentsMargins(0, 0, 0, 0);
	//	pChart->setBackgroundRoundness(0);
	//	pChart->setBackgroundBrush(QBrush(QColor(0, 123, 183)));
	//	pChart->createDefaultAxes();
	//	pChart->axes(Qt::Horizontal).first()->setRange(0, 20);
	//	pChart->axes(Qt::Vertical).first()->setRange(0, 10);
	//	pChart->axes(Qt::Horizontal).first()->setTitleVisible(false);
	//	pChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
	//	pChart->axes(Qt::Horizontal).first()->setGridLineVisible(true);
	////	pChart->axes(Qt::Horizontal).first()->setGridLineColor(QColor(34, 157, 217));
	//	QBrush br(QBrush(QColor(34, 157, 217)));
	//	pChart->axes(Qt::Horizontal).first()->setGridLinePen(QPen(br,1, Qt::PenStyle::DotLine));
	//	QValueAxis* axisx = (QValueAxis * )pChart->axes(Qt::Horizontal).first();
	//	//axisx->setTickType(QValueAxis::TicksDynamic);
	//	//axisx->setTickAnchor(0.0);
	//	//axisx->setTickInterval(10);
	//	axisx->setTickCount(4);
	//	pChart->axes(Qt::Vertical).first()->hide();
	//	auto pChartView = new QChartView(pChart);
	//	//pChartView->setContentsMargins(margins);
	//	pChartView->setRenderHint(QPainter::Antialiasing);
	//	pChartView->setWindowFlag(Qt::WindowStaysOnTopHint, true);	// ale ked sa meni hodnota tak je potrebny nejaky refresh
	//	pChartView->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip | Qt::WindowTransparentForInput);
	//	pChartView->resize(124, 79);  // a to je 3X3  takze default to je 41 X 26
	//	pChartView->show();
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
	connect(pSettingsAction, &QAction::triggered, this, &QWidget::showNormal);

	QAction* pPassThroughAction = new QAction(tr("&Pass-Through Mode"), this);
	pPassThroughAction->setCheckable(true);
	pPassThroughAction->setChecked(true);
	connect(pPassThroughAction, &QAction::triggered, this, &QWidget::hide);

	QAction* pBackUpAction = new QAction(tr("&Backup Positions"), this);
	pBackUpAction->setIcon(pStyle->standardIcon(QStyle::SP_DialogSaveButton));
	connect(pBackUpAction, &QAction::triggered, this, &QWidget::showNormal);

	QAction* pRestoreAction = new QAction(tr("&Restore Positions"), this);
	pRestoreAction->setIcon(pStyle->standardIcon(QStyle::SP_BrowserReload));
	connect(pRestoreAction, &QAction::triggered, this, &QWidget::showNormal);

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
	pTrayIconMenu->addAction(pPassThroughAction);
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

void PerformanceMonitor::ResetSettings(const bool bPositions)
{
	m_globalOptions.Reset();
	m_cpuOptions.Reset();
	m_ramOptions.Reset();
	m_diskOptions.Reset();
	m_netOptions.Reset();
	
	m_cpuOptions.m_backgroundColor = QColor(0, 123, 183);
	m_ramOptions.m_backgroundColor = QColor(0, 128, 0);
	m_diskOptions.m_backgroundColor = QColor(176, 119, 0);
	m_netOptions.m_backgroundColor = QColor(149, 149, 37);
	ChartOptions* const optionsList[4]{ &m_netOptions, &m_diskOptions, &m_ramOptions, &m_cpuOptions };
	for (ChartOptions* options : optionsList)
	{
		options->m_lineColor = options->GenerateLineColor(options->m_backgroundColor);
	}
	
	if (bPositions)
	{
		constexpr int width = c_unitWidth * c_defaultWidthMul;
		constexpr int height = c_unitHeight * c_defaultHeightMul;
		const QSize size(width, height);
		m_cpuOptions.m_size = size;
		m_ramOptions.m_size = size;
		m_diskOptions.m_size = size;
		m_netOptions.m_size = size;

		const QScreen* screen = QGuiApplication::primaryScreen();
		const QRect screenGeometry = screen->geometry();
		const int screenheight = screenGeometry.height();
		const int screenWidth = screenGeometry.width();
		QPoint addPoint(0, height);
		QPoint startPosition(screenWidth - (width*1.2), 80);
		
		for (ChartOptions* options : optionsList)
		{
			options->m_position = startPosition;
			startPosition += addPoint;
		}
	}
	m_timer.setInterval(m_globalOptions.m_updateInterval * 1000);
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
			pdhStatus = PdhAddCounter(m_hQuery, L"\\PhysicalDisk(_Total)\\% Disk Read Time", 0, &pTotalWriteCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalWriteCounter)
			{
				ShowError("Cannot initialize disc write counter.");
				return;
			}
			void* pTotalReadCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\PhysicalDisk(_Total)\\% Disk Write Time", 0, &pTotalReadCounter);
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
			pdhStatus = PdhAddCounter(m_hQuery, L"\\Network Interface(*)\\Bytes Sent/sec", 0, &pTotalSentCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalSentCounter)
			{
				ShowError("Cannot initialize disc write counter.");
				return;
			}
			void* pTotalReceivedCounter = nullptr;
			pdhStatus = PdhAddCounter(m_hQuery, L"\\Network Interface(*)\\Bytes Received/sec", 0, &pTotalReceivedCounter);
			if (pdhStatus != ERROR_SUCCESS || !pTotalReceivedCounter)
			{
				ShowError("Cannot initialize disc read counter.");
				return;
			}
			pNetWidget->SetCounters({ pTotalSentCounter, pTotalReceivedCounter });
		}
	}

	if (ChartWidget* pRAMWidget = FindChart(m_ramOptions.m_optionName))
	{
		

	//	auto j = statex.ullTotalVirtual;

		// \Memory\Available Bytes
		// \Memory\Committed Bytes
		// \Memory\Available MBytes

		/*void* pTotalSystemCounter = nullptr;
		void* pTotalUserCounter = nullptr;
		pdhStatus = PdhAddCounter(m_hQuery, L"\\Processor(_Total)\\% Privileged Time", 0, &pTotalSystemCounter);
		if (pdhStatus != ERROR_SUCCESS || !pTotalSystemCounter)
		{
			ShowError("Cannot initialize CPU system counter.");
			return;
		}
		pdhStatus = PdhAddCounter(m_hQuery, L"\\\\Processor(_Total)\\% User Time", 0, &pTotalUserCounter);
		if (pdhStatus != ERROR_SUCCESS || !pTotalUserCounter)
		{
			ShowError("Cannot initialize CPU user counter.");
			return;
		}
		pRAMWidget->SetCounters({ pTotalSystemCounter, pTotalUserCounter });*/
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
					values[i] = value.doubleValue*3;
				}
				pCpuWidget->AddData(values);
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
					values[i] = value.doubleValue-2;
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
					status = PdhGetFormattedCounterValue(counters[i], PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &ret, &value);
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