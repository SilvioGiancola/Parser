#include "game.h"

Game::Game()
{
    this->homeTeam = new Team();
    this->awayTeam = new Team();

    homeTeamScore = 0;
    awayTeamScore = 0;

    VideoLink = QUrl();
}

Game::Game(Team *Home, Team *Away)
{
    this->homeTeam = Home;
    this->awayTeam = Away;

    homeTeamScore = 0;
    awayTeamScore = 0;

    VideoLink = QUrl();
}





bool Game::operator==(const Game &rhs)const
{
    return (( (*this->homeTeam) == (*rhs.homeTeam)) && ((*this->awayTeam) == (*rhs.awayTeam) ));
}


bool Game::isEmpty()
{
   // qDebug() << this->printScoreWithTeamNames();
   // qDebug() << Game().printScoreWithTeamNames();
    return ((*this) == Game());
}

QUrl Game::getLocalVideoURL()
{
    return VideoLink;
}

void Game::setLocalVideoURL(QUrl url)
{
    VideoLink = url;
}

bool Game::hasVideo()
{
    return VideoLink.isValid();
}




void Game::setflashScoreID(QString ID)
{
    this->ID = ID;
}

QString Game::getFlashScoreID()
{
    return this->ID;
}



bool Game::setYoutubeURL(QString link)
{
    setYoutubeURL(QUrl(link));
    return false;
}

bool Game::setYoutubeURL(QUrl link)
{
    YoutubeLink = link;
    return false;
}
QUrl Game::getYoutubeURL()
{
    return YoutubeLink;
}




void Game::setHomeTeam(Team *HomeTeam)
{
    this->homeTeam = HomeTeam;
}

void Game::setAwayTeam(Team *AwayTeam)
{
    this->homeTeam = AwayTeam;
}
void Game::setHomeTeamScore(int value)
{
    this->homeTeamScore = value;
}

void Game::setAwayTeamScore(int value)
{
    this->awayTeamScore = value;
}

Team* Game::getHomeTeam()
{
    return homeTeam;
}

Team* Game::getAwayTeam()
{
    return awayTeam;
}

int Game::getHomeTeamScore()
{
    return homeTeamScore;
}

int Game::getAwayTeamScore()
{
    return awayTeamScore;
}






QString Game::printScore()
{
    if (this->isEmpty())
        return QString();
    else
        return QString("%1 - %2")
                .arg(this->homeTeamScore)
                .arg(this->awayTeamScore);
}

QString Game::printScoreWithTeamNames()
{
    return QString("%1 %2 - %4 %3")
            .arg(homeTeam->getFullName())
            .arg(this->homeTeamScore)
            .arg(awayTeam->getFullName())
            .arg(this->awayTeamScore);
}

bool Game::parseScore(QString score) //format is "4 - 1"
{
    try
    {
        setHomeTeamScore(score.mid(0,1).toInt());
        setAwayTeamScore(score.mid(4,1).toInt());
    }
    catch (...)
    {
        return true;
    }
    return false;
}



QDateTime Game::getDateTime()
{
    return gameTime;
}


QString Game::printTime()
{
    return gameTime.toString("dd/MM/yyyy - hh:mm");
}

bool Game::parseTime(QString time, QString format)
{
    gameTime = QDateTime::fromString(time, format);
    return gameTime.isValid();
}

bool Game::setTime(QDateTime time)
{
    gameTime = time;
   // if (gameTime.date().month()<8) gameTime = gameTime.addYears(1);
    return gameTime.isValid();
}







QList<EventBase> Game::getEvents()
{
    return Events;
}

void Game::clearEvents()
{
    Events.clear();
}

bool Game::addEvent(EventBase event)
{
    Events.append(event);
    return false;
}



bool Game::importJSonObject(QJsonObject GameJson, QString gameKeyID)
{
    try
    {

        this->setflashScoreID(gameKeyID);
        this->parseTime(GameJson["gameDate"].toString());
        this->parseScore(GameJson["gameScore"].toString());

        this->YoutubeLink = QUrl::fromUserInput(GameJson["UrlYoutube"].toString());
        this->VideoLink = QUrl::fromUserInput(GameJson["UrlLocal"].toString());

        QJsonArray annotations = GameJson["annotations"].toArray();



        foreach(const QJsonValue & annotation, annotations)
        {

            EventBase event;
            event.setLabel(annotation.toObject()["label"].toString());
            event.setDescription(annotation.toObject()["label"].toString());

            QJsonArray segments = annotation.toObject()["segment"].toArray();

            event.parseStartTime(segments.at(0).toString());
            event.parseEndTime(segments.at(1).toString());

            if (QString::compare(annotation.toObject()["team"].toString(),"home") == 0)
                event.setHomeTeam();
            else if (QString::compare(annotation.toObject()["team"].toString(),"away") == 0)
                event.setAwayTeam();

            this->addEvent(event);
        }
    }
    catch (...)
    {
        return true;
    }

    return false;
}


QJsonObject Game::exportJSonObject()
{
    QJsonObject res;

    QJsonArray annotations;
    foreach(EventBase event, Events)
    {
       // qDebug() << "Export an event!" << event.getDescription();
        if (!event.isEmpty())
        {
            QJsonObject annotationObject;

            QJsonArray segment;
            segment.append(event.printStartTime());
            segment.append(event.printEndTime());
            annotationObject["segment"] = segment;
            annotationObject["label"] = event.getLabel();
            annotationObject["team"] = event.isHomeTeam()?"home":"away";

            annotations.append(annotationObject);
        }
    }

    res["gameHomeTeam"] = this->homeTeam->getFullName();
    res["gameAwayTeam"] = this->awayTeam->getFullName();
    res["gameScore"] = this->printScore();
    res["gameDate"] = this->printTime();
    res["annotations"] = annotations;
    // res["subset"] = "football";
    // res["resolution"] = "320x240";
    //  res["duration"] = 90*60;
    res["UrlYoutube"] = this->YoutubeLink.toString();
    res["UrlLocal"] = this->VideoLink.toString();
    return res;
}
