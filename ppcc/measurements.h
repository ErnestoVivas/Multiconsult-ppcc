#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <memory>
#include <vector>
#include <list>
#include <utility>

#include "xlsxdocument.h"
#include "xlsxworkbook.h"

class SheetData {

    // One 'SheetData' object represents one excel sheet
    // with measurement data from various days

public:
    SheetData();
    SheetData(const QString&);
    ~SheetData();

    QString sheetName;
    QString xAxisLabel;
    QString yAxisLabel;
    short numOfDays;
    std::vector<std::pair<QString, int> > daysAndCounting;
    std::vector<QVariant> allDays;
    std::vector<QVariant> timestamps;
    std::vector<QVariant> measurements;
    std::vector<std::shared_ptr<QLineSeries> > measurementSeries;

    int extractDays();
    int extractLineSeries();
};

class MeasurementsDocument {

    // MeasurementsDocument is one Document
    // consisting of one or more sheets

public:
    MeasurementsDocument();
    MeasurementsDocument(const QString&);
    ~MeasurementsDocument();

    QString docName;
    std::shared_ptr<QXlsx::Document> measurementsDoc;
    std::vector<std::shared_ptr<SheetData> > sheets;

private:
    bool parseDocumentData();

};

#endif // MEASUREMENTS_H
