//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "enumerations.h"

Sector enumerations::getSectorFromInt(int numericSector) {
    Sector sector = Sector::otherSector;
    if(numericSector == 0) {
        sector = Sector::residential;
    } else if(numericSector == 1) {
        sector = Sector::commercial;
    } else if(numericSector == 2) {
        sector = Sector::industrial;
    } else if(numericSector == 3) {
        sector = Sector::pumping;
    } else if(numericSector == 4) {
        sector = Sector::publicLighting;
    } else if(numericSector == 5) {
        sector = Sector::otherSector;
    } else if(numericSector == -2) {
        sector = Sector::customSector;
    }
    return sector;
}

ResidentialRange enumerations::getResRangeFromInt(int numericResRange) {
    ResidentialRange resRange = ResidentialRange::otherResRange;
    if(numericResRange == 0) {
        resRange = ResidentialRange::A;
    } else if(numericResRange == 1) {
        resRange = ResidentialRange::B;
    } else if(numericResRange == 2) {
        resRange = ResidentialRange::C;
    } else if(numericResRange == 3) {
        resRange = ResidentialRange::D;
    } else if(numericResRange == 4) {
        resRange = ResidentialRange::E;
    } else if(numericResRange == 5) {
        resRange = ResidentialRange::F;
    } else if(numericResRange == 6) {
        resRange = ResidentialRange::otherResRange;
    } else if(numericResRange == -2) {
        resRange = ResidentialRange::customResRange;
    } else if(numericResRange == -1) {
        resRange = ResidentialRange::X;
    }
    return resRange;
}

Commercial enumerations::getCommercialFromInt(int numericCommercial) {
    Commercial commercial = Commercial::otherCommercial;
    if(numericCommercial == 0) {
        commercial = Commercial::finances;
    } else if(numericCommercial == 1) {
        commercial = Commercial::wholesale;
    } else if(numericCommercial == 2) {
        commercial = Commercial::retail;
    } else if(numericCommercial == 3) {
        commercial = Commercial::government;
    } else if(numericCommercial == 4) {
        commercial = Commercial::health;
    } else if(numericCommercial == 5) {
        commercial = Commercial::hotels;
    } else if(numericCommercial == 6) {
        commercial = Commercial::education;
    } else if(numericCommercial == 7) {
        commercial = Commercial::otherCommercial;
    } else if(numericCommercial == -2) {
        commercial = Commercial::customCommercial;
    } else if(numericCommercial == -1) {
        commercial = Commercial::notCommercial;
    }
    return commercial;
}

Industrial enumerations::getIndustrialFromInt(int numericIndustrial) {
    Industrial industrial = Industrial::otherIndustrial;
    if(numericIndustrial == 0) {
        industrial = Industrial::food;
    } else if(numericIndustrial == 1) {
        industrial = Industrial::paper;
    } else if(numericIndustrial == 2) {
        industrial = Industrial::plastics;
    } else if(numericIndustrial == 3) {
        industrial = Industrial::chemistry;
    } else if(numericIndustrial == 4) {
        industrial = Industrial::textiles;
    } else if(numericIndustrial == 5) {
        industrial = Industrial::otherIndustrial;
    } else if(numericIndustrial == -2) {
        industrial = Industrial::customIndustrial;
    } else if(numericIndustrial == -1) {
        industrial = Industrial::notIndustrial;
    }
    return industrial;
}

CustomSubSector enumerations::getCustomSubSecFromInt(int numericCustomSubSec) {
    CustomSubSector customSubSec = CustomSubSector::otherCustomSubSector;
    if(numericCustomSubSec == 0) {
        customSubSec = CustomSubSector::otherCustomSubSector;
    } else if(numericCustomSubSec == -2) {
        customSubSec = CustomSubSector::customSubSector;
    } else if(numericCustomSubSec == -1) {
        customSubSec = CustomSubSector::notCustomSubSector;
    }
    return customSubSec;
}

QString enumerations::getStringFromSector(int sector) {
    QString sectorString = "";
    if(sector == 0) {
        sectorString = "Residencial";
    } else if(sector == 1) {
        sectorString = "Comercial";
    } else if(sector == 2) {
        sectorString = "Industrial";
    } else if(sector == 3) {
        sectorString = "Bombeo";
    } else if(sector == 4) {
        sectorString = "Alumbrado público";
    } else if(sector == 5) {
        sectorString = "Otros";
    } else if(sector == -2) {
        sectorString = "Personalizado";
    }
    return sectorString;
}

QString enumerations::getStringFromSubSector(int sector, int subSector) {
    QString subSectorString = "";
    if(sector == 0) {
        if(subSector == 0) {
            subSectorString = "50 kWh/mes";
        } else if(subSector == 1) {
            subSectorString = "51 - 100 kWh/mes";
        } else if(subSector == 2) {
            subSectorString = "101 - 150 kWh/mes";
        } else if(subSector == 3) {
            subSectorString = "151 - 250 kWh/mes";
        } else if(subSector == 4) {
            subSectorString = "251 - 500 kWh/mes";
        } else if(subSector == 5) {
            subSectorString = ">500 kWh/mes";
        } else if(subSector == 6) {
            subSectorString = "Otros";
        } else if(subSector == -2) {
            subSectorString = "Personalizado";
        }
    } else if(sector == 1) {
        if(subSector == 0) {
            subSectorString = "Financieras";
        } else if(subSector == 1) {
            subSectorString = "Comercio al por mayor";
        } else if(subSector == 2) {
            subSectorString = "Comercio al por menor";
        } else if(subSector == 3) {
            subSectorString = "Gobierno";
        } else if(subSector == 4) {
            subSectorString = "Salud";
        } else if(subSector == 5) {
            subSectorString = "Hoteles";
        } else if(subSector == 6) {
            subSectorString = "Educación";
        } else if(subSector == 7) {
            subSectorString = "Otros";
        } else if(subSector == -2) {
            subSectorString = "Personalizado";
        }
    } else if(sector == 2) {
        if(subSector == 0) {
            subSectorString = "Alimentos y bebidas";
        } else if(subSector == 1) {
            subSectorString = "Papel, cartón";
        } else if(subSector == 2) {
            subSectorString = "Caucho, plástico";
        } else if(subSector == 3) {
            subSectorString = "Química";
        } else if(subSector == 4) {
            subSectorString = "Textil";
        } else if(subSector == 5) {
            subSectorString = "Otros";
        } else if(subSector == -2) {
            subSectorString = "Personalizado";
        }
    } else if(sector == -2) {
        if(subSector == 0) {
            subSectorString = "Otros";
        } else if(subSector == -2) {
            subSectorString = "Personalizado";
        }
    }
    return subSectorString;
}

QString enumerations::getStringFromFreq(int freq) {
    QString freqString = "";
    if(freq == 0) {
        freqString = "15 min";
    } else if(freq == 1) {
        freqString = "1 h";
    }
    return freqString;
}

QString enumerations::getStringFromDay(int day) {
    QString dayString = "";
    if(day == 1) {
        dayString = "lunes";
    } else if(day == 2) {
        dayString = "martes";
    } else if(day == 3) {
        dayString = "miércoles";
    } else if(day == 4) {
        dayString = "jueves";
    } else if(day == 5) {
        dayString = "viernes";
    } else if(day == 6) {
        dayString = "sábado";
    } else if(day == 7) {
        dayString = "domingo";
    }
    return dayString;
}
