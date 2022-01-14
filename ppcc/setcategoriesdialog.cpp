#include "setcategoriesdialog.h"
#include "ui_setcategoriesdialog.h"

SetCategoriesDialog::SetCategoriesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SetCategoriesDialog) {
    ui->setupUi(this);
}

SetCategoriesDialog::SetCategoriesDialog(QWidget *parent, Sector &s, ResidentialRange &RR, Commercial &com,
                                         Industrial &ind, Frequency& fr, DateFormat& df, QString &fileName) :
    QDialog(parent), ui(new Ui::SetCategoriesDialog) {
    ui->setupUi(this);
    ui->labelSetCategories->setText("Seleccione Categorias para el archivo '" + fileName + " ':");

    currentSector = &s;
    currentRR = &RR;
    currentSubCom = &com;
    currentSubInd = &ind;
    currentFreq = &fr;
    currentDateFormat = &df;

    // radio buttons ids are assigned ascending, as they appear on the dialog
    sectorGroup.addButton(ui->radioButtonRes, 0);
    sectorGroup.addButton(ui->radioButtonCommercial, 1);
    sectorGroup.addButton(ui->radioButtonIndustrial, 2);
    sectorGroup.addButton(ui->radioButtonPumping, 3);
    sectorGroup.addButton(ui->radioButtonPublicLight, 4);
    resRangeGroup.addButton(ui->radioButtonResA, 0);
    resRangeGroup.addButton(ui->radioButtonResB, 1);
    resRangeGroup.addButton(ui->radioButtonResC, 2);
    resRangeGroup.addButton(ui->radioButtonResD, 3);
    resRangeGroup.addButton(ui->radioButtonResE, 4);
    resRangeGroup.addButton(ui->radioButtonResF, 5);
    subCommercialGroup.addButton(ui->radioButtonFinances, 0);
    subCommercialGroup.addButton(ui->radioButtonWholesale, 1);
    subCommercialGroup.addButton(ui->radioButtonRetail, 2);
    subCommercialGroup.addButton(ui->radioButtonGov, 3);
    subCommercialGroup.addButton(ui->radioButtonHealth, 4);
    subCommercialGroup.addButton(ui->radioButtonHotels, 5);
    subCommercialGroup.addButton(ui->radioButtonEducation, 6);
    subCommercialGroup.addButton(ui->radioButtonOtherCommercial, 7);
    subIndustrialGroup.addButton(ui->radioButtonFood, 0);
    subIndustrialGroup.addButton(ui->radioButtonPaper, 1);
    subIndustrialGroup.addButton(ui->radioButtonPlastics, 2);
    subIndustrialGroup.addButton(ui->radioButtonChemistry, 3);
    subIndustrialGroup.addButton(ui->radioButtonTextiles, 4);
    subIndustrialGroup.addButton(ui->radioButtonOtherIndustrial, 5);
    frequencyGroup.addButton(ui->radioButton15min, 0);
    frequencyGroup.addButton(ui->radioButton1h, 0);

    // set date format entries in comboBox
    ui->comboBoxDateFormat->addItem("dd mm aaaa");
    ui->comboBoxDateFormat->addItem("mm dd aaaa");
    ui->comboBoxDateFormat->addItem("aaaa mm dd");
    ui->comboBoxDateFormat->addItem("aaaa dd mm");
    ui->comboBoxDateFormat->setCurrentIndex(0);

    // toggle selected buttons
    ui->radioButtonRes->setChecked(true);
    ui->radioButtonResA->setChecked(true);
    ui->radioButtonFinances->setChecked(true);
    ui->radioButtonFood->setChecked(true);
    ui->radioButton15min->setChecked(true);

    // toggle active buttons
    ui->radioButtonFinances->setDisabled(true);
    ui->radioButtonWholesale->setDisabled(true);
    ui->radioButtonRetail->setDisabled(true);
    ui->radioButtonGov->setDisabled(true);
    ui->radioButtonHealth->setDisabled(true);
    ui->radioButtonHotels->setDisabled(true);
    ui->radioButtonEducation->setDisabled(true);
    ui->radioButtonOtherCommercial->setDisabled(true);
    ui->radioButtonFood->setDisabled(true);
    ui->radioButtonPaper->setDisabled(true);
    ui->radioButtonPlastics->setDisabled(true);
    ui->radioButtonChemistry->setDisabled(true);
    ui->radioButtonTextiles->setDisabled(true);
    ui->radioButtonOtherIndustrial->setDisabled(true);

    connect(ui->buttonOk, SIGNAL(clicked()), this, SLOT(exitCategoriesDialog()));
    connect(ui->radioButtonRes, SIGNAL(toggled(bool)), this, SLOT(setNotResidential(bool)));
    connect(ui->radioButtonCommercial, SIGNAL(toggled(bool)), this, SLOT(setNotCommercial(bool)));
    connect(ui->radioButtonIndustrial, SIGNAL(toggled(bool)), this, SLOT(setNotIndustrial(bool)));
}

SetCategoriesDialog::~SetCategoriesDialog() {
    delete ui;
}

