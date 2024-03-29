//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//  Author: Ernesto Vivas, Multiconsult Nicaragua
//  Date: January 2022
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef EXPORTDIAGRAMDIALOG_H
#define EXPORTDIAGRAMDIALOG_H

#include <QDialog>

namespace Ui {
class ExportDiagramDialog;
}

class ExportDiagramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDiagramDialog(QWidget *parent = nullptr);
    ExportDiagramDialog(QWidget *parent, short&);
    ~ExportDiagramDialog();

private:
    Ui::ExportDiagramDialog *ui;

    short* exportMethod;

private slots:
    void imageSelected();
    void excelSelected();
};

#endif // EXPORTDIAGRAMDIALOG_H
