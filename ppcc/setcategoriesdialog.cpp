//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "setcategoriesdialog.h"
#include "ui_setcategoriesdialog.h"

SetCategoriesDialog::SetCategoriesDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SetCategoriesDialog) {
    ui->setupUi(this);
}

SetCategoriesDialog::SetCategoriesDialog(QWidget *parent, Categories &docCategories, QString &fileName) :
    QDialog(parent), ui(new Ui::SetCategoriesDialog) {
    ui->setupUi(this);
    ui->labelSetCategories->setText("Seleccione Categorias para el archivo '" + fileName + "':");

    currentSector = &docCategories.sector;
    currentRR = &docCategories.resRange;
    currentSubCom = &docCategories.commercial;
    currentSubInd = &docCategories.industrial;
    currentSubCustom = &docCategories.customSubSec;
    currentCustomSectorStr = &docCategories.customSectorStr;
    currentCustomResRangeStr = &docCategories.customResRangeStr;
    currentCustomCommercialStr = &docCategories.customCommercialStr;
    currentCustomIndustrialStr = &docCategories.customIndustrialStr;
    currentCustomSubSectorStr = &docCategories.customSubSectorStr;

    // Assign radio button ids corresponding to enums
    sectorGroup.addButton(ui->radioButtonRes, 0);
    sectorGroup.addButton(ui->radioButtonCommercial, 1);
    sectorGroup.addButton(ui->radioButtonIndustrial, 2);
    sectorGroup.addButton(ui->radioButtonPumping, 3);
    sectorGroup.addButton(ui->radioButtonPublicLight, 4);
    sectorGroup.addButton(ui->radioButtonOtherSector, 5);
    sectorGroup.addButton(ui->radioButtonCustomSector, -2);
    resRangeGroup.addButton(ui->radioButtonResA, 0);
    resRangeGroup.addButton(ui->radioButtonResB, 1);
    resRangeGroup.addButton(ui->radioButtonResC, 2);
    resRangeGroup.addButton(ui->radioButtonResD, 3);
    resRangeGroup.addButton(ui->radioButtonResE, 4);
    resRangeGroup.addButton(ui->radioButtonResF, 5);
    resRangeGroup.addButton(ui->radioButtonOtherResRange, 6);
    resRangeGroup.addButton(ui->radioButtonCustomResRange, -2);
    subCommercialGroup.addButton(ui->radioButtonFinances, 0);
    subCommercialGroup.addButton(ui->radioButtonWholesale, 1);
    subCommercialGroup.addButton(ui->radioButtonRetail, 2);
    subCommercialGroup.addButton(ui->radioButtonGov, 3);
    subCommercialGroup.addButton(ui->radioButtonHealth, 4);
    subCommercialGroup.addButton(ui->radioButtonHotels, 5);
    subCommercialGroup.addButton(ui->radioButtonEducation, 6);
    subCommercialGroup.addButton(ui->radioButtonOtherCommercial, 7);
    subCommercialGroup.addButton(ui->radioButtonCustomCommercial, -2);
    subIndustrialGroup.addButton(ui->radioButtonFood, 0);
    subIndustrialGroup.addButton(ui->radioButtonPaper, 1);
    subIndustrialGroup.addButton(ui->radioButtonPlastics, 2);
    subIndustrialGroup.addButton(ui->radioButtonChemistry, 3);
    subIndustrialGroup.addButton(ui->radioButtonTextiles, 4);
    subIndustrialGroup.addButton(ui->radioButtonOtherIndustrial, 5);
    subIndustrialGroup.addButton(ui->radioButtonCustomIndustrial, -2);
    subCustomGroup.addButton(ui->radioButtonCustomSubCat, -2);
    subCustomGroup.addButton(ui->radioButtonOtherCustomSubCat, 0);

    // toggle selected buttons
    ui->radioButtonRes->setChecked(true);
    ui->radioButtonResA->setChecked(true);
    ui->radioButtonFinances->setChecked(true);
    ui->radioButtonFood->setChecked(true);
    ui->radioButtonCustomSubCat->setChecked(true);

    // toggle active buttons and line edits
    ui->radioButtonFinances->setDisabled(true);
    ui->radioButtonWholesale->setDisabled(true);
    ui->radioButtonRetail->setDisabled(true);
    ui->radioButtonGov->setDisabled(true);
    ui->radioButtonHealth->setDisabled(true);
    ui->radioButtonHotels->setDisabled(true);
    ui->radioButtonEducation->setDisabled(true);
    ui->radioButtonCustomCommercial->setDisabled(true);
    ui->radioButtonOtherCommercial->setDisabled(true);
    ui->radioButtonFood->setDisabled(true);
    ui->radioButtonPaper->setDisabled(true);
    ui->radioButtonPlastics->setDisabled(true);
    ui->radioButtonChemistry->setDisabled(true);
    ui->radioButtonTextiles->setDisabled(true);
    ui->radioButtonCustomIndustrial->setDisabled(true);
    ui->radioButtonOtherIndustrial->setDisabled(true);
    ui->radioButtonCustomSubCat->setDisabled(true);
    ui->radioButtonOtherCustomSubCat->setDisabled(true);

    // setup line edits
    ui->lineEditCustomSector->setDisabled(true);
    ui->lineEditCustomSector->setPlaceholderText("Personalizar");
    ui->lineEditCustomResRange->setDisabled(true);
    ui->lineEditCustomResRange->setPlaceholderText("Personalizar");
    ui->lineEditCustomCommercial->setDisabled(true);
    ui->lineEditCustomCommercial->setPlaceholderText("Personalizar");
    ui->lineEditCustomIndustrial->setDisabled(true);
    ui->lineEditCustomIndustrial->setPlaceholderText("Personalizar");
    ui->lineEditCustomSubCat->setDisabled(true);
    ui->lineEditCustomSubCat->setPlaceholderText("Personalizar");

    connect(ui->buttonOk, SIGNAL(clicked()), this, SLOT(exitCategoriesDialog()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(exitOnCancel()));
    connect(ui->radioButtonRes, SIGNAL(toggled(bool)), this, SLOT(setNotResidential(bool)));
    connect(ui->radioButtonCommercial, SIGNAL(toggled(bool)), this, SLOT(setNotCommercial(bool)));
    connect(ui->radioButtonIndustrial, SIGNAL(toggled(bool)), this, SLOT(setNotIndustrial(bool)));
    connect(ui->radioButtonCustomSector, SIGNAL(toggled(bool)), this, SLOT(setNotCustomSector(bool)));
    connect(ui->radioButtonCustomResRange, SIGNAL(toggled(bool)), this, SLOT(setLineEditCustomResRange(bool)));
    connect(ui->radioButtonCustomCommercial, SIGNAL(toggled(bool)), this, SLOT(setLineEditCustomCommercial(bool)));
    connect(ui->radioButtonCustomIndustrial, SIGNAL(toggled(bool)), this, SLOT(setLineEditCustomIndustrial(bool)));
    connect(ui->radioButtonCustomSubCat, SIGNAL(toggled(bool)), this, SLOT(setLineEditCustomSubCat(bool)));
}

SetCategoriesDialog::~SetCategoriesDialog() {
    delete ui;
}

void SetCategoriesDialog::exitOnCancel() {
    emit importProcessCanceled();
    this->close();
}

void SetCategoriesDialog::exitCategoriesDialog() {
    int checkedSector = sectorGroup.checkedId();
    int checkedRR = resRangeGroup.checkedId();
    int checkedCommercial = subCommercialGroup.checkedId();
    int checkedIndustrial = subIndustrialGroup.checkedId();
    int checkedSubCustom = subCustomGroup.checkedId();

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
    } else if(checkedSector == 5) {
        *currentSector = Sector::otherSector;
    } else if(checkedSector == -2) {
        *currentSector = Sector::customSector;
        *currentCustomSectorStr = ui->lineEditCustomSector->text();
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
    } else if(checkedRR == 6) {
        *currentRR = ResidentialRange::otherResRange;
    } else if(checkedRR == -2) {
        *currentRR = ResidentialRange::customResRange;
        *currentCustomResRangeStr = ui->lineEditCustomResRange->text();
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
    } else if(checkedCommercial == -2) {
        *currentSubCom = Commercial::customCommercial;
        *currentCustomCommercialStr = ui->lineEditCustomCommercial->text();
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
    } else if(checkedIndustrial == -2) {
        *currentSubInd = Industrial::customIndustrial;
        *currentCustomIndustrialStr = ui->lineEditCustomIndustrial->text();
    }

    if(!(ui->radioButtonCustomSubCat->isEnabled())) {
        *currentSubCustom = CustomSubSector::notCustomSubSector;
    } else if(checkedSubCustom == 0) {
        *currentSubCustom = CustomSubSector::otherCustomSubSector;
    } else if(checkedSubCustom == -2) {
        *currentSubCustom = CustomSubSector::customSubSector;
        *currentCustomSubSectorStr = ui->lineEditCustomSubCat->text();
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
        ui->radioButtonOtherResRange->setDisabled(true);
        ui->radioButtonCustomResRange->setDisabled(true);
        ui->lineEditCustomResRange->setDisabled(true);
    } else {
        ui->radioButtonResA->setDisabled(false);
        ui->radioButtonResB->setDisabled(false);
        ui->radioButtonResC->setDisabled(false);
        ui->radioButtonResD->setDisabled(false);
        ui->radioButtonResE->setDisabled(false);
        ui->radioButtonResF->setDisabled(false);
        ui->radioButtonOtherResRange->setDisabled(false);
        ui->radioButtonCustomResRange->setDisabled(false);
        this->setLineEditCustomResRange(ui->radioButtonCustomResRange->isChecked());
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
        ui->radioButtonCustomCommercial->setDisabled(true);
        ui->lineEditCustomCommercial->setDisabled(true);
    } else {
        ui->radioButtonFinances->setDisabled(false);
        ui->radioButtonWholesale->setDisabled(false);
        ui->radioButtonRetail->setDisabled(false);
        ui->radioButtonGov->setDisabled(false);
        ui->radioButtonHealth->setDisabled(false);
        ui->radioButtonHotels->setDisabled(false);
        ui->radioButtonEducation->setDisabled(false);
        ui->radioButtonOtherCommercial->setDisabled(false);
        ui->radioButtonCustomCommercial->setDisabled(false);
        this->setLineEditCustomCommercial(ui->radioButtonCustomCommercial->isChecked());
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
        ui->radioButtonCustomIndustrial->setDisabled(true);
        ui->lineEditCustomIndustrial->setDisabled(true);
    } else {
        ui->radioButtonFood->setDisabled(false);
        ui->radioButtonPaper->setDisabled(false);
        ui->radioButtonPlastics->setDisabled(false);
        ui->radioButtonChemistry->setDisabled(false);
        ui->radioButtonTextiles->setDisabled(false);
        ui->radioButtonOtherIndustrial->setDisabled(false);
        ui->radioButtonCustomIndustrial->setDisabled(false);
        this->setLineEditCustomIndustrial(ui->radioButtonCustomIndustrial->isChecked());
    }
}

