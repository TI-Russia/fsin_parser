#include <QDebug>
#include <QDir>
#include <QFile>
#include <QXmlStreamWriter>

#include "parser.h"
#include "organization.h"

void Parser::Process()
{
    getPersonsBounds();
    getOrgansBounds();
    buildOrganList();
    buildOrgansXMLs();
}

int Parser::countUsedRows()
{
    return m_adapter.countUsedRows();
}

void Parser::getPersonsBounds() 
{
    QList<int> personStarts;
    int personsTableEnd = countUsedRows();
    //int personsTableEnd = 231;
    for (int i = personsTableStart; i <= personsTableEnd; i++) {
        cellAddress = "A" + QString::number(i);
        //qDebug() << "person discovery - processing: " << cellAddress;
        ExcelCell currentCell = m_adapter.getExcelCell(cellAddress);
        if (!currentCell.isEmpty && !currentCell.isHeader) {
            personStarts.append(i);
        }
        else if (currentCell.isHeader) {
            headerPositions.append(i);
        }
    }
    int personStart;
    int personEnd;
    for (int i = 0; i < personStarts.length() - 1; i++) {
        personStart = personStarts.at(i);
        personEnd = personStarts.at(i + 1) - 1;
        for (int j = personStarts.at(i); j < personStarts.at(i + 1); j++) {
            if (headerPositions.indexOf(j) != -1) {
                personEnd = j - 1;
            }
        }
        personBounds.append(QPair<int, int>(personStart, personEnd));
    }
    personStart = personStarts.at(personStarts.length() - 1);
    personEnd = personsTableEnd;
    personBounds.append(QPair<int, int>(personStart, personEnd));
    personStarts.clear();
}

void Parser::getOrgansBounds() {
    QPair<int, int> pair;
    for (int i = 0; i < headerPositions.length() - 1; i++) {
        qDebug() << i + 1 << " organ start: " << headerPositions.at(i) << "organ end: " << headerPositions.at(i + 1) - 1;
        //qDebug() << "this: " << getExcelCell("A"+QString::number(headerPositions.at(i)))->cellText << "next: " << getExcelCell("A"+QString::number(headerPositions.at(i+1)))->cellText;
        pair.first = headerPositions.at(i) + 1;
        pair.second = headerPositions.at(i + 1) - 1;
        organsBounds.append(pair);
    }
    pair.first = headerPositions.at(headerPositions.length() - 1) + 1;
    pair.second = countUsedRows();
    organsBounds.append(pair);
    qDebug() << headerPositions.length() << " organ start: " << headerPositions.at(headerPositions.length() - 1) << "organ end: " << countUsedRows();
}

void Parser::buildOrganList() 
{
    QString topOrgName;
    QListIterator<QPair<int, int>> organIter(organsBounds);
    while (organIter.hasNext()) {
        QPair<int, int> organPair = organIter.next();
        QPair<int, int> organPersonBounds;
        QPair<QString, QPair<int, int>> organBoundsWithName;
        // перебираем все персоны по порядку
        int personNum = 0;
        int firstPerson = 0;
        int lastPerson = -1;
        bool topLevel = false;
        QString orgName;
        QListIterator<QPair<int, int>> personIter(personBounds);
        while (personIter.hasNext()) {
            QPair<int, int> personPair = personIter.next();
            //qDebug() << "examining person " << personPair.first << " to " << personPair.second;
            // персона в начале организации
            if (organPair.first == personPair.first) {
                organPersonBounds.first = personNum;
                firstPerson = personNum;
                ExcelCell header = m_adapter.getExcelCell("A" + QString::number(organPair.first - 1));
                organBoundsWithName.first = header.cellText();
                orgName = header.cellText();
                topLevel = header.foreground_color == "ff00b0f0";
                if (topLevel)
                {
                    topOrgName = orgName;
                }
                orgName = header.cellText();
                qDebug() << "ORGAN: #" << organPair.first - 1 << " " << header.cellText() << " color bg: " << header.background_color.c_str() << " color fg: " << header.foreground_color.c_str();
            }
            // персона в конце организации
            if (organPair.second == personPair.second) {
                organPersonBounds.second = personNum;
                lastPerson = personNum;
                organBoundsWithName.second = organPersonBounds;
                organsBoundsList.append(organBoundsWithName);
                organizations.append(Organization(orgName, topOrgName, firstPerson, lastPerson, topLevel));
                break;
            }
            personNum++;
        }
    }
}

