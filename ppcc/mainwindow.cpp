//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), fileManagerSelectedFile(-1) {

    // setup ui elements
    ui->setupUi(this);
    ui->textEditDisplayDocument->setReadOnly(true);
    ui->lineEditFileFreq->setReadOnly(true);
    ui->lineEditFileCat->setReadOnly(true);
    ui->lineEditFileSubCat->setReadOnly(true);
    //setupComboBoxesFileCategories();
    //updateFileSubCatComboBox(0);


    // setup widgets containing the different functions to generate diagrams
    simpleDiagramFunction = new SimpleDiagramFunction(this);
    siteAnalysis = new SiteAnalysis(this);
    sectorDayAnalysis = new SectorDayAnalysis(this);
    sectorWeekAnalysis = new SectorWeekAnalysis(this);
    ui->stackedWidgetDiagramFunctions->addWidget(simpleDiagramFunction);
    ui->stackedWidgetDiagramFunctions->addWidget(siteAnalysis);
    ui->stackedWidgetDiagramFunctions->addWidget(sectorDayAnalysis);
    ui->stackedWidgetDiagramFunctions->addWidget(sectorWeekAnalysis);
    ui->comboBoxSelectFunction->addItem("Diagrama simple");
    ui->comboBoxSelectFunction->addItem("Diagrama de sitio y día de semana");
    ui->comboBoxSelectFunction->addItem("Diagrama de sector y dia de semana");
    ui->comboBoxSelectFunction->addItem("Diagrama de sector y semana laboral");

    // set pointers and smart pointers for correct resource management
    this->measurementsChart = std::make_shared<QChart>();
    this->auxiliaryUpdateChart = std::make_shared<QChart>();
    xAxis = new QValueAxis();

    // setup xAxis: always stays the same, only label changes on each chart
    xAxis->setMin(0.0);
    xAxis->setMax(24.0);
    xAxis->setLabelFormat("%i");
    xAxis->setTickCount(25);
    xAxis->setTitleText("Hora");

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
    //connect(ui->comboBoxFileCat, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFileSubCatComboBox(int)));
    //connect(ui->comboBoxFileCat, SIGNAL(currentIndexChanged(int)), this, SLOT(setFileCategory(int)));
    //connect(ui->comboBoxFileSubCat, SIGNAL(currentIndexChanged(int)), this, SLOT(setFileSubCategory(int)));
    connect(ui->buttonRemoveDocument, SIGNAL(clicked()), this, SLOT(removeDocument()));

    // functions
    connect(ui->comboBoxSelectFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(selectFunction(int)));
    connect(simpleDiagramFunction, SIGNAL(selectedDocChanged(int)), this, SLOT(updateDaysSimpleDiagramFunction(int)));
    //connect(simpleDiagramFunction, SIGNAL(selectedSheetChanged(int, int)), this, SLOT(updateDaysSimpleDiagramFunction(int, int)));
    //connect(siteAnalysis, SIGNAL(selectedDocChanged(int)), this, SLOT(updateSheetListSiteAnalysis(int)));
    connect(ui->buttonGenerateDiagram, SIGNAL(clicked()), this, SLOT(generateDiagram()));
    connect(ui->buttonExportDiagram, SIGNAL(clicked()), this, SLOT(exportDiagram()));
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
        delete displayedSeries[i];
    }
    measurementsChart->removeAxis(xAxis);
    measurementsChart.reset();
    delete xAxis;

    // delete rest
    delete ui;
}

// function can be used later
//void MainWindow::setupComboBoxesFileCategories() {
//    ui->comboBoxFileCat->addItem("Residencial");
//    ui->comboBoxFileCat->addItem("Comercial");
//    ui->comboBoxFileCat->addItem("Industrial");
//    ui->comboBoxFileCat->addItem("Bombeo");
//    ui->comboBoxFileCat->addItem("Alumbrado público");
//}

// function can be used later
/*
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
        ui->comboBoxFileSubCat->addItem("Caucho, plástico");
        ui->comboBoxFileSubCat->addItem("Quimica");
        ui->comboBoxFileSubCat->addItem("Textil");
        ui->comboBoxFileSubCat->addItem("Otros");
    }
}
*/

