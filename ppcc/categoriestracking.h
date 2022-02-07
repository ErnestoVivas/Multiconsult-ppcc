#ifndef CATEGORIESTRACKING_H
#define CATEGORIESTRACKING_H

#include <vector>

#include <QString>
#include <QComboBox>
#include <QStandardItemModel>
#include <QDebug>
#include <QtCore/QObject>

#include "enumerations.h"

struct CategoriesTracking {

    CategoriesTracking();

    // tracking of sectors
    std::vector<int> sectorsAndCount;
    std::vector<std::pair<QString, int> > customSectorsAndCount;

    // tracking of subcatgeories
    std::vector<int> resRangesAndCount;
    std::vector<int> commercialsAndCount;
    std::vector<int> industrialsAndCount;

    std::vector<std::pair<QString, int> > customResRangesAndCount;
    std::vector<std::pair<QString, int> > customCommercialsAndCount;
    std::vector<std::pair<QString, int> > customIndustrialsAndCount;
    std::vector<std::pair<QString, int> > customSubCatsAndCount;

    // functions to update and keep track of the existing categories
    void initCategories();
    void initComboBox(QComboBox*);
    void setComboBoxItemEnabled(QComboBox*, int, bool);
    void updateSectorComboBox(QComboBox*);
    void updateResRangeComboBox(QComboBox*);
    void updateCommercialComboBox(QComboBox*);
    void updateIndustrialComboBox(QComboBox*);
    void updateCustomSubCatsComboBox(QComboBox*);

    void updateStandardCategories(Categories&);
    void printCategories();


    void updateSubCatComboBox(int, QComboBox*);
};

#endif // CATEGORIESTRACKING_H
