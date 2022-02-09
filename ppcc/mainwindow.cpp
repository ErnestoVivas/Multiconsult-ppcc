//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow),  currentDiagramType(-1),
        fileManagerSelectedFile(-1), importCanceled(false) {

    // setup ui elements
    ui->setupUi(this);
    ui->textEditDisplayDocument->setReadOnly(true);
    ui->lineEditFileFreq->setReadOnly(true);
    ui->lineEditFileCat->setReadOnly(true);
    ui->lineEditFileSubCat->setReadOnly(true);
    ui->spinBoxYMin->setRange(0, 999999);
    ui->spinBoxYMax->setRange(0, 999999);
    ui->buttonChangeCategories->setDisabled(true);

    // setup widgets containing the different functions to generate diagrams
    simpleDiagramFunction = new SimpleDiagramFunction(this);
    siteAnalysis = new SiteAnalysis(this);
    sectorDayAnalysis = new SectorDayAnalysis(this);
    sectorWeekAnalysis = new SectorWeekAnalysis(this);
    sectorSubCatsAnalysis = new SectorSubCatsAnalysis(this);
    ui->stackedWidgetDiagramFunctions->addWidget(simpleDiagramFunction);
    ui->stackedWidgetDiagramFunctions->addWidget(siteAnalysis);
    ui->stackedWidgetDiagramFunctions->addWidget(sectorDayAnalysis);
    ui->stackedWidgetDiagramFunctions->addWidget(sectorWeekAnalysis);
    ui->stackedWidgetDiagramFunctions->addWidget(sectorSubCatsAnalysis);
    ui->comboBoxSelectFunction->addItem("CC de fechas específicas de un sitio");
    ui->comboBoxSelectFunction->addItem("CC de días de semana por sitio");
    ui->comboBoxSelectFunction->addItem("CC de días de semana por categoría");
    ui->comboBoxSelectFunction->addItem("CC de semana laboral por categoría");
    ui->comboBoxSelectFunction->addItem("CC de subcategorías de sector");

    // setup categories tracking
    categoriesTracking.initCategories();
    categoriesTracking.initComboBox(sectorDayAnalysis->getSectorComboBox());
    categoriesTracking.initComboBox(sectorDayAnalysis->getSubCatComboBox());
    categoriesTracking.initComboBox(sectorWeekAnalysis->getSectorComboBox());
    categoriesTracking.initComboBox(sectorWeekAnalysis->getSubCatComboBox());
    categoriesTracking.initComboBox(sectorSubCatsAnalysis->getSectorComboBox());

    // set pointers and smart pointers for correct resource management
    this->measurementsChart = std::make_shared<QChart>();
    this->auxiliaryUpdateChart = std::make_shared<QChart>();
    xAxis = new QValueAxis();
    yAxis = new QValueAxis();

    // setup axes: x axis always stays the same, only label changes on each chart
    xAxis->setMin(0.0);
    xAxis->setMax(24.0);
    xAxis->setLabelFormat("%i");
    xAxis->setTickCount(25);
    xAxis->setTitleText("Hora");

    // set auxChart parameters, display auxChart at startup
    this->auxiliaryUpdateChart->legend()->hide();
    this->auxiliaryUpdateChart->createDefaultAxes();



    // connect interactive elements with respective functions

    // menu
    connect(ui->menuImportDocuments, SIGNAL(triggered()), this, SLOT(importDocument()));
    connect(ui->menuImportDataBase, SIGNAL(triggered()), this, SLOT(importDataBase()));
    connect(ui->menuImportTotalNatDoc, SIGNAL(triggered()), this, SLOT(importTotalNatDoc()));
    connect(ui->menuQuit, SIGNAL(triggered()), this, SLOT(exitProgram()));
    connect(ui->menuRemoveFile, SIGNAL(triggered()), this, SLOT(removeDocument()));
    connect(ui->menuRemoveAllFiles, SIGNAL(triggered()), this, SLOT(removeAllDocuments()));
    connect(ui->menuSelectFunction, SIGNAL(triggered()), this, SLOT(selectFunction()));
    connect(ui->menuExport, SIGNAL(triggered()), this, SLOT(exportDiagram()));
    connect(ui->menuConfigDiagram, SIGNAL(triggered()), this, SLOT(configDiagram()));
    connect(ui->menuGenerateDiagram, SIGNAL(triggered()), this, SLOT(generateDiagram()));
    connect(ui->menuAboutSpanish, SIGNAL(triggered()), this, SLOT(openReadmeSpanish()));
    connect(ui->menuAboutEnglish, SIGNAL(triggered()), this, SLOT(openReadmeEnglish()));
    connect(ui->menuSave, SIGNAL(triggered()), this, SLOT(saveDataBase()));

    // file management
    connect(ui->buttonImportDocuments, SIGNAL(clicked()), this, SLOT(selectImportMethod()));
    connect(ui->listWidgetDocuments, SIGNAL(currentRowChanged(int)), this, SLOT(getFileCategories(int)));
    connect(ui->buttonRemoveDocument, SIGNAL(clicked()), this, SLOT(removeDocument()));
    connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveDataBase()));

    // functions for diagram generation
    connect(ui->comboBoxSelectFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(selectFunction(int)));
    connect(simpleDiagramFunction, SIGNAL(selectedDocChanged(int)), this, SLOT(updateDaysSimpleDiagramFunction(int)));
    connect(ui->buttonGenerateDiagram, SIGNAL(clicked()), this, SLOT(generateDiagram()));
    connect(ui->buttonExportDiagram, SIGNAL(clicked()), this, SLOT(exportDiagram()));
    connect(sectorDayAnalysis, SIGNAL(requestSubCatsUpdate(int, QComboBox*)), this, SLOT(updateSubCatComboBox(int, QComboBox*)));
    connect(sectorWeekAnalysis, SIGNAL(requestSubCatsUpdate(int, QComboBox*)), this, SLOT(updateSubCatComboBox(int, QComboBox*)));

    // diagram configuration
    connect(ui->buttonRefreshDiagram, SIGNAL(clicked()), this, SLOT(refreshDiagram()));
    connect(ui->buttonAdvancedDiagramConfiguration, SIGNAL(clicked()), this, SLOT(configDiagram()));
}

MainWindow::~MainWindow() {

    // remove diagram function widgets from stacked widget container and delete them
    ui->stackedWidgetDiagramFunctions->removeWidget(sectorSubCatsAnalysis);
    ui->stackedWidgetDiagramFunctions->removeWidget(sectorWeekAnalysis);
    ui->stackedWidgetDiagramFunctions->removeWidget(sectorDayAnalysis);
    ui->stackedWidgetDiagramFunctions->removeWidget(siteAnalysis);
    ui->stackedWidgetDiagramFunctions->removeWidget(simpleDiagramFunction);
    delete sectorSubCatsAnalysis;
    delete sectorWeekAnalysis;
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
    measurementsChart->removeAxis(xAxis);
    measurementsChart->removeAxis(yAxis);
    measurementsChart.reset();
    delete yAxis;
    delete xAxis;

    // delete rest
    delete ui;
}


void MainWindow::updateSubCatComboBox(int sectorIndex, QComboBox* subCatComboBox) {
    this->categoriesTracking.updateSubCatComboBox(sectorIndex, subCatComboBox);
}

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

void MainWindow::openReadmeSpanish() {
    QDir currentWorkingDirectory;
    QString readmeFilePath = currentWorkingDirectory.absolutePath() + "/LEEME.md";
    QDesktopServices::openUrl(QUrl("file:///" + readmeFilePath));
}