void MainWindow::getFileCategories(int selectedFile) {
    if(selectedFile >= 0) {
        int sector = documents[selectedFile].docSector;
        int subCat = documents[selectedFile].getSubCategory();
        int freq = documents[selectedFile].docFreq;
        QString catString = enumerations::getStringFromSector(sector);
        QString subCatString = enumerations::getStringFromSubSector(sector, subCat);
        QString freqString = enumerations::getStringFromFreq(freq);
        if(sector == -2) {
            catString = documents[selectedFile].customSectorStr;
        }
        if(subCat == -2) {
            subCatString = documents[selectedFile].customSubSectorStr;
        }
        ui->lineEditFileCat->setText(catString);
        ui->lineEditFileSubCat->setText(subCatString);
        ui->lineEditFileFreq->setText(freqString);
        this->fileManagerSelectedFile = selectedFile;
        ui->textEditDisplayDocument->clear();
        ui->textEditDisplayDocument->setText(documents[selectedFile].documentDataAsText);
    } else {
        this->fileManagerSelectedFile = selectedFile;
        ui->lineEditFileCat->setText("");
        ui->lineEditFileSubCat->setText("");
        ui->lineEditFileFreq->setText("");
        //ui->comboBoxFileCat->setCurrentIndex(0);
    }
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
            selectCategories->setWindowTitle("Importar archivo");
            selectCategories->exec();
            delete selectCategories;
            documents.emplace_back(MeasurementsDocument(docFileNames[i]));
            documents.back().docSector = docCategories.sector;
            documents.back().docResRange = docCategories.resRange;
            documents.back().docSubCommercial = docCategories.commercial;
            documents.back().docSubIndustrial = docCategories.industrial;
            documents.back().docCustomSubSector = docCategories.customSubSec;
            documents.back().customSectorStr = docCategories.customSectorStr;
            documents.back().documentDataAsText = parseDocumentDataAsText(documents.size() - 1);

            if(documents.back().docResRange == -2) {
                documents.back().customSubSectorStr = docCategories.customResRangeStr;
                bool resRangeDoesNotExist = true;
                for(int i = 0; i < sectorDayAnalysis->customResRanges.size(); ++i) {
                    if(documents.back().customSubSectorStr == sectorDayAnalysis->customResRanges[i]) {
                        resRangeDoesNotExist = false;
                        break;
                    }
                }
                if(resRangeDoesNotExist) {
                    sectorDayAnalysis->customResRanges.append(documents.back().customSubSectorStr);
                    sectorWeekAnalysis->customResRanges.append(documents.back().customSubSectorStr);
                }
            } else if(documents.back().docSubCommercial == -2) {
                documents.back().customSubSectorStr = docCategories.customCommercialStr;
                bool commercialDoesNotExist = true;
                for(int i = 0; i < sectorDayAnalysis->customCommercials.size(); ++i) {
                    if(documents.back().customSubSectorStr == sectorDayAnalysis->customCommercials[i]) {
                        commercialDoesNotExist = false;
                        break;
                    }
                }
                if(commercialDoesNotExist) {
                    sectorDayAnalysis->customCommercials.append(documents.back().customSubSectorStr);
                    sectorWeekAnalysis->customCommercials.append(documents.back().customSubSectorStr);
                }
            } else if(documents.back().docSubIndustrial == -2) {
                documents.back().customSubSectorStr = docCategories.customIndustrialStr;
                bool industrialDoesNotExist = true;
                for(int i = 0; i < sectorDayAnalysis->customIndustrials.size(); ++i) {
                    if(documents.back().customSubSectorStr == sectorDayAnalysis->customIndustrials[i]) {
                        industrialDoesNotExist = false;
                        break;
                    }
                }
                if(industrialDoesNotExist) {
                    sectorDayAnalysis->customIndustrials.append(documents.back().customSubSectorStr);
                    sectorWeekAnalysis->customIndustrials.append(documents.back().customSubSectorStr);
                }
            } else if(documents.back().docCustomSubSector == -2) {
                documents.back().customSubSectorStr = docCategories.customSubSectorStr;
                bool subSecDoesNotExist = true;
                for(int i = 0; i < sectorDayAnalysis->customSubSectors.size(); ++i) {
                    if(documents.back().customSubSectorStr == sectorDayAnalysis->customSubSectors[i]) {
                        subSecDoesNotExist = false;
                        break;
                    }
                }
                if(subSecDoesNotExist) {
                    sectorDayAnalysis->customSubSectors.append(documents.back().customSubSectorStr);
                    sectorWeekAnalysis->customSubSectors.append(documents.back().customSubSectorStr);
                }
            }

            if(documents.back().docSector == -2) {
                bool sectorDoesNotExist = true;
                for(int i = 0; i < sectorDayAnalysis->customSectors.size(); ++i) {
                    if(documents.back().customSectorStr == sectorDayAnalysis->customSectors[i]) {
                        sectorDoesNotExist = false;
                        break;
                    }
                }
                if(sectorDoesNotExist) {
                    sectorDayAnalysis->customSectors.append(documents.back().customSectorStr);
                    sectorWeekAnalysis->customSectors.append(documents.back().customSectorStr);
                }
            }
            sectorDayAnalysis->setupComboBoxSector();
            sectorWeekAnalysis->setupComboBoxSector();

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
    } else if(functionIndex == 3) {
        ui->stackedWidgetDiagramFunctions->setCurrentWidget(sectorWeekAnalysis);
    }
}

