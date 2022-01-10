#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), numOfDays(0) {
    ui->setupUi(this);
    ui->comboBoxSelectDoc->setPlaceholderText("Seleccionar documento");
    ui->comboBoxSelectDoc->placeholderText();
    ui->comboBoxSelectDoc->setCurrentIndex(-1);

    ui->listWidgetDays->setSelectionMode(QAbstractItemView::MultiSelection);

    // set pointers and small pointers for correct resource management
    this->measurementData = NULL;
    this->measurementsChart = std::make_shared<QChart>();

    // connect interactive elements with respective functions
    connect(ui->buttonImportDocuments, SIGNAL(clicked()), this, SLOT(importDocument()));
    connect(ui->buttonOpenFile, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->buttonGenerateDiagram, SIGNAL(clicked()), this, SLOT(generateDiagram()));
    connect(ui->buttonSaveDiagram, SIGNAL(clicked()), this, SLOT(saveDiagram()));
    connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSheetList(int)));
    connect(ui->comboBoxSelectSheet, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDaysEntries(int)));
}

MainWindow::~MainWindow() {
    delete ui;

    if(this->measurementData) {
        delete this->measurementData;
    }

    for(unsigned short int i = 0; i < measurementSeries.size(); ++i) {
        delete measurementSeries[i];
    }
}

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


// TODO: open file will be replaced by importDocument
void MainWindow::openFile() {
    QString spreadsheetFileName = QFileDialog::getOpenFileName(this,
        tr("Abrir archivo Excel"), "~/", tr("Formato Excel (*.xls *.xlsx)"));
    ui->lineEditOpenFile->setText(spreadsheetFileName);
    this->measurementData = new QXlsx::Document(spreadsheetFileName);

    readDocument(this->measurementData);
}

void MainWindow::importDocument() {
    QString docFileName = QFileDialog::getOpenFileName(this,
        tr("Abrir archivo Excel"), "~/", tr("Formato Excel (*.xlsx)"));
    documents.push_back(MeasurementsDocument(docFileName));
    addEntryComboBoxDocSelection(docFileName);
}

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
}

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
    for(unsigned short i = 0; i < documents[docIndex].sheets.size(); ++i) {
        ui->comboBoxSelectSheet->addItem(documents[docIndex].sheets[i]->sheetName);
    }
}

void MainWindow::updateDaysEntries(int sheetIndex) {
    QString dayEntry = "Day Number " + QString::number(sheetIndex);
    ui->listWidgetDays->addItem(dayEntry);

    // debug: display current sheet on the display
    if(sheetIndex >= 0) {
        ui->textEditDisplaySheet->clear();
        int docIndex = ui->comboBoxSelectDoc->currentIndex();
        int sheetLength = documents[docIndex].sheets[sheetIndex]->days.size();
        QString currentDay, currentTime, currentValue;
        for(int i = 0; i < sheetLength; ++i) {
            currentDay = documents[docIndex].sheets[sheetIndex]->days[i].toString();
            currentTime = documents[docIndex].sheets[sheetIndex]->timestamps[i].toString();
            currentValue = documents[docIndex].sheets[sheetIndex]->measurements[i].toString();
            ui->textEditDisplaySheet->append(currentDay + "  " + currentTime + "  " + currentValue);
        }
    }
}
