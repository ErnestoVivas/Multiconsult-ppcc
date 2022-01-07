#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QLineSeries>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QDebug>
#include <vector>

#include "xlsxdocument.h"
//#include "QXlsx/header/xlsxdocument.h"
//#include "QXlsx/header/xlsxchartsheet.h"
//#include "QXlsx/header/xlsxcellrange.h"
//#include "xlsxchart.h"
//#include "xlsxrichstring.h"
//#include "xlsxworkbook.h"
//using namespace QXlsx;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QXlsx::Document* measurementData;
    QChart* measurementsChart;
    short int numOfDays;
    std::vector<QDate> days;
    std::vector<QLineSeries*> measurementSeries;

    void readDocument(QXlsx::Document*);

private slots:
    void openFile();
    void generateDiagram();
    void saveDiagram();
};
#endif // MAINWINDOW_H