void SetCategoriesDialog::exitCategoriesDialog() {
    int checkedSector = sectorGroup.checkedId();
    int checkedRR = resRangeGroup.checkedId();
    int checkedCommercial = subCommercialGroup.checkedId();
    int checkedIndustrial = subIndustrialGroup.checkedId();
    int checkedFreq = frequencyGroup.checkedId();

    if(checkedSector == 0) {
        *currentSector = Sector::residential;
    } else if(checkedSector == 1) {
        *currentSector = Sector::commercial;
    } else if(checkedSector == 2) {
        *currentSector = Sector::industrial;
    } else if(checkedSector == 3) {
        *currentSector = Sector::pumping;
    } else if(checkedSector == 4) {
        *currentSector = Sector::publicLighting;
    }

    if(!(ui->radioButtonResA->isEnabled())) {
        *currentRR = ResidentialRange::X;
    } else if(checkedRR == 0) {
        *currentRR = ResidentialRange::A;
    } else if(checkedRR == 1) {
        *currentRR = ResidentialRange::B;
    } else if(checkedRR == 2) {
        *currentRR = ResidentialRange::C;
    } else if(checkedRR == 3) {
        *currentRR = ResidentialRange::D;
    } else if(checkedRR == 4) {
        *currentRR = ResidentialRange::E;
    } else if(checkedRR == 5) {
        *currentRR = ResidentialRange::F;
    }

    if(!(ui->radioButtonFinances->isEnabled())) {
        *currentSubCom = Commercial::notCommercial;
    } else if(checkedCommercial == 0) {
        *currentSubCom = Commercial::finances;
    } else if(checkedCommercial == 1) {
        *currentSubCom = Commercial::wholesale;
    } else if(checkedCommercial == 2) {
        *currentSubCom = Commercial::retail;
    } else if(checkedCommercial == 3) {
        *currentSubCom = Commercial::government;
    } else if(checkedCommercial == 4) {
        *currentSubCom = Commercial::health;
    } else if(checkedCommercial == 5) {
        *currentSubCom = Commercial::hotels;
    } else if(checkedCommercial == 6) {
        *currentSubCom = Commercial::education;
    } else if(checkedCommercial == 7) {
        *currentSubCom = Commercial::otherCommercial;
    }

    if(!(ui->radioButtonFood->isEnabled())) {
        *currentSubInd = Industrial::notIndustrial;
    } else if(checkedIndustrial == 0) {
        *currentSubInd = Industrial::food;
    } else if(checkedIndustrial == 1) {
        *currentSubInd = Industrial::paper;
    } else if(checkedIndustrial == 2) {
        *currentSubInd = Industrial::plastics;
    } else if(checkedIndustrial == 3) {
        *currentSubInd = Industrial::chemistry;
    } else if(checkedIndustrial == 4) {
        *currentSubInd = Industrial::textiles;
    } else if(checkedIndustrial == 5) {
        *currentSubInd = Industrial::otherIndustrial;
    }

    if(checkedFreq == 0) {
        *currentFreq = Frequency::quarterHour;
    } else {
        *currentFreq = Frequency::hour;
    }

    if(ui->comboBoxDateFormat->currentIndex() == 0) {
        *currentDateFormat = DateFormat::dayMonthYear;
    } else if(ui->comboBoxDateFormat->currentIndex() == 1) {
        *currentDateFormat = DateFormat::monthDayYear;
    } else if(ui->comboBoxDateFormat->currentIndex() == 2) {
        *currentDateFormat = DateFormat::yearMonthDay;
    } else if(ui->comboBoxDateFormat->currentIndex() == 3) {
        *currentDateFormat = DateFormat::yearDayMonth;
    }

    this->close();
}

void SetCategoriesDialog::setNotResidential(bool resSectorState) {
    if(!resSectorState) {
        ui->radioButtonResA->setDisabled(true);
        ui->radioButtonResB->setDisabled(true);
        ui->radioButtonResC->setDisabled(true);
        ui->radioButtonResD->setDisabled(true);
        ui->radioButtonResE->setDisabled(true);
        ui->radioButtonResF->setDisabled(true);
    } else {
        ui->radioButtonResA->setDisabled(false);
        ui->radioButtonResB->setDisabled(false);
        ui->radioButtonResC->setDisabled(false);
        ui->radioButtonResD->setDisabled(false);
        ui->radioButtonResE->setDisabled(false);
        ui->radioButtonResF->setDisabled(false);
    }
}

void SetCategoriesDialog::setNotCommercial(bool comSectorState) {
    if(!comSectorState) {
        ui->radioButtonFinances->setDisabled(true);
        ui->radioButtonWholesale->setDisabled(true);
        ui->radioButtonRetail->setDisabled(true);
        ui->radioButtonGov->setDisabled(true);
        ui->radioButtonHealth->setDisabled(true);
        ui->radioButtonHotels->setDisabled(true);
        ui->radioButtonEducation->setDisabled(true);
        ui->radioButtonOtherCommercial->setDisabled(true);
    } else {
        ui->radioButtonFinances->setDisabled(false);
        ui->radioButtonWholesale->setDisabled(false);
        ui->radioButtonRetail->setDisabled(false);
        ui->radioButtonGov->setDisabled(false);
        ui->radioButtonHealth->setDisabled(false);
        ui->radioButtonHotels->setDisabled(false);
        ui->radioButtonEducation->setDisabled(false);
        ui->radioButtonOtherCommercial->setDisabled(false);
    }
}

void SetCategoriesDialog::setNotIndustrial(bool indSectorState) {
    if(!indSectorState) {
        ui->radioButtonFood->setDisabled(true);
        ui->radioButtonPaper->setDisabled(true);
        ui->radioButtonPlastics->setDisabled(true);
        ui->radioButtonChemistry->setDisabled(true);
        ui->radioButtonTextiles->setDisabled(true);
        ui->radioButtonOtherIndustrial->setDisabled(true);
    } else {
        ui->radioButtonFood->setDisabled(false);
        ui->radioButtonPaper->setDisabled(false);
        ui->radioButtonPlastics->setDisabled(false);
        ui->radioButtonChemistry->setDisabled(false);
        ui->radioButtonTextiles->setDisabled(false);
        ui->radioButtonOtherIndustrial->setDisabled(false);
    }
}
