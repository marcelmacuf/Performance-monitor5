#pragma once

#include <QChartView>
#include <QPointer>

class PerformanceMonitor;

constexpr int c_DefaultUpdateInterval = 1;
constexpr int c_DefaultTransparency = 90;	// %
class ChartGlobalOptions
{
public:
	ChartGlobalOptions(const QString& optionName) : m_optionName(optionName) {}
	bool Load(QSettings& settings)
	{
		constexpr int checkValue = INT_MAX;
		settings.beginGroup(m_optionName);
		m_transparency = settings.value("Transparency", INT_MAX).toInt();
		if (m_transparency == checkValue)
		{
			m_transparency = c_DefaultTransparency;
			return false;
		}
		m_processPriority = settings.value("ProcessPriority", eDefaultPriority).value<ProcessPriority>();
		m_chartWindowStyle = settings.value("CharWindowStyle", eDefaultStyle).value<ChartWindowStyle>();
		m_updateInterval = settings.value("UpdateInterval", c_DefaultUpdateInterval).toInt();
		m_bAutostartWithWindows = settings.value("Autostart", true).toBool();
		m_bPassThroughtMode = settings.value("PassThrough", true).toBool();
		m_bAllwaysOnTheTop = settings.value("AllwaysOnTop", true).toBool();
		m_bAllwaysShowLabel = settings.value("AllwaysShowLabel", false).toBool();
		settings.endGroup();
		return true;
	}
	void Save(QSettings& settings) const
	{
		settings.beginGroup(m_optionName);
		settings.setValue("Transparency", m_transparency);
		settings.setValue("ProcessPriority", m_processPriority);
		settings.setValue("CharWindowStyle", m_chartWindowStyle);
		settings.setValue("UpdateInterval", m_updateInterval);
		settings.setValue("Autostart", m_bAutostartWithWindows);
		settings.setValue("PassThrough", m_bPassThroughtMode);
		settings.setValue("AllwaysOnTop", m_bAllwaysOnTheTop);
		settings.setValue("AllwaysShowLabel", m_bAllwaysShowLabel);
		settings.endGroup();
	}
	void Reset()
	{
		m_transparency = c_DefaultTransparency;
		m_processPriority = eDefaultPriority;
		m_chartWindowStyle = eDefaultStyle;
		m_updateInterval = c_DefaultUpdateInterval;
		m_bAutostartWithWindows = true;
		m_bPassThroughtMode = true;
		m_bAllwaysOnTheTop = true;
		m_bAllwaysShowLabel = false;
	}
	const QString m_optionName;
	enum ChartWindowStyle : int { eFlat, eStyle2, eStyle3, eDefaultStyle = eFlat };
	enum ProcessPriority : int { eBelowNormal, eNormal, eAboveNormal, eDefaultPriority = eNormal };
	int m_transparency{ c_DefaultTransparency };	// in %
	ProcessPriority m_processPriority{ eDefaultPriority };
	ChartWindowStyle m_chartWindowStyle{ eDefaultStyle };
	int m_updateInterval{ c_DefaultUpdateInterval };				// Seconds.
	bool m_bAutostartWithWindows{ true };
	bool m_bPassThroughtMode{ true };
	bool m_bAllwaysOnTheTop{ true };
	bool m_bAllwaysShowLabel{ false };
};

class ChartOptions
{
public:
	ChartOptions(const ChartGlobalOptions& pChartGlobalOptions, const QString& optionName) : 
		m_chartGlobalOptions(pChartGlobalOptions), m_optionName(optionName)
	{
	}
	bool Load(QSettings& settings)
	{
		settings.beginGroup(m_optionName);
		m_position = settings.value("Position").value<QPoint>();
		m_size = settings.value("Size").value<QSize>();
		m_ChartType = settings.value("ChartType", eDefaultCharStyle).value<ChartStyle>();
		m_backgroundColor = settings.value("BackgroundColor").value<QColor>();
		m_lineColor = settings.value("LineColor").value<QColor>();
		m_bManualForeground = settings.value("ManualForeground", true).toBool();
		m_bDoubleLine = settings.value("DoubleLine", false).toBool();
		m_bShowGraph = settings.value("ShowGraph", true).toBool();
		const bool bRet = LoadOther(settings);
		settings.endGroup();
		return bRet;
	}
	void Save(QSettings& settings) const
	{
		settings.beginGroup(m_optionName);
		settings.setValue("Position", m_position);
		settings.setValue("Size", m_size);
		settings.setValue("ChartType", m_ChartType);
		settings.setValue("BackgroundColor", m_backgroundColor);
		settings.setValue("LineColor", m_lineColor);
		settings.setValue("ManualForeground", m_bManualForeground);
		settings.setValue("DoubleLine", m_bDoubleLine);
		settings.setValue("ShowGraph", m_bShowGraph);
		SaveOther(settings);
		settings.endGroup();
	}
	virtual bool LoadOther(QSettings& settings) { return true; }
	virtual void SaveOther(QSettings& settings) const {}
	virtual void Reset()
	{
		m_ChartType = eDefaultCharStyle;
		m_bManualForeground = false;
		m_bDoubleLine = false;
		m_bShowGraph = true;
	}
	static QColor GenerateLineColor(const QColor& srcColor)
	{
		constexpr int c_lineColorScale = 85;
		return QColor(std::min(255, srcColor.red() + c_lineColorScale), std::min(255, srcColor.green() + c_lineColorScale), std::min(255, srcColor.blue() + c_lineColorScale));
	}
	static QColor GenerateGridColor(const QColor& srcColor)
	{
		constexpr int c_gridColorScale = 34;
		return QColor(std::min(255, srcColor.red() + c_gridColorScale), std::min(255, srcColor.green() + c_gridColorScale), std::min(255, srcColor.blue() + c_gridColorScale));
	}

