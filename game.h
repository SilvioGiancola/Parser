#ifndef GAME_H
#define GAME_H

#include <QUrl>
#include <QWebEnginePage>
#include <QObject>

#include <team.h>
#include <eventbase.h>

class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    Game(Team *Home, Team *Away);
    Game(QString ID);

    bool setYoutubeURL(QString link);
    bool setYoutubeURL(QUrl link);

    void setflashScoreID(QString ID);
    QString getFlashScoreID();
    void loadFlashScoreInformation();

    void setHomeTeam(Team *HomeTeam);
    void setAwayTeam(Team *AwayTeam);
    void setHomeTeamScore(int value);
    void setAwayTeamScore(int value);

   /* Team* getHomeTeam();
    Team* getAwayTeam();*/
    int getHomeTeamScore();
    int getAwayTeamScore();

    QList<EventBase> getHomeEvents();
    QList<EventBase> getAwayEvents();

    bool operator==(const Game &rhs)const;

    QString printScore();
    QString printScoreWithTeamNames();
    QString printTime();
    bool parseTime(QString time, QString format);

    Team *homeTeam;
    Team *awayTeam;

signals:
    void newDataGrabbed();
private:

    int homeTeamScore = 0;
    int awayTeamScore = 0;

    QList<EventBase> homeEvents;
    QList<EventBase> awayEvents;

    QUrl YoutubeLink;

    QDateTime gameTime;

  //  QUrl FlashScoreLink;

    QWebEnginePage *FlashScoreWebEngine = new QWebEnginePage();

    QString ID;

private slots:

    void on_FlashScoreWebEngine_loadFinished(bool);
};

#endif // GAME_H
