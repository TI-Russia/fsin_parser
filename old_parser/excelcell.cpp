#include "excelcell.h"
#include <QDebug>

ExcelCell::ExcelCell(QAxObject* qAxCell){
    cellText = qAxCell->property("Value").toString().trimmed();
    isEmpty = cellText.isEmpty();
    color = qAxCell->querySubObject("Interior")->property("ColorIndex").toInt();
    isHeader = qAxCell->property("MergeCells").toBool();
    //isHeader = ( (color == 33 || color == 15 || color == 48 ) ? true : false);
}
ExcelCell::~ExcelCell(){

}


