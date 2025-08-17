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
		const QPen linePen(QBrush(color), pChartData->m_lineSize, Qt::PenStyle::SolidLine, Qt::PenCapStyle::FlatCap, Qt::PenJoinStyle::SvgMiterJoin);
		pLineSeries->setPen(linePen);
		pChart->addSeries(pLineSeries);
	}
	pChart->legend()->hide();
	if (pChartData->m_chartGlobalOptions.m_bAllwaysShowLabel)
	{
		QFont font;
		font.setPixelSize(10);
		QGraphicsTextItem* pTitle = new QGraphicsTextItem(pChartData->m_optionName);
		pTitle->setPos(-3, -5);
		pTitle->setFont(font);
		pTitle->setDefaultTextColor(pChartData->m_lineColor);
		pChart->scene()->addItem(pTitle);
	}
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
	const QPen gridPen(gridBrush, pChartData->m_lineSize, Qt::PenStyle::DotLine);
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
	const double scaledValues[2]{ values[0] * m_firstGraphScale, values[1] * m_firstGraphScale };
	constexpr int xEnd = c_XAxisRange + 1;
	const bool bShowSecond = scaledValues[1] >= 100;
	m_firstData.append(QPointF(xEnd, bShowSecond ? c_YUnderValue : scaledValues[0]));
	m_secondData.append(QPointF(xEnd, scaledValues[1]));
	MoveData(m_firstData);
	MoveData(m_secondData);

	QChart* pChart = chart();
	const QList<QAbstractSeries*> series = pChart->series();
	if (series.size() > 2)
	{
		m_thirdData.append(QPointF(xEnd, bShowSecond ? values[0] : c_YUnderValue));
		m_fourthData.append(QPointF(xEnd, bShowSecond ? values[1] : c_YUnderValue));
		MoveData(m_thirdData);
		MoveData(m_fourthData);
	}

	const QVector<QPointF>* data[]{ &m_firstData , &m_secondData, &m_thirdData, &m_fourthData };
	for (size_t i = 0, seriesSize = series.size(); i < seriesSize && i < std::size(data);i++)
	{
		QXYSeries* pLineSeries = static_cast<QXYSeries*>(series[i]);
		pLineSeries->replace(*data[i]);
	}

	repaint();
}

void ChartWidget::SetPassThroughMode(const bool bPassThrough)
{
	setWindowFlag(Qt::WindowTransparentForInput, bPassThrough);
	show();
}

ChartWidget::Boundary ChartWidget::GetInflatedBoundary() const
{
	constexpr int snappPixels = 4;
	constexpr int snappDouble = snappPixels * 2;
	const QRect thisR = rect();
	const QPoint originalPos = pos();
	const QPoint myPos(originalPos);
	const QRect originRec(myPos, thisR.size());
	Boundary inflatedBoundary;
	inflatedBoundary.originRec = originRec;
	inflatedBoundary.originLeftRect = QRect(originRec.left() - snappPixels, originRec.top() - snappPixels, snappDouble, originRec.height() + snappDouble);
	inflatedBoundary.originTopRect = QRect(originRec.left() - snappPixels, originRec.top() - snappPixels, originRec.width() + snappDouble, snappDouble);
	inflatedBoundary.originRightRec = QRect(originRec.left() + originRec.width() - snappPixels, originRec.top() - snappPixels, snappDouble, originRec.height() + snappDouble);
	inflatedBoundary.originBottomRec = QRect(originRec.left() - snappPixels, originRec.top() + originRec.height() - snappPixels, originRec.width() + snappDouble, snappDouble);
	return inflatedBoundary;
}

