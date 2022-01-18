#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow) {

    // setup ui elements
    ui->setupUi(this);
    ui->textEditDisplaySheet->setReadOnly(true);
    setupComboBoxesFileCategories();
    updateFileSubCatComboBox(0);
    ui->comboBoxFileFreq->addItem("15 min");
    ui->comboBoxFileFreq->addItem("1 h");

    // setup widgets containting the different functions to generate diagrams
    simpleDiagramFunction = new SimpleDiagramFunction(this);
    siteAnalysis = new SiteAnalysis(this);
    sectorDayAnalysis = new SectorDayAnalysis(this);
    ui->stackedWidgetDiagramFunctions->addWidget(simpleDiagramFunction);
    ui->stackedWidgetDiagramFunctions->addWidget(siteAnalysis);
    ui->stackedWidgetDiagramFunctions->addWidget(sectorDayAnalysis);
    ui->comboBoxSelectFunction->addItem("Diagrama simple");
    ui->comboBoxSelectFunction->addItem("Análisis de sitio");
    ui->comboBoxSelectFunction->addItem("Análisis de sector por dia");

    // set pointers and smart pointers for correct resource management
    this->measurementsChart = std::make_shared<QChart>();
    this->auxiliaryUpdateChart = std::make_shared<QChart>();
    xAxis = new QValueAxis();
    yAxis = new QValueAxis();

    // set auxChart parameters
    this->auxiliaryUpdateChart->legend()->hide();
    this->auxiliaryUpdateChart->createDefaultAxes();


    // connect interactive elements with respective functions

    // menu
    connect(ui->menuImportDocuments, SIGNAL(triggered()), this, SLOT(importDocument()));
    connect(ui->menuQuit, SIGNAL(triggered()), this, SLOT(exitProgram()));

    // file management
    connect(ui->buttonImportDocuments, SIGNAL(clicked()), this, SLOT(importDocument()));
    connect(ui->listWidgetDocuments, SIGNAL(currentRowChanged(int)), this, SLOT(getFileCategories(int)));
    connect(ui->comboBoxFileCat, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFileSubCatComboBox(int)));
    connect(ui->comboBoxFileCat, SIGNAL(currentIndexChanged(int)), this, SLOT(setFileCategory(int)));
    connect(ui->comboBoxFileSubCat, SIGNAL(currentIndexChanged(int)), this, SLOT(setFileSubCategory(int)));
    connect(ui->comboBoxFileFreq, SIGNAL(currentIndexChanged(int)), this, SLOT(setFileFreq(int)));

    // functions
    connect(ui->comboBoxSelectFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(selectFunction(int)));
    connect(simpleDiagramFunction, SIGNAL(selectedDocChanged(int)), this, SLOT(updateSheetListSimpleDiagramFunction(int)));
    connect(simpleDiagramFunction, SIGNAL(selectedSheetChanged(int, int)), this, SLOT(updateDaysSimpleDiagramFunction(int, int)));
    connect(siteAnalysis, SIGNAL(selectedDocChanged(int)), this, SLOT(updateSheetListSiteAnalysis(int)));
    connect(ui->buttonGenerateDiagram, SIGNAL(clicked()), this, SLOT(generateDiagram()));
    connect(ui->buttonExportDiagram, SIGNAL(clicked()), this, SLOT(saveDiagram()));
}

MainWindow::~MainWindow() {

    // remove diagram function widgets from stacked widget container and delete them
    ui->stackedWidgetDiagramFunctions->removeWidget(sectorDayAnalysis);
    ui->stackedWidgetDiagramFunctions->removeWidget(siteAnalysis);
    ui->stackedWidgetDiagramFunctions->removeWidget(simpleDiagramFunction);
    delete sectorDayAnalysis;
    delete siteAnalysis;
    delete simpleDiagramFunction;

    // delete chart pointer elements (necessary, segfault if not)
    ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());
    for(int i = 0; i < this->displayedSeries.size(); ++i) {
        displayedSeries[i]->detachAxis(xAxis);
        displayedSeries[i]->detachAxis(yAxis);
        delete displayedSeries[i];
    }
    //this->xAxisDateTime.reset();
    //this->yAxis.reset();
    //this->xAxisValue.reset();
    measurementsChart->removeAxis(xAxis);
    measurementsChart->removeAxis(yAxis);
    measurementsChart.reset();
    delete xAxis;
    delete yAxis;

    // delete rest
    delete ui;
}

