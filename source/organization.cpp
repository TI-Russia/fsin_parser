#include "organization.h"

Organization::Organization(QString name, QString folder, int person_first, int person_last, bool topLevel)
: name(name), folder(folder), person_first(person_first), person_last(person_last), topLevel(topLevel)
{
}

Organization::Organization()
{
}
