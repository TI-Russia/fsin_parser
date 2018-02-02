#pragma once
#include <QString>

class Organization
{
public:
    Organization();
    Organization(QString name, QString folder, int person_first, int person_last, bool topLevel);

    QString name;
    QString folder;
    int person_first = -1;
    int person_last = -1;
    bool topLevel = false;
};

