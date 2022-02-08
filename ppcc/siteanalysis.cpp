//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "siteanalysis.h"
#include "ui_siteanalysis.h"

SiteAnalysis::SiteAnalysis(QWidget *parent) :
    QWidget(parent), ui(new Ui::SiteAnalysis) {
    ui->setupUi(this);
    this->selectedDocIndex = -1;

    visualizationGroup.addButton(ui->radioButtonComparison, 0);
    visualizationGroup.addButton(ui->radioButtonAverage, 1);
    visualizationGroup.addButton(ui->radioButtonSum, 2);
    ui->radioButtonComparison->setChecked(true);

    ui->comboBoxSelectDay->addItem("Lunes");
    ui->comboBoxSelectDay->addItem("Martes");
    ui->comboBoxSelectDay->addItem("Miércoles");
    ui->comboBoxSelectDay->addItem("Jueves");
    ui->comboBoxSelectDay->addItem("Viernes");
    ui->comboBoxSelectDay->addItem("Sábado");
    ui->comboBoxSelectDay->addItem("Domingo");

    connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentDoc(int)));
}

SiteAnalysis::~SiteAnalysis() {
    delete ui;
}

void SiteAnalysis::addEntryComboBoxSelectDoc(const QString &newDocument) {
    ui->comboBoxSelectDoc->addItem(newDocument);
    int lastItemIndex = (ui->comboBoxSelectDoc->count()) - 1;
    ui->comboBoxSelectDoc->setCurrentIndex(lastItemIndex);
}

void SiteAnalysis::removeDocument(int docToRemove) {
    ui->comboBoxSelectDoc->removeItem(docToRemove);
}


int SiteAnalysis::getDay() {
    return ui->comboBoxSelectDay->currentIndex();
}

int SiteAnalysis::getVisType() {
    return visualizationGroup.checkedId();
}

void SiteAnalysis::setCurrentDoc(int newDocIndex) {
    this->selectedDocIndex = newDocIndex;
}
