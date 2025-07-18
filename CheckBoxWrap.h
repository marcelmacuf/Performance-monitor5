#pragma once
#include <QCheckBox>
#include <QHBoxLayout>

#include "clickablelabel.h"

/**
 * @author thibsc
 */
class CheckBoxWrap : public QCheckBox
{
    Q_OBJECT

        Q_PROPERTY(bool wordwrap READ isWordWrap WRITE setWordWrap)
        Q_PROPERTY(QString text READ text WRITE setText)

public:
    CheckBoxWrap(QWidget* parent = Q_NULLPTR);
    CheckBoxWrap(const QString& text, QWidget* parent = Q_NULLPTR);
    ~CheckBoxWrap();
    bool isWordWrap() const;
    void setWordWrap(bool wordwrap);
    QString text() const;
    void setText(const QString& text);
    QSize sizeHint() const override;

private slots:
    void labelIsClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void init();
    const int separation = 5;
    QHBoxLayout* m_hMainLayout;
    ClickableLabel* m_label;

};
