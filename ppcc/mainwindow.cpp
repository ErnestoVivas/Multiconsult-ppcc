#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->comboBoxSelectDoc->setCurrentIndex(-1);
    ui->comboBoxSelectDoc->setPlaceholderText("Seleccionar documento");
    ui->listWidgetDays->setSelectionMode(QAbstractItemView::MultiSelection);

    // set pointers and smart pointers for correct resource management
    this->measurementsChart = std::make_shared<QChart>();
    this->auxiliaryUpdateChart = std::make_shared<QChart>();

    // set auxChart parameters
    this->auxiliaryUpdateChart->legend()->hide();
    this->auxiliaryUpdateChart->createDefaultAxes();

    // connect interactive elements with respective functions
    connect(ui->buttonImportDocuments, SIGNAL(clicked()), this, SLOT(importDocument()));
    connect(ui->buttonGenerateDiagram, SIGNAL(clicked()), this, SLOT(generateDiagram()));
    connect(ui->buttonExportDiagram, SIGNAL(clicked()), this, SLOT(saveDiagram()));
    connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSheetList(int)));
    connect(ui->comboBoxSelectSheet, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDaysEntries(int)));
    connect(ui->menuImportDocuments, SIGNAL(triggered()), this, SLOT(importDocument()));
    connect(ui->menuQuit, SIGNAL(triggered()), this, SLOT(exitProgram()));
}

MainWindow::~MainWindow() {
    ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());
    for(int i = 0; i < this->displayedSeries.size(); ++i) {
        delete displayedSeries[i];
    }
    this->xAxisDateTime.reset();
    this->yAxis.reset();
    this->xAxisValue.reset();
    measurementsChart.reset();
    delete ui;
}

void MainWindow::importDocument() {
    QStringList docFileNames = QFileDialog::getOpenFileNames(this,
            tr("Abrir archivo(s) Excel"), "/home", tr("Formato Excel (*.xlsx)"));

    // Add document and set classification of each document
    Sector sec;
    ResidentialRange rr;
    Commercial subCom;
    Industrial subInd;
    for(int i = 0; i < docFileNames.size(); ++i) {
        QString fileNameOnly = docFileNames[i].section("/",-1,-1);
        if(!docFileNames[i].isEmpty()) {
            SetCategoriesDialog* selectCategories =
                    new SetCategoriesDialog(this, sec, rr, subCom, subInd, fileNameOnly);
            selectCategories->exec();
            delete selectCategories;
            documents.emplace_back(MeasurementsDocument(docFileNames[i]));
            documents.back().docSector = sec;
            documents.back().docResRange = rr;
            documents.back().docSubCommercial = subCom;
            documents.back().docSubIndustrial = subInd;

            qDebug() << documents.back().docSector << ", "
                     << documents.back().docResRange << ", "
                     << documents.back().docSubCommercial << ", "
                     << documents.back().docSubIndustrial;

            addEntryComboBoxDocSelection(fileNameOnly);
            if(ui->comboBoxSelectDoc->currentIndex() == -1) {
                ui->comboBoxSelectDoc->setCurrentIndex(0);
            }
        }
    }
}