int MainWindow::generateSimpleDiagram() {

    int returnVal = -1;

    // get inputs from the user
    std::vector<int> selectedDaysIndices = simpleDiagramFunction->getSelectedDays();
    int selectedDocIndex = simpleDiagramFunction->selectedDocIndex;
    int selectedSheetIndex = 0;
    //int selectedSheetIndex = simpleDiagramFunction->selectedSheetIndex; // not used anymore

    // update diagram only if there are valid entries
    if(selectedDaysIndices.size() > 0) {

        // while chart is being updated, auxChart is set in chartView to avoid crash
        // (ownership of measurementsChart by chartView is released)
        ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());


        // delete QLineSeries from old chart, empty series list
        for(int i = 0; i < this->displayedSeries.size(); ++i) {
            displayedSeries[i]->detachAxis(xAxis);
            delete displayedSeries[i];
        }
        displayedSeries.clear();

        // reset measurementsChart to create new clean chart
        measurementsChart->removeAxis(xAxis);
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

        // configure axes
        xAxis->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
        measurementsChart->createDefaultAxes();
        QList<QAbstractAxis*> defaultChartAxes = measurementsChart->axes();
        measurementsChart->removeAxis(defaultChartAxes[0]);
        defaultChartAxes[1]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);
        measurementsChart->addAxis(xAxis, Qt::AlignBottom);
        for(int i = 0; i < displayedSeries.size(); ++i) {
            displayedSeries[i]->attachAxis(xAxis);
        }

        // setup and display chart
        measurementsChart->legend()->setVisible(true);
        measurementsChart->legend()->setAlignment(Qt::AlignBottom);
        measurementsChart->setTitle("Consumo diario de energía");
        ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
        ui->graphicsViewChart->setChart(measurementsChart.get());
        this->displayDiagramDataAsText();
        returnVal = 0;

    } else {
        QMessageBox::information(this, tr("No se puede generar el diagrama."),
                  tr("No ha seleccionado elementos para visualizar."),
                  QMessageBox::Ok);
    }
    return returnVal;
}


int MainWindow::generateSiteAnalysisDiagram() {
    int result = -1;

    // get inputs from the user and find weekdays
    int selectedDocIndex = siteAnalysis->selectedDocIndex;
    //int selectedSheetIndex = siteAnalysis->selectedSheetIndex;
    int selectedSheetIndex = 0;                        // update 24 jan: all documents have 1 sheet
    int selectedDay = siteAnalysis->getDay() + 1;      // to compare with QDate: 1=mon,...,7=sun
    int visType = siteAnalysis->getVisType();

    if(selectedDocIndex >= 0) {
        std::vector<int> weekdayIndices = findWeekdays(selectedDay, selectedDocIndex, selectedSheetIndex);

        if(weekdayIndices.size() == 0) {
            QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                      tr("No se han encontrado datos para el día indicado."),
                      QMessageBox::Ok);
            return result;
        }

        // set to AuxChart while updating main chart
        ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());


        // delete QLineSeries from old chart, empty series list
        for(int i = 0; i < this->displayedSeries.size(); ++i) {
            displayedSeries[i]->detachAxis(xAxis);
            delete displayedSeries[i];
        }
        displayedSeries.clear();

        // reset measurementsChart to create new clean chart
        measurementsChart->removeAxis(xAxis);
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

        QString diagramTitle = "Consumo de energía días " + enumerations::getStringFromDay(selectedDay);
        if(visType == 1) {
            displayedSeries = getAverageFromSeries(displayedSeries, documents[selectedDocIndex].docFreq, false);
            diagramTitle = "Promedio del consumo diario de energía";
        } else if(visType == 2) {
            displayedSeries = getAverageFromSeries(displayedSeries, documents[selectedDocIndex].docFreq, true);
            diagramTitle = "Suma del consumo diario de energía";
        }

        for(int i = 0; i < displayedSeries.size(); ++i) {
            measurementsChart->addSeries(displayedSeries[i]);
        }

        // configure axes
        xAxis->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel);
        measurementsChart->createDefaultAxes();
        QList<QAbstractAxis*> defaultChartAxes = measurementsChart->axes();
        measurementsChart->removeAxis(defaultChartAxes[0]);
        defaultChartAxes[1]->setTitleText(documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel);
        measurementsChart->addAxis(xAxis, Qt::AlignBottom);
        for(int i = 0; i < displayedSeries.size(); ++i) {
            displayedSeries[i]->attachAxis(xAxis);
        }

        // setup and display chart
        measurementsChart->legend()->setVisible(true);
        measurementsChart->legend()->setAlignment(Qt::AlignBottom);
        measurementsChart->setTitle(diagramTitle);
        ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
        ui->graphicsViewChart->setChart(measurementsChart.get());
        this->displayDiagramDataAsText();
        result = 0;
    } else {
        QMessageBox::information(this, tr("No se puede generar el diagrama."),
                tr("No se han encontrado archivos."),
                QMessageBox::Ok);
    }
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

