//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt library
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
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>

// QXlsx
#include "xlsxdocument.h"

// C++ STL
#include <vector>
#include <tuple>
#include <memory>

// custom classes, widgets
#include "measurements.h"
#include "categoriestracking.h"
#include "setcategoriesdialog.h"
#include "simplediagramfunction.h"
#include "siteanalysis.h"
#include "sectordayanalysis.h"
#include "sectorweekanalysis.h"
#include "sectorsubcatsanalysis.h"
#include "sectorandtotalnatdiagram.h"
#include "selectimportmethod.h"
#include "selectfunction.h"
#include "configdiagram.h"
#include "exportdiagramdialog.h"


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
    SectorSubCatsAnalysis* sectorSubCatsAnalysis;
    SectorAndTotalNatDiagram* sectorAndTotalNatDiagram;

    // chart parameters (should be a class of it's own...)
    short currentDiagramType;
    QString currentSelectedCategory;
    QString currentSelectedSubCategory;
    QString currentDayType;                         // used for both day and daytype
    int currentVisType;
    QValueAxis* xAxis;
    QValueAxis* yAxis;
    std::shared_ptr<QChart> measurementsChart;      // holds the official chart with the correct data
    std::shared_ptr<QChart> auxiliaryUpdateChart;   // pseudo chart to which chartView is set while official chart is being updated
    QList<QLineSeries*> displayedSeries;            // holds only the series currently displayed in the chart
    QLabel pointLabel;

    // File management parameters
    std::vector<MeasurementsDocument> documents;
    MeasurementsDocument docTotalNationalLoad;
    bool docTotalNationalLoadExists;
    int fileManagerSelectedFile;
    bool importCanceled;

    // Tracking of contained categories
    CategoriesTracking categoriesTracking;

    // file management functions (not used as slots)
    void configDocumentCategories(MeasurementsDocument&, Categories&);
    void readDocument(QXlsx::Document*);
    Categories getDocCategories(int&);

    // sub functions to export diagram data as excel
    QString convertTimeToStr(double);
    void writeLineSeriesToExcel(QXlsx::Document&, int);
    void saveSimpleDiagramAsExcel(QString&);
    void saveSiteDayDiagramAsExcel(QString&);
    void saveSectorDayDiagramAsExcel(QString&);
    void saveSectorWeekDiagramAsExcel(QString&);
    void saveSubCatsDiagramAsExcel(QString&);
    void saveSectorTotalNatDiagramAsExcel(QString&);

    // sub functions for diagram generation
    void resetMeasurementsChart();
    void findMinMaxDisplayedSeries(double&, double&);
    void findMaxLineSeries(QLineSeries*, double&, double&);
    void configureChartAxes(QString&, QString&, double&, double&);
    bool compareDates(QDate&, QString&, DateFormat&, bool&);
    std::vector<int> findWeekdays(int&, int&, int&);
    QList<QLineSeries*> getMeasurementDayLineSeries(int&, int&);
    void copyLineSeries(QLineSeries*&, QLineSeries*&);
    QList<QLineSeries*> getAverageFromSeries(QList<QLineSeries*>&, Frequency, bool);
    QList<QLineSeries*> transformAllTo15MinTicks(QList<QLineSeries*>&);

    // Diagram display functions
    void configSeriesForHover();
    void displayDiagramDataAsText();
    void displaySimpleDiagramAsText();
    void displaySiteAnalysisDiagramAsText();
    void displaySectorWeekdayDiagramAsText();
    void displaySectorWeekDiagramAsText();
    void displaySectorSubCatsDiagramAsText();
    void displaySectorTotalNatDiagramAsText();

private slots:

    // general gui / program functions
    void openReadmeSpanish();
    void openReadmeEnglish();
    void exitProgram();

    // file management functions
    void selectImportMethod();
    void importDocument();
    void importDataBase();
    void importTotalNatDoc();
    void cancelImport();
    void saveDataBase();
    void getFileCategories(int);
    void setFileCategory(int);
    void setFileSubCategory(int);
    void removeDocument();
    void removeAllDocuments();
    QString parseDocumentDataAsText(int);

    //int generateClassifiedDiagram();

    // Functions to communicate with diagram function widgets
    void selectFunction();
    void selectFunction(int);
    void updateDaysSimpleDiagramFunction(int);
    void updateSubCatComboBox(int, QComboBox*);

    // Diagram generation functions
    int generateDiagram();
    int generateSimpleDiagram();
    int generateSiteAnalysisDiagram();
    int generateSectorWeekdayDiagram();
    int generateSectorWeekDiagram();
    int generateSectorSubCatsDiagram();
    int generateSectorAndTotalNatDiagram();
    void showPointOnHover(const QPointF&, bool);

    // Diagram functions after generation
    void refreshDiagram();
    void configDiagram();
    void exportDiagram();
};

#endif // MAINWINDOW_H
