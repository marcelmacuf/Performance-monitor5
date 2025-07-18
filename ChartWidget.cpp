#include "stdafx.h"
#include "ChartWidget.h"
#include <QLineSeries>
#include <QValueAxis>

ChartWidget::ChartWidget(const ChartOptions* pChartData)
	: QChartView()
{
	m_firstData.reserve(61);
	m_secondData.reserve(61);
	setRenderHint(QPainter::Antialiasing);
	setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
	setAccessibleName(pChartData->m_optionName);
	LoadSettings(pChartData);
}

ChartWidget::~ChartWidget()
{
}

void ChartWidget::LoadSettings(const ChartOptions* pChartData)
{
	QChart* pChart = new QChart;
	setChart(pChart);
	size_t seriesCount = 2;
	if (const ChartDoubleOptions* pOptions = dynamic_cast<const ChartDoubleOptions*>(pChartData))
	{
		if (pOptions->m_bShowSeparateGraphs)
			seriesCount = 1;
	}
	for (size_t i = 0; i < seriesCount; i++)
	{
		QLineSeries* pLineSeries = new QLineSeries(pChart);
		QColor color = pChartData->m_lineColor;
		if (i > 0)
		{
			color = pChartData->GenerateLineColor(pChartData->m_lineColor);
		}
		const QPen linePen(QBrush(color), pChartData->m_bDoubleLine ? 2 : 1);
		pLineSeries->setPen(linePen);
		pChart->addSeries(pLineSeries);
	}

	pChart->legend()->hide();
	pChart->setMargins(QMargins());
	pChart->layout()->setContentsMargins(0, 0, 0, 0);
	pChart->setBackgroundRoundness(0);
	pChart->setBackgroundBrush(QBrush(pChartData->m_backgroundColor));
	pChart->createDefaultAxes();
	
	QList<QAbstractAxis*> axes = pChart->axes();
	QAbstractAxis* pYAxis = axes[1];
	pYAxis->setRange(0, 100);

	QValueAxis* pXAxis = (QValueAxis*)axes[0];
	pXAxis->setRange(0, 60);

	const QBrush gridBrush(pChartData->GenerateGridColor(pChartData->m_backgroundColor));
	QPen gridPen(gridBrush, 1, Qt::PenStyle::DotLine);
	for (size_t i = 0, axesSize = axes.size(); i < axesSize; i++)
	{
		QAbstractAxis* pAxis = axes[i];
		//pAxis->hide();
		pAxis->setTitleVisible(false);
		pAxis->setLabelsVisible(false);
		pAxis->setGridLineVisible(true);
		pAxis->setMinorGridLineVisible(true);
		pAxis->setLineVisible(false);
		pAxis->setGridLinePen(gridPen);
		QValueAxis* pValueAxis = static_cast<QValueAxis*>(pAxis);
		const int tickCount = i == 0 ? 4 : 3;
		pValueAxis->setTickCount(tickCount);
	}
	
	const ChartGlobalOptions& chartGlobalOptions = pChartData->m_chartGlobalOptions;
	setWindowFlag(Qt::WindowStaysOnTopHint, chartGlobalOptions.m_bAllwaysOnTheTop);	// ale ked sa meni hodnota tak je potrebny nejaky refresh
	setWindowFlag(Qt::WindowTransparentForInput, chartGlobalOptions.m_bPassThroughtMode);
	setWindowOpacity(pChartData->m_chartGlobalOptions.m_transparency);

	resize(pChartData->m_size);
	move(pChartData->m_position);
	setVisible(pChartData->m_bShowGraph);
}

void ChartWidget::AddData(const double values[2])
{
	const auto MoveData = [](QVector<QPointF>& data)
		{
			const QPointF sub(1.0, 0);
			for (QPointF& point : data)
			{
				point -= sub;
			}
			if (const size_t dataSize = data.size(); dataSize > 60)
			{
				for (size_t i = 0, size = dataSize - 1;i < size;i++)
				{
					data[i] = data[i + 1];
				}
				data.pop_back();
			}
		};

	m_firstData.append(QPointF(61, values[0]));
	m_secondData.append(QPointF(61, values[1]));
	MoveData(m_firstData);
	MoveData(m_secondData);

	const QVector<QPointF>* data[]{ &m_firstData , &m_secondData };

	QChart* pChart = chart();
	const QList<QAbstractSeries*> series = pChart->series();
	for (size_t i = 0, seriesSize = series.size(); i < seriesSize && i < std::size(data);i++)
	{
		QLineSeries* pLineSeries = static_cast<QLineSeries*>(series[i]);
		pLineSeries->clear();
		pLineSeries->append(*data[i]);
	}

	repaint();
}

