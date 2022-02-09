#include "selectimportmethod.h"
#include "ui_selectimportmethod.h"

SelectImportMethod::SelectImportMethod(QWidget* parent) :
    QDialog(parent), ui(new Ui::SelectImportMethod) {
    ui->setupUi(this);
}

SelectImportMethod::SelectImportMethod(QWidget* parent, int& importMethod) :
    QDialog(parent), ui(new Ui::SelectImportMethod) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    currentImportMethod = &importMethod;
    *currentImportMethod = -1;

    connect(ui->buttonImportDocs, SIGNAL(clicked()), this, SLOT(importDocsSelected()));
    connect(ui->buttonImportDB, SIGNAL(clicked()), this, SLOT(importDBSelected()));
    connect(ui->buttonImportTotalNatDoc, SIGNAL(clicked()), this, SLOT(importTotalNatDocSelected()));
}

SelectImportMethod::~SelectImportMethod() {
    delete ui;
}

void SelectImportMethod::importDocsSelected() {
    *currentImportMethod = 0;
    this->close();
}

void SelectImportMethod::importDBSelected() {
    *currentImportMethod = 1;
    this->close();
}

void SelectImportMethod::importTotalNatDocSelected() {
    *currentImportMethod = 2;
    this->close();
}
