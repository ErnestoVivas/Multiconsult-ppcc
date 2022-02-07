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

void SectorWeekAnalysis::setupComboBoxSector() {
    ui->comboBoxSelectSector->clear();
    ui->comboBoxSelectSector->addItem("Residencial");
    ui->comboBoxSelectSector->addItem("Comercial");
    ui->comboBoxSelectSector->addItem("Industrial");
    ui->comboBoxSelectSector->addItem("Bombeo");
    ui->comboBoxSelectSector->addItem("Alumbrado público");
    ui->comboBoxSelectSector->addItem("Otros");
    for(int i = 0; i < customSectors.size(); ++i) {
        ui->comboBoxSelectSector->addItem(customSectors[i]);
    }
}

void SectorWeekAnalysis::updateSelectSubCatComboBox(int sectorIndex) {

    emit requestSubCatsUpdate(sectorIndex, ui->comboBoxSelectSubCategory);
    /*
    ui->comboBoxSelectSubCategory->clear();
    if(sectorIndex == 0) {
        ui->comboBoxSelectSubCategory->addItem("50 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("51 - 100 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("101 - 150 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("151 - 250 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("251 - 500 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem(">500 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("Otros");
        for(int i = 0; i < customResRanges.size(); ++i) {
            ui->comboBoxSelectSubCategory->addItem(customResRanges[i]);
        }
    } else if(sectorIndex == 1) {
        ui->comboBoxSelectSubCategory->addItem("Financieras");
        ui->comboBoxSelectSubCategory->addItem("Comercio al por mayor");
        ui->comboBoxSelectSubCategory->addItem("Comercio al por menor");
        ui->comboBoxSelectSubCategory->addItem("Gobierno");
        ui->comboBoxSelectSubCategory->addItem("Salud");
        ui->comboBoxSelectSubCategory->addItem("Hoteles");
        ui->comboBoxSelectSubCategory->addItem("Educación");
        ui->comboBoxSelectSubCategory->addItem("Otros");
        for(int i = 0; i < customCommercials.size(); ++i) {
            ui->comboBoxSelectSubCategory->addItem(customCommercials[i]);
        }
    } else if(sectorIndex == 2) {
        ui->comboBoxSelectSubCategory->addItem("Alimentos y bebidas");
        ui->comboBoxSelectSubCategory->addItem("Papel, cartón");
        ui->comboBoxSelectSubCategory->addItem("Caucho, plastico");
        ui->comboBoxSelectSubCategory->addItem("Quimica");
        ui->comboBoxSelectSubCategory->addItem("Textil");
        ui->comboBoxSelectSubCategory->addItem("Otros");
        for(int i = 0; i < customIndustrials.size(); ++i) {
            ui->comboBoxSelectSubCategory->addItem(customIndustrials[i]);
        }
    } else if(sectorIndex > 5) {
        ui->comboBoxSelectSubCategory->addItem("Otros");
        for(int i = 0; i < customSubSectors.size(); ++i) {
            ui->comboBoxSelectSubCategory->addItem(customSubSectors[i]);
        }
    }
    */
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
