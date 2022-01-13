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
    SetCategoriesDialog(QWidget *parent, Sector&, ResidentialRange&, Commercial&, Industrial&, QString&);
    ~SetCategoriesDialog();

private:
    Ui::SetCategoriesDialog *ui;

    QButtonGroup sectorGroup;
    QButtonGroup resRangeGroup;
    QButtonGroup subCommercialGroup;
    QButtonGroup subIndustrialGroup;

    SystemVoltage* currentSV;
    Sector* currentSector;
    ResidentialRange* currentRR;
    Commercial* currentSubCom;
    Industrial* currentSubInd;

private slots:
    void exitCategoriesDialog();
    void setNotResidential(bool);
    void setNotCommercial(bool);
    void setNotIndustrial(bool);
};

#endif // SETCATEGORIESDIALOG_H