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
	QSize GetMinimumSize() const;
	const QVector<ChartWidget*>& GetGraphs() const { return m_pGraphs; }
public slots:
	void BackUpPositions();
	void ShowMenu(const QPoint& pos);
protected:
	void closeEvent(QCloseEvent* pVvent) override;
	void showEvent(QShowEvent* pEvent) override;
private slots:
	void HandleTimeout();
	void RestorePositions();
	void PassThroughMode();
	void SaveDataFromUi();
	void ButtonBoxClicked(QAbstractButton* pButton);
private:
	void CreateTrayActions();
	void InitUiElements();
	void ValidateData();
	void LoadDataToUi(const ChartGlobalOptions& globalOptions, const ChartCpuOptions& cpuOptions, const ChartOptions& ramOptions,
					  const ChartDoubleOptions& diskOptions, const ChartNetOptions& netOptions);
	void LoadDataToChart(const ChartOptions& options, QCheckBox* pVisible, QComboBox* pGraph, QCheckBox* pDoubleLine, QComboBox* pSize,
						 ColorButton* pBack, QCheckBox* pManualForeground, ColorButton* pForeground);
	void SaveDataFromChart(ChartOptions& options, QCheckBox* pVisible, QComboBox* pGraph, QCheckBox* pDoubleLine, QComboBox* pSize,
						   ColorButton* pBack, QCheckBox* pManualForeground, ColorButton* pForeground);
	void ResetSettings(ChartGlobalOptions& globalOptions, ChartCpuOptions& cpuOptions, ChartOptions& ramOptions,
					   ChartDoubleOptions& diskOptions, ChartNetOptions& netOptions, const bool bPositions);
	void ResetUi();
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


	QSystemTrayIcon* m_pTrayIcon{ nullptr };
	QAction* m_pPassThroughAction{ nullptr };
	
	QTimer m_timer;
	void* m_hQuery = nullptr;
};

