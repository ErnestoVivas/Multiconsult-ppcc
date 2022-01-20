//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
#include "sectordayanalysis.h"
#include "sectorweekanalysis.h"
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

signals:
    void updateTextEditDocumentData(int);

private:

    // gui
    Ui::MainWindow* ui;

    // diagram function widgets
    SimpleDiagramFunction* simpleDiagramFunction;
    SiteAnalysis* siteAnalysis;
    SectorDayAnalysis* sectorDayAnalysis;
    SectorWeekAnalysis* sectorWeekAnalysis;

    // chart parameters
    QValueAxis* xAxis;
    QValueAxis* yAxis;                              // currently not used
    std::shared_ptr<QChart> measurementsChart;      // holds the official chart with the correct data
    std::shared_ptr<QChart> auxiliaryUpdateChart;   // pseudo chart to which chartView is set while official chart is being updated
    QList<QLineSeries*> displayedSeries;            // holds only the series currently displayed in the chart

    // File management parameters
    std::vector<MeasurementsDocument> documents;
    int fileManagerSelectedFile;

    // file management functions (not used as slots)
    void setupComboBoxesFileCategories();
    void readDocument(QXlsx::Document*);

    // sub functions for diagram generation
    bool compareDates(QDate&, QString&, DateFormat&, bool&);
    std::vector<int> findWeekdays(int&, int&, int&);
    QList<QLineSeries*> getAverageFromSeries(QList<QLineSeries*>&, Frequency, bool);
    QList<QLineSeries*> transformAllTo15MinTicks(QList<QLineSeries*>&);
    void displayDiagramDataAsText();

private slots:

    // general gui / program functions
    void exitProgram();

    // file management functions
    void importDocument();
    void saveDiagram();
    void updateFileSubCatComboBox(int);
    void getFileCategories(int);
    void setFileCategory(int);
    void setFileSubCategory(int);
    void displayDocumentDataAsText(int);
    //void setFileFreq(int);          // currently not used

    //int generateClassifiedDiagram();

    // Functions to communicate with diagram function widgets
    void selectFunction(int);
    void updateSheetListSimpleDiagramFunction(int);
    void updateDaysSimpleDiagramFunction(int, int);
    void updateSheetListSiteAnalysis(int);

    // Diagram generation functions
    int generateDiagram();
    int generateSimpleDiagram();
    int generateSiteAnalysisDiagram();
    int generateSectorWeekdayDiagram();
    int generateSectorWeekDiagram();
};

#endif // MAINWINDOW_H
