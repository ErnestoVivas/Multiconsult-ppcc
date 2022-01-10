#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <memory>
#include <vector>
#include <list>
#include "xlsxdocument.h"


class MeasurementsSheet {

    // One 'MeasurementsSheet' object represents one excel sheet
    // with measurement data from various days

public:
    MeasurementsSheet();
    ~MeasurementsSheet();

    QString sheetName;
    short numOfDays;
    std::vector<QDate> days;
    std::vector<QLineSeries*> measurementSeries;
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
    std::vector<std::shared_ptr<MeasurementsSheet> > sheets;

};

#endif // MEASUREMENTS_H
