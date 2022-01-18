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
    ui->comboBoxSelectSector->addItem("Residencial");
    ui->comboBoxSelectSector->addItem("Comercial");
    ui->comboBoxSelectSector->addItem("Industrial");
    ui->comboBoxSelectSector->addItem("Bombeo");
    ui->comboBoxSelectSector->addItem("Alumbrado público");
}

void SectorDayAnalysis::updateSelectSubCatComboBox(int sectorIndex) {
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

int SectorDayAnalysis::getSelectedSector() {
    return ui->comboBoxSelectSector->currentIndex();
}

int SectorDayAnalysis::getSelectedSubCat() {
    return ui->comboBoxSelectSubCategory->currentIndex();
}

int SectorDayAnalysis::getDay() {
    return ui->comboBoxSelectDay->currentIndex();
}

int SectorDayAnalysis::getVisType() {
    return visualizationGroup.checkedId();
}
