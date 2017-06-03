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
    QString getShortName();

    bool operator==(const Team &rhs)const;

    enum
    {
        Home,
        Away
    };

private:
    QString FullName;
    QString ShortName;

    QList<Player> Players;


};

#endif // TEAM_H
