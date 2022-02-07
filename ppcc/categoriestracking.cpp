#include "categoriestracking.h"

CategoriesTracking::CategoriesTracking() {

}

void CategoriesTracking::initCategories() {
    for(int i = 0; i < 6; ++i) {
        sectorsAndCount.emplace_back(0);
        industrialsAndCount.emplace_back(0);
    }
    for(int i = 0; i < 7; ++i) {
        resRangesAndCount.emplace_back(0);
    }
    for(int i = 0; i < 8; ++i) {
        commercialsAndCount.emplace_back(0);
    }
}

void CategoriesTracking::initComboBox(QComboBox* comboBox) {
    for(int i = 0; i < comboBox->count(); ++i) {
        this->setComboBoxItemEnabled(comboBox, i, false);
    }
    comboBox->setCurrentIndex(-1);
}


void CategoriesTracking::setComboBoxItemEnabled(QComboBox* comboBox, int itemIndex, bool enabled) {
    auto* model = qobject_cast<QStandardItemModel*>(comboBox->model());
    if(model == nullptr) {
        return;
    }
    auto* item = model->item(itemIndex);
    if(item == nullptr) {
        return;
    }
    item->setEnabled(enabled);
}

void CategoriesTracking::updateSectorComboBox(QComboBox* sectorComboBox) {

    sectorComboBox->clear();
    sectorComboBox->addItem("Residencial");
    sectorComboBox->addItem("Comercial");
    sectorComboBox->addItem("Industrial");
    sectorComboBox->addItem("Bombeo");
    sectorComboBox->addItem("Alumbrado público");
    sectorComboBox->addItem("Otros");

    // update standard sectors
    int firstValidIndex = -1;
    for(unsigned int i = 0; i < sectorsAndCount.size(); ++i) {
        if(sectorsAndCount[i] > 0) {
            this->setComboBoxItemEnabled(sectorComboBox, i, true);
            if(firstValidIndex == -1) {
                firstValidIndex = i;
            }
        } else {
            this->setComboBoxItemEnabled(sectorComboBox, i, false);
        }
    }

    // update custom sectors
    for(unsigned int i = 0; i < customSectorsAndCount.size(); ++i) {
        if(customSectorsAndCount[i].second > 0) {
            sectorComboBox->addItem(customSectorsAndCount[i].first);
            if(firstValidIndex == -1) {
                firstValidIndex = i + 6;
            }
        }
    }
    sectorComboBox->setCurrentIndex(firstValidIndex);
}

void CategoriesTracking::updateSubCatComboBox(int sectorIndex, QComboBox* subCatComboBox) {

    subCatComboBox->clear();
    if(sectorIndex == 0) {
        subCatComboBox->addItem("50 kWh/mes");
        subCatComboBox->addItem("51 - 100 kWh/mes");
        subCatComboBox->addItem("101 - 150 kWh/mes");
        subCatComboBox->addItem("151 - 250 kWh/mes");
        subCatComboBox->addItem("251 - 500 kWh/mes");
        subCatComboBox->addItem(">500 kWh/mes");
        subCatComboBox->addItem("Otros");
        this->updateResRangeComboBox(subCatComboBox);
    } else if(sectorIndex == 1) {
        subCatComboBox->addItem("Financieras");
        subCatComboBox->addItem("Comercio al por mayor");
        subCatComboBox->addItem("Comercio al por menor");
        subCatComboBox->addItem("Gobierno");
        subCatComboBox->addItem("Salud");
        subCatComboBox->addItem("Hoteles");
        subCatComboBox->addItem("Educación");
        subCatComboBox->addItem("Otros");
        this->updateCommercialComboBox(subCatComboBox);
    } else if(sectorIndex == 2) {
        subCatComboBox->addItem("Alimentos y bebidas");
        subCatComboBox->addItem("Papel, cartón");
        subCatComboBox->addItem("Caucho, plastico");
        subCatComboBox->addItem("Quimica");
        subCatComboBox->addItem("Textil");
        subCatComboBox->addItem("Otros");
        this->updateIndustrialComboBox(subCatComboBox);
    } else if(sectorIndex > 5) {
        subCatComboBox->addItem("Otros");
        this->updateCustomSubCatsComboBox(subCatComboBox);
    }
}



void CategoriesTracking::updateResRangeComboBox(QComboBox* resRangeComboBox) {

    int firstValidIndex = -1;

    // update standard res ranges
    for(unsigned int i = 0; i < resRangesAndCount.size(); ++i) {
        if(resRangesAndCount[i] > 0) {
            this->setComboBoxItemEnabled(resRangeComboBox, i, true);
            if(firstValidIndex == -1) {
                firstValidIndex = i;
            }
        } else {
            this->setComboBoxItemEnabled(resRangeComboBox, i, false);
        }
    }

    // add custom res ranges (funcion is only used for comboboxes that are being resetted)
    for(unsigned int i = 0; i < customResRangesAndCount.size(); ++i) {
        if(customResRangesAndCount[i].second > 0) {
            resRangeComboBox->addItem(customResRangesAndCount[i].first);
            if(firstValidIndex == -1) {
                firstValidIndex = i + 7;
            }
        }
    }
    resRangeComboBox->setCurrentIndex(firstValidIndex);
}

