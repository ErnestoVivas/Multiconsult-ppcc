#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QLineSeries>
#include <QComboBox>
#include <QListWidget>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QDebug>
#include <vector>
#include <memory>

#include "measurements.h"
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


class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::vector<MeasurementsDocument> documents;
    std::shared_ptr<QChart> measurementsChart;      // holds the official chart with the correct data
    std::shared_ptr<QChart> auxiliaryUpdateChart;   // pseudo chart to which chartView is set while official chart is being updated
    QList<QLineSeries*> displayedSeries;            // holds the only the series currently displayed in the chart
    int selectedDocIndex;
    int selectedSheetIndex;

    void readDocument(QXlsx::Document*);

private slots:
    int generateDiagram();
    void importDocument();
    void addEntryComboBoxDocSelection(const QString&);
    void updateSheetList(int);
    void updateDaysEntries(int);
    void saveDiagram();
};

#endif // MAINWINDOW_H
