#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player
{
public:
    Player();
    Player(QString Name, int number);

public:
    QString Name;
    int number;
};

#endif // PLAYER_H
