#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow) {

    // setup ui elements
    ui->setupUi(this);
    ui->comboBoxSelectDoc->setCurrentIndex(-1);
    ui->comboBoxSelectDoc->setPlaceholderText("Seleccionar documento");
    ui->listWidgetDays->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->comboBoxSelectSector->addItem("Residencial");
    ui->comboBoxSelectSector->addItem("Comercio y servicios");
    ui->comboBoxSelectSector->addItem("Industrial");
    ui->comboBoxSelectSector->addItem("Bombeo");
    ui->comboBoxSelectSector->addItem("Alumbrado publico");
    ui->comboBoxSelectSector->setCurrentIndex(0);
    this->updateComboBoxSelectSubCat(0);
    classificationVisTypeGroup.addButton(ui->radioButtonSum, 0);
    classificationVisTypeGroup.addButton(ui->radioButtonAverage, 1);
    classificationVisTypeGroup.addButton(ui->radioButtonCompare, 2);
    ui->radioButtonSum->setChecked(true);
    ui->lineEditSector->setReadOnly(true);
    ui->lineEditSubCat->setReadOnly(true);

    // set pointers and smart pointers for correct resource management
    this->measurementsChart = std::make_shared<QChart>();
    this->auxiliaryUpdateChart = std::make_shared<QChart>();

    // set auxChart parameters
    this->auxiliaryUpdateChart->legend()->hide();
    this->auxiliaryUpdateChart->createDefaultAxes();

    // connect interactive elements with respective functions
    connect(ui->buttonImportDocuments, SIGNAL(clicked()), this, SLOT(importDocument()));
    connect(ui->buttonGenerateSimpleDiagram, SIGNAL(clicked()), this, SLOT(generateSimpleDiagram()));
    connect(ui->buttonGenerateClassifiedDiagram, SIGNAL(clicked()), this, SLOT(generateClassifiedDiagram()));
    connect(ui->buttonExportDiagram, SIGNAL(clicked()), this, SLOT(saveDiagram()));
    connect(ui->comboBoxSelectDoc, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSheetList(int)));
    connect(ui->comboBoxSelectSheet, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDaysEntries(int)));
    connect(ui->comboBoxSelectSector, SIGNAL(currentIndexChanged(int)), this, SLOT(updateComboBoxSelectSubCat(int)));
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
    Frequency freq;
    DateFormat df;
    for(int i = 0; i < docFileNames.size(); ++i) {
        QString fileNameOnly = docFileNames[i].section("/",-1,-1);
        if(!docFileNames[i].isEmpty()) {
            SetCategoriesDialog* selectCategories =
                    new SetCategoriesDialog(this, sec, rr, subCom, subInd, freq, df, fileNameOnly);
            selectCategories->exec();
            delete selectCategories;
            documents.emplace_back(MeasurementsDocument(docFileNames[i]));
            documents.back().docSector = sec;
            documents.back().docResRange = rr;
            documents.back().docSubCommercial = subCom;
            documents.back().docSubIndustrial = subInd;
            documents.back().docFreq = freq;
            documents.back().docDateFormat = df;
            addEntryComboBoxDocSelection(fileNameOnly);
            if(ui->comboBoxSelectDoc->currentIndex() == -1) {
                ui->comboBoxSelectDoc->setCurrentIndex(0);
            }
        }
    }
}

int MainWindow::generateSimpleDiagram() {

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

void MainWindow::addEntryComboBoxDocSelection(const QString& newDocument) {
    ui->comboBoxSelectDoc->addItem(newDocument);
}

void MainWindow::updateSheetList(int docIndex) {
    ui->comboBoxSelectSheet->clear();
    this->selectedDocIndex = docIndex;
    for(unsigned short i = 0; i < documents[docIndex].sheets.size(); ++i) {
        ui->comboBoxSelectSheet->addItem(documents[docIndex].sheets[i]->sheetName);
    }

    // display document sector and subsector (only to check, can be removed later)
    ui->lineEditSector->setText(QString::number(documents[docIndex].docSector));
    if(documents[docIndex].docResRange != ResidentialRange::X) {
        ui->lineEditSubCat->setText(QString::number(documents[docIndex].docResRange));
    } else if(documents[docIndex].docSubCommercial != Commercial::notCommercial) {
        ui->lineEditSubCat->setText(QString::number(documents[docIndex].docSubCommercial));
    } else if(documents[docIndex].docSubIndustrial != Industrial::notIndustrial) {
        ui->lineEditSubCat->setText(QString::number(documents[docIndex].docSubIndustrial));
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

void MainWindow::updateComboBoxSelectSubCat(int currentSector) {
    ui->comboBoxSelectSubCategory->clear();
    if(currentSector == 0) {
        ui->comboBoxSelectSubCategory->addItem("50 - 100 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("101 - 150 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("151 - 250 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem("251 - 500 kWh/mes");
        ui->comboBoxSelectSubCategory->addItem(">500 kWh/mes");
        ui->comboBoxSelectSubCategory->setCurrentIndex(0);
    } else if(currentSector == 1) {
        ui->comboBoxSelectSubCategory->addItem("Financieras (bancos)");
        ui->comboBoxSelectSubCategory->addItem("Comercio al por mayor");
        ui->comboBoxSelectSubCategory->addItem("Comercio al por menor");
        ui->comboBoxSelectSubCategory->addItem("Gobierno");
        ui->comboBoxSelectSubCategory->addItem("Salud");
        ui->comboBoxSelectSubCategory->addItem("Hoteles");
        ui->comboBoxSelectSubCategory->addItem("Educacion");
        ui->comboBoxSelectSubCategory->addItem("Otros");
        ui->comboBoxSelectSubCategory->setCurrentIndex(0);
    } else if(currentSector == 2) {
        ui->comboBoxSelectSubCategory->addItem("Alimentos y bebidas");
        ui->comboBoxSelectSubCategory->addItem("Papel, carton");
        ui->comboBoxSelectSubCategory->addItem("Caucho, plastico");
        ui->comboBoxSelectSubCategory->addItem("Quimica");
        ui->comboBoxSelectSubCategory->addItem("Textil");
        ui->comboBoxSelectSubCategory->addItem("Otros");
        ui->comboBoxSelectSubCategory->setCurrentIndex(0);
    }
}

void MainWindow::exitProgram() {
    QCoreApplication::quit();
}
