#include "realty.h"

Realty::Realty(){

}
// setters
void Realty::setRealtyType(int realtyRealtyType){
    realtyType = realtyRealtyType;
}
void Realty::setObjectType(int realtyObjectType){
    objectType = realtyObjectType;
}
void Realty::setOwnershipType(int realtyOwnershipType){
    ownershipType = realtyOwnershipType;
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
int Realty::getRealtyType(){
    return realtyType;
}
int Realty::getObjectType(){
    return objectType;
}
int Realty::getOwnershipType(){
    return ownershipType;
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

