#include "measurements.h"

SheetData::SheetData() {

}

SheetData::SheetData(const QString& currentSheetName) {
    this->sheetName = currentSheetName;
}

SheetData::~SheetData() {

}

MeasurementsDocument::MeasurementsDocument() {}

MeasurementsDocument::MeasurementsDocument(const QString &docFileName) {
    this->docName = docFileName;
    this->measurementsDoc = std::make_shared<QXlsx::Document>(docFileName);

    // get doc sheets and set up dataSheet objects
    foreach(QString currentSheetName, this->measurementsDoc->sheetNames()) {
        sheets.push_back(std::make_shared<SheetData>(currentSheetName));
    }

    // get Data from sheets and write it to the sheet objects
    parseDocumentData();
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
        QVariant currentCellValue;
        for(int row = 1; row < maxRow; ++row) {
            for(int col = 0; col < maxCol; ++col) {
                currentCellValue = measurementsDoc->read(row+1,col+1);
                if(col == 0) {
                    sheets[sheetIndexNumber]->days.push_back(currentCellValue);
                }
                if(col == 1) {
                    sheets[sheetIndexNumber]->timestamps.push_back(currentCellValue);
                }
                if(col == 2) {
                    sheets[sheetIndexNumber]->measurements.push_back(currentCellValue);
                }
            }
        }

        qDebug() << sheets[sheetIndexNumber]->days.size();
        qDebug() << sheets[sheetIndexNumber]->timestamps.size();
        qDebug() << sheets[sheetIndexNumber]->measurements.size();
        ++sheetIndexNumber;
    }

    return result;
}