QList<QLineSeries*> MainWindow::getAverageFromSeries(QList<QLineSeries*> &oldLineSeries, Frequency freq, bool sum) {

    // sum == true: sum and not average is calculated

    QList<QLineSeries*> newLineSeries;
    newLineSeries.append(new QLineSeries());
    double totalNumOfSeries = oldLineSeries.size();         // >= 1 (was checked before)
    double totalNumOfValues = oldLineSeries[0]->count();
    bool dataIsRectangular = true;                          // rectangular == all series have the same num of values

    // Check, that oldLineSeries is rectangular and all dimensions >= 0
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        double currentNumOfValues = oldLineSeries[i]->count();
        if(currentNumOfValues != totalNumOfValues) {
            dataIsRectangular = false;
        }
    }

    // Calculate average and fill new line series (inverted traversion!)
    if(dataIsRectangular) {
        for(int j = 0; j < totalNumOfValues; ++j) {
            double currentSeriesSum = 0.0;
            double currentAverageValue = 0.0;
            for(int i = 0; i < totalNumOfSeries; ++i) {
                currentSeriesSum += oldLineSeries[i]->at(j).y();
            }
            if(!sum) {
                currentAverageValue = currentSeriesSum / totalNumOfSeries;
                newLineSeries[0]->append(oldLineSeries[0]->at(j).x(), currentAverageValue);
            } else {
                newLineSeries[0]->append(oldLineSeries[0]->at(j).x(), currentSeriesSum);
            }
        }
    } else {

        // setup grid
        std::vector<std::pair<double, double> > gridPointsAndCount;
        std::vector<double> gridPointsValueSum;
        double currentGridPoint = 0.0;
        if(freq == Frequency::hour) {
            for(int i = 0; i < 24; ++i, currentGridPoint += 1.0) {
                gridPointsAndCount.emplace_back(std::make_pair(currentGridPoint, 0.0));
            }
        } else {
            for(int i = 0; i < 96; ++i, currentGridPoint += 0.25) {
                gridPointsAndCount.emplace_back(std::make_pair(currentGridPoint, 0.0));
            }
        }
        gridPointsValueSum.resize(gridPointsAndCount.size(), 0.0);

        // count number of values of each grid point
        for(int i = 0; i < oldLineSeries.size(); ++i) {
            for(int j = 0; j < oldLineSeries[i]->count(); ++j) {
                for(unsigned int k = 0; k < gridPointsAndCount.size(); ++k) {
                    if(oldLineSeries[i]->at(j).x() == gridPointsAndCount[k].first) {
                        gridPointsAndCount[k].second += 1.0;
                    }
                }
            }
        }

        // debug
        //for(unsigned int i = 0; i < gridPointsAndCount.size(); ++i) {
        //    qDebug() << "(" << gridPointsAndCount[i].first << "," << gridPointsAndCount[i].second << ")";
        //}

        // Calculate average and fill new line series

        // traverse through grid
        for(unsigned int i = 0; i < gridPointsAndCount.size(); ++i) {

            // traverse through line series
            for(int j = 0; j < oldLineSeries.size(); ++j) {
                for(int k = 0; k < oldLineSeries[j]->count(); ++k) {
                    //qDebug() << "I am here, " << i << "," << j << "," << k;
                    if(oldLineSeries[j]->at(k).x() == gridPointsAndCount[i].first) {
                        gridPointsValueSum[i] += oldLineSeries[j]->at(k).y();
                    }
                }
            }
        }

        // debug print out value sum
        //for(unsigned int i = 0; i < gridPointsValueSum.size(); ++i) {
        //    qDebug() << "Point: " << gridPointsAndCount[i].first << ", value sum: " << gridPointsValueSum[i];
        //}

        // calculate average value or sum for each point and append it to new line series
        for(unsigned int i = 0; i < gridPointsValueSum.size(); ++i) {
            if(!sum) {
                if(gridPointsAndCount[i].second != 0) {                 // avoid division by 0
                    double currentAverageValue = gridPointsValueSum[i] / gridPointsAndCount[i].second;
                    newLineSeries[0]->append(gridPointsAndCount[i].first, currentAverageValue);
                }
            } else {
                newLineSeries[0]->append(gridPointsAndCount[i].first, gridPointsValueSum[i]);
            }
        }

        //QMessageBox::warning(this, tr("No se puede generar el diagrama."),
        //          tr("No se puede calcular el promedio. Cada dia\n"
        //             "debe tener la misma cantidad de mediciones."),
        //          QMessageBox::Ok);
        //return newLineSeries;
    }
    newLineSeries[0]->setName("Promedio");

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
    if((sector == -2) && (subCat == -2)) {
        QString sectorStr = sectorDayAnalysis->getSelectedSectorString();
        QString subCatStr = sectorDayAnalysis->getSelectedSubCatString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].customSectorStr == sectorStr) && (documents[i].customSubSectorStr == subCatStr)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else if(sector == -2) {
        QString sectorStr = sectorDayAnalysis->getSelectedSectorString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].customSectorStr == sectorStr) && (documents[i].getSubCategory() == subCat)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else if(subCat == -2) {
        QString subCatStr = sectorDayAnalysis->getSelectedSubCatString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].docSector == sector) && (documents[i].customSubSectorStr == subCatStr)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else {
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].docSector == sector) && (documents[i].getSubCategory() == subCat)) {
                correctFileIndices.emplace_back(i);
            }
        }
    }

    // check if all measurements have the same frequency; if not data needs to be prepared later
    bool dataNeedsTransformation = false;
    Frequency currentFreq;
    if(correctFileIndices.size() >= 1) {
        currentFreq = documents[correctFileIndices[0]].docFreq;
    } else {
        QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos para las categorías indicadas."),
                  QMessageBox::Ok);
        return result;
    }
    for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
        if(documents[correctFileIndices[i]].docFreq != currentFreq) {
            currentFreq = Frequency::quarterHour;
            dataNeedsTransformation = true;
            qDebug() << "Data needs transformation.";
            break;
        }
    }

    // Now: find the corresponding weekdays in the corresponding files
    std::vector<std::tuple<int, int, int> > weekdayIndices;    // 0: doc, 1: sheet, 2: lineSeries, 3: freq
    int currentDoc;
    int currentSheet;
    int currentLineSeries;
    //int currentFreq;
    for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
        for(unsigned int j = 0; j < documents[correctFileIndices[i]].sheets.size(); ++j) {
            currentDoc = correctFileIndices[i];
            currentSheet = j;
            //currentFreq = documents[correctFileIndices[i]].docFreq;
            std::vector<int> currSheetWeekdayIndices = findWeekdays(selectedDay, correctFileIndices[i], currentSheet);
            for(unsigned int k = 0; k < currSheetWeekdayIndices.size(); ++k) {
                currentLineSeries = currSheetWeekdayIndices[k];
                //weekdayIndices.emplace_back(std::make_tuple(currentDoc, currentSheet, currentLineSeries, currentFreq));
                weekdayIndices.emplace_back(std::make_tuple(currentDoc, currentSheet, currentLineSeries));
            }
        }
    }

    if(weekdayIndices.size() == 0) {
        QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos para el día indicado."),
                  QMessageBox::Ok);
        return result;
    }

    // debug (TODO: adapt to tuple)
    //qDebug() << "File: " << correctFileIndices[i] << ", sheet: " << j << ", LineSeries: " << currSheetWeekdayIndices[k];

    // set to AuxChart while updating main chart
    ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());

    // delete QLineSeries from old chart, empty series list
    for(int i = 0; i < this->displayedSeries.size(); ++i) {
        displayedSeries[i]->detachAxis(xAxis);
        delete displayedSeries[i];
    }
    displayedSeries.clear();

    // reset measurementsChart to create new clean chart
    measurementsChart->removeAxis(xAxis);
    measurementsChart.reset();
    this->measurementsChart = std::make_shared<QChart>();

    // copy by value corresponding series from sheets to current displayed series
    for(unsigned short i = 0; i < weekdayIndices.size(); ++i) {

        // get indices
        int currDocIndex = std::get<0>(weekdayIndices[i]);
        int currSheetIndex = std::get<1>(weekdayIndices[i]);
        int currLineSeriesIndex = std::get<2>(weekdayIndices[i]);
        displayedSeries.append(new QLineSeries());
        displayedSeries[i]->setName(
                documents[currDocIndex].sheets[currSheetIndex]->measurementSeries[currLineSeriesIndex]->name());
        QVector<QPointF> dataPoints =
            documents[currDocIndex].sheets[currSheetIndex]->measurementSeries[currLineSeriesIndex]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            displayedSeries[i]->append(dataPoints[j]);
        }
    }

    if(dataNeedsTransformation) {

        // data will always be tranformed to 15 min ticks
        displayedSeries = transformAllTo15MinTicks(displayedSeries);

        // displayedSeries won't be empty, as this point is only reached when
        // weekdayIndices is not empty
        //if(displayedSeries.size() == 0) {
        //    return result;
        //}
    }

    if(visType == 1) {
        displayedSeries = getAverageFromSeries(displayedSeries, currentFreq, false);
        //qDebug() << "Method not implemented for this case";
    } else if(visType == 2) {
        displayedSeries = getAverageFromSeries(displayedSeries, currentFreq, true);
    }

    for(int i = 0; i < displayedSeries.size(); ++i) {
        measurementsChart->addSeries(displayedSeries[i]);
    }

    // setup axes
    xAxis->setTitleText("Hora");
    measurementsChart->createDefaultAxes();
    QList<QAbstractAxis*> defaultChartAxes = measurementsChart->axes();
    measurementsChart->removeAxis(defaultChartAxes[0]);
    defaultChartAxes[1]->setTitleText("kW");
    measurementsChart->addAxis(xAxis, Qt::AlignBottom);
    for(int i = 0; i < displayedSeries.size(); ++i) {
        displayedSeries[i]->attachAxis(xAxis);
    }

    // setup and display chart
    measurementsChart->legend()->setVisible(true);
    measurementsChart->legend()->setAlignment(Qt::AlignBottom);
    measurementsChart->setTitle("Consumo de energia");
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
    this->displayDiagramDataAsText();
    result = 0;

    return result;
}

