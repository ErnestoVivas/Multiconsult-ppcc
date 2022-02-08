#include "selectfunction.h"
#include "ui_selectfunction.h"

SelectFunction::SelectFunction(QWidget *parent) :
    QDialog(parent), ui(new Ui::SelectFunction) {

    ui->setupUi(this);
    this->setupRadioButtons();


    // setup table widget
    ui->tableWidgetFunctions->setColumnCount(3);
    ui->tableWidgetFunctions->setRowCount(5);
    ui->tableWidgetFunctions->setShowGrid(true);

    ui->tableWidgetFunctions->setColumnWidth(0, 40);

    ui->tableWidgetFunctions->setHorizontalHeaderLabels({"", "Función", "Descripción"});
    //ui->tableWidgetFunctions->verticalHeader()->hide();

    ui->tableWidgetFunctions->setCellWidget(0, 1, new QLabel("Visualizar fechas específicas\nde las mediciones de un sitio"));
    ui->tableWidgetFunctions->setCellWidget(0, 2, new QLabel());
    //ui->tableWidgetFunctions->item(0, 1)->setText("Visualizar fechas específicas\nde las mediciones de un sitio");
    //ui->tableWidgetFunctions->item(0, 2)->setText("Seleccione uno de los archivos importados y luego una o varias\n"
    //                                              "fechas de las mediciones de ese archivo. Se visualizarán las\n"
    //                                              "curvas de carga de las fechas especificadas.");

    this->addRadioButtonsToFunctionsTable();

}

SelectFunction::~SelectFunction() {
    delete ui;
}

void SelectFunction::setupRadioButtons() {
    for(int i = 0; i < 5; ++i) {
        functionButtons.append(new QRadioButton("", this));
        selectFunctionsGroup.addButton(functionButtons[i], i);
    }
}

void SelectFunction::addRadioButtonsToFunctionsTable() {

    // ownership of all Widgets is passed to tableWidget!
    for(int i = 0; i < 5; ++i) {
        QWidget* radioButtonWidget = new QWidget();
        QHBoxLayout* HLayout = new QHBoxLayout(radioButtonWidget);
        HLayout->addWidget(functionButtons[i]);
        HLayout->setAlignment(Qt::AlignCenter);
        HLayout->setContentsMargins(0,0,0,0);
        ui->tableWidgetFunctions->setCellWidget(i, 0, radioButtonWidget);
    }
}