void MainWindow::setupComboBoxesFileCategories() {
    ui->comboBoxFileCat->addItem("Residencial");
    ui->comboBoxFileCat->addItem("Comercial");
    ui->comboBoxFileCat->addItem("Industrial");
    ui->comboBoxFileCat->addItem("Bombeo");
    ui->comboBoxFileCat->addItem("Alumbrado público");
}

void MainWindow::updateFileSubCatComboBox(int sectorIndex) {
    ui->comboBoxFileSubCat->clear();
    if(sectorIndex == 0) {
        ui->comboBoxFileSubCat->addItem("50 kWh/mes");
        ui->comboBoxFileSubCat->addItem("51 - 100 kWh/mes");
        ui->comboBoxFileSubCat->addItem("101 - 150 kWh/mes");
        ui->comboBoxFileSubCat->addItem("151 - 250 kWh/mes");
        ui->comboBoxFileSubCat->addItem("251 - 500 kWh/mes");
        ui->comboBoxFileSubCat->addItem(">500 kWh/mes");
    } else if(sectorIndex == 1) {
        ui->comboBoxFileSubCat->addItem("Financieras");
        ui->comboBoxFileSubCat->addItem("Comercio al por mayor");
        ui->comboBoxFileSubCat->addItem("Comercio al por menor");
        ui->comboBoxFileSubCat->addItem("Gobierno");
        ui->comboBoxFileSubCat->addItem("Salud");
        ui->comboBoxFileSubCat->addItem("Hoteles");
        ui->comboBoxFileSubCat->addItem("Educación");
        ui->comboBoxFileSubCat->addItem("Otros");
    } else if(sectorIndex == 2) {
        ui->comboBoxFileSubCat->addItem("Alimentos y bebidas");
        ui->comboBoxFileSubCat->addItem("Papel, cartón");
        ui->comboBoxFileSubCat->addItem("Caucho, plastico");
        ui->comboBoxFileSubCat->addItem("Quimica");
        ui->comboBoxFileSubCat->addItem("Textil");
        ui->comboBoxFileSubCat->addItem("Otros");
    }
}

void MainWindow::getFileCategories(int selectedFile) {
    int sector = documents[selectedFile].docSector;
    int subCat = documents[selectedFile].getSubCategory();
    int freq = documents[selectedFile].docFreq;
    ui->comboBoxFileCat->setCurrentIndex(sector);
    ui->comboBoxFileSubCat->setCurrentIndex(subCat);
    ui->comboBoxFileFreq->setCurrentIndex(freq);
}

void MainWindow::setFileCategory(int newSector) {
    if(ui->listWidgetDocuments->currentRow() >= 0) {
        documents[ui->listWidgetDocuments->currentRow()].setSector(newSector);
    }
}

void MainWindow::setFileSubCategory(int newSubCat) {
    if(ui->listWidgetDocuments->currentRow() >= 0) {
        documents[ui->listWidgetDocuments->currentRow()].setSubCategory(newSubCat);
    }
}

void MainWindow::setFileFreq(int newFreq) {
    if(ui->listWidgetDocuments->currentRow() >= 0) {
        if(newFreq == 0) {
            documents[ui->listWidgetDocuments->currentRow()].docFreq = Frequency::quarterHour;
        } else if(newFreq == 1) {
            documents[ui->listWidgetDocuments->currentRow()].docFreq = Frequency::hour;
        }
    }
}

