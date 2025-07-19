#include "stdafx.h"
#include "ColorButton.h"

ColorButton::ColorButton(QWidget* parent) : QPushButton(parent)
{
	connect(this, &QPushButton::released, this, &ColorButton::ChooseColor);
	setAutoFillBackground(true);
}

QColor ColorButton::GetColor() const
{
	const QPalette& pal = palette();
	return pal.color(QPalette::Button);
}

void ColorButton::SetColor(const QColor& color)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Button, color);
	setPalette(pal);
}

void ColorButton::ChooseColor()
{
	QColorDialog* dialog = m_pDialog.data();
	if (dialog) 
	{
		dialog->show();
		dialog->raise();
		dialog->activateWindow();
		return;
	}
	
	dialog = new QColorDialog(this);
	dialog->setCurrentColor(GetColor());
	dialog->setWindowIcon(windowIcon());
	dialog->setModal(true);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	QObject::connect(dialog, &QDialog::accepted, this, [this]() { ColorChosen();});
	m_pDialog = dialog;
	dialog->show();
}

void ColorButton::ColorChosen()
{
	QColorDialog* dialog = m_pDialog.data();
	if (!dialog)
	{
		return;
	}

	if (dialog->selectedColor().isValid()) 
	{
		SetColor(dialog->selectedColor());
	}
}
