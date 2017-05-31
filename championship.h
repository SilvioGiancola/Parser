#ifndef CHAMPIONSHIP_H
#define CHAMPIONSHIP_H

#include <QStringList>
#include <QString>
#include <QList>
#include <QWebEnginePage>
#include <QObject>
#include <QEventLoop>

#include <game.h>
#include <team.h>

class Championship : public QObject
{
    Q_OBJECT
public:
    Championship();
    Championship(QString Name, int Year);

    bool addTeam(Team *team);
    QStringList getListOfTeamsName();
    void printListofTeams();
    int countTeams();

    bool addGame(Game *newgame);
    QList<Game*> getAllGames();
    int countGames();
    void clearTeams();
    Game* getGame(Team * homeTeam, Team * awayTeam);

    bool setName(QString Name);


    void loadTeams(QString Name, int Year);
    void loadGames(QString Name, int Year);
    void loadMoreGames();
    void launchJS();

private:
    QString Name;
    int Year;
    QList<Team*> Teams;

    QList<Game*> Games;



    QWebEnginePage *FlashScoreTeamsListWebEngine;
    QWebEnginePage *FlashScoreWebEngine;

    //results_championship = new QWebEnginePage();

private slots:

    void on_FlashScoreWebEngine_loadFinished(bool);
    void on_teams_loadFinished(bool);

signals:
    void newTeamAdded();
    void newGameAdded(Game* newgame);
};

#endif // CHAMPIONSHIP_H