void MainWindow::importDocument() {
    QStringList docFileNames = QFileDialog::getOpenFileNames(this,
            tr("Abrir archivo(s) Excel"), "/home", tr("Formato Excel (*.xlsx)"));

    // Add document to the documents list and set classification of the document
    Categories docCategories;
    for(int i = 0; i < docFileNames.size(); ++i) {
        QString fileNameOnly = docFileNames[i].section("/",-1,-1);
        if(!docFileNames[i].isEmpty()) {
            SetCategoriesDialog* selectCategories =
                    new SetCategoriesDialog(this, docCategories, fileNameOnly);

            selectCategories->exec();
            delete selectCategories;
            documents.emplace_back(MeasurementsDocument(docFileNames[i]));
            documents.back().docSector = docCategories.sector;
            documents.back().docResRange = docCategories.resRange;
            documents.back().docSubCommercial = docCategories.commercial;
            documents.back().docSubIndustrial = docCategories.industrial;
            documents.back().docFreq = docCategories.frequency;
            documents.back().docDateFormat = docCategories.dateFormat;

            // add corresponding entries to the function widgets
            simpleDiagramFunction->addEntryComboBoxSelectDoc(fileNameOnly);
            siteAnalysis->addEntryComboBoxSelectDoc(fileNameOnly);
            ui->listWidgetDocuments->addItem(fileNameOnly);
            if(ui->listWidgetDocuments->count() == 1) {
                ui->listWidgetDocuments->setCurrentRow(0);
            }
        }
    }
}

void MainWindow::selectFunction(int functionIndex) {
    if(functionIndex == 0) {
        ui->stackedWidgetDiagramFunctions->setCurrentWidget(simpleDiagramFunction);
    } else if(functionIndex == 1) {
        ui->stackedWidgetDiagramFunctions->setCurrentWidget(siteAnalysis);
    } else if(functionIndex == 2) {
        ui->stackedWidgetDiagramFunctions->setCurrentWidget(sectorDayAnalysis);
    }
}

int MainWindow::generateSimpleDiagram() {

    int returnVal = -1;

    // get inputs from the user
    std::vector<int> selectedDaysIndices = simpleDiagramFunction->getSelectedDays();
    int selectedDocIndex = simpleDiagramFunction->selectedDocIndex;
    int selectedSheetIndex = simpleDiagramFunction->selectedSheetIndex;

    // update diagram only if there are valid entries
    if(selectedDaysIndices.size() > 0) {

        // while chart is being updated, auxChart is set in chartView to avoid crash
        // (ownership of measurementsChart by chartView is released)
        ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());


        // delete QLineSeries from old chart, empty series list
        for(int i = 0; i < this->displayedSeries.size(); ++i) {
            displayedSeries[i]->detachAxis(xAxis);
            displayedSeries[i]->detachAxis(yAxis);
            delete displayedSeries[i];
        }
        displayedSeries.clear();

        // reset measurementsChart to create new clean chart
        //this->xAxisDateTime.reset();
        //this->yAxis.reset();
        //this->xAxisValue.reset();
        measurementsChart->removeAxis(xAxis);
        measurementsChart->removeAxis(yAxis);
        measurementsChart.reset();
        delete xAxis;
        delete yAxis;

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

        // setup axes
        xAxis = new QValueAxis();
        xAxis->setMin(0.0);
        xAxis->setMax(24.0);
        xAxis->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
        xAxis->setTickCount(13);
        measurementsChart->addAxis(xAxis, Qt::AlignBottom);

        yAxis = new QValueAxis();
        yAxis->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);
        measurementsChart->addAxis(yAxis, Qt::AlignLeft);

        for(int i = 0; i < displayedSeries.size(); ++i) {
            displayedSeries[i]->attachAxis(xAxis);
            displayedSeries[i]->attachAxis(yAxis);
        }

        // setup and display chart
        measurementsChart->legend()->setVisible(true);
        measurementsChart->legend()->setAlignment(Qt::AlignBottom);
        //measurementsChart->createDefaultAxes();
        //QList<QAbstractAxis*> chartAxes = measurementsChart->axes();
        //chartAxes[0]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
        //chartAxes[1]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);
        //chartAxes[0]->setMin(0.0);
        //chartAxes[0]->setMax(24.0);
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