	const ChartGlobalOptions& m_chartGlobalOptions;
	const QString m_optionName;
	enum ChartStyle : int { eLine, ePile, eDefaultCharStyle = eLine };
	QPoint m_position;
	QSize m_size;
	QColor m_backgroundColor;
	QColor m_lineColor;
	ChartStyle m_ChartType{ eDefaultCharStyle };
	bool m_bManualForeground{ false };
	bool m_bDoubleLine{ false };
	bool m_bShowGraph{ true };
};

class ChartCpuOptions final : public ChartOptions
{
public:
	using ChartOptions::ChartOptions;
	bool LoadOther(QSettings& settings) override
	{
		m_bOneGraphForEachCore = settings.value("OneGraphForEachCore", false).toBool();
		return true;
	}
	void SaveOther(QSettings& settings) const override
	{
		settings.setValue("OneGraphForEachCore", m_bOneGraphForEachCore);
	}
	void Reset() override
	{
		ChartOptions::Reset();
		m_bOneGraphForEachCore = false;
	}
	bool m_bOneGraphForEachCore{ false };
};

class ChartDoubleOptions : public ChartOptions
{
public:
	using ChartOptions::ChartOptions;
	bool LoadOther(QSettings& settings) override
	{
		m_bShowSeparateGraphs = settings.value("SeparateGraphs", false).toBool();
		return true;
	}
	void SaveOther(QSettings& settings) const override
	{
		settings.setValue("SeparateGraphs", m_bShowSeparateGraphs);
	}
	void Reset() override
	{
		ChartOptions::Reset();
		m_bShowSeparateGraphs = false;
	}
	bool m_bShowSeparateGraphs{ false };
};

constexpr int c_DefaultMaxBandwithIndex = 7;	// 100 Mb.
class ChartNetOptions : public ChartDoubleOptions
{
public:
	using ChartDoubleOptions::ChartDoubleOptions;
	bool LoadOther(QSettings& settings) override
	{
		const bool bRet = ChartDoubleOptions::LoadOther(settings);
		m_netInterface = settings.value("NetInterface").toString();
		m_maxNetworkBandwidthIndex = settings.value("MaxBandwith", c_DefaultMaxBandwithIndex).toInt();
		m_bIgnoreSmallTrafic = settings.value("IgnoreSmallTrafic", false).toBool();
		return bRet;
	}
	void SaveOther(QSettings& settings) const override
	{
		ChartDoubleOptions::SaveOther(settings);
		settings.setValue("NetInterface", m_netInterface);
		settings.setValue("MaxBandwith", m_maxNetworkBandwidthIndex);
		settings.setValue("IgnoreSmallTrafic", m_bIgnoreSmallTrafic);
	}
	void Reset() override
	{
		ChartDoubleOptions::Reset();
		m_bIgnoreSmallTrafic = true;
		m_maxNetworkBandwidthIndex = c_DefaultMaxBandwithIndex;
	}
	QString m_netInterface;
	int m_maxNetworkBandwidthIndex{ c_DefaultMaxBandwithIndex };
	bool m_bIgnoreSmallTrafic{ true };
};


class ChartWidget : public QChartView
{
	Q_OBJECT

public:
	ChartWidget(PerformanceMonitor* pPerfMonitor, const ChartOptions* pChartData);
	~ChartWidget();

	void LoadSettings(const ChartOptions* pChartData);
	void AddData(const double values[2]);

	void SetPassThroughMode(const bool bPassThrough);
	void SetCounters(const QVector<void*>& counters) { m_counters = counters; }
	const QVector<void*>& GetCounters() const { return m_counters; }
	bool event(QEvent* event) override
	{
		if (event->type() == QEvent::MouseButtonPress)
			windowHandle()->startSystemMove();
		return QChartView::event(event);
	}
private:
	QVector<void*> m_counters;
	QVector<QPointF> m_firstData;
	QVector<QPointF> m_secondData;
	const PerformanceMonitor* m_pPerfMonitor{ nullptr };
};