int MainWindow::generateDiagram() {

    int returnVal = -1;

    // get list of selected days
    std::vector<int> selectedDaysIndices;
    for(int i = 0; i < ui->listWidgetDays->count(); ++i) {
        if(ui->listWidgetDays->item(i)->isSelected()) {
            selectedDaysIndices.emplace_back(i);
            // debug is correct
            //qDebug() << "Day " <<
            //    documents[selectedDocIndex].sheets[selectedSheetIndex]->daysAndCounting[i].first
            //    << "with index " << i << "is selected";
        }
    }

    // update diagram only if there are valid entries
    if(selectedDaysIndices.size() > 0) {

        // while chart is being updated, auxChart is set in chartView to avoid crash
        // (ownership of measurementsChart by chartView is released)
        ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());


        // delete QLineSeries from old chart, empty series list
        for(int i = 0; i < this->displayedSeries.size(); ++i) {
            delete displayedSeries[i];
        }
        displayedSeries.clear();

        // reset measurementsChart to create new clean chart
        this->xAxisDateTime.reset();
        this->yAxis.reset();
        this->xAxisValue.reset();
        measurementsChart.reset();
        this->measurementsChart = std::make_shared<QChart>();

        // copy by value series from sheets to current displayed series
        for(unsigned short i = 0; i < selectedDaysIndices.size(); ++i) {
            displayedSeries.append(new QLineSeries());
            displayedSeries[i]->setName(
                    documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[selectedDaysIndices[i]]->name());
            QVector<QPointF> dataPoints =
                documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[selectedDaysIndices[i]]->pointsVector();
            for(int j = 0; j < dataPoints.size(); ++j) {
                displayedSeries[i]->append(dataPoints[j]);
            }
            measurementsChart->addSeries(displayedSeries[i]);
        }

        /*      TODO: implement correct date time axes
        if(documents[selectedDocIndex].sheets[selectedSheetIndex]->xIsTime) {
            this->xAxisDateTime = std::make_shared<QDateTimeAxis>();
            this->xAxisDateTime->setTickCount(7);
            this->xAxisDateTime->setFormat("hh:mm:ss");
            this->xAxisDateTime->setMin(QDateTime(QDate(0,0,0), QTime(0,0,0)));
            this->xAxisDateTime->setMin(QDateTime(QDate(0,0,0), QTime(23,59,59)));
            this->xAxisDateTime->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
            this->yAxis = std::make_shared<QValueAxis>();
            this->yAxis->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);
            measurementsChart->addAxis(this->xAxisDateTime.get(), Qt::AlignBottom);
            measurementsChart->addAxis(this->yAxis.get(), Qt::AlignLeft);
            for(int i = 0; i < displayedSeries.size(); ++i) {
                displayedSeries[i]->attachAxis(this->xAxisDateTime.get());
                displayedSeries[i]->attachAxis(this->yAxis.get());
            }
        } else {
            this->xAxisValue = std::make_shared<QValueAxis>();
            this->xAxisValue->setTickCount(13);
            this->xAxisValue->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
            this->yAxis = std::make_shared<QValueAxis>();
            this->yAxis->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);
            measurementsChart->addAxis(this->xAxisValue.get(), Qt::AlignBottom);
            measurementsChart->addAxis(this->yAxis.get(), Qt::AlignLeft);
            for(int i = 0; i < displayedSeries.size(); ++i) {
                displayedSeries[i]->attachAxis(this->xAxisValue.get());
                displayedSeries[i]->attachAxis(this->yAxis.get());
            }
        }*/

        // setup and display chart
        measurementsChart->legend()->setVisible(true);
        measurementsChart->legend()->setAlignment(Qt::AlignBottom);
        measurementsChart->createDefaultAxes();

        // set Axis labels
        QList<QAbstractAxis*> chartAxes = measurementsChart->axes();
        // debug (is correct)
        //for(int i = 0; i < chartAxes.size(); ++i) {
        //    qDebug() << "Axis number " << i;
        //}
        chartAxes[0]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
        chartAxes[1]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);

        measurementsChart->setTitle("Consumo de energia");
        ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
        ui->graphicsViewChart->setChart(measurementsChart.get());

        returnVal = 0;
    } else {
        QMessageBox::information(this, tr("No se puede generar el diagrama."),
                  tr("No ha importado ningun archivo o no ha seleccionado elementos para visualizar."),
                  QMessageBox::Ok);
    }
    return returnVal;
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
        //int sheetLength = documents[docIndex].sheets[sheetIndex]->allDays.size();
        //QString currentDay, currentTime, currentValue;

        for(unsigned int i = 0; i < documents[docIndex].sheets[sheetIndex]->daysAndCounting.size(); ++i) {
            ui->textEditDisplaySheet->append(documents[docIndex].sheets[sheetIndex]->daysAndCounting[i].first);
            QVector<QPointF> lineSeriesPoints =
                    documents[docIndex].sheets[sheetIndex]->measurementSeries[i]->pointsVector();
            for(int j = 0; j < lineSeriesPoints.size(); ++j) {
                QString rowText = QString::number(lineSeriesPoints[j].x()) + ", "
                                  + QString::number(lineSeriesPoints[j].y());
                ui->textEditDisplaySheet->append(rowText);
            }
            ui->textEditDisplaySheet->append("");
        }

        /* OLD Display method (does not consider QLineSeries)
        for(int i = 0; i < sheetLength; ++i) {
            currentDay = documents[docIndex].sheets[sheetIndex]->allDays[i].toString();
            currentTime = documents[docIndex].sheets[sheetIndex]->timestamps[i].toString();
            currentValue = documents[docIndex].sheets[sheetIndex]->measurements[i].toString();
            ui->textEditDisplaySheet->append(currentDay + "  " + currentTime + "  " + currentValue);
        }*/
    }
}

void MainWindow::exitProgram() {
    QCoreApplication::quit();
}
