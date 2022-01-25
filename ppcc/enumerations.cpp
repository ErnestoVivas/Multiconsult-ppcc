//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "enumerations.h"

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
