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
    ui->comboBoxSelectSector->addItem("Residencial");
    ui->comboBoxSelectSector->addItem("Comercial");
    ui->comboBoxSelectSector->addItem("Industrial");
    ui->comboBoxSelectSector->addItem("Bombeo");
    ui->comboBoxSelectSector->addItem("Alumbrado público");
}

void SectorWeekAnalysis::updateSelectSubCatComboBox(int sectorIndex) {
    ui->comboBoxSelectSubCategory->clear();
    if(sectorIndex == 0) {
        ui->comboBoxSelectSubCategory->addItem("50 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("51 - 100 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("101 - 150 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("151 - 250 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("251 - 500 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem(">500 kWh/mes");
    } else if(sectorIndex == 1) {
        ui->comboBoxSelectSubCategory->addItem("Financieras");
        ui->comboBoxSelectSubCategory->addItem("Comercio al por mayor");
        ui->comboBoxSelectSubCategory->addItem("Comercio al por menor");
        ui->comboBoxSelectSubCategory->addItem("Gobierno");
        ui->comboBoxSelectSubCategory->addItem("Salud");
        ui->comboBoxSelectSubCategory->addItem("Hoteles");
        ui->comboBoxSelectSubCategory->addItem("Educación");
        ui->comboBoxSelectSubCategory->addItem("Otros");
    } else if(sectorIndex == 2) {
        ui->comboBoxSelectSubCategory->addItem("Alimentos y bebidas");
        ui->comboBoxSelectSubCategory->addItem("Papel, cartón");
        ui->comboBoxSelectSubCategory->addItem("Caucho, plastico");
        ui->comboBoxSelectSubCategory->addItem("Quimica");
        ui->comboBoxSelectSubCategory->addItem("Textil");
        ui->comboBoxSelectSubCategory->addItem("Otros");
    }
}

int SectorWeekAnalysis::getSelectedSector() {
    return ui->comboBoxSelectSector->currentIndex();
}

int SectorWeekAnalysis::getSelectedSubCat() {
    return ui->comboBoxSelectSubCategory->currentIndex();
}

int SectorWeekAnalysis::getVisType() {
    return visualizationGroup.checkedId();
}