int MainWindow::generateSiteAnalysisDiagram() {
    int result = -1;

    // get inputs from the user and find weekdays
    int selectedDocIndex = siteAnalysis->selectedDocIndex;
    int selectedSheetIndex = siteAnalysis->selectedSheetIndex;
    int selectedDay = siteAnalysis->getDay() + 1;      // to compare with QDate: 1=mon,...,7=sun
    int visType = siteAnalysis->getVisType();
    std::vector<int> weekdayIndices = findWeekdays(selectedDay, selectedDocIndex, selectedSheetIndex);

    if(weekdayIndices.size() == 0) {
        QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos para el dia indicado."),
                  QMessageBox::Ok);
        return result;
    }

    // set to AuxChart while updating main chart
    ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());


    // delete QLineSeries from old chart, empty series list
    for(int i = 0; i < this->displayedSeries.size(); ++i) {
        delete displayedSeries[i];
    }
    displayedSeries.clear();

    // reset measurementsChart to create new clean chart
    //this->xAxisDateTime.reset();
    //this->yAxis.reset();
    //this->xAxisValue.reset();
    measurementsChart.reset();
    this->measurementsChart = std::make_shared<QChart>();


    // copy by value corresponding series from sheets to current displayed series
    for(unsigned short i = 0; i < weekdayIndices.size(); ++i) {
        displayedSeries.append(new QLineSeries());
        displayedSeries[i]->setName(
                documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[weekdayIndices[i]]->name());
        QVector<QPointF> dataPoints =
            documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[weekdayIndices[i]]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            displayedSeries[i]->append(dataPoints[j]);
        }
    }

    if(visType == 1) {
        displayedSeries = getAverageFromSeries(displayedSeries);
    } else if(visType == 2) {
        displayedSeries = getSumFromSeries(displayedSeries);
    }

    for(int i = 0; i < displayedSeries.size(); ++i) {
        measurementsChart->addSeries(displayedSeries[i]);
    }

    // setup and display chart
    measurementsChart->legend()->setVisible(true);
    measurementsChart->legend()->setAlignment(Qt::AlignBottom);
    measurementsChart->createDefaultAxes();
    QList<QAbstractAxis*> chartAxes = measurementsChart->axes();
    chartAxes[0]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
    chartAxes[1]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);
    chartAxes[0]->setMin(0.0);
    chartAxes[0]->setMax(24.0);
    measurementsChart->setTitle("Consumo de energia");
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
    result = 0;

    return result;
}

std::vector<int> MainWindow::findWeekdays(int &selectedDay, int &selectedDocIndex, int &selectedSheetIndex) {
    std::vector<int> weekdayIndices;
    QString currentDay;
    QString currentMonth;
    QString currentYear;
    int candidateDay;
    int candidateMonth;
    int candidateYear;
    for(unsigned int i = 0; i < documents[selectedDocIndex].sheets[selectedSheetIndex]->daysAndCounting.size(); ++i) {
        QString candidateDateStr = documents[selectedDocIndex].sheets[selectedSheetIndex]->daysAndCounting[i].first;
        if(documents[selectedDocIndex].sheets[selectedSheetIndex]->datesAreStrings) {
            if(candidateDateStr.contains("/")) {

                // Assumption: All dates provided by excel sheet have the format dd/mm/yyyy
                currentDay = candidateDateStr.section("/",0,0);
                currentMonth = candidateDateStr.section("/",1,1);
                currentYear = candidateDateStr.section("/",2,2);

            } else if(candidateDateStr.contains("-")) {
                currentDay = candidateDateStr.section("-",0,0);
                currentMonth = candidateDateStr.section("-",1,1);
                currentYear = candidateDateStr.section("-",2,2);
            }


        } else {
            currentDay = candidateDateStr.section("-",2,2);
            currentMonth = candidateDateStr.section("-",1,1);
            currentYear = candidateDateStr.section("-",0,0);
        }
        candidateDay = currentDay.toInt();
        candidateMonth = currentMonth.toInt();
        candidateYear = currentYear.toInt();
        QDate candidateDate = QDate(candidateYear, candidateMonth, candidateDay);
        if(candidateDate.dayOfWeek() == selectedDay) {
            weekdayIndices.emplace_back(i);
        }
    }
    return weekdayIndices;
}

