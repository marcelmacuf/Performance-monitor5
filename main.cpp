#include "stdafx.h"
#include "PerformanceMonitor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("Ferry");
	QCoreApplication::setOrganizationDomain("ksb-csr.net");
	QCoreApplication::setApplicationName("Performance monitor 5");
    PerformanceMonitor window;
    window.move(1200, 500);
    window.show();
    return app.exec();
}
