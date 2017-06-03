#include "team.h"

Team::Team()
{    
    this->FullName = "";
}

Team::Team(QString FullName)
{
    this->FullName = FullName;
    this->ShortName = FullName;
}


QString Team::getFullName()
{
    return FullName;
}

QString Team::getShortName()
{
    return ShortName;
}

bool Team::operator==(const Team &rhs)const
{
    return (QString::compare(this->FullName, rhs.FullName, Qt::CaseInsensitive) == 0);
}

