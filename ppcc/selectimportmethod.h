#ifndef SELECTIMPORTMETHOD_H
#define SELECTIMPORTMETHOD_H

#include <QDialog>

namespace Ui {
class SelectImportMethod;
}

class SelectImportMethod : public QDialog
{
    Q_OBJECT

public:
    explicit SelectImportMethod(QWidget* parent = nullptr);
    SelectImportMethod(QWidget* parent, int&);
    ~SelectImportMethod();

private:
    Ui::SelectImportMethod *ui;
    int* currentImportMethod;

private slots:
    void importDocsSelected();
    void importDBSelected();
    void importTotalNatDocSelected();
};

#endif // SELECTIMPORTMETHOD_H
