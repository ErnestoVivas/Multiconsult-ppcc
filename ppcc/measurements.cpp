#include "measurements.h"

SheetData::SheetData() {

}

SheetData::SheetData(const QString& currentSheetName) {
    this->sheetName = currentSheetName;
    this->numOfDays = 0;
}

SheetData::~SheetData() {

}

int SheetData::extractDays() {
    int returnVal = -1;                             // '-1' when sheet is empty
    if(this->allDays.size() >= 1) {
        QVariant currentDayVal = this->allDays[0];
        QVariant prevDayVal = this->allDays[0];
        short dayCounter = 0;
        for(unsigned short i = 0; i < this->allDays.size(); ++i) {
            if(this->allDays.size() == 1) {
                this->numOfDays = 1;
                ++dayCounter;
                daysAndCounting.emplace_back(std::make_pair(prevDayVal.toString(), dayCounter));
            } else if(i == 0) {
                this->numOfDays = 1;
                ++dayCounter;
            } else if(i == (this->allDays.size() - 1)) {    // assuming there is never a day with just 1 measurement
                ++dayCounter;
                daysAndCounting.emplace_back(std::make_pair(prevDayVal.toString(), dayCounter));
            } else {
                currentDayVal = this->allDays[i];
                if(currentDayVal != prevDayVal) {
                    ++(this->numOfDays);
                    daysAndCounting.emplace_back(std::make_pair(prevDayVal.toString(), dayCounter));
                    dayCounter = 1;
                } else {
                    ++dayCounter;
                }
                prevDayVal = this->allDays[i];
            }
        }

        // debugging
        /*
        qDebug() << "Sheet: " << this->sheetName << ", numOfDays: " << this->numOfDays;
        for(unsigned short s = 0; s < daysAndCounting.size(); ++s) {
            qDebug() << daysAndCounting[s].first << ", measurements: " << daysAndCounting[s].second;
        }
        */
        returnVal = 0;
    }

    return returnVal;
}

int SheetData::extractLineSeries() {
    int returnVal = -1;
    if(this->allDays.size() >= 1) {

        // reserve vector space and create corresponding number of lineSeries
        this->measurementSeries.reserve(this->daysAndCounting.size());
        int numOfMeasurements;
        int measurementsCount = 0;
        double currentTimeValue;
        double currentDataValue;
        for(unsigned int i = 0; i < daysAndCounting.size(); ++i) {
            this->measurementSeries.emplace_back(std::make_shared<QLineSeries>());
            this->measurementSeries[i]->setName(daysAndCounting[i].first);

            numOfMeasurements = daysAndCounting[i].second;

            // case 1: timestamps are interpreted as dateTime objects
            // qDebug() << "Measurements of day " << daysAndCounting[i].first << ":";
            QTime currentTimestamp;
            if( (this->timestamps[measurementsCount].type() == 14) ||
                (this->timestamps[measurementsCount].type() == 15) ||
                (this->timestamps[measurementsCount].type() == 16) ) {
                // qDebug() << "Timestamp is of type " << this->timestamps[measurementsCount].typeName();
                this->xIsTime = true;
                for(int j = 0; j < numOfMeasurements; ++j) {
                    if(this->timestamps[j + measurementsCount].canConvert(15)) {
                        currentTimestamp = this->timestamps[j + measurementsCount].toTime();
                    }
                    if(this->timestamps[measurementsCount+j].type() == 14) {
                        currentTimestamp = QTime(0,0,0);
                    }
                    currentTimeValue = currentTimestamp.msecsSinceStartOfDay();
                    currentDataValue = this->measurements[j + measurementsCount].toReal();
                    this->measurementSeries[i]->append(currentTimeValue, currentDataValue);
                    // qDebug() << "(" << currentTimeValue << "," << currentDataValue << ")";
                }
            }

            // case 2: timestamps are interpreted as strings
            else {
                this->xIsTime = false;

                // Problem: sometimes "00:00:00" is not interpreted as QTime but as
                // QString, so conversion from string to double leads always 0. The first
                // method fixes this condition, second uses direct string to double conversion
                for(int j = 0; j < numOfMeasurements; ++j) {
                    QString currentTimeStr = this->timestamps[j + measurementsCount].toString();
                    if(currentTimeStr.contains(":")) {
                        QString hourString = currentTimeStr.section(":", 0, 0);
                        QString minuteString = currentTimeStr.section(":", 1, 1);
                        // qDebug() << hourString << ":" << minuteString;
                        QTime timeAsQTime = QTime(hourString.toInt(), minuteString.toInt());
                        currentTimeValue = timeAsQTime.msecsSinceStartOfDay();
                    } else {
                        currentTimeValue = this->timestamps[j + measurementsCount].toReal();
                    }
                    currentDataValue = this->measurements[j + measurementsCount].toReal();
                    this->measurementSeries[i]->append(currentTimeValue, currentDataValue);
                    // qDebug() << "(" << currentTimeValue << "," << currentDataValue << ")";
                }
            }
            measurementsCount += numOfMeasurements;
        }
        returnVal = 0;
    }
    return returnVal;
}


