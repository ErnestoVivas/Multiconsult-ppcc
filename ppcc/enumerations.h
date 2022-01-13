#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

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
    otherCommercial = 7
};

enum Industrial {
    food = 0,
    paper = 1,
    plastics = 2,
    chemistry = 3,
    textiles = 4,
    otherIndustrial = 5
};

#endif // ENUMERATIONS_H
