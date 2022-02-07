//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "sectordayanalysis.h"
#include "ui_sectordayanalysis.h"

SectorDayAnalysis::SectorDayAnalysis(QWidget *parent) :
    QWidget(parent), ui(new Ui::SectorDayAnalysis) {
    ui->setupUi(this);

    visualizationGroup.addButton(ui->radioButtonComparison, 0);
    visualizationGroup.addButton(ui->radioButtonAverage, 1);
    visualizationGroup.addButton(ui->radioButtonSum, 2);
    ui->radioButtonComparison->setChecked(true);

    this->setupComboBoxSector();
    this->updateSelectSubCatComboBox(0);

    ui->comboBoxSelectDay->addItem("Lunes");
    ui->comboBoxSelectDay->addItem("Martes");
    ui->comboBoxSelectDay->addItem("Miercoles");
    ui->comboBoxSelectDay->addItem("Jueves");
    ui->comboBoxSelectDay->addItem("Viernes");
    ui->comboBoxSelectDay->addItem("Sabado");
    ui->comboBoxSelectDay->addItem("Domingo");

    connect(ui->comboBoxSelectSector, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSelectSubCatComboBox(int)));
}

SectorDayAnalysis::~SectorDayAnalysis() {
    delete ui;
}

void SectorDayAnalysis::setupComboBoxSector() {
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

void SectorDayAnalysis::updateSelectSubCatComboBox(int sectorIndex) {
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

int SectorDayAnalysis::getSelectedSector() {
    int currentIndex = ui->comboBoxSelectSector->currentIndex();
    if(currentIndex > 5) {
        currentIndex = -2;
    }
    return currentIndex;
}

QString SectorDayAnalysis::getSelectedSectorString() {
    int currentItem = ui->comboBoxSelectSector->currentIndex();
    return ui->comboBoxSelectSector->itemText(currentItem);
}

int SectorDayAnalysis::getSelectedSubCat() {
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

QString SectorDayAnalysis::getSelectedSubCatString() {
    int currentItem = ui->comboBoxSelectSubCategory->currentIndex();
    return ui->comboBoxSelectSubCategory->itemText(currentItem);
}

int SectorDayAnalysis::getDay() {
    return ui->comboBoxSelectDay->currentIndex();
}

int SectorDayAnalysis::getVisType() {
    return visualizationGroup.checkedId();
}


QComboBox* SectorDayAnalysis::getSectorComboBox() {
    return ui->comboBoxSelectSector;
}

QComboBox* SectorDayAnalysis::getSubCatComboBox() {
    return ui->comboBoxSelectSubCategory;
}