MeasurementsDocument::MeasurementsDocument() {}

MeasurementsDocument::MeasurementsDocument(const QString &docFileName) {
    this->docName = docFileName;
    this->measurementsDoc = std::make_shared<QXlsx::Document>(docFileName);

    // get doc sheets and set up dataSheet objects
    foreach(QString currentSheetName, this->measurementsDoc->sheetNames()) {
        sheets.emplace_back(std::make_shared<SheetData>(currentSheetName));
    }

    // get Data from sheets and write it to the sheet objects
    bool readDoc = parseDocumentData();
    if(!readDoc) {
        qDebug() << "Could not read Excel file.";
        sheets.resize(0);
    }

}

MeasurementsDocument::~MeasurementsDocument() {}

bool MeasurementsDocument::parseDocumentData() {
    bool result = false;
    if(!this->measurementsDoc->load()) {
        return false;
    }

    // parse sheets one by one - sheets in document are stored in
    // the same order as in sheet data vector
    int sheetIndexNumber = 0;
    foreach(QString currentSheetName, this->measurementsDoc->sheetNames()) {

        // get the current sheet
        QXlsx::AbstractSheet* currentSheet = measurementsDoc->sheet(currentSheetName);
        if(currentSheet == NULL) {
            continue;
        }

        // get full cells of the current sheet
        int maxRow = -1;
        int maxCol = -1;
        currentSheet->workbook()->setActiveSheet(sheetIndexNumber);
        QXlsx::Worksheet* wsheet = (QXlsx::Worksheet*) currentSheet->workbook()->activeSheet();
        if(wsheet == NULL) {
            continue;
        }

        QVector<QXlsx::CellLocation> clList = wsheet->getFullCells(&maxRow, &maxCol);
        //QVector<QVector<QString> > cellValues;

        // iterate through the cells of the sheet and set values
        // sheet itself indexes like Matlab (0,1,2) -> (1,2,3)
        if(maxCol == 2) {

            // when there are 2 columns, date and time are stored in the same cell (1st col)
            QVariant currentCellValue;
            for(int row = 0; row < maxRow; ++row) {
                for(int col = 0; col < maxCol; ++col) {
                    currentCellValue = measurementsDoc->read(row+1,col+1);
                    if(row == 0) {
                        if(col == 0) {
                            sheets[sheetIndexNumber]->xAxisLabel = currentCellValue.toString();
                        }
                        if(col == 1) {
                            sheets[sheetIndexNumber]->yAxisLabel = currentCellValue.toString();
                        }
                    }
                    else {
                        if(col == 0) {
                            if(currentCellValue.canConvert(14)) {
                                QDateTime currentCellValueDT = currentCellValue.toDateTime();
                                if(currentCellValueDT.date().toString().isEmpty()) {
                                    qDebug() << "Could not parse Excel file. Data is corrupt";
                                    return false;
                                }
                                sheets[sheetIndexNumber]->allDays.emplace_back(currentCellValueDT.date());
                                sheets[sheetIndexNumber]->timestamps.emplace_back(currentCellValueDT.time());
                            } else {
                                qDebug() << "Could not parse Excel file.";
                                return false;
                            }
                        }
                        if(col == 1) {
                            sheets[sheetIndexNumber]->measurements.emplace_back(currentCellValue);
                        }
                    }
                    result = true;
                }
            }


        }
        else if(maxCol == 3) {
            QVariant currentCellValue;
            for(int row = 0; row < maxRow; ++row) {
                for(int col = 0; col < maxCol; ++col) {
                    currentCellValue = measurementsDoc->read(row+1,col+1);
                    if(row == 0) {
                        if(col == 1) {
                            sheets[sheetIndexNumber]->xAxisLabel = currentCellValue.toString();
                        }
                        if(col == 2) {
                            sheets[sheetIndexNumber]->yAxisLabel = currentCellValue.toString();
                        }
                    } else {
                        if(col == 0) {
                            sheets[sheetIndexNumber]->allDays.emplace_back(currentCellValue);
                        }
                        if(col == 1) {
                            sheets[sheetIndexNumber]->timestamps.emplace_back(currentCellValue);
                        }
                        if(col == 2) {
                            sheets[sheetIndexNumber]->measurements.emplace_back(currentCellValue);
                        }
                    }
                }
            }
            result = true;
        }
        // get the days of each sheet and setup lineSeries for each day
        sheets[sheetIndexNumber]->extractDays();
        sheets[sheetIndexNumber]->extractLineSeries();
        ++sheetIndexNumber;
    }

    return result;
}
