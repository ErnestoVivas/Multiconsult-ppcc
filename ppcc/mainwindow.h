#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QLineSeries>
#include <QComboBox>
#include <QListWidget>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QDate>
#include <QDebug>
#include <vector>
#include <tuple>
#include <memory>

#include "measurements.h"
#include "setcategoriesdialog.h"
#include "simplediagramfunction.h"
#include "siteanalysis.h"
#include "xlsxdocument.h"
//#include "QXlsx/header/xlsxdocument.h"
//#include "QXlsx/header/xlsxchartsheet.h"
//#include "QXlsx/header/xlsxcellrange.h"
//#include "xlsxchart.h"
//#include "xlsxrichstring.h"
//#include "xlsxworkbook.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    SimpleDiagramFunction* simpleDiagramFunction;
    SiteAnalysis* siteAnalysis;

    QButtonGroup classificationVisTypeGroup;

    std::vector<MeasurementsDocument> documents;
    std::shared_ptr<QDateTimeAxis> xAxisDateTime;
    std::shared_ptr<QValueAxis> xAxisValue;
    std::shared_ptr<QValueAxis> yAxis;
    std::shared_ptr<QChart> measurementsChart;      // holds the official chart with the correct data
    std::shared_ptr<QChart> auxiliaryUpdateChart;   // pseudo chart to which chartView is set while official chart is being updated
    QList<QLineSeries*> displayedSeries;            // holds only the series currently displayed in the chart

    void readDocument(QXlsx::Document*);

    QList<QLineSeries*> getAverageFromSeries(QList<QLineSeries*>&);
    QList<QLineSeries*> getSumFromSeries(QList<QLineSeries*>&);

private slots:
    bool compareDates(QDate&, QString&, DateFormat&, bool&);
    std::vector<int> findWeekdays(int&, int&, int&);

    //int generateClassifiedDiagram();
    void importDocument();

    void saveDiagram();
    void exitProgram();


    void selectFunction(int);

    void updateSheetListSimpleDiagramFunction(int);
    void updateDaysSimpleDiagramFunction(int, int);
    void updateSheetListSiteAnalysis(int);

    int generateDiagram();
    int generateSimpleDiagram();
    int generateSiteAnalysisDiagram();
};

#endif // MAINWINDOW_H
