#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <memory>
#include <vector>
#include <list>

#include "xlsxdocument.h"
#include "xlsxworkbook.h"

class SheetData {

    // One 'MeasurementsSheet' object represents one excel sheet
    // with measurement data from various days

public:
    SheetData();
    SheetData(const QString&);
    ~SheetData();

    QString sheetName;
    short numOfDays;
    std::vector<QVariant> days;
    std::vector<QVariant> timestamps;
    std::vector<QVariant> measurements;
    std::vector<std::shared_ptr<QLineSeries> > measurementSeries;
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
