#include "exportdiagramdialog.h"
#include "ui_exportdiagramdialog.h"

ExportDiagramDialog::ExportDiagramDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::ExportDiagramDialog) {
    ui->setupUi(this);
}

ExportDiagramDialog::ExportDiagramDialog(QWidget* parent, short &expMethod) :
    QDialog(parent), ui(new Ui::ExportDiagramDialog) {
    ui->setupUi(this);
    this->exportMethod = &expMethod;

    connect(ui->buttonImage, SIGNAL(clicked()), this, SLOT(imageSelected()));
    connect(ui->buttonExcel, SIGNAL(clicked()), this, SLOT(excelSelected()));
    connect(ui->buttonCSV, SIGNAL(clicked()), this, SLOT(csvSelected()));
}

ExportDiagramDialog::~ExportDiagramDialog() {
    delete ui;
}

void ExportDiagramDialog::imageSelected() {
    *exportMethod = 0;
    this->close();
}

void ExportDiagramDialog::excelSelected() {
    *exportMethod = 1;
    this->close();
}

void ExportDiagramDialog::csvSelected() {
    *exportMethod = 2;
    this->close();
}
