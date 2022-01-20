//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef SECTORWEEKANALYSIS_H
#define SECTORWEEKANALYSIS_H

#include <QWidget>
#include <QButtonGroup>

#include "enumerations.h"

namespace Ui {
class SectorWeekAnalysis;
}

class SectorWeekAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit SectorWeekAnalysis(QWidget *parent = nullptr);
    ~SectorWeekAnalysis();

    int getSelectedSector();
    int getSelectedSubCat();
    int getVisType();

private:
    Ui::SectorWeekAnalysis *ui;
    QButtonGroup visualizationGroup;

    Categories selectedCategories;

    void setupComboBoxSector();

private slots:
    void updateSelectSubCatComboBox(int);
};

#endif // SECTORWEEKANALYSIS_H
