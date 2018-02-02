 #include "excelhandler.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>


ExcelHandler::ExcelHandler(QObject *parent) : QObject(parent){
    usedColumns = "ABCDEFGHIJKL";
    personsTableStart = 4;
    cellAddress = "";
    relationTypes.append(QString("не определено"));
    relationTypes.append(QString("супруг"));
    relationTypes.append(QString("супруга"));
    relationTypes.append(QString("несовершеннолетний ребенок"));
    objectTypes.append(QString("не определено"));
    objectTypes.append(QString("садовый участок"));
    objectTypes.append(QString("нежилой дом"));
    objectTypes.append(QString("недостроенный дом"));
    objectTypes.append(QString("машино-место"));
    objectTypes.append(QString("комнаты"));
    objectTypes.append(QString("комната"));
    objectTypes.append(QString("квартира"));
    objectTypes.append(QString("иное недвижимое имущество"));
    objectTypes.append(QString("земельный участок"));
    objectTypes.append(QString("жилой дом"));
    objectTypes.append(QString("жилое строение"));
    objectTypes.append(QString("жилое помещение"));
    objectTypes.append(QString("дом"));
    objectTypes.append(QString("дачный дом"));
    objectTypes.append(QString("дачное строение"));
    objectTypes.append(QString("дача"));
    objectTypes.append(QString("гараж"));
    ownershipTypes.append(QString("не определено"));
    ownershipTypes.append(QString("индивидуальная"));
    ownershipTypes.append(QString("общая совместная"));
    ownershipTypes.append(QString("общая долевая"));
    countries.append(QString("не определено"));
    countries.append(QString("Беларусь"));
    countries.append(QString("Грузия"));
    countries.append(QString("Казахстан"));
    countries.append(QString("Литва"));
    countries.append(QString("Португалия"));
    countries.append(QString("Россия"));
    countries.append(QString("США"));
    countries.append(QString("Украина"));

    //xmlOutput = new QString();

}
void ExcelHandler::openExcelFile(QString pathToFile){
    QAxObject* excel = new QAxObject("Excel.Application");
    excelWorkbooks = excel->querySubObject("WorkBooks");
    excelWorkbooks->dynamicCall("Open (const QString&)", pathToFile);
    QAxObject* activeWorkbook = excel->querySubObject("ActiveWorkBook");
    excelFile = activeWorkbook->querySubObject( "WorkSheets(const QString&)", QString("2013") );
}
ExcelCell* ExcelHandler::getExcelCell(QString cellNum){
    QAxObject* sourceQAxObject = excelFile->querySubObject("Range(const QString&)",  cellNum);
    ExcelCell* cell = new ExcelCell(sourceQAxObject);
    delete sourceQAxObject;
    return cell;
}
int ExcelHandler::countUsedRows(){
    QString currentColumn = "";
    int currentColumnCount = 0;
    int currentMax = 0;
    for(int i=0; i<usedColumns.length(); i++){
        currentColumn = usedColumns.mid(i,1);
        currentColumnCount = countRowsInColumn(currentColumn);
        if(currentColumnCount > currentMax){
            currentMax = currentColumnCount;
        }
    }
    return currentMax;
}
int ExcelHandler::countRowsInColumn(QString columnKey){
    int rowsCount = excelFile->querySubObject("Rows")->dynamicCall("Count").toInt();
    QAxObject* queryOne = excelFile->querySubObject("Cells(int,const QString&)", rowsCount, columnKey);
    // -4162 is the enumeration for 'xlUp'
    QAxObject* queryTwo = queryOne->querySubObject("End(int)",-4162);
    int columnRows = queryTwo->dynamicCall("Row").toInt();
    return columnRows;
}
void ExcelHandler::getHeaderPositions(){
    int personsTableEnd = countUsedRows();
    for(int i = personsTableStart; i <= personsTableEnd; i++){
        qDebug() << "header discovery - processing: " << cellAddress;
        cellAddress = "A"+QString::number(i);
        ExcelCell* currentCell = getExcelCell(cellAddress);
        if(currentCell->isHeader){
            headerPositions.append(i);
        }
        delete currentCell;
    }
}
void ExcelHandler::getPersonsBounds(){
    QList<int> personStarts;
    int personsTableEnd = countUsedRows();
    //int personsTableEnd = 231;
    for(int i = personsTableStart; i <= personsTableEnd; i++){
        cellAddress = "A"+QString::number(i);
        //qDebug() << "person discovery - processing: " << cellAddress;
        ExcelCell* currentCell = getExcelCell(cellAddress);
        if(!currentCell->isEmpty && !currentCell->isHeader){
            personStarts.append(i);
        }
        else if(currentCell->isHeader){
            headerPositions.append(i);
        }
        delete currentCell;
    }
    int personStart;
    int personEnd;
    for(int i = 0; i < personStarts.length()-1; i++){
        personStart = personStarts.at(i);
        personEnd =  personStarts.at(i+1)-1;
        for(int j = personStarts.at(i); j < personStarts.at(i+1); j++){
            if(headerPositions.indexOf(j) != -1){
                personEnd = j-1;
            }
        }
        personBounds.append(QPair<int, int>(personStart, personEnd));
    }
    personStart = personStarts.at(personStarts.length()-1);
    personEnd = personsTableEnd;
    personBounds.append(QPair<int, int>(personStart, personEnd));
    personStarts.clear();
}
Person ExcelHandler::buildSinglePerson(QPair<int, int> bounds){
    //qDebug() << "buildSinglePerson init: bounds " + QString::number(bounds.first) + " - " + QString::number(bounds.second);
    Person person;
    QString personName = getExcelCell("A"+QString::number(bounds.first))->cellText;
    person.setId(organPersons.length()+1);
    if(relationTypes.indexOf(personName) == -1){
        person.setName(personName);
    }
    else {
        person.setRelationType(relationTypes.indexOf(personName));
        QListIterator<Person> personIterator(organPersons);
        personIterator.toBack();
        while(personIterator.hasPrevious()){
            Person currentPerson  = personIterator.previous();
            if(!currentPerson.getName().isNull()){
                person.setRelativeOf(currentPerson.getId());
                break;
            }
        }
    }
    person.setPosition(getExcelCell("B"+QString::number(bounds.first))->cellText + " " + currentOrgan);
    ExcelCell* cell;
    for(int i = bounds.first; i <= bounds.second; i++){
        cell = getExcelCell("C"+QString::number(i));
        if(!cell->isEmpty){
            //Realty* realty = new Realty();
            Realty realty;
            realty.setRealtyType(1);
            int objectType = 0;
            if(objectTypes.indexOf(cell->cellText) != -1){
                objectType = objectTypes.indexOf(cell->cellText);
            }
            realty.setObjectType(objectType);
            cell = getExcelCell("D"+QString::number(i));
            int ownershipType = 0;
            double ownershipPart = 0;
            QString text = cell->cellText;
            if(ownershipTypes.indexOf(text) == -1){
                if(text.indexOf("-") != -1){
                    QString option = text.split("-").at(0).trimmed();
                    if(option == "общая долевая"){
                        ownershipType = 3;
                        QString ownershipPartText = text.split("-").at(1).trimmed();
                        if(ownershipPartText.indexOf("/") != -1){
                            ownershipPart = ownershipPartText.split("/").at(0).toDouble() / ownershipPartText.split("/").at(1).toDouble();
                        }
                    }
                }
                else{
                    ownershipType = 0;
                }
            }
            else{
                ownershipType = ownershipTypes.indexOf(text);
            }
            realty.setOwnershipPart(ownershipPart);
            realty.setOwnershipType(ownershipType);
            cell = getExcelCell("E"+QString::number(i));
            QString number = cell->cellText.split(" ").join("");
            number = number.replace(QString(","), QString("."));
            double square = number.toDouble();
            realty.setSquare(square);
            cell = getExcelCell("F"+QString::number(i));
            int country;
            if(!cell->isEmpty && countries.indexOf(cell->cellText) != -1){
                country = countries.indexOf(cell->cellText);
            }
            else{
                country = 0;
            }
            realty.setCountry(country);
            person.addRealty(realty);
        }
        else {
            break;
        }
    }
    for(int i = bounds.first; i <= bounds.second; i++){
        cell = getExcelCell("G"+QString::number(i));
        if(!cell->isEmpty){
            Realty realty;
            realty.setRealtyType(2);
            int objectType = 0;
            if(objectTypes.indexOf(cell->cellText) != -1){
                objectType = objectTypes.indexOf(cell->cellText);
            }
            realty.setObjectType(objectType);
            cell = getExcelCell("H"+QString::number(i));
            QString number = cell->cellText.split(" ").join("");
            number = number.replace(QString(","), QString("."));
            double square = cell->cellText.toDouble();
            realty.setSquare(square);
            cell = getExcelCell("I"+QString::number(i));
            int country;
            if(!cell->isEmpty && countries.indexOf(cell->cellText) != -1 ){
                country = countries.indexOf(cell->cellText);
            }
            else{
                country = 0;
            }
            realty.setCountry(country);
            person.addRealty(realty);
        }
        else {
            break;
        }
    }
    for(int i = bounds.first; i <= bounds.second; i++){
        cell = getExcelCell("J"+QString::number(i));
        if(!cell->isEmpty){
            Transport transport;
            transport.setName(cell->cellText);
            person.addTransport(transport);
        }
        else{
            break;
        }
    }
    cell = getExcelCell("K"+QString::number(bounds.first));
    QString incomeText = cell->cellText;
    if(incomeText.indexOf("(") != -1){
        int leftBrace = incomeText.indexOf("(");
        int rightBrace = incomeText.indexOf(")");
        int length = rightBrace - leftBrace - 1;
        QString incomeComment = incomeText.mid(leftBrace+1, length);
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
    cell = getExcelCell("L"+QString::number(bounds.first));
    person.setIncomeSource(cell->cellText);
    organPersons.append(person);
    return person;
}
void ExcelHandler::getOrgansBounds(){
    QPair<int,int> pair;
    for(int i = 0; i < headerPositions.length()-1;i++){
        qDebug() << i+1 << " organ start: " << headerPositions.at(i) << "organ end: " << headerPositions.at(i+1)-1;
        //qDebug() << "this: " << getExcelCell("A"+QString::number(headerPositions.at(i)))->cellText << "next: " << getExcelCell("A"+QString::number(headerPositions.at(i+1)))->cellText;
        pair.first = headerPositions.at(i)+1;
        pair.second = headerPositions.at(i+1)-1;
        organsBounds.append(pair);
    }
    pair.first = headerPositions.at(headerPositions.length()-1)+1;
    pair.second = countUsedRows();
    organsBounds.append(pair);
    qDebug() <<  headerPositions.length() << " organ start: " << headerPositions.at(headerPositions.length()-1) << "organ end: " << countUsedRows();
}
void ExcelHandler::buildOrganList(){
    QListIterator<QPair<int,int>> organIter(organsBounds);
    while(organIter.hasNext()){
        QPair<int,int> organPair = organIter.next();
        QPair<int,int> organPersonBounds;
        QPair<QString,QPair<int,int>> organBoundsWithName;
        int personNum = 0;
        QListIterator<QPair<int,int>> personIter(personBounds);
        while(personIter.hasNext()){
            QPair<int,int> personPair = personIter.next();
            //qDebug() << "examining person " << personPair.first << " to " << personPair.second;
            if(organPair.first == personPair.first){
                organPersonBounds.first = personNum;
                organBoundsWithName.first = getExcelCell("A"+QString::number(organPair.first-1))->cellText;
            }
            if(organPair.second == personPair.second){
                organPersonBounds.second = personNum;
                organBoundsWithName.second = organPersonBounds;
                organsBoundsList.append(organBoundsWithName);
                break;
            }
            personNum++;
        }
    }
}
void ExcelHandler::buildOrgansXMLs(){
    qDebug() << "building organs XMLs with organsBoundsList length: " << organsBoundsList.length();
    QListIterator<QPair<QString,QPair<int,int>>> iter(organsBoundsList);
    while(iter.hasNext()){
        organPersons.clear();
        QPair<QString,QPair<int,int>> pair = iter.next();
        currentOrgan = pair.first;
        QList<QPair<int,int>> personsBounds;
        for(int i = pair.second.first; i <= pair.second.second; i++){
            personsBounds.append(personBounds.at(i));
        }
        QList<Person> personsList = buildPersonObjects(personsBounds);
        startXMLDocument();
        personListToXML(personsList);
        finalizeXMLDocument();


        qDebug() << "the organ is: " << pair.first;
        qDebug() << "starts with person: " << pair.second.first;
        qDebug() << "ends with person: " << pair.second.second;
    }
	QMessageBox endMsg;
	endMsg.setText("Процесс конвертации завершён");
	endMsg.exec();
    qDebug() << "total persons: " << personBounds.length();
}
void ExcelHandler::startXMLDocument(){
    xmlOutput = "";
    xmlWriter = new QXmlStreamWriter(&xmlOutput);
    QString schemaPath = "declarationXMLtemplate_Schema _transport_merged.xsd";
    xmlWriter->setAutoFormatting(true);
    xmlWriter->setCodec("UTF-8");
    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement("persons");
    xmlWriter->writeAttribute("xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance");
    xmlWriter->writeAttribute("xsi:noNamespaceSchemaLocation",schemaPath);
}
void ExcelHandler::singlePersonToXML(Person person){
    xmlWriter->writeStartElement("person");
    xmlWriter->writeStartElement("id");
    xmlWriter->writeCharacters(QString::number(person.getId()));
    xmlWriter->writeEndElement();
    qDebug() << "writing XML";
    qDebug() << "id: " << person.getId();

    if(!person.getName().isNull()){
        xmlWriter->writeStartElement("name");
        xmlWriter->writeCharacters(person.getName());
        xmlWriter->writeEndElement(); // name
        xmlWriter->writeStartElement("relativeOf");
        xmlWriter->writeAttribute("xsi:nil","true");
        xmlWriter->writeEndElement(); // relativeOf
        xmlWriter->writeStartElement("relationType");
        xmlWriter->writeAttribute("xsi:nil","true");
        xmlWriter->writeEndElement(); // relationType
        xmlWriter->writeStartElement("position");
        xmlWriter->writeCharacters(person.getPosition());
        xmlWriter->writeEndElement(); // position

        qDebug() << "name: " << person.getName();
        qDebug() << "position: " << person.getPosition();
    }
    else{
        xmlWriter->writeStartElement("name");
        xmlWriter->writeAttribute("xsi:nil","true");
        xmlWriter->writeEndElement(); // name
        xmlWriter->writeStartElement("relativeOf");
        xmlWriter->writeCharacters(QString::number(person.getRelativeOf()));
        xmlWriter->writeEndElement(); // relativeOf
        xmlWriter->writeStartElement("relationType");
        xmlWriter->writeCharacters(QString::number(person.getRelationType()));
        xmlWriter->writeEndElement(); // relationType
        xmlWriter->writeStartElement("position");
        xmlWriter->writeAttribute("xsi:nil","true");
        xmlWriter->writeEndElement(); // position

        qDebug() << "relative of: " << person.getRelativeOf();
        qDebug() << "relation type: " << relationTypes.at(person.getRelationType());
    }
    xmlWriter->writeStartElement("realties");
    qDebug() << "realties: ";
    if(person.getRealties().length() != 0){
        QListIterator<Realty> realtyIterator(person.getRealties());
        int realties = 0;
        while(realtyIterator.hasNext()){
            realties++;
            xmlWriter->writeStartElement("realty");
            qDebug() << QString::number(realties) << ":";
            Realty currentRealty = realtyIterator.next();
            QString realtyTypeText = currentRealty.getRealtyType() == 1 ? QString("в собственности") : QString("в пользовании");
            xmlWriter->writeStartElement("realtyType");
            xmlWriter->writeCharacters(QString::number(currentRealty.getRealtyType()));
            xmlWriter->writeEndElement(); // realtyType
            xmlWriter->writeStartElement("objectType");
            xmlWriter->writeCharacters(QString::number(currentRealty.getObjectType()));
            xmlWriter->writeEndElement(); // objectType

            qDebug() << "realty type: " << realtyTypeText;
            qDebug() << "object type: " << objectTypes.at(currentRealty.getObjectType());
            if(currentRealty.getRealtyType() == 1){
                qDebug() << "ownership type: " << ownershipTypes.at(currentRealty.getOwnershipType());
                xmlWriter->writeStartElement("ownershipType");
                xmlWriter->writeCharacters(QString::number(currentRealty.getOwnershipType()));
                xmlWriter->writeEndElement(); // ownershipType
                xmlWriter->writeStartElement("ownershipPart");
                if(currentRealty.getOwnershipType() == 3){
                    xmlWriter->writeCharacters(QString::number(currentRealty.getOwnershipPart()));
                    qDebug() << "ownership part: " << QString::number(currentRealty.getOwnershipPart());
                }
                else{
                    xmlWriter->writeAttribute("xsi:nil","true");
                    qDebug() << "ownership part: none";
                }
                xmlWriter->writeEndElement(); // ownershipPart
            }
            else {
                xmlWriter->writeStartElement("ownershipType");
                xmlWriter->writeAttribute("xsi:nil","true");
                xmlWriter->writeEndElement(); // ownershipType
                xmlWriter->writeStartElement("ownershipPart");
                xmlWriter->writeAttribute("xsi:nil","true");
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
        xmlWriter->writeAttribute("xsi:nil","true");
        qDebug() << "no realties";
    }
    xmlWriter->writeEndElement(); // realties
    xmlWriter->writeStartElement("transports");
    qDebug() << "transports: ";
    if(person.getTransports().length() != 0){
        int transports = 0;
        QListIterator<Transport> transportIterator(person.getTransports());
        while(transportIterator.hasNext()){
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
    else{
        xmlWriter->writeAttribute("xsi:nil","true");
        qDebug() << "no transports";
    }
    xmlWriter->writeEndElement(); // transports
    QString incomeString;
    incomeString.setNum(person.getIncome(),'f', 2);
    qDebug() << "income: " << incomeString;
    xmlWriter->writeStartElement("income");
    if(!(incomeString == "0.00")){
        xmlWriter->writeCharacters(incomeString);
    }
    else{
        xmlWriter->writeAttribute("xsi:nil","true");
    }
    xmlWriter->writeEndElement(); // income
    //qDebug() << "income: " << person.getIncome();
    xmlWriter->writeStartElement("incomeComment");
    if(!person.getIncomeComment().isNull()){
        xmlWriter->writeCharacters(person.getIncomeComment());
    }
    else{
        xmlWriter->writeAttribute("xsi:nil","true");
    }
    xmlWriter->writeEndElement(); // incomeComment
    xmlWriter->writeStartElement("incomeSource");
    if(!person.getIncomeSource().isNull()){
        xmlWriter->writeCharacters(person.getIncomeSource());
    }
    else{
        xmlWriter->writeAttribute("xsi:nil","true");
    }
    xmlWriter->writeEndElement(); // incomeSource
    qDebug() << "income comment: " << person.getIncomeComment();
    qDebug() << "income source: " << person.getIncomeSource();
    xmlWriter->writeEndElement(); // person
}
void ExcelHandler::finalizeXMLDocument(){
    xmlWriter->writeEndElement(); // persons
    xmlWriter->writeEndDocument();
    QString docName = currentOrgan + ".xml";
    QFile data(docName);
    if(data.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream ts(&data);
        ts.setCodec("UTF-8");
        ts << xmlOutput;
        data.close();
    } else {
        qDebug() << "Error writing persons.xml";
    }
    delete xmlWriter;
    //delete xmlOutput;
}
QList<Person> ExcelHandler::buildPersonObjects(QList<QPair<int,int>> bounds){
    QList<Person> persons;
    QListIterator<QPair<int,int>> iter(bounds);
    int current = 1;
    int all = personBounds.count();
    while(iter.hasNext()){
        QPair<int,int> currentPair = iter.next();
        //qDebug() << "building person objects: " + QString::number(current) + " of " + QString::number(all);
        persons.append(buildSinglePerson(currentPair));
        current++;
    }
    return persons;
}
void ExcelHandler::personListToXML(QList<Person> personList){
    QListIterator<Person> iter(personList);
    int current = 1;
    int all = persons.count();
    while(iter.hasNext()){
        qDebug() << "converting to XML: " + QString::number(current) + " of " + QString::number(all);
        singlePersonToXML(iter.next());
        current++;
    }
}
void ExcelHandler::printRowRanges(){
    QListIterator<QPair<int,int>> iter(personBounds);
    while(iter.hasNext()){
        qDebug() << iter.next();
    }
}
void ExcelHandler::closeExcelFile(){
    excelWorkbooks->querySubObject("Close(void)");
}
ExcelHandler::~ExcelHandler(){
}
