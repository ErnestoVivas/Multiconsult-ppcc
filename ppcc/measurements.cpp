#include "measurements.h"

MeasurementsSheet::MeasurementsSheet() {

}

MeasurementsSheet::~MeasurementsSheet() {

}

MeasurementsDocument::MeasurementsDocument() {}

MeasurementsDocument::MeasurementsDocument(const QString &docFileName) {
    this->docName = docFileName;
}

MeasurementsDocument::~MeasurementsDocument() {}
