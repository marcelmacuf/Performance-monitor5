#pragma once

#include <QSlider>

class MySlider  : public QSlider
{
	Q_OBJECT

public:
	MySlider(QWidget *pParent);
	~MySlider();
protected :
	void paintEvent(QPaintEvent* pPaintEvent) override;
};

