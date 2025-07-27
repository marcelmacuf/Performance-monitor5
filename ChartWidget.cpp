#include "stdafx.h"
#include "ChartWidget.h"
#include <QLineSeries>
#include <QValueAxis>

#include "PerformanceMonitor.h"

constexpr int c_XAxisRange = 60;
constexpr double c_YAxisMin = 0.0;
constexpr double c_YUnderValue = -2;
ChartWidget::ChartWidget(PerformanceMonitor* pPerfMonitor, const ChartOptions* pChartData)
	: QChartView(), m_pPerfMonitor(pPerfMonitor)
{
	m_firstData.reserve(c_XAxisRange+1);
	m_secondData.reserve(c_XAxisRange+1);
	m_thirdData.reserve(c_XAxisRange + 1);
	m_fourthData.reserve(c_XAxisRange + 1);
	const QPointF firstValue(c_XAxisRange, c_YUnderValue);
	m_firstData.append(firstValue);
	m_secondData.append(firstValue);
	m_thirdData.append(firstValue);
	m_fourthData.append(firstValue);
	//setRenderHint(QPainter::Antialiasing);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
	setAccessibleName(pChartData->m_optionName);
	LoadSettings(pChartData);
}

ChartWidget::~ChartWidget()
{
	
}

void ChartWidget::LoadSettings(const ChartOptions* pChartData)
{
	QChart* pChart = new QChart;
	pChart->setMinimumSize(m_pPerfMonitor->GetMinimumSize());
	setChart(pChart);
	size_t seriesCount = 2;
	if (const ChartDoubleOptions* pOptions = dynamic_cast<const ChartDoubleOptions*>(pChartData))
	{
		if (pOptions->m_bShowSeparateGraphs)
			seriesCount = 1;
	}
	else if (const ChartCpuOptions* pCpuOptions = dynamic_cast<const ChartCpuOptions*>(pChartData))
	{
		if (pCpuOptions->m_bOneGraphForEachCore)
			seriesCount = 1;
		else
			seriesCount = 4;
	}
	for (size_t i = 0; i < seriesCount; i++)
	{
		QLineSeries* pLineSeries = new QLineSeries(pChart);
		QColor color = pChartData->m_lineColor;
		if (i == 1)
		{
			color = pChartData->GenerateLineColor(pChartData->m_lineColor);
		}
		else if (i == 2)
		{
			color = pChartData->GenerateSecondLineColor(pChartData->m_lineColor);
		}
		else if (i == 3)
		{
			color = pChartData->GenerateLineColor(pChartData->GenerateSecondLineColor(pChartData->m_lineColor));
		}
		const QPen linePen(QBrush(color), pChartData->m_lineSize);
		pLineSeries->setPen(linePen);
		pChart->addSeries(pLineSeries);
	}

	pChart->legend()->hide();
	constexpr int marginToHideAxis = -2;
	pChart->setMargins(QMargins(marginToHideAxis,0,0, marginToHideAxis));
	pChart->layout()->setContentsMargins(0, 0, 0, 0);
	pChart->setBackgroundRoundness(0);
	pChart->setBackgroundBrush(QBrush(pChartData->m_backgroundColor));
	pChart->createDefaultAxes();
	
	QList<QAbstractAxis*> axes = pChart->axes();
	QAbstractAxis* pYAxis = axes[1];
	pYAxis->setRange(0, 100);

	QValueAxis* pXAxis = (QValueAxis*)axes[0];
	pXAxis->setRange(0, c_XAxisRange);

	const QBrush gridBrush(pChartData->GenerateGridColor(pChartData->m_backgroundColor));
	QPen gridPen(gridBrush, pChartData->m_lineSize, Qt::PenStyle::DotLine);
	for (size_t i = 0, axesSize = axes.size(); i < axesSize; i++)
	{
		QAbstractAxis* pAxis = axes[i];
		//pAxis->hide();
		pAxis->setTitleVisible(false);
		pAxis->setLabelsVisible(false);
		pAxis->setGridLineVisible(true);
		pAxis->setMinorGridLineVisible(false);
		pAxis->setLineVisible(false);
		pAxis->setGridLinePen(gridPen);
		QValueAxis* pValueAxis = static_cast<QValueAxis*>(pAxis);
		const int tickCount = i == 0 ? 4 : 3;
		pValueAxis->setTickCount(tickCount);
	}
	
	const ChartGlobalOptions& chartGlobalOptions = pChartData->m_chartGlobalOptions;
	setWindowFlag(Qt::WindowStaysOnTopHint, chartGlobalOptions.m_bAllwaysOnTheTop);	// ale ked sa meni hodnota tak je potrebny nejaky refresh
	setWindowFlag(Qt::WindowTransparentForInput, chartGlobalOptions.m_bPassThroughtMode);
	setWindowOpacity(0.01 * pChartData->m_chartGlobalOptions.m_transparency);
	resize(pChartData->m_size);
	move(pChartData->m_position);
	setVisible(pChartData->m_bShowGraph);
}

