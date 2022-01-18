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
    int getDay();
    int getVisType();

private:
    Ui::SectorDayAnalysis *ui;
    QButtonGroup visualizationGroup;

    Categories selectedCategories;

    void setupComboBoxSector();

private slots:
    void updateSelectSubCatComboBox(int);
};

#endif // SECTORDAYANALYSIS_H