void SetCategoriesDialog::setNotCustomSector(bool customSectorState) {
    if(!customSectorState) {
        ui->radioButtonCustomSubCat->setDisabled(true);
        ui->lineEditCustomSubCat->setDisabled(true);
        ui->radioButtonOtherCustomSubCat->setDisabled(true);
        ui->lineEditCustomSector->setDisabled(true);
    } else {
        ui->lineEditCustomSector->setDisabled(false);
        ui->radioButtonCustomSubCat->setDisabled(false);
        ui->radioButtonOtherCustomSubCat->setDisabled(false);
        this->setLineEditCustomSubCat(ui->radioButtonCustomSubCat->isChecked());
    }
}

void SetCategoriesDialog::setLineEditCustomResRange(bool customResRangeState) {
    if(!customResRangeState) {
        ui->lineEditCustomResRange->setDisabled(true);
    } else {
        ui->lineEditCustomResRange->setDisabled(false);
    }
}

void SetCategoriesDialog::setLineEditCustomCommercial(bool customComState) {
    if(!customComState) {
        ui->lineEditCustomCommercial->setDisabled(true);
    } else {
        ui->lineEditCustomCommercial->setDisabled(false);
    }
}

void SetCategoriesDialog::setLineEditCustomIndustrial(bool customIndState) {
    if(!customIndState) {
        ui->lineEditCustomIndustrial->setDisabled(true);
    } else {
        ui->lineEditCustomIndustrial->setDisabled(false);
    }
}

void SetCategoriesDialog::setLineEditCustomSubCat(bool customSubCatState) {
    if(!customSubCatState) {
        ui->lineEditCustomSubCat->setDisabled(true);
    } else {
        ui->lineEditCustomSubCat->setDisabled(false);
    }
}
