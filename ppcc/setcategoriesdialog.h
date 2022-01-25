//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef SETCATEGORIESDIALOG_H
#define SETCATEGORIESDIALOG_H

#include <QDialog>
#include <QButtonGroup>

#include "enumerations.h"

namespace Ui {
class SetCategoriesDialog;
}

class SetCategoriesDialog : public QDialog {
    Q_OBJECT

public:
    explicit SetCategoriesDialog(QWidget *parent = nullptr);
    SetCategoriesDialog(QWidget *parent, Categories&, QString&);
    ~SetCategoriesDialog();

private:
    Ui::SetCategoriesDialog *ui;

    QButtonGroup sectorGroup;
    QButtonGroup resRangeGroup;
    QButtonGroup subCommercialGroup;
    QButtonGroup subIndustrialGroup;
    QButtonGroup subCustomGroup;

    SystemVoltage* currentSV;
    Sector* currentSector;
    ResidentialRange* currentRR;
    Commercial* currentSubCom;
    Industrial* currentSubInd;
    CustomSubSector* currentSubCustom;
    Frequency* currentFreq;
    DateFormat* currentDateFormat;

    QString* currentCustomSectorStr;
    QString* currentCustomResRangeStr;
    QString* currentCustomCommercialStr;
    QString* currentCustomIndustrialStr;
    QString* currentCustomSubSectorStr;

private slots:
    void exitCategoriesDialog();
    void setNotResidential(bool);
    void setNotCommercial(bool);
    void setNotIndustrial(bool);
    void setNotCustomSector(bool);

    void setLineEditCustomResRange(bool);
    void setLineEditCustomCommercial(bool);
    void setLineEditCustomIndustrial(bool);
    void setLineEditCustomSubCat(bool);
};

#endif // SETCATEGORIESDIALOG_H
