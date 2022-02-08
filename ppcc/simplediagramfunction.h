//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef SIMPLEDIAGRAMFUNCTION_H
#define SIMPLEDIAGRAMFUNCTION_H

#include <QWidget>
#include <vector>


namespace Ui {
class SimpleDiagramFunction;
}

class SimpleDiagramFunction : public QWidget
{
    Q_OBJECT

public:

    // constructor and destructor
    explicit SimpleDiagramFunction(QWidget *parent = nullptr);
    ~SimpleDiagramFunction();

    // variables and functions that will be accessed from main window
    int selectedDocIndex;
    int selectedSheetIndex;
    void addEntryComboBoxSelectDoc(const QString&);
    void updateDays(const QString&);
    void removeDocument(int);
    std::vector<int> getSelectedDays();


public slots:

    // signals and slots to communicate with the main window and handle user inputs
    void clearDaysEntries(int);

signals:
    void selectedDocChanged(int);

private:
    Ui::SimpleDiagramFunction *ui;
};

#endif // SIMPLEDIAGRAMFUNCTION_H
