#ifndef EXCELCELL_H
#define EXCELCELL_H

#include <QAxObject>
#include <QString>

class ExcelCell
{
    //Q_OBJECT
public:
    explicit ExcelCell(QAxObject* qAxCell);
    QString cellText;
    bool isHeader;
    bool isEmpty;
    int color;
    //int headerLevel;
    ~ExcelCell();
signals:

public slots:
};

#endif // EXCELCELL_H