QList<QLineSeries*> MainWindow::getAverageFromSeries(QList<QLineSeries*> &oldLineSeries) {


    QList<QLineSeries*> newLineSeries;
    newLineSeries.append(new QLineSeries());
    double totalNumOfSeries = oldLineSeries.size();
    double totalNumOfValues = oldLineSeries[0]->count();

    // Check, that oldLineSeries is rectangular and all dimensions >= 0
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        double currentNumOfValues = oldLineSeries[i]->count();
        if(currentNumOfValues != totalNumOfValues) {
            QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                      tr("No se puede calcular el promedio. Cada dia\n"
                         "debe tener la misma cantidad de mediciones."),
                      QMessageBox::Ok);
            return newLineSeries;
        }
    }

    // TODO: implement average calculation for non rectangular line series


    // Calculate average and fill new line series (inverted traversion!)
    for(int j = 0; j < totalNumOfValues; ++j) {
        double currentSeriesSum = 0.0;
        double currentAverageValue = 0.0;
        for(int i = 0; i < totalNumOfSeries; ++i) {
            currentSeriesSum += oldLineSeries[i]->at(j).y();
        }
        currentAverageValue = currentSeriesSum / totalNumOfSeries;
        newLineSeries[0]->append(oldLineSeries[0]->at(j).x(), currentAverageValue);
    }
    newLineSeries[0]->setName("Promedio");

    // delete old line series
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        delete oldLineSeries[i];
    }
    oldLineSeries.clear();

    return newLineSeries;
}

QList<QLineSeries*> MainWindow::getSumFromSeries(QList<QLineSeries*> &oldLineSeries) {

    QList<QLineSeries*> newLineSeries;
    newLineSeries.append(new QLineSeries());
    double totalNumOfSeries = oldLineSeries.size();
    double totalNumOfValues = oldLineSeries[0]->count();

    // Check, that oldLineSeries is rectangular and all dimensions >= 0
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        double currentNumOfValues = oldLineSeries[i]->count();
        if(currentNumOfValues != totalNumOfValues) {
            QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                      tr("No se puede calcular la suma. Cada dia\n"
                         "debe tener la misma cantidad de mediciones."),
                      QMessageBox::Ok);
            return newLineSeries;
        }
    }

    // TODO: implement average calculation for non rectangular line series

    // Inverted traversion!
    for(int j = 0; j < totalNumOfValues; ++j) {
        double currentSeriesSum = 0.0;
        for(int i = 0; i < totalNumOfSeries; ++i) {
            currentSeriesSum += oldLineSeries[i]->at(j).y();
        }
        newLineSeries[0]->append(oldLineSeries[0]->at(j).x(), currentSeriesSum);
    }
    newLineSeries[0]->setName("Suma");

    // delete old line series
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        delete oldLineSeries[i];
    }
    oldLineSeries.clear();

    return newLineSeries;
}


