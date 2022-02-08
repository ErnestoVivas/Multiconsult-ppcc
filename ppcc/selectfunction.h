#ifndef SELECTFUNCTION_H
#define SELECTFUNCTION_H

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>


namespace Ui {
class SelectFunction;
}

class SelectFunction : public QDialog {

    Q_OBJECT

public:
    explicit SelectFunction(QWidget *parent = nullptr);
    ~SelectFunction();

private:
    Ui::SelectFunction *ui;

    QList<QRadioButton*> functionButtons;
    QButtonGroup selectFunctionsGroup;

    void setupRadioButtons();
    void addRadioButtonsToFunctionsTable();

};

#endif // SELECTFUNCTION_H
