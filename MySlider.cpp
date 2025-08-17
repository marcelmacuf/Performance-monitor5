#include "stdafx.h"
#include "MySlider.h"

MySlider::MySlider(QWidget* pParent)
	: QSlider(pParent)
{
}

MySlider::~MySlider()
{
}

void MySlider::paintEvent(QPaintEvent* pPaintEvent)
{
	QSlider::paintEvent(pPaintEvent);

	QPainter* painter = new QPainter(this);
	painter->setPen(QPen(Qt::black));

	const QRect rect = this->geometry();

	const int numTicks = (maximum() - minimum()) / tickInterval();

	QFontMetrics fontMetrics = QFontMetrics(this->font());

	QStyleOptionSlider slider; 
	initStyleOption(&slider);
	const int available = style()->pixelMetric(QStyle::PM_SliderSpaceAvailable, &slider, this); 
	const int fudge = style()->pixelMetric(QStyle::PM_SliderLength, &slider, this) / 2; 
	if (this->orientation() == Qt::Horizontal) 
	{
		const int fontHeight = fontMetrics.height();
		for (int i = 0; i <= numTicks; i++) 
		{
			const int tickNum = minimum() + (tickInterval() * i);
			int xPos = QStyle::sliderPositionFromValue(minimum(), maximum(), tickNum, available) + fudge;
		//	if (i != 0)
			{
				int textWidth = fontMetrics.tightBoundingRect(QString::number(tickNum)).width();
				//if (i != numTicks)
					textWidth /= 2;
				xPos -= textWidth;
			}
			painter->drawText(QPoint(xPos, rect.height() /* fontHeight */ ), QString::number(tickNum));
		}

	}
	else if (this->orientation() == Qt::Vertical) {

		//do something else for vertical here, I haven't implemented it because I don't need it
	}
	else {
		return;
	}

	painter->drawRect(rect);
}

