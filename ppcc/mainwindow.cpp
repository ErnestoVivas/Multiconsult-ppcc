#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow), numOfDays(0) {
    ui->setupUi(this);

    // set pointers to NULL to check for destroying in destructor
    this->measurementData = NULL;

    // connect interactive elements with respective functions
    connect(ui->buttonOpenFile, SIGNAL(clicked()), this, SLOT(openFile()));
}

MainWindow::~MainWindow() {
    delete ui;

    if(this->measurementData)
        delete this->measurementData;
}

void MainWindow::readDocument(QXlsx::Document* measurementData) {
    if(measurementData->load()) {

        // Document has 3 columns and a variable number of rows
        // indexing off excel sheets starts at (1,1)
        measurementData->selectSheet("Hoja1");

        bool cellNotEmpty = true;
        QVariant currentCellValue;
        QString currentRow;
        QString cellValueString;
        QString prevCellValueString;

        // parse measurementData, convert cell Data if necessary
        for(int i = 1; cellNotEmpty; ++i) {
            for(int j = 1; j < 4; ++j) {

                // read current cell value as iterating through the document
                currentCellValue = measurementData->read(i,j);

                // convert wrongly detected times
                if((j == 2) && (i > 1)) {
                    if(currentCellValue.canConvert(15)) {
                        currentCellValue = currentCellValue.toTime();
                    }
                    if(currentCellValue.type() == 14) {
                        currentCellValue = QVariant(QTime(0,0,0));
                    }
                }

                // convert cell value to string (to print), check for end of doc and count days
                cellValueString = currentCellValue.toString();
                if((j == 1) && (!cellValueString.isEmpty())) {
                    if(i == 1) {
                        prevCellValueString = cellValueString;
                        ++numOfDays;
                        days.push_back(currentCellValue.toDate());
                    } else {
                        if(cellValueString != prevCellValueString) {
                            ++numOfDays;
                            days.push_back(currentCellValue.toDate());
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

            // display spreadsheet
            ui->textEditDisplaySheet->append(currentRow);
            currentRow = "";
        }
        qDebug() << days.size();
    }
}

void MainWindow::openFile() {
    QString spreadsheetFileName = QFileDialog::getOpenFileName(this,
        tr("Abrir archivo Excel"), "~/", tr("Formato Excel (*.xls *.xlsx)"));
    ui->lineEditOpenFile->setText(spreadsheetFileName);
    this->measurementData = new QXlsx::Document(spreadsheetFileName);

    readDocument(this->measurementData);
}

