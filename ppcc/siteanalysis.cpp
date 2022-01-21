#include "siteanalysis.h"
#include "ui_siteanalysis.h"

SiteAnalysis::SiteAnalysis(QWidget *parent) :
    QWidget(parent), ui(new Ui::SiteAnalysis) {
    ui->setupUi(this);

    visualizationGroup.addButton(ui->radioButtonComparison, 0);
    visualizationGroup.addButton(ui->radioButtonAverage, 1);
    visualizationGroup.addButton(ui->radioButtonSum, 2);
    ui->radioButtonComparison->setChecked(true);

    ui->comboBoxSelectDay->addItem("Lunes");
    ui->comboBoxSelectDay->addItem("Martes");
    ui->comboBoxSelectDay->addItem("Miercoles");
    ui->comboBoxSelectDay->addItem("Jueves");
    ui->comboBoxSelectDay->addItem("Viernes");
    ui->comboBoxSelectDay->addItem("Sabado");
    ui->comboBoxSelectDay->addItem("Domingo");

    connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(clearSheetList(int)));
    connect(ui->comboBoxSelectSheet, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentSheet(int)));
}

SiteAnalysis::~SiteAnalysis() {
    delete ui;
}

void SiteAnalysis::addEntryComboBoxSelectDoc(const QString &newDocument) {
    ui->comboBoxSelectDoc->addItem(newDocument);
    int lastItemIndex = (ui->comboBoxSelectDoc->count()) - 1;
    ui->comboBoxSelectDoc->setCurrentIndex(lastItemIndex);
}

void SiteAnalysis::updateSheetList(const QString& newSheetEntry) {
    ui->comboBoxSelectSheet->addItem(newSheetEntry);
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

void SiteAnalysis::clearSheetList(int newDocIndex) {
    ui->comboBoxSelectSheet->clear();
    this->selectedDocIndex = newDocIndex;
    emit selectedDocChanged(newDocIndex);
}

void SiteAnalysis::setCurrentSheet(int currentSheet) {
    this->selectedSheetIndex = currentSheet;
}
