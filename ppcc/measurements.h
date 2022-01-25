//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <QtCharts>
#include <QtCharts/QLineSeries>
#include <cmath>
#include <memory>
#include <vector>
#include <list>
#include <utility>

#include "enumerations.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"


class SheetData {

    // One 'SheetData' object represents one excel sheet
    // with measurement data from various days

public:
    SheetData();
    SheetData(const QString&);
    ~SheetData();

    // general sheet information
    QString sheetName;
    QString dateLabel;
    QString xAxisLabel;
    QString yAxisLabel;

    // aux variables needed for diagram generation
    Frequency sheetFreq;
    bool firstDayIsComplete;
    bool lastDayIsComplete;
    bool oneDayOnly;
    bool twoDaysOnly;
    bool sheetIsPlottable;
    bool xIsTime;
    bool datesAreStrings;
    short numOfDays;

    // data storage
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

    // general document information
    QString docName;
    QString documentDataAsText;
    std::shared_ptr<QXlsx::Document> measurementsDoc;
    std::vector<std::shared_ptr<SheetData> > sheets;

    // Classification of the document
    QString customSectorStr;
    QString customSubSectorStr;
    SystemVoltage docSystemVoltage;
    Sector docSector;
    ResidentialRange docResRange;
    Commercial docSubCommercial;
    Industrial docSubIndustrial;
    CustomSubSector docCustomSubSector;
    Frequency docFreq;
    DateFormat docDateFormat;

    // getters and setters
    int getSubCategory();
    void setSector(int);
    void setSubCategory(int);

private:
    bool parseDocumentData();

};

#endif // MEASUREMENTS_H
