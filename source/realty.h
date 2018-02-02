#ifndef REALTY_H
#define REALTY_H

#include <QString>

class Realty{
    public:
        Realty();
        // setters
        void setRealtyName(QString text);
        void setRealtyType(int realtyRealtyType);
        void setObjectType(int realtyObjectType);
        void setOwnershipType(int realtyOwnershipType);
        void setOwnershipText(QString text);
        void setOwnershipPart(double realtyOwnershipPart);
        void setSquare(double realtySquare);
        void setCountry(int realtyCountry);
        // getters
        QString getRealtyName();
        int getRealtyType();
        int getObjectType();
        int getOwnershipType();
        QString getOwnershipText();
        double getOwnershipPart();
        double getSquare();
        int getCountry();
        ~Realty();
    private:
        QString realtyName;
        int realtyType;
        int objectType;
        int ownershipType;
        double ownershipPart;
        double square;
        int country;
        QString ownershipText;
};

#endif // REALTY_H
