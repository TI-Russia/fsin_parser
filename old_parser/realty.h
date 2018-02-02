#ifndef REALTY_H
#define REALTY_H


class Realty{
    public:
        Realty();
        // setters
        void setRealtyType(int realtyRealtyType);
        void setObjectType(int realtyObjectType);
        void setOwnershipType(int realtyOwnershipType);
        void setOwnershipPart(double realtyOwnershipPart);
        void setSquare(double realtySquare);
        void setCountry(int realtyCountry);
        // getters
        int getRealtyType();
        int getObjectType();
        int getOwnershipType();
        double getOwnershipPart();
        double getSquare();
        int getCountry();
        ~Realty();
    private:
        int realtyType;
        int objectType;
        int ownershipType;
        double ownershipPart;
        double square;
        int country;
};

#endif // REALTY_H
