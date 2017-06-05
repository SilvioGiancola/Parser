#ifndef CHAMPIONSHIP_H
#define CHAMPIONSHIP_H

#include <QStringList>
#include <QString>
#include <QList>
#include <QWebEnginePage>
#include <QObject>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QDomDocument>

#include <game.h>
#include <team.h>

class Championship : public QObject
{
    Q_OBJECT
public:
    Championship();
    Championship(QString Name, int Year);


    bool hasTeam(Team *thisteam);
    bool addTeam(Team *team);
    QList<Team*> getAllTeams();
    QStringList getListOfTeamsName();
    void printListofTeams();
    int countTeams();


    bool hasGame(Game *thisgame);
    //bool fillGame(Game *newgame);
    QList<Game*> getAllGames();
    int countGames();
    Game* getGame(Game * thisgame);
    Game* getGame(Team * homeTeam, Team * awayTeam);
    Game* getGame(QString homeTeam, QString awayTeam);




    void loadTeams();
    void loadGames();
    void loadMoreGames();
    void launchJS();


    QString printChampionship();


    QString getLeague();
    int getYear();


   void saveJSON(QString filename);
   bool loadJSON(QString filename);
public slots:
    void setLeague(QString);
    void setYear(int);


private:
    QString League;
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
    void gameEdited(Game* newgame);
};

#endif // CHAMPIONSHIP_H
