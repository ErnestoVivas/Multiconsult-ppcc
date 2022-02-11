#ifndef SELECTFUNCTION_H
#define SELECTFUNCTION_H

#include <QDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

namespace Ui {
class SelectFunction;
}

class SelectFunction : public QDialog {

    Q_OBJECT

public:
    SelectFunction(QWidget *parent = nullptr);
    SelectFunction(QWidget *parent, int& );
    ~SelectFunction();

private:
    Ui::SelectFunction *ui;
    int* indexRadioButton;
    int currentFunction;
    QList<QRadioButton*> functionButtons;
    QButtonGroup selectFunctionsGroup;

    void setupRadioButtons();
    void addRadioButtonsToFunctionsTable();

private slots:
    void exitDialog();
};

#endif // SELECTFUNCTION_H