int MainWindow::generateSectorWeekdayDiagram() {

    int result = -1;

    // get categories and weekday that will be visualized
    int sector = sectorDayAnalysis->getSelectedSector();
    int subCat = sectorDayAnalysis->getSelectedSubCat();
    int selectedDay = sectorDayAnalysis->getDay() + 1;
    int visType = sectorDayAnalysis->getVisType();

    // First: find measurementDocs corresponding to the selected Category and store indices
    std::vector<int> correctFileIndices;
    for(unsigned int i = 0; i < this->documents.size(); ++i) {
        if((documents[i].docSector == sector) && (documents[i].getSubCategory() == subCat)) {
            correctFileIndices.emplace_back(i);
        }
    }

    // Now: find the corresponding weekdays in the corresponding files
    std::vector<std::tuple<int, int, int, int> > weekdayIndices;    // 0: doc, 1: sheet, 2: lineSeries, 3: freq
    int currentDoc;
    int currentSheet;
    int currentLineSeries;
    int currentFreq;
    for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
        for(int j = 0; j < documents[correctFileIndices[i]].sheets.size(); ++j) {
            std::vector<int> currSheetWeekdayIndices = findWeekdays(selectedDay, correctFileIndices[i], j);
            for(unsigned int k = 0; k < currSheetWeekdayIndices.size(); ++k) {
                currentDoc = correctFileIndices[i];
                currentSheet = j;
                currentLineSeries = currSheetWeekdayIndices[k];
                currentFreq = documents[correctFileIndices[i]].docFreq;
                weekdayIndices.emplace_back(std::make_tuple(currentDoc, currentSheet, currentLineSeries, currentFreq));
            }
        }
    }

    if(weekdayIndices.size() == 0) {
        QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos para el dia indicado."),
                  QMessageBox::Ok);
        return result;
    }

    // debug (TODO: adapt to tuple)
    //qDebug() << "File: " << correctFileIndices[i] << ", sheet: " << j << ", LineSeries: " << currSheetWeekdayIndices[k];

    // set to AuxChart while updating main chart
    ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());


    // delete QLineSeries from old chart, empty series list
    for(int i = 0; i < this->displayedSeries.size(); ++i) {
        delete displayedSeries[i];
    }
    displayedSeries.clear();

    // reset measurementsChart to create new clean chart
    //this->xAxisDateTime.reset();
    //this->yAxis.reset();
    //this->xAxisValue.reset();
    measurementsChart.reset();
    this->measurementsChart = std::make_shared<QChart>();

    // copy by value corresponding series from sheets to current displayed series
    for(unsigned short i = 0; i < weekdayIndices.size(); ++i) {

        // get indices
        int currDocIndex = std::get<0>(weekdayIndices[i]);
        int currSheetIndex = std::get<1>(weekdayIndices[i]);
        int currLineSeriesIndex = std::get<2>(weekdayIndices[i]);
        displayedSeries.append(new QLineSeries());
    //    displayedSeries[i]->setName(
    //            documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[weekdayIndices[i]]->name());
        QVector<QPointF> dataPoints =
            documents[currDocIndex].sheets[currSheetIndex]->measurementSeries[currLineSeriesIndex]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            displayedSeries[i]->append(dataPoints[j]);
        }
    }

    if(visType == 1) {
        displayedSeries = getAverageFromSeries(displayedSeries);
    } else if(visType == 2) {
        displayedSeries = getSumFromSeries(displayedSeries);
    }

    for(int i = 0; i < displayedSeries.size(); ++i) {
        measurementsChart->addSeries(displayedSeries[i]);
    }

    // setup and display chart
    measurementsChart->legend()->setVisible(true);
    measurementsChart->legend()->setAlignment(Qt::AlignBottom);
    measurementsChart->createDefaultAxes();
    QList<QAbstractAxis*> chartAxes = measurementsChart->axes();
    chartAxes[0]->setTitleText("Hora");
    chartAxes[1]->setTitleText("kW");
    chartAxes[0]->setMin(0.0);
    chartAxes[0]->setMax(24.0);
    measurementsChart->setTitle("Consumo de energia");
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
    result = 0;

    return result;
}
/* commented out for testing, use it later again
int MainWindow::generateClassifiedDiagram() {
    int result = -1;

    // get date and categories to be visualized in the diagram
    int sector = ui->comboBoxSelectSector->currentIndex();
    int subCategory = ui->comboBoxSelectSubCategory->currentIndex();
    QDate selectedDate = ui->dateEditSelectDate->date();
    qDebug() << sector << ", " << subCategory << ", " << selectedDate.toString();

    // find lineSeries corresponding to the selected date and categories
    std::vector<std::tuple<int, int, int> > lineSeriesIndices;
    for(unsigned int i = 0; i < this->documents.size(); ++i) {
        if((this->documents[i].docSector == sector) && (this->documents[i].getSubCategory() == subCategory)) {
            for(unsigned int j = 0; j < this->documents[i].sheets.size(); ++j) {
                for(unsigned int k = 0; k < this->documents[i].sheets[j]->daysAndCounting.size(); ++k) {
                    if(compareDates(selectedDate, this->documents[i].sheets[j]->daysAndCounting[k].first,
                                    this->documents[i].docDateFormat, this->documents[i].sheets[j]->datesAreStrings)) {
                        lineSeriesIndices.emplace_back(std::make_tuple(i, j, k));
                    }
                }
            }
        }
    }

    // debug: print out found indices
    for(unsigned int i = 0; i < lineSeriesIndices.size(); ++i) {
        qDebug() << "Found day: " << std::get<0>(lineSeriesIndices[i]) << ", "
                 << std::get<1>(lineSeriesIndices[i]) << ", " << std::get<2>(lineSeriesIndices[i]);
    }
    if(lineSeriesIndices.size() >= 1) {
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
        for(unsigned short i = 0; i < lineSeriesIndices.size(); ++i) {
            int lineSeriesDocIndex = std::get<0>(lineSeriesIndices[i]);
            int lineSeriesSheetIndex = std::get<1>(lineSeriesIndices[i]);
            int lineSeriesLineSeriesIndex = std::get<2>(lineSeriesIndices[i]);

            displayedSeries.append(new QLineSeries());
            displayedSeries[i]->setName(
                    documents[lineSeriesDocIndex].sheets[lineSeriesSheetIndex]->measurementSeries[lineSeriesLineSeriesIndex]->name());
            QVector<QPointF> dataPoints =
                documents[lineSeriesDocIndex].sheets[lineSeriesSheetIndex]->measurementSeries[lineSeriesLineSeriesIndex]->pointsVector();
            for(int j = 0; j < dataPoints.size(); ++j) {
                displayedSeries[i]->append(dataPoints[j]);
            }
            measurementsChart->addSeries(displayedSeries[i]);
        }

        // setup and display chart
        measurementsChart->legend()->setVisible(true);
        measurementsChart->legend()->setAlignment(Qt::AlignBottom);
        measurementsChart->createDefaultAxes();

        // set Axis labels
        QList<QAbstractAxis*> chartAxes = measurementsChart->axes();
        chartAxes[0]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
        chartAxes[1]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);

        measurementsChart->setTitle("Consumo de energia");
        ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
        ui->graphicsViewChart->setChart(measurementsChart.get());

        result = 0;
    } else {
        QMessageBox::information(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos correspondientes."),
                  QMessageBox::Ok);
    }
    return result;
}
*/


