#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->comboBoxSelectDoc->setCurrentIndex(-1);
    ui->comboBoxSelectDoc->setPlaceholderText("Seleccionar documento");
    ui->listWidgetDays->setSelectionMode(QAbstractItemView::MultiSelection);

    // set pointers and small pointers for correct resource management
    this->measurementsChart = std::make_shared<QChart>();
    this->measurementsChart->createDefaultAxes();
    this->measurementsChart->setTitle("Consumo de energia");
    this->measurementsChart->legend()->hide();
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());

    // connect interactive elements with respective functions
    connect(ui->buttonImportDocuments, SIGNAL(clicked()), this, SLOT(importDocument()));
    connect(ui->buttonGenerateDiagram, SIGNAL(clicked()), this, SLOT(generateDiagram()));
    connect(ui->buttonSaveDiagram, SIGNAL(clicked()), this, SLOT(saveDiagram()));
    connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSheetList(int)));
    connect(ui->comboBoxSelectSheet, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDaysEntries(int)));
}

MainWindow::~MainWindow() {
    delete ui;
}

/*
void MainWindow::readDocument(QXlsx::Document* measurementData) {
    if(measurementData->load()) {

        // Document has 3 columns and a variable number of rows
        // indexing of excel sheets starts at (1,1)
        measurementData->selectSheet("Hoja1");

        bool cellNotEmpty = true;
        QVariant currentCellValue;
        QString currentRow;
        QString cellValueString;
        QString prevCellValueString;

        double currentTimeDataValue;
        double currentDataValue;

        // parse measurementData, convert cell Data if necessary
        for(int i = 1; cellNotEmpty; ++i) {
            for(int j = 1; j < 4; ++j) {

                // read current cell value as iterating through the document
                currentCellValue = measurementData->read(i,j);
                qDebug() << currentCellValue.toReal();

                // convert wrongly detected times
                if((j == 2) && (i > 1)) {
                    if(currentCellValue.canConvert(15)) {
                        currentCellValue = currentCellValue.toTime();
                    }
                    if(currentCellValue.type() == 14) {
                        currentCellValue = QVariant(QTime(0,0,0));
                    }
                    QTime temptime = currentCellValue.toTime();
                    currentTimeDataValue = temptime.msecsSinceStartOfDay();
                }

                if((j == 3) && (i > 1)) {
                    currentDataValue = currentCellValue.toReal();
                }

                // convert cell value to string (to print), check for end of doc and count days
                // TODO: check for correct parsing of time or hour format!
                cellValueString = currentCellValue.toString();

                qDebug() << cellValueString;
                if((j == 1) && (!(cellValueString.isEmpty()))) {
                    if(i == 2) {
                        prevCellValueString = cellValueString;
                        numOfDays = 1;
                        days.push_back(currentCellValue.toDate());
                        measurementSeries.push_back(new QLineSeries);
                    } else if(i > 2) {
                        if(cellValueString != prevCellValueString) {
                            ++numOfDays;
                            days.push_back(currentCellValue.toDate());
                            measurementSeries.push_back(new QLineSeries);
                        }
                        prevCellValueString = cellValueString;
                    }
                }
                if(cellValueString.isEmpty()) {
                    cellNotEmpty = false;
                } else {
                    currentRow += cellValueString + "  ";
                }
            }

            // display spreadsheet and update measurement series
            if(i > 1) {
                measurementSeries[numOfDays-1]->append(currentTimeDataValue, currentDataValue);
            }
            ui->textEditDisplaySheet->append(currentRow);
            currentRow = "";
        }
        //qDebug() << days.size();
        //qDebug() << measurementSeries.size();
    }
}
*/

/*
// TODO: open file will be replaced by importDocument
void MainWindow::openFile() {
    QString spreadsheetFileName = QFileDialog::getOpenFileName(this,
        tr("Abrir archivo Excel"), "~/", tr("Formato Excel (*.xls *.xlsx)"));
    ui->lineEditOpenFile->setText(spreadsheetFileName);
    this->measurementData = new QXlsx::Document(spreadsheetFileName);

    readDocument(this->measurementData);
}
*/


