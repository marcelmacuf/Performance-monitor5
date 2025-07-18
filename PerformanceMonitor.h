#pragma once

#include <QtWidgets/QDialog>
#include "ui_PerformanceMonitor.h"
#include "ChartWidget.h"

class PerformanceMonitor : public QDialog
{
    Q_OBJECT

public:
    PerformanceMonitor(QWidget *parent = nullptr);
    ~PerformanceMonitor();
protected:
	void closeEvent(QCloseEvent* event) override
	{
		if (!event->spontaneous() || !isVisible())
			return;
		hide();
		event->ignore();
	}
private slots:
	void HandleTimeout();
private:
	void CreateTrayActions();
	void ResetSettings(const bool bPositions);
	void CreatePerfCounters();
	void ReleasePerfCounters();
	void ShowError(const QString& errorMessage);
	ChartWidget* FindChart(const QString& accesibleName) const;
    Ui::PerformanceMonitorClass ui;

	ChartGlobalOptions m_globalOptions;
	ChartCpuOptions m_cpuOptions;
	ChartOptions m_ramOptions;
	ChartDoubleOptions m_diskOptions;
	ChartNetOptions m_netOptions;

	std::unique_ptr<QSettings>m_pAppSettings;
	QVector<ChartWidget*> m_pGraphs;


	QSystemTrayIcon* m_pTrayIcon;
	
	QTimer m_timer;
	void* m_hQuery = nullptr;
};

