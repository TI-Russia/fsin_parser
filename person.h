#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include "realty.h"
#include "transport.h"

#include <QList>

class Person  {
    public:
        Person();
        // setters
        void setId(int persId);
        void setName(QString persName);
        void setRelativeOf(int persRelativeOf);
        void setRelationType(int persRelationType);
        void setPosition(QString persPosition);
        void addRealty(Realty persRealty);
        void addTransport(Transport persTransport);
        void setIncome(double persIncome);
        void setIncomeComment(QString persIncomeComment);
        void setIncomeSource(QString persIncomeSource);
        // getters
        int getId();
        QString getName();
        int getRelativeOf();
        int getRelationType();
        QString getPosition();
        QList<Realty> getRealties();
        QList<Transport> getTransports();
        double getIncome();
        QString getIncomeComment();
        QString getIncomeSource();
        ~Person();
    private:
        int id;
        QString name;
        int relativeOf;
        int relationType;
        QString position;
        QList<Realty> realties;
        QList<Transport> transports;
        double income;
        QString incomeComment;
        QString incomeSource;
};

#endif // PERSON_H
