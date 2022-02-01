//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

