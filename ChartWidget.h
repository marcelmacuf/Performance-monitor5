#pragma once

#include <QChartView>

class ChartGlobalOptions
{
public:
	ChartGlobalOptions(const QString& optionName) {}
	bool Load(QSettings& settings)
	{
		settings.beginGroup(m_optionName);
		if (!settings.contains("Transparency"))
			return false;	// Cannot find config file. Generate positions and colors.
		m_transparency = settings.value("Transparency", 1.0).toDouble();
		m_chartStyle = settings.value("CharStyle", eStyle1).value<ChartStyle>();
		m_updateInterval = settings.value("UpdateInterval", 1).toInt();
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
		settings.setValue("CharStyle", m_chartStyle);
		settings.setValue("UpdateInterval", m_updateInterval);
		settings.setValue("Autostart", m_bAutostartWithWindows);
		settings.setValue("PassThrough", m_bPassThroughtMode);
		settings.setValue("AllwaysOnTop", m_bAllwaysOnTheTop);
		settings.setValue("AllwaysShowLabel", m_bAllwaysShowLabel);
		settings.endGroup();
	}
	void Reset()
	{
		m_transparency = 1.0;
		m_chartStyle = eStyle1;
		m_updateInterval = 1;
		m_bAutostartWithWindows = true;
		m_bPassThroughtMode = true;
		m_bAllwaysOnTheTop = true;
		m_bAllwaysShowLabel = false;
	}
	const QString m_optionName;
	enum ChartStyle { eStyle1, eStyle2, eStyle3 };
	double m_transparency{ 1.0 };
	ChartStyle m_chartStyle{ eStyle1 };
	int m_updateInterval{ 1 };				// Seconds.
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
		m_backgroundColor = settings.value("BackgroundColor").value<QColor>();
		m_lineColor = settings.value("LineColor").value<QColor>();
		m_bLineStyleType = settings.value("LineStyle", true).toBool();
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
		settings.setValue("BackgroundColor", m_backgroundColor);
		settings.setValue("LineColor", m_lineColor);
		settings.setValue("LineStyle", m_bLineStyleType);
		settings.setValue("DoubleLine", m_bDoubleLine);
		settings.setValue("ShowGraph", m_bShowGraph);
		SaveOther(settings);
		settings.endGroup();
	}
	virtual bool LoadOther(QSettings& settings) { return true; }
	virtual void SaveOther(QSettings& settings) const {}
	virtual void Reset()
	{
		m_bLineStyleType = true;
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
	QPoint m_position;
	QSize m_size;
	QColor m_backgroundColor;
	QColor m_lineColor;
	bool m_bLineStyleType{ true };
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

class ChartNetOptions : public ChartDoubleOptions
{
public:
	using ChartDoubleOptions::ChartDoubleOptions;
	bool LoadOther(QSettings& settings) override
	{
		const bool bRet = ChartDoubleOptions::LoadOther(settings);
		m_netInterface = settings.value("NetInterface").toString();
		m_maxNetworkBandwidth = settings.value("MaxBandwith", 100000).toInt();;
		m_bIgnoreSmallTrafic = settings.value("IgnoreSmallTrafic", false).toBool();
		return bRet;
	}
	void SaveOther(QSettings& settings) const override
	{
		ChartDoubleOptions::SaveOther(settings);
		settings.setValue("NetInterface", m_netInterface);
		settings.setValue("MaxBandwith", m_maxNetworkBandwidth);
		settings.setValue("IgnoreSmallTrafic", m_bIgnoreSmallTrafic);
	}
	void Reset() override
	{
		ChartDoubleOptions::Reset();
		m_bIgnoreSmallTrafic = true;
		m_maxNetworkBandwidth = 100000;
	}
	QString m_netInterface;
	int m_maxNetworkBandwidth{ 100000 };	// In Kb\s. Default 100 Mb\s
	bool m_bIgnoreSmallTrafic{ true };
};


class ChartWidget  : public QChartView
{
	Q_OBJECT

public:
	ChartWidget(const ChartOptions* pChartData);
	~ChartWidget();

	void LoadSettings(const ChartOptions* pChartData);
	void AddData(const double values[2]);

	void SetCounters(const QVector<void*>& counters) { m_counters = counters; }
	const QVector<void*>& GetCounters() const { return m_counters; }
private:
	QVector<void*> m_counters;
	QVector<QPointF> m_firstData;
	QVector<QPointF> m_secondData;
};

