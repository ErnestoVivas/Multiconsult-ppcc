//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

#include <QString>

// enumerations to classify measurements
enum SystemVoltage {
    lowVoltage = 0,
    mediumVoltage = 1,
    highVoltage = 2
};

enum Sector {
    residential = 0,
    commercial = 1,
    industrial = 2,
    pumping = 3,
    publicLighting = 4
};

enum ResidentialRange {
    A = 0,                  // <= 50 kWh/month
    B = 1,                  // 51 to 100 kWh/month
    C = 2,                  // 101 to 150 kWh/month
    D = 3,                  // 151 to 250 kWh/month
    E = 4,                  // 251 to 500 kWh/month
    F = 5,                  // > 500 kWh/month
    X = -1                  // site is not residential
};

enum Commercial {
    finances = 0,
    wholesale = 1,
    retail = 2,
    government = 3,
    health = 4,
    hotels = 5,
    education = 6,
    otherCommercial = 7,
    notCommercial = -1
};

enum Industrial {
    food = 0,
    paper = 1,
    plastics = 2,
    chemistry = 3,
    textiles = 4,
    otherIndustrial = 5,
    notIndustrial = -1
};

enum Frequency {
    quarterHour = 0,
    hour = 1
};

enum DateFormat {
    dayMonthYear = 0,
    monthDayYear = 1,
    yearMonthDay = 2,
    yearDayMonth = 3
};

struct Categories {
    SystemVoltage systemVoltage;
    Sector sector;
    ResidentialRange resRange;
    Commercial commercial;
    Industrial industrial;
    Frequency frequency;
    DateFormat dateFormat;
};

namespace enumerations {
    QString getStringFromSector(int);
    QString getStringFromSubSector(int, int);
    QString getStringFromFreq(int);
    QString getStringFromDay(int);
}

#endif // ENUMERATIONS_H
