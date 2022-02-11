#include "selectfunction.h"
#include "ui_selectfunction.h"

SelectFunction::SelectFunction(QWidget *parent, int& selectedFunction) :
    QDialog(parent), ui(new Ui::SelectFunction) {

    ui->setupUi(this);
    this->setupRadioButtons();
    indexRadioButton = &selectedFunction;
    // setup table widget
    ui->tableWidgetFunctions->setColumnCount(3);
    ui->tableWidgetFunctions->setRowCount(6);
    ui->tableWidgetFunctions->setShowGrid(true);

    ui->tableWidgetFunctions->setColumnWidth(0, 40);
    ui->tableWidgetFunctions->setColumnWidth(1, 200);
    ui->tableWidgetFunctions->setColumnWidth(2, 600);
    for(int i = 0; i < 3; ++i) {
        ui->tableWidgetFunctions->setRowHeight(i, 40);
    }
    ui->tableWidgetFunctions->setRowHeight(3, 60);
    ui->tableWidgetFunctions->setRowHeight(4, 60);

    ui->tableWidgetFunctions->setHorizontalHeaderLabels({"", "Función", "Descripción"});
    ui->tableWidgetFunctions->verticalHeader()->hide();

    ui->tableWidgetFunctions->setCellWidget(0, 1, new QLabel("Visualizar fechas específicas\nde las mediciones de un sitio"));
    ui->tableWidgetFunctions->setCellWidget(0, 2, new QLabel("Seleccione uno de los archivos importados y luego una o varias fechas\n"
                                                             "Se visualizarán las curvas de carga de las fechas especificadas."));
    ui->tableWidgetFunctions->setCellWidget(1, 1, new QLabel("Análisis de los días de semana\nde las mediciones de un sitio"));
    ui->tableWidgetFunctions->setCellWidget(1, 2, new QLabel("Seleccione uno de los archivos importados y un día de la semana (lunes a domingo). Se visualizarán\n"
                                                             "todas las mediciones o el promedio de todas las mediciones del día de semana especificado."));
    ui->tableWidgetFunctions->setCellWidget(2, 1, new QLabel("Curvas de carga de días\nde semana (por categoría)"));
    ui->tableWidgetFunctions->setCellWidget(2, 2, new QLabel("Seleccione un sector, una subcategoría y un día de la semana (lunes a domingo).Se visualizarán\n"
                                                             "todas las mediciones o el promedio de todas las mediciones de la categoría y del día especificados."));
    ui->tableWidgetFunctions->setCellWidget(3, 1, new QLabel("Curvas de carga de los\ndías laborales (por categoría)"));
    ui->tableWidgetFunctions->setCellWidget(3, 2, new QLabel("Seleccione un sector y una subcategoría. Se calculará el consumo promedio de los días lunes a viernes\n"
                                                             "de todas las mediciones de la categoría correspondiente. Puede visualizar el promedio de cada día\n"
                                                             "o el promedio total de todos los días."));
    ui->tableWidgetFunctions->setCellWidget(4, 1, new QLabel("Curvas de carga de las\nsubcategorías de un sector"));
    ui->tableWidgetFunctions->setCellWidget(4, 2, new QLabel("Seleccione un sector y un día de semana. Se calculará el promedio de las mediciones correspondientes\n"
                                                             "y se visualizará el resultado desagregado por cada subcategoría o el total de todas las subcategorías."));
    ui->tableWidgetFunctions->setCellWidget(5,1, new QLabel("Comparar sector con CC nacional"));
    ui->tableWidgetFunctions->setCellWidget(5,2, new QLabel(""));

    this->addRadioButtonsToFunctionsTable();
    connect(ui->buttonOk, SIGNAL(clicked()), this, SLOT(exitDialog()));
}

SelectFunction::~SelectFunction() {
    delete ui;
}

void SelectFunction::setupRadioButtons() {
    for(int i = 0; i < 6; ++i) {
        if (i == 0) {
            functionButtons.append(new QRadioButton("", this));
            selectFunctionsGroup.addButton(functionButtons[i], i);
            selectFunctionsGroup.button(i)->click();
        }
        else {
            functionButtons.append(new QRadioButton("", this));
            selectFunctionsGroup.addButton(functionButtons[i], i);
        }
    }
}

void SelectFunction::addRadioButtonsToFunctionsTable() {

    // ownership of all Widgets is passed to tableWidget!
    for(int i = 0; i < 6; ++i) {
        QWidget* radioButtonWidget = new QWidget();
        QHBoxLayout* HLayout = new QHBoxLayout(radioButtonWidget);
        HLayout->addWidget(functionButtons[i]);
        HLayout->setAlignment(Qt::AlignCenter);
        HLayout->setContentsMargins(0,0,0,0);
        ui->tableWidgetFunctions->setCellWidget(i, 0, radioButtonWidget);
    }
}

void SelectFunction::exitDialog() {
    *indexRadioButton = selectFunctionsGroup.checkedId();
    this->close();
}