void ChartWidget::AddData(const double values[2])
{
	const auto MoveData = [](QVector<QPointF>& data)
		{
			QPointF& last = data.back();
			if (last.y() < c_YAxisMin)
				last.setY(c_YUnderValue);	// Move value under graph, so it will not be visible.
			const QPointF sub(1.0, 0);
			for (QPointF& point : data)
			{
				point -= sub;
			}
			if (const size_t dataSize = data.size(); dataSize > c_XAxisRange)
			{
				for (size_t i = 0, size = dataSize - 1;i < size;i++)
				{
					data[i] = data[i + 1];
				}
				data.pop_back();
			}
		};
	m_firstData.append(QPointF(c_XAxisRange+1,values[0] * m_firstGraphScale));
	m_secondData.append(QPointF(c_XAxisRange+1, values[1] * m_firstGraphScale));
	MoveData(m_firstData);
	MoveData(m_secondData);

	QChart* pChart = chart();
	const QList<QAbstractSeries*> series = pChart->series();

	const auto GenerateSecondData= [](double input, const QPointF& last)
	{
		double yAxis = last.y();
		if (yAxis < 100)
		{
			yAxis = c_YUnderValue;
		}
		else
		{
			yAxis = input;
		}
		return QPointF(last.x()/* + 0.5*/, yAxis);
	};

	if (series.size() > 2)
	{
		QPointF last1 = m_firstData.back();
		if (last1.y() < 100)
		{
			last1.setY(c_YUnderValue);
		}
		m_thirdData.append(GenerateSecondData(values[0], m_firstData.back()));
		m_fourthData.append(GenerateSecondData(values[1], m_secondData.back()));
		MoveData(m_thirdData);
		MoveData(m_fourthData);
	}

	const QVector<QPointF>* data[]{ &m_firstData , &m_secondData, &m_thirdData, &m_fourthData };

	
	for (size_t i = 0, seriesSize = series.size(); i < seriesSize && i < std::size(data);i++)
	{
		QLineSeries* pLineSeries = static_cast<QLineSeries*>(series[i]);
		pLineSeries->clear();
		pLineSeries->append(*data[i]);
	}

	repaint();
}

void ChartWidget::SetPassThroughMode(const bool bPassThrough)
{
	setWindowFlag(Qt::WindowTransparentForInput, bPassThrough);
	show();
}

bool ChartWidget::event(QEvent* pEvent)
{
	if (pEvent->type() == QEvent::MouseButtonPress)
	{
		const QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(pEvent);
		if (pMouseEvent->button() == Qt::LeftButton)
		{
			windowHandle()->startSystemMove();
			if (pMouseEvent->modifiers() & Qt::KeyboardModifier::ControlModifier)
			{

			}
		}
	}
	else if (pEvent->type() == QEvent::MouseButtonRelease)
	{
		const QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(pEvent);
		if (pMouseEvent->button() == Qt::LeftButton)
		{
			m_pPerfMonitor->BackUpPositions();
		}
		else if (pMouseEvent->button() == Qt::RightButton)
		{
			m_pPerfMonitor->ShowMenu(pMouseEvent->globalPosition().toPoint());
		}
	}
	else if (pEvent->type() == QEvent::Move)
	{
		const QMoveEvent* pMoveEvent = static_cast<QMoveEvent*>(pEvent);
		if (pMoveEvent->spontaneous())
		{
			constexpr int snappPixels = 5;
			constexpr int snappDouble = snappPixels * 2;
			constexpr int widgetRectSize = 1;
			const QRect thisR = rect();
			const QPoint originalPos = pos();
			QPoint myPos(originalPos);
			const QRect originRec(myPos, thisR.size());
			const QRect originLeftRect(originRec.left() - snappPixels, originRec.top() - snappPixels, snappDouble, originRec.height() + snappDouble);
			const QRect originTopRect(originRec.left() - snappPixels, originRec.top() - snappPixels, originRec.width() + snappDouble, snappDouble);
			const QRect originRightRec(originRec.left() + originRec.width() - snappPixels, originRec.top() - snappPixels, snappDouble, originRec.height() + snappDouble);
			const QRect originBottomRec(originRec.left() - snappPixels, originRec.top() + originRec.height() - snappPixels, originRec.width() + snappDouble, snappDouble);


			const QVector<ChartWidget*>& graphs = m_pPerfMonitor->GetGraphs();
			for (ChartWidget* pWidget : graphs)
			{
				if (pWidget == this)
					continue;
				const QRect widgetR = pWidget->rect();
				const QPoint widgetPos = pWidget->pos();
				const QRect widgetRect(widgetPos, widgetR.size());
				const QRect widgetLeft(widgetRect.left(),										 widgetRect.top(), widgetRectSize, widgetRect.height());
				const QRect widgetRight(widgetRect.left() - widgetRectSize + widgetRect.width(), widgetRect.top(), widgetRectSize, widgetRect.height());
				const QRect widgetTop(widgetRect.left(),										   widgetRect.top(), widgetRect.width(), widgetRectSize);
				const QRect widgetBottom(widgetRect.left(), widgetRect.top() - widgetRectSize + widgetRect.height(), widgetRect.width(), widgetRectSize);
				if (originLeftRect.intersects(widgetLeft))
				{
					myPos.setX(widgetRect.left());
				}
				else if (originLeftRect.intersects(widgetRight))
				{
					myPos.setX(widgetRect.left() + widgetRect.width());
				}
				else if (originRightRec.intersects(widgetLeft))
				{
					myPos.setX(widgetRect.left() - originRec.width());
				}
				else if (originRightRec.intersects(widgetRight))
				{
					myPos.setX(widgetRect.left() + widgetRect.width() - originRec.width());
				}


				if (originTopRect.intersects(widgetTop))
				{
					myPos.setY(widgetRect.top());
				}
				else if (originTopRect.intersects(widgetBottom))
				{
					myPos.setY(widgetRect.top() + widgetRect.height());
				}
				else if (originBottomRec.intersects(widgetTop))
				{
					myPos.setY(widgetRect.top() - originRec.height());
				}
				else if (originBottomRec.intersects(widgetBottom))
				{
					myPos.setY(widgetRect.top() + widgetRect.height() - originRec.height());
				}
			}
			move(myPos);
			const QPoint& oldPos = pMoveEvent->oldPos();
			const QPoint& newPos = pMoveEvent->pos();
		}
	}

	return QChartView::event(pEvent);
}

