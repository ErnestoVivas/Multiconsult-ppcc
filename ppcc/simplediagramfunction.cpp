//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "simplediagramfunction.h"
#include "ui_simplediagramfunction.h"

SimpleDiagramFunction::SimpleDiagramFunction(QWidget *parent) :
    QWidget(parent), ui(new Ui::SimpleDiagramFunction) {
    ui->setupUi(this);
    ui->listWidgetDays->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(clearDaysEntries(int)));
    //connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(clearSheetList(int)));
    //connect(ui->comboBoxSelectSheet, SIGNAL(currentIndexChanged(int)), this, SLOT(clearDaysEntries(int)));
}

SimpleDiagramFunction::~SimpleDiagramFunction() {
    delete ui;
}

void SimpleDiagramFunction::addEntryComboBoxSelectDoc(const QString &newDocument) {
    ui->comboBoxSelectDoc->addItem(newDocument);
    int lastItemIndex = (ui->comboBoxSelectDoc->count()) - 1;
    ui->comboBoxSelectDoc->setCurrentIndex(lastItemIndex);
}

//void SimpleDiagramFunction::updateSheetList(const QString& newSheetEntry) {
//    ui->comboBoxSelectSheet->addItem(newSheetEntry);
//}

void SimpleDiagramFunction::updateDays(const QString &day) {
    ui->listWidgetDays->addItem(day);
}

std::vector<int> SimpleDiagramFunction::getSelectedDays() {
    std::vector<int> selectedDaysIndices;
    for(int i = 0; i < ui->listWidgetDays->count(); ++i) {
        if(ui->listWidgetDays->item(i)->isSelected()) {
            selectedDaysIndices.emplace_back(i);
        }
    }
    return selectedDaysIndices;
}

void SimpleDiagramFunction::removeDocument(int docToRemove) {
    ui->comboBoxSelectDoc->removeItem(docToRemove);
}

//void SimpleDiagramFunction::clearSheetList(int newDocIndex) {
//    ui->comboBoxSelectSheet->clear();
//    this->selectedDocIndex = newDocIndex;
//    emit selectedDocChanged(newDocIndex);
//}

void SimpleDiagramFunction::clearDaysEntries(int newDocIndex) {
    ui->listWidgetDays->clear();
    this->selectedDocIndex = newDocIndex;
    emit selectedDocChanged(newDocIndex);
}

//void SimpleDiagramFunction::clearDaysEntries(int newSheetIndex) {
//    ui->listWidgetDays->clear();
//    this->selectedSheetIndex = newSheetIndex;
//    emit selectedSheetChanged(newSheetIndex, this->selectedDocIndex);
//}