QList<QLineSeries*> MainWindow::transformAllTo15MinTicks(QList<QLineSeries*> &oldLineSeries) {
    QList<QLineSeries*> newLineSeries;

    // assumption: each lineSeries has at least 2 points
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        double x0 = oldLineSeries[i]->at(0).x();
        double x1 = oldLineSeries[i]->at(1).x();
        if((x1 - x0) == 0.25) {

            // lineSeries has already tick of 15 min, copy it to new line series list
            newLineSeries.append(new QLineSeries());
            for(int j = 0; j < oldLineSeries[i]->count(); ++j) {
                double xVal = oldLineSeries[i]->at(j).x();
                double yVal = oldLineSeries[i]->at(j).y();
                newLineSeries[i]->append(xVal, yVal);
                newLineSeries[i]->setName(oldLineSeries[i]->name());
            }
        } else {
            newLineSeries.append(new QLineSeries());
            for(int j = 1; j < oldLineSeries[i]->count(); ++j) {
                double yDelta = oldLineSeries[i]->at(j).y() - oldLineSeries[i]->at(j-1).y();
                double xValFirst = oldLineSeries[i]->at(j-1).x();
                double yValFirst = oldLineSeries[i]->at(j-1).y();
                newLineSeries[i]->append(xValFirst, yValFirst);
                newLineSeries[i]->append(xValFirst + 0.25, yValFirst + 0.25 * yDelta);
                newLineSeries[i]->append(xValFirst + 0.5, yValFirst + 0.5 * yDelta);
                newLineSeries[i]->append(xValFirst + 0.75, yValFirst + 0.75 * yDelta);
                if(j == (oldLineSeries[i]->count() - 1)) {
                    double xValLast = oldLineSeries[i]->at(j-1).x();
                    double yValLast = oldLineSeries[i]->at(j-1).y();
                    newLineSeries[i]->append(xValLast, yValLast);
                }
            }
            newLineSeries[i]->setName(oldLineSeries[i]->name());
        }
    }

    // delete old line series
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        delete oldLineSeries[i];
    }
    oldLineSeries.clear();

    return newLineSeries;
}

