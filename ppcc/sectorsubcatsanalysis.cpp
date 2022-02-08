//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "sectorsubcatsanalysis.h"
#include "ui_sectorsubcatsanalysis.h"

SectorSubCatsAnalysis::SectorSubCatsAnalysis(QWidget *parent) :
    QWidget(parent), ui(new Ui::SectorSubCatsAnalysis) {
    ui->setupUi(this);

    // setup radio buttons
    daysGroup.addButton(ui->radioButtonWorkDay, 0);
    daysGroup.addButton(ui->radioButtonSaturday, 1);
    daysGroup.addButton(ui->radioButtonSunday, 2);
    visualizationGroup.addButton(ui->radioButtonAverageDisag, 0);
    visualizationGroup.addButton(ui->radioButtonAverageTotal, 1);
    ui->radioButtonWorkDay->setChecked(true);
    ui->radioButtonAverageDisag->setChecked(true);

    this->setupComboBoxSector();
}

SectorSubCatsAnalysis::~SectorSubCatsAnalysis() {
    delete ui;
}

// function to construct the widget, not used for updating combobox
void SectorSubCatsAnalysis::setupComboBoxSector() {
    ui->comboBoxSelectSector->clear();
    ui->comboBoxSelectSector->addItem("Residencial");
    ui->comboBoxSelectSector->addItem("Comercial");
    ui->comboBoxSelectSector->addItem("Industrial");
    ui->comboBoxSelectSector->addItem("Bombeo");
    ui->comboBoxSelectSector->addItem("Alumbrado público");
    ui->comboBoxSelectSector->addItem("Otros");
}

int SectorSubCatsAnalysis::getSelectedSector() {
    int currentIndex = ui->comboBoxSelectSector->currentIndex();
    if(currentIndex > 5) {
        currentIndex = -2;
    }
    return currentIndex;
}

QString SectorSubCatsAnalysis::getSelectedSectorString() {
    int currentItem = ui->comboBoxSelectSector->currentIndex();
    return ui->comboBoxSelectSector->itemText(currentItem);
}

int SectorSubCatsAnalysis::getDay() {
    return daysGroup.checkedId();
}

int SectorSubCatsAnalysis::getVisType() {
    return visualizationGroup.checkedId();
}

QComboBox* SectorSubCatsAnalysis::getSectorComboBox() {
    return ui->comboBoxSelectSector;
}
