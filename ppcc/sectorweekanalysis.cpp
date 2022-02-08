//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "sectorweekanalysis.h"
#include "ui_sectorweekanalysis.h"

SectorWeekAnalysis::SectorWeekAnalysis(QWidget *parent) :
    QWidget(parent), ui(new Ui::SectorWeekAnalysis) {
    ui->setupUi(this);

    visualizationGroup.addButton(ui->radioButtonAverageDisag, 0);
    visualizationGroup.addButton(ui->radioButtonAverage, 1);
    ui->radioButtonAverageDisag->setChecked(true);

    this->setupComboBoxSector();
    this->updateSelectSubCatComboBox(0);

    connect(ui->comboBoxSelectSector, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSelectSubCatComboBox(int)));
}

SectorWeekAnalysis::~SectorWeekAnalysis() {
    delete ui;
}

// function to construct the widget, not used for updating combobox
void SectorWeekAnalysis::setupComboBoxSector() {
    ui->comboBoxSelectSector->clear();
    ui->comboBoxSelectSector->addItem("Residencial");
    ui->comboBoxSelectSector->addItem("Comercial");
    ui->comboBoxSelectSector->addItem("Industrial");
    ui->comboBoxSelectSector->addItem("Bombeo");
    ui->comboBoxSelectSector->addItem("Alumbrado público");
    ui->comboBoxSelectSector->addItem("Otros");
}

void SectorWeekAnalysis::updateSelectSubCatComboBox(int sectorIndex) {
    emit requestSubCatsUpdate(sectorIndex, ui->comboBoxSelectSubCategory);
}

QString SectorWeekAnalysis::getSelectedSectorString() {
    int currentItem = ui->comboBoxSelectSector->currentIndex();
    return ui->comboBoxSelectSector->itemText(currentItem);
}

int SectorWeekAnalysis::getSelectedSector() {
    int currentIndex = ui->comboBoxSelectSector->currentIndex();
    if(currentIndex > 5) {
        currentIndex = -2;
    }
    return currentIndex;
}

QString SectorWeekAnalysis::getSelectedSubCatString() {
    int currentItem = ui->comboBoxSelectSubCategory->currentIndex();
    return ui->comboBoxSelectSubCategory->itemText(currentItem);
}

int SectorWeekAnalysis::getSelectedSubCat() {
    int currentSec = ui->comboBoxSelectSector->currentIndex();
    int currentSubSecIndex = ui->comboBoxSelectSubCategory->currentIndex();
    if( ((currentSec == 0) && (currentSubSecIndex > 6)) ||
        ((currentSec == 1) && (currentSubSecIndex > 7)) ||
        ((currentSec == 2) && (currentSubSecIndex > 5)) ||
        ((currentSec > 5) && (currentSubSecIndex > 0)) ) {
        currentSubSecIndex = -2;
    }
    return currentSubSecIndex;
}

int SectorWeekAnalysis::getVisType() {
    return visualizationGroup.checkedId();
}

QComboBox* SectorWeekAnalysis::getSectorComboBox() {
    return ui->comboBoxSelectSector;
}

QComboBox* SectorWeekAnalysis::getSubCatComboBox() {
    return ui->comboBoxSelectSubCategory;
}