QList<Person> Parser::buildPersonObjects(QList<QPair<int, int>> bounds) {
    QList<Person> persons;
    QListIterator<QPair<int, int>> iter(bounds);
    int current = 1;
    int all = personBounds.count();
    while (iter.hasNext()) {
        QPair<int, int> currentPair = iter.next();
        //qDebug() << "building person objects: " + QString::number(current) + " of " + QString::number(all);
        persons.append(buildSinglePerson(currentPair));
        current++;
    }
    return persons;
}

Person Parser::buildSinglePerson(QPair<int, int> bounds) {
    //qDebug() << "buildSinglePerson init: bounds " + QString::number(bounds.first) + " - " + QString::number(bounds.second);
    Person person;
    QString personName = m_adapter.getExcelCell("A" + QString::number(bounds.first)).cellText();
    person.setId(organPersons.length() + 1);
    if (relationTypes.indexOf(personName) == -1) {
        person.setName(personName);
    }
    else {
        person.setRelationType(relationTypes.indexOf(personName));
        QListIterator<Person> personIterator(organPersons);
        personIterator.toBack();
        while (personIterator.hasPrevious()) {
            Person currentPerson = personIterator.previous();
            if (!currentPerson.getName().isNull()) {
                person.setRelativeOf(currentPerson.getId());
                break;
            }
        }
    }
    person.setPosition(m_adapter.getExcelCell("B" + QString::number(bounds.first)).cellText() + " " + currentOrgan);
    //ExcelCell cell;
    for (int i = bounds.first; i <= bounds.second; i++) {
        ExcelCell cell = m_adapter.getExcelCell("C" + QString::number(i));
        if (!cell.isEmpty) {
            //Realty* realty = new Realty();
            Realty realty;
            realty.setRealtyName(cell.cellText());
            realty.setRealtyType(1);
            int objectType = 0;
            if (objectTypes.indexOf(cell.cellText()) != -1) {
                objectType = objectTypes.indexOf(cell.cellText());
            }
            realty.setObjectType(objectType);
            cell = m_adapter.getExcelCell("D" + QString::number(i));
            int ownershipType = 0;
            double ownershipPart = 0;
            QString text = cell.cellText();
            if (!ownershipTypes.contains(text)) {
                //if (text.indexOf("-") != -1) {
                //    QString option = text.split("-").at(0).trimmed();
                //    if (option == QString(u8"общая долевая")) {
                //        ownershipType = 3;
                //        QString ownershipPartText = text.split("-").at(1).trimmed();
                //        if (ownershipPartText.indexOf("/") != -1) {
                //            ownershipPart = ownershipPartText.split("/").at(0).toDouble() / ownershipPartText.split("/").at(1).toDouble();
                //        }
                //    }
                //}
                //else {
                //    ownershipType = 0;
                //}
                QStringList parts = text.split(" ");
                if (parts.size() == 2 && parts.at(0) == QString(u8"долевая"))
                {
                    ownershipType = 3;
                    QStringList fraction = parts.at(1).split("/");
                    if (fraction.size() == 2)
                    {
                        ownershipPart = fraction.at(0).toDouble() / fraction.at(1).toDouble();
                    }
                    else
                    {
                        qDebug() << "error: cannot parse ownership text " << text;
                        errorCount++;
                    }
                }
                else
                {
                    qDebug() << "error: cannot parse ownership text " << text;
                    errorCount++;
                }
            }
            else {
                ownershipType = ownershipTypes.value(text);
            }
            realty.setOwnershipPart(ownershipPart);
            realty.setOwnershipType(ownershipType);
            realty.setOwnershipText(text);
            cell = m_adapter.getExcelCell("E" + QString::number(i));
            QString number = cell.cellText().split(" ").join("");
            number = number.replace(QString(","), QString("."));
            double square = number.toDouble();
            realty.setSquare(square);
            cell = m_adapter.getExcelCell("F" + QString::number(i));
            int country;
            if (!cell.isEmpty && countries.indexOf(cell.cellText()) != -1) {
                country = countries.indexOf(cell.cellText());
            }
            else {
                country = 0;
            }
            realty.setCountry(country);
            person.addRealty(realty);
        }
        else {
            break;
        }
    }
    for (int i = bounds.first; i <= bounds.second; i++) {
        ExcelCell cell = m_adapter.getExcelCell("G" + QString::number(i));
        if (!cell.isEmpty) {
            Realty realty;
            realty.setRealtyName(cell.cellText());
            realty.setRealtyType(2);
            int objectType = 0;
            if (objectTypes.indexOf(cell.cellText()) != -1) {
                objectType = objectTypes.indexOf(cell.cellText());
            }
            realty.setObjectType(objectType);
            cell = m_adapter.getExcelCell("H" + QString::number(i));
            QString number = cell.cellText().split(" ").join("");
            number = number.replace(QString(","), QString("."));
            double square = cell.cellText().toDouble();
            realty.setSquare(square);
            cell = m_adapter.getExcelCell("I" + QString::number(i));
            int country;
            if (!cell.isEmpty && countries.indexOf(cell.cellText()) != -1) {
                country = countries.indexOf(cell.cellText());
            }
            else {
                country = 0;
            }
            realty.setCountry(country);
            person.addRealty(realty);
        }
        else {
            break;
        }
    }
    for (int i = bounds.first; i <= bounds.second; i++) {
        ExcelCell cell = m_adapter.getExcelCell("J" + QString::number(i));
        if (!cell.isEmpty) {
            Transport transport;
            transport.setName(cell.cellText());
            person.addTransport(transport);
        }
        else {
            break;
        }
    }
    ExcelCell cell = m_adapter.getExcelCell("K" + QString::number(bounds.first));
    QString incomeText = cell.cellText();
    if (incomeText.indexOf("(") != -1) {
        int leftBrace = incomeText.indexOf("(");
        int rightBrace = incomeText.indexOf(")");
        int length = rightBrace - leftBrace - 1;
        QString incomeComment = incomeText.mid(leftBrace + 1, length);
        person.setIncomeComment(incomeComment);
        QString income = incomeText.left(leftBrace).trimmed();
        income = income.replace(QString(","), QString("."));
        person.setIncome(income.toDouble());
    }
    else {
        incomeText = incomeText.trimmed();
        incomeText = incomeText.split(" ").join("");
        incomeText = incomeText.replace(QString(","), QString("."));
        person.setIncome(incomeText.toDouble());
    }
    cell = m_adapter.getExcelCell("L" + QString::number(bounds.first));
    person.setIncomeSource(cell.cellText());
    organPersons.append(person);
    return person;
}