void MainWindow::openReadmeEnglish() {
    QDir currentWorkingDirectory;
    QString readmeFilePath = currentWorkingDirectory.absolutePath() + "/README.md";
    QDesktopServices::openUrl(QUrl("file:///" + readmeFilePath));
}

void MainWindow::cancelImport() {
    this->importCanceled = true;
}

void MainWindow::selectImportMethod() {
    int importMethod = -1;
    SelectImportMethod* selectImportMethod = new SelectImportMethod(this, importMethod);
    selectImportMethod->setWindowTitle("Importar Mediciones");
    selectImportMethod->exec();
    delete selectImportMethod;
    if(importMethod == 0) {
        this->importDocument();
    } else if(importMethod == 1) {
        this->importDataBase();
    } else if(importMethod == 2) {
        this->importTotalNatDoc();
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
            connect(selectCategories, SIGNAL(importProcessCanceled()), this, SLOT(cancelImport()));
            selectCategories->setWindowTitle("Importar archivo");
            selectCategories->exec();
            delete selectCategories;
            if(this->importCanceled) {
                this->importCanceled = false;
                return;
            }
            documents.emplace_back(MeasurementsDocument(docFileNames[i]));
            documents.back().docSector = docCategories.sector;
            documents.back().docResRange = docCategories.resRange;
            documents.back().docSubCommercial = docCategories.commercial;
            documents.back().docSubIndustrial = docCategories.industrial;
            documents.back().docCustomSubSector = docCategories.customSubSec;
            documents.back().customSectorStr = docCategories.customSectorStr;
            documents.back().documentDataAsText = parseDocumentDataAsText(documents.size() - 1);
            this->configDocumentCategories(documents.back(), docCategories);

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

void MainWindow::importDataBase() {
    QString dataBaseFileName = QFileDialog::getOpenFileName(this, tr("Importar base de datos"),
                                                            "/home", tr("Archivos de configuración (*.txt)"));
    if(!dataBaseFileName.isEmpty()) {
        QFile dataBaseConfigFile(dataBaseFileName);
        if(dataBaseConfigFile.open(QIODevice::ReadOnly)) {
            QTextStream dataBaseIn(&dataBaseConfigFile);
            QString firstLine = dataBaseIn.readLine();
            QString currentDocFileName;
            if((firstLine == "REFS") || (firstLine == "FILES")) {
                QString currentLine;
                while(!dataBaseIn.atEnd()) {
                    currentLine = dataBaseIn.readLine();
                    QStringList newDocParameters = currentLine.split(",");
                    if(newDocParameters.size() != 8) {
                        continue;
                    }
                    Categories newDocCategories;
                    if(firstLine == "REFS") {
                        documents.emplace_back(MeasurementsDocument(newDocParameters[0]));
                        currentDocFileName = newDocParameters[0].section("/",-1,-1);
                    } else {
                        int lastDirIndex = dataBaseFileName.lastIndexOf("/");
                        QString dataBaseFileDir = dataBaseFileName.left(lastDirIndex + 1);
                        QString newDocFileName = dataBaseFileDir + newDocParameters[0];
                        documents.emplace_back(MeasurementsDocument(newDocFileName));
                        currentDocFileName = newDocParameters[0];
                    }
                    documents.back().docSector = enumerations::getSectorFromInt(newDocParameters[1].toInt());
                    newDocCategories.sector = enumerations::getSectorFromInt(newDocParameters[1].toInt());
                    documents.back().docResRange = enumerations::getResRangeFromInt(newDocParameters[2].toInt());
                    newDocCategories.resRange = enumerations::getResRangeFromInt(newDocParameters[2].toInt());
                    documents.back().docSubCommercial = enumerations::getCommercialFromInt(newDocParameters[3].toInt());
                    newDocCategories.commercial = enumerations::getCommercialFromInt(newDocParameters[3].toInt());
                    documents.back().docSubIndustrial = enumerations::getIndustrialFromInt(newDocParameters[4].toInt());
                    newDocCategories.industrial = enumerations::getIndustrialFromInt(newDocParameters[4].toInt());
                    documents.back().docCustomSubSector = enumerations::getCustomSubSecFromInt(newDocParameters[5].toInt());
                    newDocCategories.customSubSec = enumerations::getCustomSubSecFromInt(newDocParameters[5].toInt());
                    documents.back().customSectorStr = newDocParameters[6];
                    newDocCategories.customSectorStr = newDocParameters[6];
                    newDocCategories.customResRangeStr = newDocParameters[7];
                    newDocCategories.customCommercialStr = newDocParameters[7];
                    newDocCategories.customIndustrialStr = newDocParameters[7];
                    newDocCategories.customSubSectorStr = newDocParameters[7];
                    documents.back().documentDataAsText = parseDocumentDataAsText(documents.size() - 1);
                    this->configDocumentCategories(documents.back(), newDocCategories);
                    simpleDiagramFunction->addEntryComboBoxSelectDoc(currentDocFileName);
                    siteAnalysis->addEntryComboBoxSelectDoc(currentDocFileName);
                    ui->listWidgetDocuments->addItem(currentDocFileName);
                    if(ui->listWidgetDocuments->count() == 1) {
                        ui->listWidgetDocuments->setCurrentRow(0);
                    }
                }
            } else {
                QMessageBox::critical(this, tr("Importar base de datos"),
                                      tr("No se ha podido importar la base de datos.\n"
                                         "El archivo de configuración no cumple con el formato requerido."));
            }
        } else {
            QMessageBox::warning(this, tr("Importar base de datos"),
                                 tr("No se ha podido importar la base de datos.\nVerifique el nombre del archivo"));
        }
    }
}

void MainWindow::importTotalNatDoc() {
    QString totalNatDocFileName = QFileDialog::getOpenFileName(this,
            tr("Abrir archivo Excel"), "/home", tr("Formato Excel (*.xlsx)"));
    if(!totalNatDocFileName.isEmpty()) {
        docTotalNationalLoad = MeasurementsDocument(totalNatDocFileName);
        QString fileNameOnly = totalNatDocFileName.section("/",-1,-1);
        const QString message = "El archivo " + fileNameOnly + " ha sido importado con éxito.";
        QMessageBox::information(this, tr("Importar curva de carga nacional"), message,
                                 QMessageBox::Ok);
    }
}

void MainWindow::saveDataBase() {

    if(this->documents.size() == 0) {
        QMessageBox::warning(this, tr("Guardar base de datos"),
                             tr("No se han importado archivos para guardar la base de datos."));
        return;
    }

    QStringList saveMethods;
    saveMethods << tr("Guardar referencias") << tr("Guardar archivos");
    bool positiveAnswer;
    QString saveMethod = QInputDialog::getItem(this, tr("Guardar base de datos"), tr("Seleccione Método"),
                                               saveMethods, 0, false, &positiveAnswer);

    if(positiveAnswer /*&& (saveMethod == "Guardar referencias")*/) {
        QString saveFileName = QFileDialog::getSaveFileName(this, tr("Guardar base de datos"),
                                                            "~/", tr("Archivo de configuración (*.txt)"));
        QFile dataBaseReferences(saveFileName);
        if(dataBaseReferences.open(QIODevice::WriteOnly)) {
            QTextStream dataStream(&dataBaseReferences);
            if(saveMethod == "Guardar referencias") {
                dataStream << "REFS\r\n";               // Win style, change on UNIX systems!
            } else {
                dataStream << "FILES\r\n";              // Win style, change on UNIX systems!
            }
            for(unsigned int i = 0; i < this->documents.size(); ++i) {
                if(saveMethod == "Guardar referencias") {
                    dataStream << this->documents[i].docName << ",";
                } else {
                    QString currentDocFileNameOnly = this->documents[i].docName.section("/",-1,-1);
                    int lastDirIndex = saveFileName.lastIndexOf("/");
                    QString destinationDirectory = saveFileName.left(lastDirIndex + 1);
                    QString destinationFile = destinationDirectory + currentDocFileNameOnly;
                    QFile::copy(this->documents[i].docName, destinationFile);
                    dataStream << currentDocFileNameOnly << ",";
                }
                dataStream << this->documents[i].docSector << ",";
                dataStream << this->documents[i].docResRange << ",";
                dataStream << this->documents[i].docSubCommercial << ",";
                dataStream << this->documents[i].docSubIndustrial << ",";
                dataStream << this->documents[i].docCustomSubSector << ",";
                dataStream << this->documents[i].customSectorStr << ",";
                dataStream << this->documents[i].customSubSectorStr << "\r\n";   // Win style, change on UNIX systems!
            }
            dataBaseReferences.close();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Ha ocurrido un error.\nNo se ha podido guardar la base de datos."));
        }
    }
}


void MainWindow::configDocumentCategories(MeasurementsDocument &newDoc, Categories &docCategories) {

    // function configures categories of a new doc, adds them to categoriesTracking and updates the corresponding comboboxes

    // standard categories
    categoriesTracking.updateStandardCategories(docCategories);

    // custom residential subcategories
    if(newDoc.docResRange == -2) {
        newDoc.customSubSectorStr = docCategories.customResRangeStr;
        bool resRangeDoesNotExist = true;
        for(unsigned int i = 0; i < categoriesTracking.customResRangesAndCount.size(); ++i) {
            if(newDoc.customSubSectorStr == categoriesTracking.customResRangesAndCount[i].first) {
                ++categoriesTracking.customResRangesAndCount[i].second;
                resRangeDoesNotExist = false;
            }
        }
        if(resRangeDoesNotExist) {
            categoriesTracking.customResRangesAndCount.emplace_back(std::make_pair(newDoc.customSubSectorStr, 1));
        }
    }

    // custom commercial subcategories
    else if(newDoc.docSubCommercial == -2) {
        newDoc.customSubSectorStr = docCategories.customCommercialStr;
        bool commercialDoesNotExist = true;
        for(unsigned int i = 0; i < categoriesTracking.customCommercialsAndCount.size(); ++i) {
            if(newDoc.customSubSectorStr == categoriesTracking.customCommercialsAndCount[i].first) {
                ++categoriesTracking.customCommercialsAndCount[i].second;
                commercialDoesNotExist = false;
            }
        }
        if(commercialDoesNotExist) {
            categoriesTracking.customCommercialsAndCount.emplace_back(std::make_pair(newDoc.customSubSectorStr, 1));
        }
    }

    // custom industrial subcategories
    else if(newDoc.docSubIndustrial == -2) {
        newDoc.customSubSectorStr = docCategories.customIndustrialStr;
        bool industrialDoesNotExist = true;
        for(unsigned int i = 0; i < categoriesTracking.customIndustrialsAndCount.size(); ++i) {
            if(newDoc.customSubSectorStr == categoriesTracking.customIndustrialsAndCount[i].first) {
                ++categoriesTracking.customIndustrialsAndCount[i].second;
                industrialDoesNotExist = false;
            }
        }
        if(industrialDoesNotExist) {
            categoriesTracking.customIndustrialsAndCount.emplace_back(std::make_pair(newDoc.customSubSectorStr, 1));
        }
    }

    // custom subcategories of a custom sector
    else if(newDoc.docCustomSubSector == -2) {
        newDoc.customSubSectorStr = docCategories.customSubSectorStr;
        bool subSecDoesNotExist = true;
        for(unsigned int i = 0; i < categoriesTracking.customSubCatsAndCount.size(); ++i) {
            if(newDoc.customSubSectorStr == categoriesTracking.customSubCatsAndCount[i].first) {
                ++categoriesTracking.customSubCatsAndCount[i].second;
                subSecDoesNotExist = false;
            }
        }
        if(subSecDoesNotExist) {
            categoriesTracking.customSubCatsAndCount.emplace_back(std::make_pair(newDoc.customSubSectorStr, 1));
        }
    }

    // custom sector
    if(newDoc.docSector == -2) {
        bool sectorDoesNotExist = true;
        for(unsigned int i = 0; i < categoriesTracking.customSectorsAndCount.size(); ++i) {
            if(newDoc.customSectorStr == categoriesTracking.customSectorsAndCount[i].first) {
                ++categoriesTracking.customSectorsAndCount[i].second;
                sectorDoesNotExist = false;
            }
        }
        if(sectorDoesNotExist) {
            categoriesTracking.customSectorsAndCount.emplace_back(std::make_pair(newDoc.customSectorStr, 1));
        }
    }

    // update sector combo boxes, subCat comboboxes are updated automatically as signals are sent by sector combo boxes
    categoriesTracking.updateSectorComboBox(sectorDayAnalysis->getSectorComboBox());
    categoriesTracking.updateSectorComboBox(sectorWeekAnalysis->getSectorComboBox());
    categoriesTracking.updateSectorComboBox(sectorSubCatsAnalysis->getSectorComboBox());

    // debug
    //categoriesTracking.printCategories();
}

void MainWindow::selectFunction() {
    SelectFunction* selectFunctionDialog = new SelectFunction();
    selectFunctionDialog->setWindowTitle("Elegir función");
    selectFunctionDialog->exec();
    delete selectFunctionDialog;
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
    } else if(functionIndex == 4) {
        ui->stackedWidgetDiagramFunctions->setCurrentWidget(sectorSubCatsAnalysis);
    }
}

void MainWindow::resetMeasurementsChart() {

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
    measurementsChart->removeAxis(xAxis);
    measurementsChart->removeAxis(yAxis);
    measurementsChart.reset();
    this->measurementsChart = std::make_shared<QChart>();
}

void MainWindow::configureChartAxes(QString &xAxisTitle, QString &yAxisTitle, double &yMin, double &yMax) {

    xAxis->setTitleText(xAxisTitle);
    yAxis->setTitleText(yAxisTitle);
    yAxis->setMin(yMin);
    yAxis->setMax(yMax);
    measurementsChart->addAxis(xAxis, Qt::AlignBottom);
    measurementsChart->addAxis(yAxis, Qt::AlignLeft);
    for(int i = 0; i < displayedSeries.size(); ++i) {
        displayedSeries[i]->attachAxis(xAxis);
        displayedSeries[i]->attachAxis(yAxis);
    }
    yAxis->applyNiceNumbers();
    ui->spinBoxYMin->setValue(yAxis->min());
    ui->spinBoxYMax->setValue(yAxis->max());
}

void MainWindow::showPointOnHover(const QPointF& point, bool state) {
    if(state) {
       //pointLabel.setText(QString::asprintf("%1.0f%", point.y()));
       //QPoint curPos = mapFromGlobal(QCursor::pos());
       //pointLabel.move(curPos.x()-pointLabel.width()/2, curPos.y()-pointLabel.height()*1.5);
       //pointLabel.show();

       qDebug() << point.x() << ", " << point.y();
    } else {
        //pointLabel.hide();
    }
}

void MainWindow::configSeriesForHover() {
    for(int i = 0; i < displayedSeries.size(); ++i) {
        connect(displayedSeries[i], SIGNAL(hovered(const QPointF&, bool)), this, SLOT(showPointOnHover(const QPointF&, bool)));
    }
}

int MainWindow::generateSimpleDiagram() {

    int returnVal = -1;

    // get inputs from the user
    std::vector<int> selectedDaysIndices = simpleDiagramFunction->getSelectedDays();
    int selectedDocIndex = simpleDiagramFunction->selectedDocIndex;
    int selectedSheetIndex = 0;                                             // only one sheet is used (update jan 22)

    // update diagram only if there are valid entries
    if(selectedDaysIndices.size() > 0) {

        this->resetMeasurementsChart();

        // copy by value series from sheets to current displayed series, find yMin and yMax
        double yMin = 0.0;
        double yMax = 0.0;
        for(unsigned short i = 0; i < selectedDaysIndices.size(); ++i) {
            displayedSeries.append(new QLineSeries());
            displayedSeries[i]->setName(
                    documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[selectedDaysIndices[i]]->name());
            QVector<QPointF> dataPoints =
                documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[selectedDaysIndices[i]]->pointsVector();
            for(int j = 0; j < dataPoints.size(); ++j) {
                if((i == 0) && (j == 0)) {
                    yMin = dataPoints[j].y();
                    yMax = dataPoints[j].y();
                }
                if(yMin > dataPoints[j].y()) {
                    yMin = dataPoints[j].y();
                }
                if(yMax < dataPoints[j].y()) {
                    yMax = dataPoints[j].y();
                }
                displayedSeries[i]->append(dataPoints[j]);
            }
            measurementsChart->addSeries(displayedSeries[i]);
        }

        // configure axes
        QString xAxisTitle = documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel;
        QString yAxisTitle = documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel;
        this->configureChartAxes(xAxisTitle, yAxisTitle, yMin, yMax);

        // setup and display chart
        this->configSeriesForHover();
        QString fileNameOnly = documents[selectedDocIndex].docName.section("/",-1,-1);
        measurementsChart->legend()->setVisible(true);
        measurementsChart->legend()->setAlignment(Qt::AlignBottom);
        measurementsChart->setTitle(fileNameOnly);
        ui->lineEditDiagramTitle->setText(measurementsChart->title());
        ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
        ui->graphicsViewChart->setChart(measurementsChart.get());
        this->displayDiagramDataAsText();
        this->currentDiagramType = 0;
        this->currentDayType = fileNameOnly;
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

        this->resetMeasurementsChart();

        // copy by value corresponding series from sheets to current displayed series
        double yMin = 0.0;
        double yMax = 0.0;
        for(unsigned short i = 0; i < weekdayIndices.size(); ++i) {
            displayedSeries.append(new QLineSeries());
            displayedSeries[i]->setName(
                    documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[weekdayIndices[i]]->name());
            QVector<QPointF> dataPoints =
                documents[selectedDocIndex].sheets[selectedSheetIndex]->measurementSeries[weekdayIndices[i]]->pointsVector();
            for(int j = 0; j < dataPoints.size(); ++j) {
                if((i == 0) && (j == 0)) {
                    yMin = dataPoints[j].y();
                    yMax = dataPoints[j].y();
                }
                if(yMin > dataPoints[j].y()) {
                    yMin = dataPoints[j].y();
                }
                if(yMax < dataPoints[j].y()) {
                    yMax = dataPoints[j].y();
                }
                displayedSeries[i]->append(dataPoints[j]);
            }
        }

        QString diagramTitle = "Curvas de carga, días " + enumerations::getStringFromDay(selectedDay);
        if(visType == 1) {
            displayedSeries = getAverageFromSeries(displayedSeries, documents[selectedDocIndex].docFreq, false);
            diagramTitle = "Curva de carga promedio, días " + enumerations::getStringFromDay(selectedDay);
            displayedSeries[0]->setName(enumerations::getStringFromDay(selectedDay));
        } else if(visType == 2) {
            displayedSeries = getAverageFromSeries(displayedSeries, documents[selectedDocIndex].docFreq, true);
            diagramTitle = "Suma de las curvas de carga, días " + enumerations::getStringFromDay(selectedDay);
            displayedSeries[0]->setName(enumerations::getStringFromDay(selectedDay));
        }

        for(int i = 0; i < displayedSeries.size(); ++i) {
            measurementsChart->addSeries(displayedSeries[i]);
        }

        // configure axes
        QString xAxisTitle = documents[selectedDocIndex].sheets[selectedSheetIndex]->xAxisLabel;
        QString yAxisTitle = documents[selectedDocIndex].sheets[selectedSheetIndex]->yAxisLabel;
        this->configureChartAxes(xAxisTitle, yAxisTitle, yMin, yMax);

        // setup and display chart
        measurementsChart->legend()->setVisible(true);
        measurementsChart->legend()->setAlignment(Qt::AlignBottom);
        measurementsChart->setTitle(diagramTitle);
        ui->lineEditDiagramTitle->setText(measurementsChart->title());
        ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
        ui->graphicsViewChart->setChart(measurementsChart.get());
        this->displayDiagramDataAsText();
        this->currentDiagramType = 1;
        this->currentSelectedCategory = documents[selectedDocIndex].docName.section("/",-1,-1);
        this->currentDayType = enumerations::getStringFromDay(selectedDay);
        this->currentVisType = visType;
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
    QString sectorStr;
    QString subCatStr;

    // First: find measurementDocs corresponding to the selected Category and store indices
    std::vector<int> correctFileIndices;
    if((sector == -2) && (subCat == -2)) {
        sectorStr = sectorDayAnalysis->getSelectedSectorString();
        subCatStr = sectorDayAnalysis->getSelectedSubCatString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].customSectorStr == sectorStr) && (documents[i].customSubSectorStr == subCatStr)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else if(sector == -2) {
        sectorStr = sectorDayAnalysis->getSelectedSectorString();
        subCatStr = enumerations::getStringFromSubSector(sector, subCat);
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].customSectorStr == sectorStr) && (documents[i].getSubCategory() == subCat)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else if(subCat == -2) {
        sectorStr = enumerations::getStringFromSector(sector);
        subCatStr = sectorDayAnalysis->getSelectedSubCatString();
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].docSector == sector) && (documents[i].customSubSectorStr == subCatStr)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else {
        sectorStr = enumerations::getStringFromSector(sector);
        subCatStr = enumerations::getStringFromSubSector(sector, subCat);
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
    for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
        for(unsigned int j = 0; j < documents[correctFileIndices[i]].sheets.size(); ++j) {
            currentDoc = correctFileIndices[i];
            currentSheet = j;
            std::vector<int> currSheetWeekdayIndices = findWeekdays(selectedDay, correctFileIndices[i], currentSheet);
            for(unsigned int k = 0; k < currSheetWeekdayIndices.size(); ++k) {
                currentLineSeries = currSheetWeekdayIndices[k];
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

    this->resetMeasurementsChart();

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
    }

    QString diagramTitle = "Curvas de carga del día " + enumerations::getStringFromDay(selectedDay)
                           + ", " + sectorStr + ", " + subCatStr;
    if(visType == 1) {
        displayedSeries = getAverageFromSeries(displayedSeries, currentFreq, false);
        diagramTitle = "Curva de carga promedio, días " + enumerations::getStringFromDay(selectedDay)
                       + ", " + sectorStr + ", " + subCatStr;
        displayedSeries[0]->setName(enumerations::getStringFromDay(selectedDay));
    } else if(visType == 2) {
        displayedSeries = getAverageFromSeries(displayedSeries, currentFreq, true);
        diagramTitle = "Suma de las curvas de carga, días " + enumerations::getStringFromDay(selectedDay)
                       + ", " + sectorStr + ", " + subCatStr;
        displayedSeries[0]->setName(enumerations::getStringFromDay(selectedDay));
    }

    for(int i = 0; i < displayedSeries.size(); ++i) {
        measurementsChart->addSeries(displayedSeries[i]);
    }

    // setup axes
    double yMin = 0.0;
    double yMax = 0.0;
    findMinMaxDisplayedSeries(yMin, yMax);
    QString xAxisTitle = "Hora";
    QString yAxisTitle = "kW";
    this->configureChartAxes(xAxisTitle, yAxisTitle, yMin, yMax);

    // setup and display chart
    measurementsChart->legend()->setVisible(true);
    measurementsChart->legend()->setAlignment(Qt::AlignBottom);
    measurementsChart->setTitle(diagramTitle);
    ui->lineEditDiagramTitle->setText(diagramTitle);
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
    this->displayDiagramDataAsText();
    this->currentDiagramType = 2;
    this->currentSelectedCategory = sectorDayAnalysis->getSelectedSectorString();
    this->currentSelectedSubCategory = sectorDayAnalysis->getSelectedSubCatString();
    this->currentDayType = enumerations::getStringFromDay(selectedDay);
    this->currentVisType = visType;
    result = 0;

    return result;
}

void MainWindow::findMinMaxDisplayedSeries(double &yMin, double &yMax) {
    for(int i = 0; i < displayedSeries.size(); ++i) {
        QVector<QPointF> dataPoints = displayedSeries[i]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            if((i == 0) && (j == 0)) {
                yMin = dataPoints[j].y();
                yMax = dataPoints[j].y();
            }
            if(yMin > dataPoints[j].y()) {
                yMin = dataPoints[j].y();
            }
            if(yMax < dataPoints[j].y()) {
                yMax = dataPoints[j].y();
            }
        }
    }
}