bool MainWindow::compareDates(QDate &selectedDate, QString &candidateDate, DateFormat &candidateDF, bool &dateWasString) {
    bool result = false;

    int selectedDay = selectedDate.day();
    int selectedMonth = selectedDate.month();
    int selectedYear = selectedDate.year();

    QString currentDay;
    QString currentMonth;
    QString currentYear;

    int candidateDay;
    int candidateMonth;
    int candidateYear;

    // analize candidate date
    if(dateWasString) {
        if(candidateDate.contains("/")) {
            if(candidateDF == DateFormat::dayMonthYear) {
                currentDay = candidateDate.section("/",0,0);
                currentMonth = candidateDate.section("/",1,1);
                currentYear = candidateDate.section("/",2,2);
            } else if(candidateDF == DateFormat::monthDayYear) {
                currentDay = candidateDate.section("/",1,1);
                currentMonth = candidateDate.section("/",0,0);
                currentYear = candidateDate.section("/",2,2);
            } else if(candidateDF == DateFormat::yearMonthDay) {
                currentDay = candidateDate.section("/",2,2);
                currentMonth = candidateDate.section("/",1,1);
                currentYear = candidateDate.section("/",0,0);
            } else if(candidateDF == DateFormat::yearDayMonth) {
                currentDay = candidateDate.section("/",1,1);
                currentMonth = candidateDate.section("/",2,2);
                currentYear = candidateDate.section("/",0,0);
            }
        } else if(candidateDate.contains("-")) {
            if(candidateDF == DateFormat::dayMonthYear) {
                currentDay = candidateDate.section("-",0,0);
                currentMonth = candidateDate.section("-",1,1);
                currentYear = candidateDate.section("-",2,2);
            } else if(candidateDF == DateFormat::monthDayYear) {
                currentDay = candidateDate.section("-",1,1);
                currentMonth = candidateDate.section("-",0,0);
                currentYear = candidateDate.section("-",2,2);
            } else if(candidateDF == DateFormat::yearMonthDay) {
                currentDay = candidateDate.section("-",2,2);
                currentMonth = candidateDate.section("-",1,1);
                currentYear = candidateDate.section("-",0,0);
            } else if(candidateDF == DateFormat::yearDayMonth) {
                currentDay = candidateDate.section("-",1,1);
                currentMonth = candidateDate.section("-",2,2);
                currentYear = candidateDate.section("-",0,0);
            }
        }
    } else {
        currentDay = candidateDate.section("-",2,2);
        currentMonth = candidateDate.section("-",1,1);
        currentYear = candidateDate.section("-",0,0);
    }

    candidateDay = currentDay.toInt();
    candidateMonth = currentMonth.toInt();
    candidateYear = currentYear.toInt();
    if((selectedDay == candidateDay) && (selectedMonth == candidateMonth) && (selectedYear == candidateYear)) {
        result = true;
    }

    return result;
}

