//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef SECTORSUBCATSANALYSIS_H
#define SECTORSUBCATSANALYSIS_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class SectorSubCatsAnalysis;
}

class SectorSubCatsAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit SectorSubCatsAnalysis(QWidget *parent = nullptr);
    ~SectorSubCatsAnalysis();

    void setupComboBoxSector();
    int getSelectedSector();
    QString getSelectedSectorString();
    int getDay();
    int getVisType();

    QList<QString> customSectors;

private:
    Ui::SectorSubCatsAnalysis *ui;

    QButtonGroup daysGroup;
    QButtonGroup visualizationGroup;
};

#endif // SECTORSUBCATSANALYSIS_H