int MainWindow::generateSectorWeekDiagram() {
    int result = -1;

    // get categories and weekday that will be visualized
    int sector = sectorWeekAnalysis->getSelectedSector();
    int subCat = sectorWeekAnalysis->getSelectedSubCat();
    int visType = sectorWeekAnalysis->getVisType();

    // First: find measurementDocs corresponding to the selected Category and store indices
    std::vector<int> correctFileIndices;
    qDebug() << sector << ", " << subCat;
    if((sector == -2) && (subCat == -2)) {
        QString sectorStr = sectorWeekAnalysis->getSelectedSectorString();
        QString subCatStr = sectorWeekAnalysis->getSelectedSubCatString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].customSectorStr == sectorStr) && (documents[i].customSubSectorStr == subCatStr)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else if(sector == -2) {
        QString sectorStr = sectorWeekAnalysis->getSelectedSectorString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].customSectorStr == sectorStr) && (documents[i].getSubCategory() == subCat)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else if(subCat == -2) {
        QString subCatStr = sectorWeekAnalysis->getSelectedSubCatString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].docSector == sector) && (documents[i].customSubSectorStr == subCatStr)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else {
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].docSector == sector) && (documents[i].getSubCategory() == subCat)) {
                correctFileIndices.emplace_back(i);
            }
        }
    }


    // check if all measurements have the same frequency; if not data needs to be prepared later
    bool dataNeedsTransformation = false;
    Frequency currentFreq;
    if(correctFileIndices.size() >= 1) {
        currentFreq = documents[correctFileIndices[0]].docFreq;
    } else {
        QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos para las categorías indicadas."),
                  QMessageBox::Ok);
        return result;
    }
    for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
        if(documents[correctFileIndices[i]].docFreq != currentFreq) {
            currentFreq = Frequency::quarterHour;
            dataNeedsTransformation = true;
            qDebug() << "Data needs transformation.";
            break;
        }
    }

    // find the corresponding weekdays in the corresponding files
    std::vector<std::vector<std::tuple<int, int, int> > > weekdayIndices;
    weekdayIndices.resize(5);
    int currentDoc;
    int currentSheet;
    int currentLineSeries;
    //int currentFreq;
    for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
        for(unsigned int j = 0; j < documents[correctFileIndices[i]].sheets.size(); ++j) {
            currentDoc = correctFileIndices[i];
            currentSheet = j;
            //currentFreq = documents[correctFileIndices[i]].docFreq;
            for(int k = 0; k < 5; ++k) {
                int weekDay = k + 1;
                std::vector<int> currSheetWeekdayIndices = findWeekdays(weekDay, correctFileIndices[i], currentSheet);
                for(unsigned int l = 0; l < currSheetWeekdayIndices.size(); ++l) {
                    currentLineSeries = currSheetWeekdayIndices[l];
                    weekdayIndices[k].emplace_back(std::make_tuple(currentDoc, currentSheet, currentLineSeries));
                }
            }
        }
    }

    short countEmptyDays = 0;
    for(int i = 0; i < 5; ++i) {
        if(weekdayIndices[i].size() == 0) {
            ++countEmptyDays;
        }
    }
    if(countEmptyDays == 5) {
        QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos para los días indicados."),
                  QMessageBox::Ok);
        return result;
    }

    // set to AuxChart while updating main chart
    ui->graphicsViewChart->setChart(auxiliaryUpdateChart.get());

    // delete QLineSeries from old chart, empty series list
    for(int i = 0; i < this->displayedSeries.size(); ++i) {
        displayedSeries[i]->detachAxis(xAxis);
        delete displayedSeries[i];
    }
    displayedSeries.clear();

    // reset measurementsChart to create new clean chart
    measurementsChart->removeAxis(xAxis);
    measurementsChart.reset();
    this->measurementsChart = std::make_shared<QChart>();

    // setup line series for each day
    QVector<QList<QLineSeries*> > weekdaySeries;
    weekdaySeries.resize(5);

    // copy by value corresponding series from sheets to the 5 current weekday line series
    for(int i = 0; i < 5; ++i) {
        for(unsigned short j = 0; j < weekdayIndices[i].size(); ++j) {

            // get indices
            int currDocIndex = std::get<0>(weekdayIndices[i][j]);
            int currSheetIndex = std::get<1>(weekdayIndices[i][j]);
            int currLineSeriesIndex = std::get<2>(weekdayIndices[i][j]);
            weekdaySeries[i].append(new QLineSeries());
            weekdaySeries[i][j]->setName(
                    documents[currDocIndex].sheets[currSheetIndex]->measurementSeries[currLineSeriesIndex]->name());
            QVector<QPointF> dataPoints =
                documents[currDocIndex].sheets[currSheetIndex]->measurementSeries[currLineSeriesIndex]->pointsVector();
            for(int k = 0; k < dataPoints.size(); ++k) {
                weekdaySeries[i][j]->append(dataPoints[k]);
            }
        }
    }


    // calculate average of each weekday copy by value to displayedSeries
    for(int i = 0; i < 5; ++i) {
        if(weekdaySeries[i].size() > 0) {                           // segfault if size == 0 !
            QString weekdaySeriesLabel = enumerations::getStringFromDay(i+1);
            if(dataNeedsTransformation) {
                weekdaySeries[i] = transformAllTo15MinTicks(weekdaySeries[i]);
            }
            weekdaySeries[i] = getAverageFromSeries(weekdaySeries[i], currentFreq, false);
            displayedSeries.append(new QLineSeries());
            displayedSeries.last()->setName(weekdaySeriesLabel);
            QVector<QPointF> dataPoints = weekdaySeries[i][0]->pointsVector();
            for(int j = 0; j < dataPoints.size(); ++j) {
                displayedSeries.last()->append(dataPoints[j]);
            }
        }
    }

    if(visType == 1) {
        displayedSeries = getAverageFromSeries(displayedSeries, currentFreq, false);
        displayedSeries[0]->setName("Promedio días lunes a viernes");
    }

    for(int i = 0; i < displayedSeries.size(); ++i) {
        measurementsChart->addSeries(displayedSeries[i]);
    }

    // setup axes
    xAxis->setTitleText("Hora");
    measurementsChart->createDefaultAxes();
    QList<QAbstractAxis*> defaultChartAxes = measurementsChart->axes();
    measurementsChart->removeAxis(defaultChartAxes[0]);
    defaultChartAxes[1]->setTitleText("kW");
    measurementsChart->addAxis(xAxis, Qt::AlignBottom);
    for(int i = 0; i < displayedSeries.size(); ++i) {
        displayedSeries[i]->attachAxis(xAxis);
    }

    // setup and display chart
    measurementsChart->legend()->setVisible(true);
    measurementsChart->legend()->setAlignment(Qt::AlignBottom);
    measurementsChart->setTitle("Promedio del consumo de energia durante días laborales");
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
    this->displayDiagramDataAsText();
    result = 0;

    return result;
}

