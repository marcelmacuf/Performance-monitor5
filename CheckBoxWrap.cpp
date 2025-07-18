#include "stdafx.h"
#include "CheckBoxWrap.h"
#include <QCheckBox>
#include <QResizeEvent>
#include <QStyle>

CheckBoxWrap::CheckBoxWrap(QWidget* parent)
	: QCheckBox(parent)
	, m_hMainLayout(new QHBoxLayout(this))
	, m_label(new ClickableLabel(this))
{
	init();
}

CheckBoxWrap::CheckBoxWrap(const QString& text, QWidget* parent)
	: QCheckBox(parent)
	, m_hMainLayout(new QHBoxLayout(this))
	, m_label(new ClickableLabel(text, this))
{
	init();
}

CheckBoxWrap::~CheckBoxWrap()
{
	delete m_label;
	delete m_hMainLayout;
}

bool CheckBoxWrap::isWordWrap() const
{
	return m_label->wordWrap();
}

void CheckBoxWrap::setWordWrap(bool wordwrap)
{
	m_label->setWordWrap(wordwrap);
}

QString CheckBoxWrap::text() const
{
	return m_label->text();
}

void CheckBoxWrap::setText(const QString& text)
{
	m_label->setText(text);
}

QSize CheckBoxWrap::sizeHint() const
{
	QFontMetrics fm(m_label->font());
	QRect r = m_label->rect();
	r.setLeft(r.left() + m_label->indent() + separation);
	QRect bRect = fm.boundingRect(r, int(Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap), m_label->text());
	QSize ret = QSize(QWidget::sizeHint().width(), bRect.height());
	return ret;
}

void CheckBoxWrap::labelIsClicked()
{
	setChecked(!isChecked());
}

void CheckBoxWrap::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	updateGeometry();
}

void CheckBoxWrap::init()
{
	setLayout(m_hMainLayout);
	QStyleOptionButton opt;
	initStyleOption(&opt);
	int indicatorW = style()->pixelMetric(QStyle::PixelMetric::PM_IndicatorWidth, &opt, this);
	// Useless in our case, we only need the indicator width
	//int indicatorH = style()->pixelMetric(QStyle::PixelMetric::PM_IndicatorHeight, &opt, this);
	m_hMainLayout->setContentsMargins(0, 0, 0, 0);
	m_hMainLayout->addWidget(m_label);
	m_label->setIndent(indicatorW + separation);
	m_label->setWordWrap(true);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	connect(m_label, SIGNAL(clicked()), this, SLOT(labelIsClicked()));
}