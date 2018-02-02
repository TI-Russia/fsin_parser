#include "realty.h"

Realty::Realty(){

}
// setters
void Realty::setRealtyName(QString text)
{
    realtyName = text;
}

void Realty::setRealtyType(int realtyRealtyType){
    realtyType = realtyRealtyType;
}
void Realty::setObjectType(int realtyObjectType){
    objectType = realtyObjectType;
}
void Realty::setOwnershipType(int realtyOwnershipType){
    ownershipType = realtyOwnershipType;
}

void Realty::setOwnershipText(QString text)
{
    ownershipText = text;
}

void Realty::setOwnershipPart(double realtyOwnershipPart){
    ownershipPart = realtyOwnershipPart;
}
void Realty::setSquare(double realtySquare){
    square = realtySquare;
}
void Realty::setCountry(int realtyCountry){
    country = realtyCountry;
}
// getters
QString Realty::getRealtyName()
{
    return realtyName;
}

int Realty::getRealtyType(){
    return realtyType;
}
int Realty::getObjectType(){
    return objectType;
}
int Realty::getOwnershipType(){
    return ownershipType;
}

QString Realty::getOwnershipText() {
    return ownershipText;
}


double Realty::getOwnershipPart(){
    return ownershipPart;
}
double Realty::getSquare(){
    return square;
}
int Realty::getCountry(){
    return country;
}

Realty::~Realty(){

}