void MainWindow::saveDiagram() {
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Guardar imagen"),
                                                        "~/", tr("Imagenes (*.png)"));
    QPixmap measurementsImage = ui->graphicsViewChart->grab();
    measurementsImage.save(saveFileName, "PNG");
}


void MainWindow::updateSheetListSimpleDiagramFunction(int newDocIndex) {
    if(newDocIndex >= 0) {
        for(unsigned short i = 0; i < documents[newDocIndex].sheets.size(); ++i) {
            simpleDiagramFunction->updateSheetList(documents[newDocIndex].sheets[i]->sheetName);
        }
    }
}

void MainWindow::updateDaysSimpleDiagramFunction(int newSheetIndex, int currDocIndex) {
    if(newSheetIndex >= 0) {
        short numOfDays = documents[currDocIndex].sheets[newSheetIndex]->daysAndCounting.size();
        for(short i = 0; i < numOfDays; ++i) {
            this->simpleDiagramFunction->updateDays(
                        documents[currDocIndex].sheets[newSheetIndex]->daysAndCounting[i].first);
        }
    }
}

void MainWindow::updateSheetListSiteAnalysis(int newDocIndex) {
    if(newDocIndex >= 0) {
        for(unsigned short i = 0; i < documents[newDocIndex].sheets.size(); ++i) {
            siteAnalysis->updateSheetList(documents[newDocIndex].sheets[i]->sheetName);
        }
    }
}


int MainWindow::generateDiagram() {
    int selectedFunction = ui->comboBoxSelectFunction->currentIndex();
    if(selectedFunction == 0) {
        this->generateSimpleDiagram();
    } else if(selectedFunction == 1) {
        this->generateSiteAnalysisDiagram();
    } else if(selectedFunction == 2) {
        this->generateSectorWeekdayDiagram();
    }
    return 0;
}

void MainWindow::exitProgram() {
    QCoreApplication::quit();
}
