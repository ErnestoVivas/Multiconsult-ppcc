//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef SITEANALYSIS_H
#define SITEANALYSIS_H

#include <QWidget>
#include <QButtonGroup>
#include <QDebug>

namespace Ui {
class SiteAnalysis;
}

class SiteAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit SiteAnalysis(QWidget *parent = nullptr);
    ~SiteAnalysis();

    void addEntryComboBoxSelectDoc(const QString&);

    int selectedDocIndex;
    //int selectedSheetIndex;                             // not used anymore
    //void updateSheetList(const QString&);               // not used anymore
    void removeDocument(int);
    int getDay();
    int getVisType();

    // signals and slots to communicate with the
    // main window and handle user inputs
public slots:
    //void clearSheetList(int);
    void setCurrentDoc(int);

signals:
    void selectedDocChanged(int);

private:
    Ui::SiteAnalysis *ui;

    QButtonGroup visualizationGroup;
};

#endif // SITEANALYSIS_H