void Parser::buildOrgansXMLs() {
    qDebug() << "building organs XMLs with organsBoundsList length: " << organsBoundsList.length();
    //QListIterator<QPair<QString, QPair<int, int>>> iter(organsBoundsList);
    QListIterator<Organization> iter(organizations);
    while (iter.hasNext()) {
        organPersons.clear();
        //QPair<QString, QPair<int, int>> pair = iter.next();
        //currentOrgan = pair.first;
        currentOrg = iter.next();
        if (currentOrg.name.isEmpty()) {
            qDebug() << "Error: empty org";
            continue;
        }
        QList<QPair<int, int>> personsBounds;
        //for (int i = pair.second.first; i <= pair.second.second; i++) {
        for (int i = currentOrg.person_first; i <= currentOrg.person_last; i++) {
            personsBounds.append(personBounds.at(i));
        }
        QList<Person> personsList = buildPersonObjects(personsBounds);
        startXMLDocument();
        personListToXML(personsList);
        finalizeXMLDocument();


        //qDebug() << "the organ is: " << pair.first;
        //qDebug() << "starts with person: " << pair.second.first;
        //qDebug() << "ends with person: " << pair.second.second;
        qDebug() << "the organ is: " << currentOrg.name;
        qDebug() << "starts with person: " << currentOrg.person_first;
        qDebug() << "ends with person: " << currentOrg.person_last;
    }
    qDebug() << u8"Процесс конвертации завершён";
    qDebug() << "total persons: " << personBounds.length();
    qDebug() << "total errors: " << errorCount;
}


