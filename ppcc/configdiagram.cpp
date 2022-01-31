#include "configdiagram.h"
#include "ui_configdiagram.h"

ConfigDiagram::ConfigDiagram(QWidget* parent) :
    QDialog(parent), ui(new Ui::ConfigDiagram) {
    ui->setupUi(this);
}

ConfigDiagram::ConfigDiagram(QWidget* parent, QChart* currentChart, QValueAxis* currentXAxis,
                             QValueAxis* currentYAxis, QList<QLineSeries*>* currentSeries) :
    QDialog(parent), ui(new Ui::ConfigDiagram), measurementsChart(currentChart),
    xAxis(currentXAxis), yAxis(currentYAxis), displayedSeries(currentSeries) {
    ui->setupUi(this);

    // setup ui elements
    ui->spinBoxYMin->setMaximum(999999);
    ui->spinBoxYMax->setMaximum(999999);
    ui->checkBoxShowLegend->setCheckable(true);
    ui->comboBoxYFormat->addItem("int");
    ui->comboBoxYFormat->addItem("float");

    if(yAxis->labelFormat() == "%i") {
        ui->comboBoxYFormat->setCurrentIndex(0);
    } else {
        ui->comboBoxYFormat->setCurrentIndex(1);
    }

    // show current diagramData
    ui->lineEditTitle->setText(measurementsChart->title());
    ui->spinBoxYMin->setValue(yAxis->min());
    ui->spinBoxYMax->setValue(yAxis->max());
    ui->spinBoxYTickCount->setValue(yAxis->tickCount());
    ui->lineEditXAxisLabel->setText(xAxis->titleText());
    ui->lineEditYAxisLabel->setText(yAxis->titleText());
    ui->checkBoxShowLegend->setChecked(measurementsChart->legend()->isVisible());

    // extract line series names
    for(int i = 0; i < displayedSeries->size(); ++i) {
        ui->listWidgetSeriesNames->addItem(displayedSeries->at(i)->name());
        ui->listWidgetSeriesNames->item(i)->setFlags(Qt::ItemIsEnabled);
        ui->listWidgetSeriesNames->item(i)->setFlags(Qt::ItemIsEditable);
    }

    // connect buttons
    connect(ui->buttonOk, SIGNAL(clicked()), this, SLOT(updateAndExit()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelAndExit()));
}

ConfigDiagram::~ConfigDiagram() {
    delete ui;
}


void ConfigDiagram::updateAndExit() {
    measurementsChart->setTitle(ui->lineEditTitle->text());
    measurementsChart->legend()->setVisible(ui->checkBoxShowLegend->isChecked());
    yAxis->setMin(ui->spinBoxYMin->value());
    yAxis->setMax(ui->spinBoxYMax->value());
    yAxis->setTickCount(ui->spinBoxYTickCount->value());
    xAxis->setTitleText(ui->lineEditXAxisLabel->text());
    yAxis->setTitleText(ui->lineEditYAxisLabel->text());

    if(ui->comboBoxYFormat->currentIndex() == 0) {
        yAxis->setLabelFormat("%i");
    } else {
        yAxis->setLabelFormat("");
    }


    this->close();
}



void ConfigDiagram::cancelAndExit() {
    this->close();
}
