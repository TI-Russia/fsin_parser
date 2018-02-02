#pragma once
#include <QList>
#include <QHash>

#include "person.h"
#include "organization.h"
#include "excel_adapter.h"

class QXmlStreamWriter;

class Parser
{
public:
    Parser(const ExcelAdapter& adapter);

    void Process();

    void getHeaderPositions();
    void getPersonsBounds();
    void getAllPersonProperites(int personStart, int personEnd);
    Person buildSinglePerson(QPair<int, int> bounds);
    QList<Person> buildPersonObjects(QList<QPair<int, int>> bounds);

    void personListToXML(QList<Person> personList);
    void singlePersonToXML(Person person);
    void startXMLDocument();
    void finalizeXMLDocument();
    void buildOrgansXMLs();

    // temporary
    void personBoundsToFile();
    void printRowRanges();
    void buildOrganList();
    QList<QPair<int, int>> personBounds;
    QList<QPair<int, int>> organsBounds;
    QList<QPair<QString, QPair<int, int>>> organsBoundsList;
    QList<Organization> organizations;
    void getOrgansBounds();
private:
    QString usedColumns;
    QList<QString> relationTypes;
    QList<QString> objectTypes;
    //QList<QString> ownershipTypes;
    QHash<QString, int> ownershipTypes;
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

    Organization currentOrg;


    const ExcelAdapter& m_adapter;
    int errorCount = 0;

};