void Parser::startXMLDocument() {
    xmlOutput = "";
    xmlWriter = new QXmlStreamWriter(&xmlOutput);
    QString schemaPath = "declarationXMLtemplate_Schema _transport_merged.xsd";
    xmlWriter->setAutoFormatting(true);
    xmlWriter->setCodec("UTF-8");
    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement("persons");
    xmlWriter->writeAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    xmlWriter->writeAttribute("xsi:noNamespaceSchemaLocation", schemaPath);
}
void Parser::singlePersonToXML(Person person) {
    xmlWriter->writeStartElement("person");
    xmlWriter->writeStartElement("id");
    xmlWriter->writeCharacters(QString::number(person.getId()));
    xmlWriter->writeEndElement();
    qDebug() << "writing XML";
    qDebug() << "id: " << person.getId();

    if (!person.getName().isNull()) {
        xmlWriter->writeStartElement("name");
        xmlWriter->writeCharacters(person.getName());
        xmlWriter->writeEndElement(); // name
        xmlWriter->writeStartElement("relativeOf");
        xmlWriter->writeAttribute("xsi:nil", "true");
        xmlWriter->writeEndElement(); // relativeOf
        xmlWriter->writeStartElement("relationType");
        xmlWriter->writeAttribute("xsi:nil", "true");
        xmlWriter->writeEndElement(); // relationType
        xmlWriter->writeStartElement("position");
        xmlWriter->writeCharacters(person.getPosition());
        xmlWriter->writeEndElement(); // position

        qDebug() << "name: " << person.getName();
        qDebug() << "position: " << person.getPosition();
    }
    else {
        xmlWriter->writeStartElement("name");
        xmlWriter->writeAttribute("xsi:nil", "true");
        xmlWriter->writeEndElement(); // name
        xmlWriter->writeStartElement("relativeOf");
        xmlWriter->writeCharacters(QString::number(person.getRelativeOf()));
        xmlWriter->writeEndElement(); // relativeOf
        xmlWriter->writeStartElement("relationType");
        xmlWriter->writeCharacters(QString::number(person.getRelationType()));
        xmlWriter->writeEndElement(); // relationType
        xmlWriter->writeStartElement("position");
        xmlWriter->writeAttribute("xsi:nil", "true");
        xmlWriter->writeEndElement(); // position

        qDebug() << "relative of: " << person.getRelativeOf();
        qDebug() << "relation type: " << relationTypes.at(person.getRelationType());
    }
    xmlWriter->writeStartElement("realties");
    qDebug() << "realties: ";
    if (person.getRealties().length() != 0) {
        QListIterator<Realty> realtyIterator(person.getRealties());
        int realties = 0;
        while (realtyIterator.hasNext()) {
            realties++;
            xmlWriter->writeStartElement("realty");
            qDebug() << QString::number(realties) << ":";
            Realty currentRealty = realtyIterator.next();
            QString realtyTypeText = currentRealty.getRealtyType() == 1 ? QString(u8"в собственности") : QString(u8"в пользовании");
            xmlWriter->writeStartElement("realtyName");
            xmlWriter->writeCharacters(currentRealty.getRealtyName());
            xmlWriter->writeEndElement(); // realtyType
            xmlWriter->writeStartElement("realtyType");
            xmlWriter->writeCharacters(QString::number(currentRealty.getRealtyType()));
            xmlWriter->writeEndElement(); // realtyType
            xmlWriter->writeStartElement("objectType");
            xmlWriter->writeCharacters(QString::number(currentRealty.getObjectType()));
            xmlWriter->writeEndElement(); // objectType

            qDebug() << "realty type: " << realtyTypeText;
            qDebug() << "object type: " << objectTypes.at(currentRealty.getObjectType());
            if (currentRealty.getRealtyType() == 1) {
                qDebug() << "ownership type: " << ownershipTypes.key(currentRealty.getOwnershipType());
                xmlWriter->writeStartElement("ownershipType");
                xmlWriter->writeCharacters(QString::number(currentRealty.getOwnershipType()));
                xmlWriter->writeEndElement(); // ownershipType
                xmlWriter->writeStartElement("ownershipPart");
                if (currentRealty.getOwnershipType() == 3) {
                    xmlWriter->writeCharacters(QString::number(currentRealty.getOwnershipPart()));
                    qDebug() << "ownership part: " << QString::number(currentRealty.getOwnershipPart());
                }
                else {
                    xmlWriter->writeAttribute("xsi:nil", "true");
                    qDebug() << "ownership part: none";
                }
                xmlWriter->writeEndElement(); // ownershipText
                xmlWriter->writeStartElement("ownershipText");
                xmlWriter->writeCharacters(currentRealty.getOwnershipText());
                xmlWriter->writeEndElement(); // ownershipText
            }
            else {
                xmlWriter->writeStartElement("ownershipType");
                xmlWriter->writeAttribute("xsi:nil", "true");
                xmlWriter->writeEndElement(); // ownershipType
                xmlWriter->writeStartElement("ownershipPart");
                xmlWriter->writeAttribute("xsi:nil", "true");
                xmlWriter->writeEndElement(); // ownershipPart
                qDebug() << "ownership type: none";
                qDebug() << "ownership part: none";
            }
            xmlWriter->writeStartElement("square");
            xmlWriter->writeCharacters(QString::number(currentRealty.getSquare()));
            xmlWriter->writeEndElement(); //square;
            xmlWriter->writeStartElement("country");
            xmlWriter->writeCharacters(QString::number(currentRealty.getCountry()));
            xmlWriter->writeEndElement(); // country
            qDebug() << "square: " << QString::number(currentRealty.getSquare());
            qDebug() << "country: " << countries.at(currentRealty.getCountry());
            xmlWriter->writeEndElement(); // realty
        }
    }
    else {
        xmlWriter->writeAttribute("xsi:nil", "true");
        qDebug() << "no realties";
    }
    xmlWriter->writeEndElement(); // realties
    xmlWriter->writeStartElement("transports");
    qDebug() << "transports: ";
    if (person.getTransports().length() != 0) {
        int transports = 0;
        QListIterator<Transport> transportIterator(person.getTransports());
        while (transportIterator.hasNext()) {
            transports++;
            qDebug() << transports << ":";
            Transport currentTransport = transportIterator.next();
            xmlWriter->writeStartElement("transport");
            xmlWriter->writeStartElement("transportName");
            xmlWriter->writeCharacters(currentTransport.getName());
            xmlWriter->writeEndElement(); // transportName
            xmlWriter->writeEndElement(); // transport
            qDebug() << currentTransport.getName();
        }
    }
    else {
        xmlWriter->writeAttribute("xsi:nil", "true");
        qDebug() << "no transports";
    }
    xmlWriter->writeEndElement(); // transports
    QString incomeString;
    incomeString.setNum(person.getIncome(), 'f', 2);
    qDebug() << "income: " << incomeString;
    xmlWriter->writeStartElement("income");
    if (!(incomeString == "0.00")) {
        xmlWriter->writeCharacters(incomeString);
    }
    else {
        xmlWriter->writeAttribute("xsi:nil", "true");
    }
    xmlWriter->writeEndElement(); // income
                                  //qDebug() << "income: " << person.getIncome();
    xmlWriter->writeStartElement("incomeComment");
    if (!person.getIncomeComment().isNull()) {
        xmlWriter->writeCharacters(person.getIncomeComment());
    }
    else {
        xmlWriter->writeAttribute("xsi:nil", "true");
    }
    xmlWriter->writeEndElement(); // incomeComment
    xmlWriter->writeStartElement("incomeSource");
    if (!person.getIncomeSource().isNull()) {
        xmlWriter->writeCharacters(person.getIncomeSource());
    }
    else {
        xmlWriter->writeAttribute("xsi:nil", "true");
    }
    xmlWriter->writeEndElement(); // incomeSource
    qDebug() << "income comment: " << person.getIncomeComment();
    qDebug() << "income source: " << person.getIncomeSource();
    xmlWriter->writeEndElement(); // person
}