void CategoriesTracking::updateCommercialComboBox(QComboBox* commercialComboBox) {

    int firstValidIndex = -1;

    // update standard commercials
    for(unsigned int i = 0; i < commercialsAndCount.size(); ++i) {
        if(commercialsAndCount[i] > 0) {
            this->setComboBoxItemEnabled(commercialComboBox, i, true);
            if(firstValidIndex == -1) {
                firstValidIndex = i;
            }
        } else {
            this->setComboBoxItemEnabled(commercialComboBox, i, false);
        }
    }

    // add custom commercials
    for(unsigned int i = 0; i < customCommercialsAndCount.size(); ++i) {
        if(customCommercialsAndCount[i].second > 0) {
            commercialComboBox->addItem(customCommercialsAndCount[i].first);
            if(firstValidIndex == -1) {
                firstValidIndex = i + 8;
            }
        }
    }
    commercialComboBox->setCurrentIndex(firstValidIndex);
}

void CategoriesTracking::updateIndustrialComboBox(QComboBox* industrialComboBox) {

    int firstValidIndex = -1;

    // update standard industrials
    for(unsigned int i = 0; i < industrialsAndCount.size(); ++i) {
        if(industrialsAndCount[i] > 0) {
            this->setComboBoxItemEnabled(industrialComboBox, i, true);
            if(firstValidIndex == -1) {
                firstValidIndex = i;
            }
        } else {
            this->setComboBoxItemEnabled(industrialComboBox, i, false);
        }
    }

    // add custom industrials
    for(unsigned int i = 0; i < customIndustrialsAndCount.size(); ++i) {
        if(customIndustrialsAndCount[i].second > 0) {
            industrialComboBox->addItem(customIndustrialsAndCount[i].first);
            if(firstValidIndex == -1) {
                firstValidIndex = i + 6;
            }
        }
    }
    industrialComboBox->setCurrentIndex(firstValidIndex);
}

void CategoriesTracking::updateCustomSubCatsComboBox(QComboBox* customSubCatComboBox) {
    int firstValidIndex = -1;
    for(unsigned int i = 0; i < customSubCatsAndCount.size(); ++i) {
        if(customSubCatsAndCount[i].second > 0) {
            customSubCatComboBox->addItem(customSubCatsAndCount[i].first);
            if(firstValidIndex == -1) {
                firstValidIndex = i + 1;
            }
        }
    }
    customSubCatComboBox->setCurrentIndex(firstValidIndex);
}


void CategoriesTracking::updateStandardCategories(Categories &newDocCategories) {
    if(newDocCategories.sector != -2) {
        ++sectorsAndCount[newDocCategories.sector];
    }
    if((newDocCategories.resRange != -1) && (newDocCategories.resRange != -2)) {
        ++resRangesAndCount[newDocCategories.resRange];
    }
    if((newDocCategories.commercial != -1) && (newDocCategories.commercial != -2)) {
        ++commercialsAndCount[newDocCategories.commercial];
    }
    if((newDocCategories.industrial != -1) && (newDocCategories.industrial != -2)) {
        ++industrialsAndCount[newDocCategories.industrial];
    }
}


void CategoriesTracking::printCategories() {
    for(unsigned int i = 0; i < sectorsAndCount.size(); ++i) {
        qDebug() << enumerations::getStringFromSector(i) << ", num of docs: " << sectorsAndCount[i];
    }
    for(unsigned int i = 0; i < customSectorsAndCount.size(); ++i) {
        qDebug() << customSectorsAndCount[i].first << ", num of docs: " << customSectorsAndCount[i].second;
    }
    qDebug() << "\n";
    for(unsigned int i = 0; i < resRangesAndCount.size(); ++i) {
        qDebug() << enumerations::getStringFromSubSector(0, i) << ", num of docs: " << resRangesAndCount[i];
    }
    for(unsigned int i = 0; i < customResRangesAndCount.size(); ++i) {
        qDebug() << customResRangesAndCount[i].first << ", num of docs: " << customResRangesAndCount[i].second;
    }
    qDebug() << "\n";
    for(unsigned int i = 0; i < commercialsAndCount.size(); ++i) {
        qDebug() << enumerations::getStringFromSubSector(1, i) << ", num of docs: " << commercialsAndCount[i];
    }
    for(unsigned int i = 0; i < customCommercialsAndCount.size(); ++i) {
        qDebug() << customCommercialsAndCount[i].first << ", num of docs: " << customCommercialsAndCount[i].second;
    }
    qDebug() << "\n";
    for(unsigned int i = 0; i < industrialsAndCount.size(); ++i) {
        qDebug() << enumerations::getStringFromSubSector(2, i) << ", num of docs: " << industrialsAndCount[i];
    }
    for(unsigned int i = 0; i < customIndustrialsAndCount.size(); ++i) {
        qDebug() << customIndustrialsAndCount[i].first << ", num of docs: " << customIndustrialsAndCount[i].second;
    }
    qDebug() << "\n";
    for(unsigned int i = 0; i < customSubCatsAndCount.size(); ++i) {
        qDebug() << customSubCatsAndCount[i].first << ", num of docs: " << customSubCatsAndCount[i].second;
    }
}
