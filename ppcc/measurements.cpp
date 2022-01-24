//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "measurements.h"

SheetData::SheetData() {

}

SheetData::SheetData(const QString& currentSheetName) {
    this->sheetName = currentSheetName;
    this->numOfDays = 0;
    this->datesAreStrings = true;
    this->sheetFreq = Frequency::hour;
    this->firstDayIsComplete = false;
    this->lastDayIsComplete = false;
    this->oneDayOnly = false;
    this->twoDaysOnly = false;
    this->sheetIsPlottable = true;
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

        // check if all days have the same number of measurements
        int numMeasurementsFirstDay;
        int numMeasurementsLastDay;
        int numOfDailyMeasurements;
        if(daysAndCounting.size() == 1) {
            this->oneDayOnly = true;
        } else if(daysAndCounting.size() == 2) {
            this->twoDaysOnly = true;
        } else if(daysAndCounting.size() > 2) {
            numMeasurementsFirstDay = daysAndCounting[0].second;
            numOfDailyMeasurements = daysAndCounting[1].second;
            numMeasurementsLastDay = daysAndCounting[daysAndCounting.size() - 1].second;
            if(numMeasurementsFirstDay == numOfDailyMeasurements) {
                this->firstDayIsComplete = true;
            }
            if(numMeasurementsLastDay == numOfDailyMeasurements) {
                this->lastDayIsComplete = true;
            }
            for(unsigned int i = 1; i < (daysAndCounting.size() - 1); ++i) {
                if(numOfDailyMeasurements != daysAndCounting[i].second) {
                    this->sheetIsPlottable = false;
                }
            }
        } else {
            this->sheetIsPlottable = false;
        }

        // debugging: print if first/last day are complete and if sheet is plottable
        //qDebug() << "First day is complete: " << firstDayIsComplete << ", ";
        //qDebug() << "Last day is complete: " << lastDayIsComplete << ", ";
        //qDebug() << "All days have the same size: " << sheetIsPlottable;

        // debugging: print days and their count
        //qDebug() << "Sheet: " << this->sheetName << ", numOfDays: " << this->numOfDays;
        //for(unsigned short s = 0; s < daysAndCounting.size(); ++s) {
        //    qDebug() << daysAndCounting[s].first << ", measurements: " << daysAndCounting[s].second;
        //}

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
                    currentTimeValue = (currentTimestamp.msecsSinceStartOfDay() / 86400000.0) * 24.0;   // map to 24 hours
                    currentDataValue = this->measurements[j + measurementsCount].toReal();
                    this->measurementSeries[i]->append(currentTimeValue, currentDataValue);
                    //qDebug() << "(" << currentTimeValue << "," << currentDataValue << ")";

                    double intPart;
                    if(std::modf(currentTimeValue, &intPart) != 0.0) {
                        this->sheetFreq = Frequency::quarterHour;
                    }
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
                        currentTimeValue = (timeAsQTime.msecsSinceStartOfDay() / 86400000.0) * 24.0;   // map to 24 hours
                    } else {
                        currentTimeValue = this->timestamps[j + measurementsCount].toReal();
                    }
                    currentDataValue = this->measurements[j + measurementsCount].toReal();
                    this->measurementSeries[i]->append(currentTimeValue, currentDataValue);
                    //qDebug() << "(" << currentTimeValue << "," << currentDataValue << ")";

                    double intPart;
                    if(std::modf(currentTimeValue, &intPart) != 0.0) {
                        this->sheetFreq = Frequency::quarterHour;
                    }
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
    this->docResRange = ResidentialRange::X;
    this->docSubCommercial = Commercial::notCommercial;
    this->docSubIndustrial = Industrial::notIndustrial;
    this->docFreq = Frequency::hour;

    // get doc sheets and set up dataSheet objects
    foreach(QString currentSheetName, this->measurementsDoc->sheetNames()) {
        sheets.emplace_back(std::make_shared<SheetData>(currentSheetName));
        break;  // update jan 24: only one sheet per document
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

        // petition of the customer: only ever read 1 sheet,
        // input format have one sheet only
        if(sheetIndexNumber == 1) {
            break;
        }

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

        //QVector<QXlsx::CellLocation> clList = wsheet->getFullCells(&maxRow, &maxCol);
        wsheet->getFullCells(&maxRow, &maxCol);

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
        else { //if(maxCol == 3) {                                  // TODO: implement correct feedback
            QVariant currentCellValue;
            for(int row = 0; row < maxRow; ++row) {
                //for(int col = 0; col < maxCol; ++col) {
                for(int col = 0; col < 3; ++col) {
                    currentCellValue = measurementsDoc->read(row+1,col+1);
                    if(row == 0) {
                        if(col == 0) {
                            sheets[sheetIndexNumber]->dateLabel = currentCellValue.toString();
                        }
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
        if((sheets[sheetIndexNumber]->allDays.size() >= 1) &&
                (sheets[sheetIndexNumber]->allDays[0].type() == 10)) {
            sheets[sheetIndexNumber]->datesAreStrings = true;
        } else {
            sheets[sheetIndexNumber]->datesAreStrings = false;
        }

        // get the days of each sheet and setup lineSeries for each day
        sheets[sheetIndexNumber]->extractDays();
        sheets[sheetIndexNumber]->extractLineSeries();
        ++sheetIndexNumber;
    }

    // set measurements frequency of the document (assuming all sheets have the same freq)
    if(sheets.size() >= 1) {
        this->docFreq = sheets[0]->sheetFreq;
    }

    return result;
}

int MeasurementsDocument::getSubCategory() {
    int subCat = -1;
    if(this->docResRange != ResidentialRange::X) {
        subCat = this->docResRange;
    } else if(this->docSubCommercial != Commercial::notCommercial) {
        subCat = this->docSubCommercial;
    } else if(this->docSubIndustrial != Industrial::notIndustrial) {
        subCat = this->docSubIndustrial;
    }
    return subCat;
}

void MeasurementsDocument::setSector(int newSector) {
    if(newSector == 0) {
        this->docSector = Sector::residential;
    } else if(newSector == 1) {
        this->docSector = Sector::commercial;
    } else if(newSector == 2) {
        this->docSector = Sector::industrial;
    } else if(newSector == 3) {
        this->docSector = Sector::pumping;
    } else if(newSector == 4) {
        this->docSector = Sector::publicLighting;
    }
}

void MeasurementsDocument::setSubCategory(int subCat) {
    if(this->docSector == Sector::residential) {
        if(subCat == 0) {
            this->docResRange = ResidentialRange::A;
        } else if(subCat == 1) {
            this->docResRange = ResidentialRange::B;
        } else if(subCat == 2) {
            this->docResRange = ResidentialRange::C;
        } else if(subCat == 3) {
            this->docResRange = ResidentialRange::D;
        } else if(subCat == 4) {
            this->docResRange = ResidentialRange::E;
        } else if(subCat == 5) {
            this->docResRange = ResidentialRange::F;
        } else {
            this->docResRange = ResidentialRange::X;
        }
        this->docSubCommercial = Commercial::notCommercial;
        this->docSubIndustrial = Industrial::notIndustrial;

    } else if(this->docSector == Sector::commercial) {
        this->docResRange = ResidentialRange::X;
        this->docSubIndustrial = Industrial::notIndustrial;
        if(subCat == 0) {
            this->docSubCommercial = Commercial::finances;
        } else if(subCat == 1) {
            this->docSubCommercial = Commercial::wholesale;
        } else if(subCat == 2) {
            this->docSubCommercial = Commercial::retail;
        } else if(subCat == 3) {
            this->docSubCommercial = Commercial::government;
        } else if(subCat == 4) {
            this->docSubCommercial = Commercial::health;
        } else if(subCat == 5) {
            this->docSubCommercial = Commercial::hotels;
        } else if(subCat == 6) {
            this->docSubCommercial = Commercial::education;
        } else if(subCat == 7) {
            this->docSubCommercial = Commercial::otherCommercial;
        }

    } else if(this->docSector == Sector::industrial) {
        this->docResRange = ResidentialRange::X;
        this->docSubCommercial = Commercial::notCommercial;
        if(subCat == 0) {
            this->docSubIndustrial = Industrial::food;
        } else if(subCat == 1) {
            this->docSubIndustrial = Industrial::paper;
        } else if(subCat == 2) {
            this->docSubIndustrial = Industrial::plastics;
        } else if(subCat == 3) {
            this->docSubIndustrial = Industrial::chemistry;
        } else if(subCat == 4) {
            this->docSubIndustrial = Industrial::textiles;
        } else if(subCat == 5) {
            this->docSubIndustrial = Industrial::otherIndustrial;
        }
    } else {
        this->docResRange = ResidentialRange::X;
        this->docSubCommercial = Commercial::notCommercial;
        this->docSubIndustrial = Industrial::notIndustrial;
    }
}