void MainWindow::importDocument() {
    QString docFileName = QFileDialog::getOpenFileName(this,
        tr("Abrir archivo Excel"), "~/", tr("Formato Excel (*.xlsx)"));
    documents.emplace_back(MeasurementsDocument(docFileName));
    addEntryComboBoxDocSelection(docFileName);
}

void MainWindow::generateDiagram() {
    // get list of selected days
    std::vector<int> selectedDaysIndices;
    //this->auxiliaryUpdateChart = std::make_shared<QChart>();
    measurementsChart->removeAllSeries();
    for(int i = 0; i < ui->listWidgetDays->count(); ++i) {
        if(ui->listWidgetDays->item(i)->isSelected()) {
            selectedDaysIndices.emplace_back(i);

            // debug is correct
            //qDebug() << "Day " <<
            //    documents[selectedDocIndex].sheets[selectedSheetIndex]->daysAndCounting[i].first
            //    << "with index " << i << "is selected";
        }
    }
    //measurementsChart.reset(new QChart());
    for(unsigned short i = 0; i < selectedDaysIndices.size(); ++i) {
        measurementsChart->addSeries(
                 documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[selectedDaysIndices[i]].get());
    }
    //measurementsChart->legend()->hide();
    //measurementsChart->createDefaultAxes();
    //measurementsChart->setTitle("Consumo energetico");
    //ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    //ui->graphicsViewChart->setChart(measurementsChart.get());
    //measurementsChart = auxiliaryUpdateChart;
    //auxiliaryUpdateChart = nullptr;
}

/*
void MainWindow::generateDiagram() {
    if(numOfDays == 0) {
        return;
    }
    measurementsChart.reset(new QChart());
    for(unsigned short i = 0; i < numOfDays; ++i) {
        measurementsChart->addSeries(measurementSeries[i]);
    }
    measurementsChart->legend()->hide();
    measurementsChart->createDefaultAxes();
    measurementsChart->setTitle("Consumo durante el dia.");
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
}*/

void MainWindow::saveDiagram() {
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Guardar imagen"),
                                                        "~/", tr("Imagenes (*.png)"));
    QPixmap measurementsImage = ui->graphicsViewChart->grab();
    measurementsImage.save(saveFileName, "PNG");
}

void MainWindow::addEntryComboBoxDocSelection(const QString& newDocument) {
    ui->comboBoxSelectDoc->addItem(newDocument);
}

void MainWindow::updateSheetList(int docIndex) {
    ui->comboBoxSelectSheet->clear();
    this->selectedDocIndex = docIndex;
    for(unsigned short i = 0; i < documents[docIndex].sheets.size(); ++i) {
        ui->comboBoxSelectSheet->addItem(documents[docIndex].sheets[i]->sheetName);
    }
}

void MainWindow::updateDaysEntries(int sheetIndex) {
    ui->listWidgetDays->clear();
    if(sheetIndex >= 0) {
        this->selectedSheetIndex = sheetIndex;
        short numberOfDays = documents[this->selectedDocIndex].sheets[this->selectedSheetIndex]->daysAndCounting.size();
        for(short i = 0; i < numberOfDays; ++i) {
            ui->listWidgetDays->addItem(
                documents[this->selectedDocIndex].sheets[this->selectedSheetIndex]->daysAndCounting[i].first);
        }
    }

    // debug: display current sheet on the display
    if(sheetIndex >= 0) {
        ui->textEditDisplaySheet->clear();
        int docIndex = ui->comboBoxSelectDoc->currentIndex();
        int sheetLength = documents[docIndex].sheets[sheetIndex]->allDays.size();
        QString currentDay, currentTime, currentValue;
        for(int i = 0; i < sheetLength; ++i) {
            currentDay = documents[docIndex].sheets[sheetIndex]->allDays[i].toString();
            currentTime = documents[docIndex].sheets[sheetIndex]->timestamps[i].toString();
            currentValue = documents[docIndex].sheets[sheetIndex]->measurements[i].toString();
            ui->textEditDisplaySheet->append(currentDay + "  " + currentTime + "  " + currentValue);
        }
    }
}
