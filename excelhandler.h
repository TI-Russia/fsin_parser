#ifndef EXCELHANDLER_H
#define EXCELHANDLER_H

#include <QObject>
#include <QAxObject>
#include <QXmlStreamWriter>
#include "excelcell.h"
#include "person.h"

class ExcelHandler : public QObject{
    Q_OBJECT
    public:
        //QString pathToFile;
        //Q_DISABLE_COPY(ExcelHandler)
        explicit ExcelHandler(QObject *parent = 0);
        void openExcelFile(QString pathToFile);
        void getHeaderPositions();
        void getPersonsBounds();
        ExcelCell* getExcelCell(QString cellNum);
        void getAllPersonProperites(int personStart, int personEnd);
        Person buildSinglePerson(QPair<int, int> bounds);
        QList<Person> buildPersonObjects(QList<QPair<int,int>> bounds);
        void personListToXML(QList<Person> personList);
        void singlePersonToXML(Person person);
        void startXMLDocument();
        void finalizeXMLDocument();
        void closeExcelFile();
        // temporary
        void personBoundsToFile();
        void printRowRanges();
        void buildOrganList();
        QList<QPair<int,int>> personBounds;
        QList<QPair<int,int>> organsBounds;
        QList<QPair<QString,QPair<int,int>>> organsBoundsList;
        void buildOrgansXMLs();
        void getOrgansBounds();
        ~ExcelHandler();
    private:
        QAxObject* excelWorkbooks;
        QAxObject* excelFile;
        QString usedColumns;
        QList<QString> relationTypes;
        QList<QString> objectTypes;
        QList<QString> ownershipTypes;
        QList<QString> countries;
        int personsTableStart;
        int countRowsInColumn(QString columnKey);
        int countUsedRows();
        QString cellAddress;
        QList<int> headerPositions;
        QString currentOrgan;
        QList<Person> persons;
        QList<Person> organPersons;
        QXmlStreamWriter* xmlWriter;
        QString xmlOutput;
    signals:

    public slots:
};

#endif // EXCELHANDLER_H