QPoint ChartWidget::Intersection(const Boundary& inflatedBoundary) const
{
	QPoint intPos(INT_MAX, INT_MAX);
	if (!isVisible())
		return intPos;
	constexpr int widgetRectSize = 1;
	const QRect widgetR = rect();
	const QPoint widgetPos = pos();
	const QRect widgetRect(widgetPos, widgetR.size());
	const QRect widgetLeft(widgetRect.left(), widgetRect.top(), widgetRectSize, widgetRect.height());
	const QRect widgetRight(widgetRect.left() - widgetRectSize + widgetRect.width(), widgetRect.top(), widgetRectSize, widgetRect.height());
	const QRect widgetTop(widgetRect.left(), widgetRect.top(), widgetRect.width(), widgetRectSize);
	const QRect widgetBottom(widgetRect.left(), widgetRect.top() - widgetRectSize + widgetRect.height(), widgetRect.width(), widgetRectSize);
	if (inflatedBoundary.originLeftRect.intersects(widgetLeft))
	{
		intPos.setX(widgetRect.left());
	}
	else if (inflatedBoundary.originLeftRect.intersects(widgetRight))
	{
		intPos.setX(widgetRect.left() + widgetRect.width());
	}
	else if (inflatedBoundary.originRightRec.intersects(widgetLeft))
	{
		intPos.setX(widgetRect.left() - inflatedBoundary.originRec.width());
	}
	else if (inflatedBoundary.originRightRec.intersects(widgetRight))
	{
		intPos.setX(widgetRect.left() + widgetRect.width() - inflatedBoundary.originRec.width());
	}


	if (inflatedBoundary.originTopRect.intersects(widgetTop))
	{
		intPos.setY(widgetRect.top());
	}
	else if (inflatedBoundary.originTopRect.intersects(widgetBottom))
	{
		intPos.setY(widgetRect.top() + widgetRect.height());
	}
	else if (inflatedBoundary.originBottomRec.intersects(widgetTop))
	{
		intPos.setY(widgetRect.top() - inflatedBoundary.originRec.height());
	}
	else if (inflatedBoundary.originBottomRec.intersects(widgetBottom))
	{
		intPos.setY(widgetRect.top() + widgetRect.height() - inflatedBoundary.originRec.height());
	}
	return intPos;
}

void ChartWidget::CollectJoinedWidgets(ChartWidget* pMovingWidget, QVector<ChartWidget*>& movingWidgets) const
{
	const size_t startSize = movingWidgets.size();
	const Boundary inflatedBoundary = pMovingWidget->GetInflatedBoundary();
	const QVector<ChartWidget*>& graphs = m_pPerfMonitor->GetGraphs();
	for (ChartWidget* pWidget : graphs)
	{
		if (pWidget == pMovingWidget)
			continue;
		if(std::ranges::find(movingWidgets, pWidget) != movingWidgets.end())
			continue;

		const QPoint intersectionPoint = pWidget->Intersection(inflatedBoundary);
		if (intersectionPoint.x() != INT_MAX || intersectionPoint.y() != INT_MAX)
		{
			movingWidgets.append(pWidget);
			CollectJoinedWidgets(pWidget, movingWidgets);
		}
	}
}

bool ChartWidget::event(QEvent* pEvent)
{
	if (pEvent->type() == QEvent::MouseButtonPress)
	{
		m_movingWidgets.clear();
		const QMouseEvent* pMouseEvent = static_cast<QMouseEvent*>(pEvent);
		if (pMouseEvent->button() == Qt::LeftButton)
		{
			windowHandle()->startSystemMove();
			if (pMouseEvent->modifiers() & Qt::KeyboardModifier::ControlModifier)
			{
				CollectJoinedWidgets(this, m_movingWidgets);
			}
		}
	}
	else if (pEvent->type() == QEvent::MouseButtonRelease)
	{
		m_movingWidgets.clear();
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
			const Boundary inflatedBoundary = GetInflatedBoundary();
			QPoint myPos(pos());
			const QVector<ChartWidget*>& graphs = m_pPerfMonitor->GetGraphs();
			for (ChartWidget* pWidget : graphs)
			{
				if (pWidget == this || m_movingWidgets.contains(pWidget))
					continue;
				const QPoint intersectionPoint = pWidget->Intersection(inflatedBoundary);
				if (intersectionPoint.x() != INT_MAX)
					myPos.setX(intersectionPoint.x());
				if (intersectionPoint.y() != INT_MAX)
					myPos.setY(intersectionPoint.y());
			}
			if (!m_movingWidgets.empty())
			{
				const QPoint& oldPos = pMoveEvent->oldPos();
				const QPoint diff = myPos - oldPos;
				for (ChartWidget* pWidget : m_movingWidgets)
				{
					QPoint widgetPos = pWidget->pos();
					widgetPos += diff;
					pWidget->move(widgetPos);
				}
			}
			move(myPos);
		}
	}

	return QChartView::event(pEvent);
}

