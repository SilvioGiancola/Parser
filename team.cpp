#include "team.h"

Team::Team()
{

}

Team::Team(QString FullName)
{
    this->FullName = FullName;
}


QString Team::getFullName()
{
    return FullName;
}

bool Team::operator==(const Team &rhs)const
{
    return (QString::compare(this->FullName, rhs.FullName, Qt::CaseInsensitive) == 0);
}

