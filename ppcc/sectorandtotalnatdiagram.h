#ifndef SECTORANDTOTALNATDIAGRAM_H
#define SECTORANDTOTALNATDIAGRAM_H

#include <QWidget>
#include <QButtonGroup>
#include <QComboBox>

namespace Ui {
class SectorAndTotalNatDiagram;
}

class SectorAndTotalNatDiagram : public QWidget
{
    Q_OBJECT

public:
    explicit SectorAndTotalNatDiagram(QWidget *parent = nullptr);
    ~SectorAndTotalNatDiagram();

    void setTotalNatDoc(QString totalNatDocFileName);
    QComboBox* getSectorComboBox();
    int getDay();
    int getSelectedSector();
    QString getSelectedSectorString();
    bool getShowSubCats();

private:
    Ui::SectorAndTotalNatDiagram *ui;

    QButtonGroup dayTypeGroup;
};

#endif // SECTORANDTOTALNATDIAGRAM_H