void Parser::personListToXML(QList<Person> personList) 
{
    QListIterator<Person> iter(personList);
    int current = 1;
    int all = persons.count();
    while (iter.hasNext()) {
        qDebug() << "converting to XML: " + QString::number(current) + " of " + QString::number(all);
        singlePersonToXML(iter.next());
        current++;
    }
}


void Parser::finalizeXMLDocument() 
{
    xmlWriter->writeEndElement(); // persons
    xmlWriter->writeEndDocument();

    //QString docName = currentOrgan + ".xml";
    QString docName;
    QString file = currentOrg.name + ".xml";
    file.replace("\"", "");
    if (!currentOrg.folder.isEmpty())
    {
        QString folder = currentOrg.folder;
        folder.replace("\"", "");
        QDir dir = QDir(folder);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        docName = QDir(folder).filePath(file);;
    } else {
        docName = file;
    }
    qDebug() << "writing to XML file " << docName;
    QFile data(docName);
    if (data.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream ts(&data);
        ts.setCodec("UTF-8");
        ts << xmlOutput;
        data.close();
    }
    else {
        qDebug() << "Error writing persons.xml";
        errorCount++;
    }
    delete xmlWriter;
    //delete xmlOutput;
}

Parser::Parser(const ExcelAdapter& adapter) : m_adapter(adapter)
{
    usedColumns = "ABCDEFGHIJKL";
    personsTableStart = 4;
    cellAddress = "";
    relationTypes.append(QString(u8"не определено"));
    relationTypes.append(QString(u8"супруг"));
    relationTypes.append(QString(u8"супруга"));
    relationTypes.append(QString(u8"несовершеннолетний ребенок"));
    objectTypes.append(QString(u8"не определено"));
    objectTypes.append(QString(u8"садовый участок"));
    objectTypes.append(QString(u8"нежилой дом"));
    objectTypes.append(QString(u8"недостроенный дом"));
    objectTypes.append(QString(u8"машино-место"));
    objectTypes.append(QString(u8"комнаты"));
    objectTypes.append(QString(u8"комната"));
    objectTypes.append(QString(u8"квартира"));
    objectTypes.append(QString(u8"иное недвижимое имущество"));
    objectTypes.append(QString(u8"земельный участок"));
    objectTypes.append(QString(u8"жилой дом"));
    objectTypes.append(QString(u8"жилое строение"));
    objectTypes.append(QString(u8"жилое помещение"));
    objectTypes.append(QString(u8"дом"));
    objectTypes.append(QString(u8"дачный дом"));
    objectTypes.append(QString(u8"дачное строение"));
    objectTypes.append(QString(u8"дача"));
    objectTypes.append(QString(u8"гараж"));
    ownershipTypes.insert(QString(u8"не определено"), 0);
    ownershipTypes.insert(QString(u8"индивидуальная"), 1);
    ownershipTypes.insert(QString(u8"общая совместная"), 2);
    ownershipTypes.insert(QString(u8"совместная"), 2);
    ownershipTypes.insert(QString(u8"общая долевая"), 3);
    countries.append(QString(u8"не определено"));
    countries.append(QString(u8"Беларусь"));
    countries.append(QString(u8"Грузия"));
    countries.append(QString(u8"Казахстан"));
    countries.append(QString(u8"Литва"));
    countries.append(QString(u8"Португалия"));
    countries.append(QString(u8"Россия"));
    countries.append(QString(u8"США"));
    countries.append(QString(u8"Украина"));

    //xmlOutput = new QString();
}
