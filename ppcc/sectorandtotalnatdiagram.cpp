#include "sectorandtotalnatdiagram.h"
#include "ui_sectorandtotalnatdiagram.h"

SectorAndTotalNatDiagram::SectorAndTotalNatDiagram(QWidget *parent) :
    QWidget(parent), ui(new Ui::SectorAndTotalNatDiagram) {
    ui->setupUi(this);

    ui->lineEditTotalNatDoc->setReadOnly(true);
    ui->checkBoxShowSubCats->setChecked(false);

    dayTypeGroup.addButton(ui->radioButtonWorkdays, 0);
    dayTypeGroup.addButton(ui->radioButtonSaturdays, 1);
    dayTypeGroup.addButton(ui->radioButtonSundays, 2);
    ui->radioButtonWorkdays->setChecked(true);
}

SectorAndTotalNatDiagram::~SectorAndTotalNatDiagram() {
    delete ui;
}

void SectorAndTotalNatDiagram::setTotalNatDoc(QString totalNatDocFileName) {
    ui->lineEditTotalNatDoc->setText(totalNatDocFileName);
}

QComboBox* SectorAndTotalNatDiagram::getSectorComboBox() {
    return ui->comboBoxSelectSector;
}

int SectorAndTotalNatDiagram::getSelectedSector() {
    int currentIndex = ui->comboBoxSelectSector->currentIndex();
    if(currentIndex > 5) {
        currentIndex = -2;
    }
    return currentIndex;
}

QString SectorAndTotalNatDiagram::getSelectedSectorString() {
    int currentItem = ui->comboBoxSelectSector->currentIndex();
    return ui->comboBoxSelectSector->itemText(currentItem);
}

int SectorAndTotalNatDiagram::getDay() {
    return dayTypeGroup.checkedId();
}

bool SectorAndTotalNatDiagram::getShowSubCats() {
    return ui->checkBoxShowSubCats->isChecked();
}
