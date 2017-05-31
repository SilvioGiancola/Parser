#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QList>

#include <player.h>

class Team
{
public:
    Team();
    Team(QString FullName);

    QString getFullName();

    bool operator==(const Team &rhs)const;

private:
    QString FullName;
    QString ShortName;

    QList<Player> Players;


};

#endif // TEAM_H
