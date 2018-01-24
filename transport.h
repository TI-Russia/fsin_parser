#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QString>

class Transport{
    public:
        Transport();
        // setters
        void setType(int transType);
        void setName(QString transName);
        // getters
        int getType();
        QString getName();
        ~Transport();
    private:
        int type;
        QString name;
};

#endif // TRANSPORT_H