/* commented out for testing, use it later again (method to find same dates on selected category)

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

void MainWindow::displayDiagramDataAsText() {

    // the way data is displayed (and exported) depends on the selected function
    int selectedFunction = ui->comboBoxSelectFunction->currentIndex();
    if(selectedFunction == 0) {
        this->displaySimpleDiagramAsText();
    } else if(selectedFunction == 1) {
        //this->generateSiteAnalysisDiagram();
    } else if(selectedFunction == 2) {
        //this->generateSectorWeekdayDiagram();
    } else if(selectedFunction == 3) {
        this->displaySectorWeekDiagramAsText();
    }
}

void MainWindow::displaySimpleDiagramAsText() {
    QString diagramTitle = measurementsChart->title();
    QString seriesName = "";
    ui->textEditDisplayDiagram->clear();
    ui->textEditDisplayDiagram->append(diagramTitle + "\n");
    ui->textEditDisplayDiagram->append("Date\tTime\tkW");

    for(int i = 0; i < displayedSeries.size(); ++i) {
        seriesName = displayedSeries[i]->name();
        QString currentX;
        QString currentY;
        QString currentLine;
        QVector<QPointF> dataPoints = displayedSeries[i]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            currentX = QString::number(dataPoints[j].x());
            currentY = QString::number(dataPoints[j].y());
            currentLine = seriesName + "\t" + currentX + "\t" + currentY;
            ui->textEditDisplayDiagram->append(currentLine);
        }
    }
}

void MainWindow::displaySectorWeekDiagramAsText() {
    QString diagramTitle = measurementsChart->title();
    QString seriesName = "";
    ui->textEditDisplayDiagram->clear();
    ui->textEditDisplayDiagram->append(diagramTitle + "\n");
    ui->textEditDisplayDiagram->append("Día\tHora\tPromedio kW");

    for(int i = 0; i < displayedSeries.size(); ++i) {
        seriesName = displayedSeries[i]->name();
        QString currentX;
        QString currentY;
        QString currentLine;
        QVector<QPointF> dataPoints = displayedSeries[i]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            currentX = QString::number(dataPoints[j].x());
            currentY = QString::number(dataPoints[j].y());
            currentLine = seriesName + "\t" + currentX + "\t" + currentY;
            ui->textEditDisplayDiagram->append(currentLine);
        }
    }

}

QString MainWindow::parseDocumentDataAsText(int selectedFile) {
    QString documentDataAsText;
    if(selectedFile >= 0) {
        QString documentName = documents[selectedFile].docName;
        documentDataAsText.append(documentName + "\n\n");

        for(unsigned int i = 0; i < documents[selectedFile].sheets.size(); ++i) {
            QString dateLabel = documents[selectedFile].sheets[i]->dateLabel;
            QString xAxisLabel = documents[selectedFile].sheets[i]->xAxisLabel;
            QString yAxisLabel = documents[selectedFile].sheets[i]->yAxisLabel;
            documentDataAsText.append(dateLabel + "\t" + xAxisLabel + "\t" + yAxisLabel + "\n");
            for(unsigned int j = 0; j < documents[selectedFile].sheets[i]->allDays.size(); ++j) {
                QString currentDay = documents[selectedFile].sheets[i]->allDays[j].toString();
                QString currentTime = documents[selectedFile].sheets[i]->timestamps[j].toString();
                QString currentValue = documents[selectedFile].sheets[i]->measurements[j].toString().left(8);
                documentDataAsText.append(currentDay + "\t" + currentTime + "\t" + currentValue + "\n");
            }
            documentDataAsText.append("\n");
        }
    }
    return documentDataAsText;
}

void MainWindow::saveDiagram() {
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Guardar imagen"),
                                                        "~/", tr("Imagenes (*.png)"));
    QPixmap measurementsImage = ui->graphicsViewChart->grab();
    measurementsImage.save(saveFileName, "PNG");
}

void MainWindow::exportDiagram() {
    short exportMethod = -1;
    ExportDiagramDialog* exportDiagramDialog = new ExportDiagramDialog(this, exportMethod);
    exportDiagramDialog->setWindowTitle("Exportar diagrama");
    exportDiagramDialog->exec();
    delete exportDiagramDialog;
    if(exportMethod == 0) {
        QString saveFileName = QFileDialog::getSaveFileName(this, tr("Guardar imagen"),
                                                            "~/", tr("Imagenes (*.png)"));
        QPixmap measurementsImage = ui->graphicsViewChart->grab();
        measurementsImage.save(saveFileName, "PNG");
    } else if(exportMethod == 1) {
        QString saveFileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo Excel"),
                                                            "~/", tr("Excel (*.xlsx)"));
        this->saveAsExcel(saveFileName);
    } else if(exportMethod == 2) {
        QString saveFileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo csv"),
                                                            "~/", tr("CSV (*.csv)"));
        this->saveAsCSV(saveFileName);
    }
}

void MainWindow::saveAsExcel(QString &saveFileName) {
    QXlsx::Document docToSave;
    QString dataText = ui->textEditDisplayDiagram->toPlainText();
    QStringList dataLines = dataText.split("\n");
    for(int i = 0; i < dataLines.size(); ++i) {
        QString currentDataLine = dataLines[i];
        QStringList dataCells = currentDataLine.split(",");
        for(int j = 0; j < dataCells.size(); ++j) {
            docToSave.write(i+1, j+1, dataCells[j]);
        }
    }
    docToSave.saveAs(saveFileName);
}

void MainWindow::saveAsCSV(QString &saveFileName) {
    QFile docToSave(saveFileName);
    if(docToSave.open(QIODevice::WriteOnly)) {
        QTextStream dataStream(&docToSave);
        QString dataText = ui->textEditDisplayDiagram->toPlainText();
        QStringList dataLines = dataText.split("\n");
        for(int i = 0; i < dataLines.size(); ++i) {
            dataStream << dataLines[i] << Qt::endl;
        }
        docToSave.close();
    }
}


//void MainWindow::updateSheetListSimpleDiagramFunction(int newDocIndex) {
//    if(newDocIndex >= 0) {
//        for(unsigned short i = 0; i < documents[newDocIndex].sheets.size(); ++i) {
//            simpleDiagramFunction->updateSheetList(documents[newDocIndex].sheets[i]->sheetName);
//        }
//    }
//}

void MainWindow::updateDaysSimpleDiagramFunction(int newDocIndex) {
    if(newDocIndex >= 0) {
        short numOfDays = documents[newDocIndex].sheets[0]->daysAndCounting.size();
        for(short i = 0; i < numOfDays; ++i) {
            this->simpleDiagramFunction->updateDays(
                        documents[newDocIndex].sheets[0]->daysAndCounting[i].first);
        }
    }
}

//void MainWindow::updateSheetListSiteAnalysis(int newDocIndex) {
//    if(newDocIndex >= 0) {
//        for(unsigned short i = 0; i < documents[newDocIndex].sheets.size(); ++i) {
//            siteAnalysis->updateSheetList(documents[newDocIndex].sheets[i]->sheetName);
//        }
//    }
//}

void MainWindow::removeDocument() {
    int docToRemove = ui->listWidgetDocuments->currentRow();
    if(docToRemove >= 0) {
        unsigned int oldDocumentsCount = documents.size();
        for(unsigned int i = docToRemove; i < (oldDocumentsCount - 1); ++i) {
            documents[i] = documents[i+1];
        }
        documents.pop_back();
        QListWidgetItem* itemToRemove = ui->listWidgetDocuments->takeItem(docToRemove);
        simpleDiagramFunction->removeDocument(docToRemove);
        siteAnalysis->removeDocument(docToRemove);
        ui->textEditDisplayDocument->clear();
        delete itemToRemove;
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
    } else if(selectedFunction == 3) {
        this->generateSectorWeekDiagram();
    }
    return 0;
}

void MainWindow::exitProgram() {
    QCoreApplication::quit();
}
