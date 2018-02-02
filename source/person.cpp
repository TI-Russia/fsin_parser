#include "person.h"

Person::Person(){

}
// setters
void Person::setId(int persId){
    id = persId;
}
void Person::setName(QString persName){
    name = persName;
}
void Person::setRelativeOf(int persRelativeOf){
    relativeOf = persRelativeOf;
}
void Person::setRelationType(int persRelationType){
    relationType = persRelationType;
}
void Person::setPosition(QString persPosition){
    position = persPosition;
}
void Person::addRealty(Realty persRealty){
    realties.append(persRealty);
}
void Person::addTransport(Transport persTransport){
    transports.append(persTransport);
}
void Person::setIncome(double persIncome){
    income = persIncome;
}
void Person::setIncomeComment(QString persIncomeComment){
    incomeComment = persIncomeComment;
}
void Person::setIncomeSource(QString persIncomeSource){
    incomeSource = persIncomeSource;
}
// getters
int Person::getId(){
    return id;
}
QString Person::getName(){
    return name;
}
int Person::getRelativeOf(){
    return relativeOf;
}
int Person::getRelationType(){
    return relationType;
}
QString Person::getPosition(){
    return position;
}
QList<Realty> Person::getRealties(){
    return realties;
}
QList<Transport> Person::getTransports(){
    return transports;
}
double Person::getIncome(){
    return income;
}
QString Person::getIncomeComment(){
    return incomeComment;
}
QString Person::getIncomeSource(){
    return incomeSource;
}
Person::~Person(){

}

