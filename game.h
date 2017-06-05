#ifndef GAME_H
#define GAME_H

#include <QUrl>
#include <QWebEnginePage>
#include <QObject>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <team.h>
#include <eventbase.h>

class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    Game(Team *Home, Team *Away);

    bool setYoutubeURL(QString link);
    bool setYoutubeURL(QUrl link);
    QUrl getYoutubeURL();

    void setflashScoreID(QString ID);
    QString getFlashScoreID();

    bool hasVideo();

    void setHomeTeam(Team *HomeTeam);
    void setAwayTeam(Team *AwayTeam);
    void setHomeTeamScore(int value);
    void setAwayTeamScore(int value);

    Team* getHomeTeam();
    Team* getAwayTeam();
    int getHomeTeamScore();
    int getAwayTeamScore();

    bool isEmpty();
    bool operator==(const Game &rhs)const;
    bool operator>=(const Game &rhs)const;
    bool operator>(const Game &rhs)const;
    bool operator<=(const Game &rhs)const;
    bool operator<(const Game &rhs)const;

    QString printScore();
    QString printScoreWithTeamNames();
    QDateTime getDateTime();
    QString printTime();
    bool parseTime(QString time, QString format = "dd/MM/yyyy - hh:mm");
    bool setTime(QDateTime time);
    bool parseScore(QString score);


    // Event Handler
    QList<EventBase> getEvents();
    void clearEvents();
    bool addEvent(EventBase event);


    //Json Exporter
    QJsonObject exportJSonObject();
    bool importJSonObject(QJsonObject json, QString gameKeyID);

signals:
    void newDataGrabbed();

private:
    Team *homeTeam;
    Team *awayTeam;

    int homeTeamScore = 0;
    int awayTeamScore = 0;

    QList<EventBase> Events;

    QUrl YoutubeLink;
    QUrl VideoLink;

    QDateTime gameTime;

  //  QUrl FlashScoreLink;

  //  QWebEnginePage *FlashScoreWebEngine;

    QString ID;

//private slots:

  //  void on_FlashScoreWebEngine_loadFinished(bool);
};

#endif // GAME_H
