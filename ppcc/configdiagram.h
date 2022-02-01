//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef CONFIGDIAGRAM_H
#define CONFIGDIAGRAM_H


#include <QDialog>
#include <QtCharts>
#include <QtCharts/QLineSeries>

namespace Ui {
class ConfigDiagram;
}

class ConfigDiagram : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDiagram(QWidget *parent = nullptr);
    ConfigDiagram(QWidget* parent, QChart*, QValueAxis*, QValueAxis*, QList<QLineSeries*>*);
    ~ConfigDiagram();

private:
    Ui::ConfigDiagram *ui;
    QChart* measurementsChart;
    QValueAxis* xAxis;
    QValueAxis* yAxis;
    QList<QLineSeries*>* displayedSeries;

private slots:
    void updateAndExit();
    void cancelAndExit();
};

#endif // CONFIGDIAGRAM_H