QList<QLineSeries*> MainWindow::transformAllTo15MinTicks(QList<QLineSeries*> &oldLineSeries) {
    QList<QLineSeries*> newLineSeries;

    // assumption: each lineSeries has at least 2 points
    for(int i = 0; i < oldLineSeries.size(); ++i) {
        if(oldLineSeries[i]->count() <= 1) {        // JUST FOR TESTING, PROGRAM HAS NOT BEET ADJUSTED
            continue;                               // FOR THIS CASE, WILL SEGFAULT IF COUNT <= 1
        }

        double x0 = oldLineSeries[i]->at(0).x();
        double x1 = oldLineSeries[i]->at(1).x();
        if((x1 - x0) == 0.25) {

            // lineSeries has already tick of 15 min, copy it to new line series list
            newLineSeries.append(new QLineSeries());
            for(int j = 0; j < oldLineSeries[i]->count(); ++j) {
                double xVal = oldLineSeries[i]->at(j).x();
                double yVal = oldLineSeries[i]->at(j).y();
                newLineSeries[i]->append(xVal, yVal);                   // better: use back(), because segfault
                newLineSeries[i]->setName(oldLineSeries[i]->name());    // if sizes are not equal (-> count <= 1)
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
                    //double xValLast = oldLineSeries[i]->at(j-1).x();
                    //double yValLast = oldLineSeries[i]->at(j-1).y();
                    double xValLast = oldLineSeries[i]->at(j).x();
                    double yValLast = oldLineSeries[i]->at(j).y();
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

QList<QLineSeries*> MainWindow::getMeasurementDayLineSeries(int &measurementFile, int &selectedDay) {
    QList<QLineSeries*> newLineSeriesList;

    int sheet = 0;
    std::vector<std::vector<int> > weekdayIndices;
    std::vector<int> weekendIndices;
    if(selectedDay == 0) {

        // monday to friday
        for(int i = 1; i <= 5; ++i) {
            weekdayIndices.emplace_back(findWeekdays(i, measurementFile, sheet));
        }
        for(int i = 0; i < 5; ++i) {
            for(unsigned short j = 0; j < weekdayIndices[i].size(); ++j) {

                int currLineSeriesIndex = weekdayIndices[i][j];
                newLineSeriesList.append(new QLineSeries());
                newLineSeriesList.back()->setName(
                        documents[measurementFile].sheets[0]->measurementSeries[currLineSeriesIndex]->name());
                QVector<QPointF> dataPoints =
                    documents[measurementFile].sheets[0]->measurementSeries[currLineSeriesIndex]->pointsVector();
                for(int k = 0; k < dataPoints.size(); ++k) {
                    newLineSeriesList.back()->append(dataPoints[k]);
                }
            }
        }
    } else if(selectedDay == 1) {

        // saturday
        int saturday = 6;
        weekendIndices = findWeekdays(saturday, measurementFile, sheet);

    } else if(selectedDay == 2) {

        // sunday
        int sunday = 7;
        weekendIndices = findWeekdays(sunday, measurementFile, sheet);
    }

    if(selectedDay != 0) {
        for(unsigned int i = 0; i < weekendIndices.size(); ++i) {

            newLineSeriesList.append(new QLineSeries());
            newLineSeriesList.back()->setName(
                    documents[measurementFile].sheets[0]->measurementSeries[weekendIndices[i]]->name());
            QVector<QPointF> dataPoints =
                documents[measurementFile].sheets[0]->measurementSeries[weekendIndices[i]]->pointsVector();
            for(int k = 0; k < dataPoints.size(); ++k) {
                newLineSeriesList.back()->append(dataPoints[k]);
            }
        }
    }
    return newLineSeriesList;
}

int MainWindow::generateSectorWeekDiagram() {
    int result = -1;

    // get categories and weekday that will be visualized
    int sector = sectorWeekAnalysis->getSelectedSector();
    int subCat = sectorWeekAnalysis->getSelectedSubCat();
    int visType = sectorWeekAnalysis->getVisType();

    // First: find measurementDocs corresponding to the selected Category and store indices
    std::vector<int> correctFileIndices;
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

    this->resetMeasurementsChart();

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

    QString diagramTitle = "Curvas de carga de los días laborales, "
                           + sectorWeekAnalysis->getSelectedSectorString() + ", "
                           + sectorWeekAnalysis->getSelectedSubCatString();
    if(visType == 1) {
        displayedSeries = getAverageFromSeries(displayedSeries, currentFreq, false);
        displayedSeries[0]->setName("Lunes a viernes");
        diagramTitle = "Curva de carga promedio de los días laborales, "
                       + sectorWeekAnalysis->getSelectedSectorString() + ", "
                       + sectorWeekAnalysis->getSelectedSubCatString();
    }

    for(int i = 0; i < displayedSeries.size(); ++i) {
        measurementsChart->addSeries(displayedSeries[i]);
    }

    // setup axes
    double yMin = 0.0;
    double yMax = 0.0;
    findMinMaxDisplayedSeries(yMin, yMax);
    QString xAxisTitle = "Hora";
    QString yAxisTitle = "Promedio kW";
    this->configureChartAxes(xAxisTitle, yAxisTitle, yMin, yMax);

    // setup and display chart
    measurementsChart->legend()->setVisible(true);
    measurementsChart->legend()->setAlignment(Qt::AlignBottom);
    measurementsChart->setTitle(diagramTitle);
    ui->lineEditDiagramTitle->setText(diagramTitle);
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
    this->displayDiagramDataAsText();
    this->currentDiagramType = 3;
    this->currentSelectedCategory = sectorWeekAnalysis->getSelectedSectorString();
    this->currentSelectedSubCategory = sectorWeekAnalysis->getSelectedSubCatString();
    this->currentVisType = visType;
    result = 0;

    return result;
}

int MainWindow::generateSectorSubCatsDiagram() {

    int result = -1;

    // get selected parameters
    int sector = sectorSubCatsAnalysis->getSelectedSector();
    int dayType = sectorSubCatsAnalysis->getDay();
    int visType = sectorSubCatsAnalysis->getVisType();

    if(dayType == 0) {
        this->currentDayType = "lunes a viernes";
    } else if(dayType == 1) {
        this->currentDayType = "sábado";
    } else if(dayType == 2) {
        this->currentDayType = "domingo";
    }

    // find all documents corresponding to the selected sector
    std::vector<int> correctFileIndices;
    //qDebug() << sector;
    if(sector == -2) {
        QString sectorStr = sectorSubCatsAnalysis->getSelectedSectorString();
        //qDebug() << sectorStr;
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if((documents[i].docSector == sector) && (documents[i].customSectorStr == sectorStr)) {
                correctFileIndices.emplace_back(i);
            }
        }
    } else if(sector >= 0) {
        for(unsigned int i = 0; i < this->documents.size(); ++i) {
            if(documents[i].docSector == sector) {
                //qDebug() << sector << ", " << documents[i].docSector;
                correctFileIndices.emplace_back(i);
            }
        }
    }

    // debug
    //qDebug() << "\nCorrect files: ";
    //for(int i = 0; i < correctFileIndices.size(); ++i) {
    //    qDebug() << correctFileIndices[i];
    //}
    //qDebug() << "\n";

    // find number of subcategories, name of subcategories and number of lineseries for each subcat
    QVector<QPair<QPair<int, QString>, int> > subCatIndices;
    QVector<QPair<QPair<int, QString>, QList<QLineSeries*> > > targetLineSeries;
    QList<QLineSeries*> currentLineSeries;

    //QVector<QString> subCatStrings;
    if(sector == -2) {
        for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
            if(i == 0) {
                //subCatStrings.push_back(documents[i].customSubSectorStr);
                subCatIndices.push_back(qMakePair(qMakePair(-2, documents[correctFileIndices[i]].customSubSectorStr), 0));
                targetLineSeries.push_back(
                            qMakePair(qMakePair(-2, documents[correctFileIndices[i]].customSubSectorStr), QList<QLineSeries*>()));
            }
            bool subCatExistsAlready = false;
            for(int j = 0; j < subCatIndices.size(); ++j) {
                if(documents[correctFileIndices[i]].customSubSectorStr == subCatIndices[j].first.second) {
                    ++subCatIndices[j].second;
                    subCatExistsAlready = true;

                    currentLineSeries = getMeasurementDayLineSeries(correctFileIndices[i], dayType);
                    for(int k = 0; k < currentLineSeries.size(); ++k) {
                        targetLineSeries[j].second.push_back(new QLineSeries());
                        copyLineSeries(currentLineSeries[k], targetLineSeries[j].second.back());
                    }
                    currentLineSeries.clear();
                }
            }
            if(!subCatExistsAlready) {
                subCatIndices.push_back(qMakePair(qMakePair(-2, documents[correctFileIndices[i]].customSubSectorStr), 1));
                targetLineSeries.push_back(
                            qMakePair(qMakePair(-2, documents[correctFileIndices[i]].customSubSectorStr), QList<QLineSeries*>()));
                targetLineSeries.back().second.push_back(new QLineSeries());

                // copy to new QLine series
                currentLineSeries = getMeasurementDayLineSeries(correctFileIndices[i], dayType);
                for(int k = 0; k < currentLineSeries.size(); ++k) {
                    targetLineSeries.back().second.push_back(new QLineSeries());
                    copyLineSeries(currentLineSeries[k], targetLineSeries.back().second.back());
                }
                currentLineSeries.clear();
            }
        }
    } else if(sector >= 0) {
        for(unsigned int i = 0; i < correctFileIndices.size(); ++i) {
            int subCatEnum = documents[correctFileIndices[i]].getSubCategory();
            QString subCatString = enumerations::getStringFromSubSector(sector, subCatEnum);
            if(subCatEnum == -2) {
                subCatString = documents[correctFileIndices[i]].customSubSectorStr;
            }
            if(i == 0) {
                subCatIndices.push_back(qMakePair(qMakePair(subCatEnum, subCatString), 0));
                targetLineSeries.push_back(
                            qMakePair(qMakePair(subCatEnum, subCatString), QList<QLineSeries*>()));
            }
            bool subCatExistsAlready = false;

            for(int j = 0; j < subCatIndices.size(); ++j) {
                if(subCatString == subCatIndices[j].first.second) {
                    ++subCatIndices[j].second;
                    subCatExistsAlready = true;
                    targetLineSeries[j].second.push_back(new QLineSeries());

                    // copy to new QLine series
                    currentLineSeries = getMeasurementDayLineSeries(correctFileIndices[i], dayType);
                    for(int k = 0; k < currentLineSeries.size(); ++k) {
                        targetLineSeries[j].second.push_back(new QLineSeries());
                        copyLineSeries(currentLineSeries[k], targetLineSeries[j].second.back());
                    }
                    currentLineSeries.clear();
                }
            }
            if(!subCatExistsAlready) {
                subCatIndices.push_back(qMakePair(qMakePair(subCatEnum, subCatString), 1));
                targetLineSeries.push_back(
                            qMakePair(qMakePair(subCatEnum, subCatString), QList<QLineSeries*>()));
                targetLineSeries.back().second.push_back(new QLineSeries());

                // copy to new QLine series
                currentLineSeries = getMeasurementDayLineSeries(correctFileIndices[i], dayType);
                for(int k = 0; k < currentLineSeries.size(); ++k) {
                    targetLineSeries.back().second.push_back(new QLineSeries());
                    copyLineSeries(currentLineSeries[k], targetLineSeries.back().second.back());
                }
                currentLineSeries.clear();
            }
        }
    }

    // Debug (keep for testing)
    //for(int i = 0; i < targetLineSeries.size(); ++i) {
    //    qDebug() << "< (" << targetLineSeries[i].first.first << ", "
    //             << targetLineSeries[i].first.second << "), "
    //             << targetLineSeries[i].second.size() << ">";
    //    for(int j = 0; j < targetLineSeries[i].second.size(); ++j) {
    //        qDebug() << targetLineSeries[i].second[j]->name()
    //                 << "num of data points: " << targetLineSeries[i].second[j]->count();
    //        if((targetLineSeries[i].second[j]->name()).isEmpty()) {
    //            //qDebug() << "Empty Line Series";
    //        }
    //    }
    //}
    //qDebug() << "\n";

    // fix bug: remove empty line series (reason unkown)
    QVector<QPair<int, int> > emptyLineSeries;
    for(int i = 0; i < targetLineSeries.size(); ++i) {
        for(int j = 0; j < targetLineSeries[i].second.size(); ++j) {
            QString currSeriesName = targetLineSeries[i].second[j]->name();
            if(currSeriesName.isEmpty()) {
                emptyLineSeries.push_back(qMakePair(i, j));
                //qDebug() << "Remove indices: " << i << ", " << j;
            }
        }
    }
    int removedCount = 0;
    for(int i = 0; i < emptyLineSeries.size(); ++i) {
        if(emptyLineSeries[i].second == 0) {
            removedCount = 0;
        } else if(emptyLineSeries[i].second > 0) {
            ++removedCount;
        }
        //qDebug() << targetLineSeries[emptyLineSeries[i].first].second[emptyLineSeries[i].second - removedCount]->count();
        targetLineSeries[emptyLineSeries[i].first].second.removeAt((emptyLineSeries[i].second) - removedCount);
        //++removedCount;
        //qDebug() << "Empty line series was removed";
    }


    if(targetLineSeries.size() == 0) {
        QMessageBox::warning(this, tr("No se puede generar el diagrama."),
                  tr("No se han encontrado datos para el sector indicado."),
                  QMessageBox::Ok);
        return result;
    }

    // calculate averages and transform (currently not checked whether it is necessary)
    QList<QLineSeries*> targetAverageLineSeries;
    for(int i = 0; i < targetLineSeries.size(); ++i) {
        if(targetLineSeries[i].second.size() > 0) {
            QList<QLineSeries*> currentLineSeries = transformAllTo15MinTicks(targetLineSeries[i].second);
            currentLineSeries = getAverageFromSeries(currentLineSeries, Frequency::quarterHour, false);
            currentLineSeries[0]->setName(targetLineSeries[i].first.second);
            targetAverageLineSeries.push_back(currentLineSeries[0]);
        }
    }

    // Debug
    //for(int i = 0; i < targetAverageLineSeries.size(); ++i) {
    //    qDebug() << targetAverageLineSeries[i]->name();
    //}

    // calculate average of all subcategories if visType == 1
    if(visType == 1) {
        targetAverageLineSeries = getAverageFromSeries(targetAverageLineSeries, Frequency::quarterHour, false);
        targetAverageLineSeries[0]->setName("Promedio de todas las subcategorías");
    }

    // Setup and display Diagram
    this->resetMeasurementsChart();

    // copy by value series from sheets to current displayed series, find yMin and yMax
    double yMin = 0.0;
    double yMax = 0.0;
    for(unsigned short i = 0; i < targetAverageLineSeries.size(); ++i) {

        displayedSeries.append(new QLineSeries());
        displayedSeries[i]->setName(targetAverageLineSeries[i]->name());
        QVector<QPointF> dataPoints = targetAverageLineSeries[i]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            if((i == 0) && (j == 0)) {
                yMin = dataPoints[j].y();
                yMax = dataPoints[j].y();
            }
            if(yMin > dataPoints[j].y()) {
                yMin = dataPoints[j].y();
            }
            if(yMax < dataPoints[j].y()) {
                yMax = dataPoints[j].y();
            }
            displayedSeries[i]->append(dataPoints[j]);
        }
        measurementsChart->addSeries(displayedSeries[i]);
    }
    QString xAxisTitle = "Hora";
    QString yAxisTitle = "Promedio kW";
    this->configureChartAxes(xAxisTitle, yAxisTitle, yMin, yMax);

    QString sectorStr = sectorSubCatsAnalysis->getSelectedSectorString();
    QString diagramTitle = "Curvas de carga promedio de las subcategorías del sector " + sectorStr;
    measurementsChart->legend()->setVisible(true);
    measurementsChart->legend()->setAlignment(Qt::AlignBottom);
    measurementsChart->setTitle(diagramTitle);
    ui->lineEditDiagramTitle->setText(diagramTitle);
    ui->graphicsViewChart->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewChart->setChart(measurementsChart.get());
    this->displaySectorSubCatsDiagramAsText();
    this->currentDiagramType = 4;
    this->currentVisType = visType;
    this->currentSelectedCategory = sectorStr;
    result = 0;

    return result;
}

void MainWindow::copyLineSeries(QLineSeries* &oldLineSeries, QLineSeries* &newLineSeries) {

    // this function copies oldLineSeries to the empty (!) newLineSeries
    newLineSeries->setName(oldLineSeries->name());
    QVector<QPointF> dataPoints = oldLineSeries->pointsVector();
    for(unsigned short i = 0; i < dataPoints.size(); ++i) {
        newLineSeries->append(dataPoints[i]);
    }
}

/* commented out for testing, use it later again (method to find same dates on selected category)

    // get date and categories to be visualized in the diagram
    int sector = ui->comboBoxSelectSector->currentIndex();
    int subCategory = ui->combo111BoxSelectSubCategory->currentIndex();
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
        this->displaySiteAnalysisDiagramAsText();
    } else if(selectedFunction == 2) {
        this->displaySectorWeekdayDiagramAsText();
    } else if(selectedFunction == 3) {
        this->displaySectorWeekDiagramAsText();
    }
    ui->textEditDisplayDiagram->moveCursor(QTextCursor::Start);
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

void MainWindow::displaySiteAnalysisDiagramAsText() {
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

void MainWindow::displaySectorWeekdayDiagramAsText() {
    QString diagramTitle = measurementsChart->title();
    QString seriesName = "";
    ui->textEditDisplayDiagram->clear();
    ui->textEditDisplayDiagram->append(diagramTitle + "\n");

    QString sectorStr = sectorDayAnalysis->getSelectedSectorString();
    QString subCatStr = sectorDayAnalysis->getSelectedSubCatString();
    ui->textEditDisplayDiagram->append("Sector\t" + sectorStr);
    ui->textEditDisplayDiagram->append("Subsector\t" + subCatStr + "\n");

    ui->textEditDisplayDiagram->append("Día\tHora\tkW");
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

    QString sectorStr = sectorWeekAnalysis->getSelectedSectorString();
    QString subCatStr = sectorWeekAnalysis->getSelectedSubCatString();
    ui->textEditDisplayDiagram->append("Sector\t" + sectorStr);
    ui->textEditDisplayDiagram->append("Subsector\t" + subCatStr + "\n");

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

void MainWindow::displaySectorSubCatsDiagramAsText() {
    ui->textEditDisplayDiagram->clear();
     ui->textEditDisplayDiagram->append(measurementsChart->title() + "\n");
     int numberOfSeries = displayedSeries.size();
     QString seriesName = "";
     for(int i = 0; i < numberOfSeries; ++i) {
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
         ui->textEditDisplayDiagram->append("");
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

QString MainWindow::convertTimeToStr(double time) {

    // function converts 1.5 to 01:30:00, 12.25 to 12:15:00 etc.
    double hour;
    double minute;
    QString hourString;
    QString minuteString;
    QString timeString;
    minute = std::modf(time, &hour);
    if(minute == 0.0) {
        minuteString = "00";
    } else if(minute == 0.25) {
        minuteString = "15";
    } else if(minute == 0.5) {
        minuteString = "30";
    } else if(minute == 0.75) {
        minuteString = "45";
    }
    hourString = QString::number(hour);
    if(hourString.size() == 1) {
        hourString.insert(0, "0");
    }
    timeString = hourString + ":" + minuteString  + ":00";
    return timeString;
}

void MainWindow::exportDiagram() {
    if(this->currentDiagramType == -1) {
        QMessageBox::warning(this, tr("No se puede exportar el diagrama."),
                  tr("No se ha generado ningún diagrama para exportar."),
                  QMessageBox::Ok);
        return;
    }
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
        if(this->currentDiagramType == 0) {
            this->saveSimpleDiagramAsExcel(saveFileName);
        } else if(this->currentDiagramType == 1) {
            this->saveSiteDayDiagramAsExcel(saveFileName);
        } else if(this->currentDiagramType == 2) {
            this->saveSectorDayDiagramAsExcel(saveFileName);
        } else if(this->currentDiagramType == 3) {
            this->saveSectorWeekDiagramAsExcel(saveFileName);
        } else if(this->currentDiagramType == 4) {
            this->saveSubCatsDiagramAsExcel(saveFileName);
        }
    }
}

void MainWindow::writeLineSeriesToExcel(QXlsx::Document &docToSave, int firstRow) {
    for(int i = 0; i < displayedSeries.size(); ++i) {
        docToSave.write(firstRow, 3*i+1, displayedSeries[i]->name());
        docToSave.write(firstRow+1, 3*i+1, "Hora");
        docToSave.write(firstRow+1, 3*i+2, "kW");
        QVector<QPointF> dataPoints = displayedSeries[i]->pointsVector();
        for(int j = 0; j < dataPoints.size(); ++j) {
            QString timeStr = convertTimeToStr(dataPoints[j].x());
            docToSave.write(j+firstRow+2, 3*i+1, timeStr);
            docToSave.write(j+firstRow+2, 3*i+2, dataPoints[j].y());
        }
    }
}

void MainWindow::saveSimpleDiagramAsExcel(QString &saveFileName) {
    QXlsx::Document docToSave;
    docToSave.write(1, 1, "Medición");
    docToSave.write(1, 2, this->currentDayType);
    docToSave.write(2, 1, "Tipo");
    docToSave.write(2, 2, "Visualización de las mediciones de las fechas especificadas");
    this->writeLineSeriesToExcel(docToSave, 4);
    docToSave.saveAs(saveFileName);
}

void MainWindow::saveSiteDayDiagramAsExcel(QString &saveFileName) {
    QXlsx::Document docToSave;
    docToSave.write(1, 1, "Medición");
    docToSave.write(1, 2, this->currentSelectedCategory);
    docToSave.write(2, 1, "Tipo");
    if(this->currentVisType == 0) {
        docToSave.write(2, 2, "Visualización de todas las mediciones del día especificado");
    } else if(this->currentVisType == 1) {
        docToSave.write(2, 2, "Promedio de todas las mediciones del día especificado");
    } else {
        docToSave.write(2, 2, "Suma de todas las mediciones del día especificado");
    }
    docToSave.write(3, 1, "Día");
    docToSave.write(3, 2, this->currentDayType);
    this->writeLineSeriesToExcel(docToSave, 5);
    docToSave.saveAs(saveFileName);
}

void MainWindow::saveSectorDayDiagramAsExcel(QString &saveFileName) {
    QXlsx::Document docToSave;
    docToSave.write(1, 1, "Sector");
    docToSave.write(1, 2, this->currentSelectedCategory);
    docToSave.write(2, 1, "Subsector");
    docToSave.write(2, 2, this->currentSelectedSubCategory);
    docToSave.write(3, 1, "Día");
    docToSave.write(3, 2, this->currentDayType);
    docToSave.write(4, 1, "Tipo");
    if(this->currentVisType == 0) {
        docToSave.write(4, 2, "Visualización de todas las mediciones");
    } else if(this->currentVisType == 1) {
        docToSave.write(4, 2, "Promedio de todas las mediciones");
    } else if(this->currentVisType == 2) {
        docToSave.write(4, 2, "Suma de todas las mediciones");
    }
    this->writeLineSeriesToExcel(docToSave, 6);
    docToSave.saveAs(saveFileName);
}

void MainWindow::saveSectorWeekDiagramAsExcel(QString &saveFileName) {
    QXlsx::Document docToSave;
    docToSave.write(1, 1, "Sector");
    docToSave.write(1, 2, this->currentSelectedCategory);
    docToSave.write(2, 1, "Subsector");
    docToSave.write(2, 2, this->currentSelectedSubCategory);
    docToSave.write(3, 1, "Tipo");
    if(this->currentVisType == 0) {
        docToSave.write(3, 2, "Promedio desagregado por los días lunes a viernes");
    } else {
        docToSave.write(3, 2, "Promedio total de los días lunes a viernes");
    }
    this->writeLineSeriesToExcel(docToSave, 5);
    docToSave.saveAs(saveFileName);
}

void MainWindow::saveSubCatsDiagramAsExcel(QString &saveFileName) {
    QXlsx::Document docToSave;
    docToSave.write(1, 1, "Sector");
    docToSave.write(1, 2, this->currentSelectedCategory);
    docToSave.write(2, 1, "Día");
    docToSave.write(2, 2, this->currentDayType);
    docToSave.write(3, 1, "Tipo");
    if(this->currentVisType == 0) {
        docToSave.write(3, 2, "Promedio de las subcategorías");
    } else {
        docToSave.write(3, 2, "Promedio total de todas las subcategorías");
    }

    for(int i = 0; i < displayedSeries.size(); ++i) {
        QString currentSubCat = displayedSeries[i]->name();
        QVector<QPointF> dataPoints = displayedSeries[i]->pointsVector();
        docToSave.write(5, 3*i+1, currentSubCat);
        docToSave.write(6, 3*i+1, "Hora");
        docToSave.write(6, 3*i+2, "kW");
        for(int j = 0; j < dataPoints.size(); ++j) {
            QString timeStr = convertTimeToStr(dataPoints[j].x());
            docToSave.write(j+7, 3*i+1, timeStr);
            docToSave.write(j+7, 3*i+2, dataPoints[j].y());
        }
    }
    docToSave.saveAs(saveFileName);
}

void MainWindow::updateDaysSimpleDiagramFunction(int newDocIndex) {
    if(newDocIndex >= 0) {
        short numOfDays = documents[newDocIndex].sheets[0]->daysAndCounting.size();
        for(short i = 0; i < numOfDays; ++i) {
            this->simpleDiagramFunction->updateDays(
                        documents[newDocIndex].sheets[0]->daysAndCounting[i].first);
        }
    }
}

Categories MainWindow::getDocCategories(int &docIndex) {
    Categories docCategories;
    docCategories.sector = this->documents[docIndex].docSector;
    docCategories.resRange = this->documents[docIndex].docResRange;
    docCategories.commercial = this->documents[docIndex].docSubCommercial;
    docCategories.industrial = this->documents[docIndex].docSubIndustrial;
    docCategories.customSubSec = this->documents[docIndex].docCustomSubSector;
    docCategories.customSectorStr = this->documents[docIndex].customSectorStr;
    docCategories.customSubSectorStr = this->documents[docIndex].customSubSectorStr; // will be used to analyze all custom sub sectors
    return docCategories;
}

void MainWindow::removeDocument() {
    int docToRemove = ui->listWidgetDocuments->currentRow();
    if(docToRemove >= 0) {
        Categories oldDocCategories = this->getDocCategories(docToRemove);
        categoriesTracking.removeEntries(oldDocCategories);
        categoriesTracking.updateSectorComboBox(sectorDayAnalysis->getSectorComboBox());
        categoriesTracking.updateSectorComboBox(sectorWeekAnalysis->getSectorComboBox());
        categoriesTracking.updateSectorComboBox(sectorSubCatsAnalysis->getSectorComboBox());
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

void MainWindow::removeAllDocuments() {
    while(ui->listWidgetDocuments->count() > 0) {
        this->removeDocument();
    }
}

void MainWindow::refreshDiagram() {
    QString newTitleText = ui->lineEditDiagramTitle->text();
    this->measurementsChart->setTitle(newTitleText);
    double yMin = ui->spinBoxYMin->value();
    double yMax = ui->spinBoxYMax->value();
    this->yAxis->setRange(yMin, yMax);
    this->yAxis->setTickCount(9);
}

void MainWindow::configDiagram() {
    ConfigDiagram* configDiagramDialog = new ConfigDiagram(this,
                                                           this->measurementsChart.get(),
                                                           xAxis, yAxis, &displayedSeries);
    configDiagramDialog->setWindowTitle("Configurar Diagrama");
    configDiagramDialog->exec();
    ui->lineEditDiagramTitle->setText(measurementsChart->title());
    ui->spinBoxYMin->setValue(yAxis->min());
    ui->spinBoxYMax->setValue(yAxis->max());
    delete configDiagramDialog;
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
    } else if(selectedFunction == 4) {
        this->generateSectorSubCatsDiagram();
    }
    return 0;
}

void MainWindow::exitProgram() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Salir", "¿Seguro que desea terminar el programa?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        QCoreApplication::quit();
    }
}
