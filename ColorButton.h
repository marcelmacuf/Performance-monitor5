#pragma once
#include <QPushButton>

class ColorButton : public QPushButton
{
	Q_OBJECT
public:
	ColorButton(QWidget* parent);
	QColor GetColor() const;
	void SetColor(const QColor& color);
private slots:
	void ChooseColor();
	void ColorChosen();
private:
	QPointer<QColorDialog>m_pDialog;
};

