//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef SECTORDAYANALYSIS_H
#define SECTORDAYANALYSIS_H

#include <QWidget>
#include <QButtonGroup>

#include "enumerations.h"

namespace Ui {
class SectorDayAnalysis;
}

class SectorDayAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit SectorDayAnalysis(QWidget *parent = nullptr);
    ~SectorDayAnalysis();

    int getSelectedSector();
    int getSelectedSubCat();
    QString getSelectedSectorString();
    QString getSelectedSubCatString();
    int getDay();
    int getVisType();

    void setupComboBoxSector();

    QList<QString> customSectors;
    QList<QString> customResRanges;
    QList<QString> customCommercials;
    QList<QString> customIndustrials;
    QList<QString> customSubSectors;

private:
    Ui::SectorDayAnalysis *ui;
    QButtonGroup visualizationGroup;

    Categories selectedCategories;



private slots:
    void updateSelectSubCatComboBox(int);
};

#endif // SECTORDAYANALYSIS_H
