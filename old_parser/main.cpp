#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QAxObject>
#include "excelhandler.h"
#include <QDebug>
#include <QXmlStreamWriter>
#include <QFile>

#include "person.h"
#include "webpagehandler.h"

// temporary fix to disable the "variable initialized but not referenced" warning
#pragma warning(disable:4189)

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    ExcelHandler* excelHandler = new ExcelHandler();
    excelHandler->openExcelFile(QString("C:/excel_file.xls"));

    QString cellNum = "A31866";

    ExcelCell* singleCell = excelHandler->getExcelCell(cellNum);
    //qDebug() << "retrieved cell: " << singleCell;
    //qDebug() << "address: " << cellNum << " isEmpty: " << singleCell->isEmpty << " isHeader: " << singleCell->isHeader;

    //QList<int> headers;
    //QList<QPair<int,int>> personBounds;
    //excelHandler->getHeaderPositions();
    //excelHandler->getPersonsBounds();
    //excelHandler->buildPersonsPropertiesList();

    //excelHandler->printRowRanges(list);
    //excelHandler->getAllPersonProperites(8,8);


    /*
    excelHandler->startXMLDocument();
    QPair<int,int> singlePerson;
    singlePerson.first = 91;
    singlePerson.second = 92;
    qDebug() << "For person from rows: " << singlePerson.first << " to " << singlePerson.second;
    Person singlePersonObject = excelHandler->buildSinglePerson(singlePerson);
    excelHandler->singlePersonToXML(singlePersonObject);

    singlePerson.first = 93;
    singlePerson.second = 93;
    qDebug() << "For person from rows: " << singlePerson.first << " to " << singlePerson.second;
    singlePersonObject = excelHandler->buildSinglePerson(singlePerson);
    excelHandler->singlePersonToXML(singlePersonObject);

    singlePerson.first = 94;
    singlePerson.second = 94;
    qDebug() << "For person from rows: " << singlePerson.first << " to " << singlePerson.second;
    singlePersonObject = excelHandler->buildSinglePerson(singlePerson);
    excelHandler->singlePersonToXML(singlePersonObject);

    singlePerson.first = 95;
    singlePerson.second = 95;
    qDebug() << "For person from rows: " << singlePerson.first << " to " << singlePerson.second;
    singlePersonObject = excelHandler->buildSinglePerson(singlePerson);
    excelHandler->singlePersonToXML(singlePersonObject);

    excelHandler->finalizeXMLDocument();



    //WebPageHandler* page = new WebPageHandler();
    excelHandler->closeExcelFile();
    */

    excelHandler->getPersonsBounds();
    excelHandler->getOrgansBounds();
    excelHandler->buildOrganList();
    excelHandler->buildOrgansXMLs();

    //excelHandler->startXMLDocument();
    //excelHandler->getPersonsBounds();
    //excelHandler->buildPersonObjects();
    //excelHandler->personListToXML();
    //excelHandler->finalizeXMLDocument();

    //excelHandler->getPersonsBounds();
    //excelHandler->buildSinglePerson(excelHandler->personBounds.at(13624));
    QLabel label("There are organs: ");
    //QLabel label("There are organs: " + QString::number(singleCell->color));
    label.show();

    //delete excelHandler;
    //delete singleCell;
    return a.exec();
}
