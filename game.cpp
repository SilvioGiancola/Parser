#include "game.h"

Game::Game()
{
    this->homeTeam = new Team();
    this->awayTeam = new Team();

    FlashScoreWebEngine = new QWebEnginePage();
    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)), this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));
}

Game::Game(Team *Home, Team *Away)
{
    this->homeTeam = Home;
    this->awayTeam = Away;

    FlashScoreWebEngine = new QWebEnginePage();
    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)), this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));
}


Game::Game(QString ID)
{
    this->homeTeam = new Team();
    this->awayTeam = new Team();

    FlashScoreWebEngine = new QWebEnginePage();
    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)), this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));

   // setflashScoreID(ID);
}


bool Game::operator==(const Game &rhs)const
{
    return ( (this->homeTeam == rhs.homeTeam) && (this->awayTeam == rhs.awayTeam));
}


void Game::setflashScoreID(QString ID)
{
    this->ID = ID;
   // FlashScoreWebEngine->load(QString("http://www.flashscore.ae/match/%1/#matchsummary").arg(ID));
}

QString Game::getFlashScoreID()
{
    return this->ID;
}




bool Game::setYoutubeURL(QString link)
{
    return setYoutubeURL(QUrl(link));
}

bool Game::setYoutubeURL(QUrl link)
{
    YoutubeLink = link;
    return false;
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
/*
Team* Game::getHomeTeam()
{
    return homeTeam;
}

Team* Game::getAwayTeam()
{
    return awayTeam;
}*/

int Game::getHomeTeamScore()
{
    return homeTeamScore;
}

int Game::getAwayTeamScore()
{
    return awayTeamScore;
}

QList<EventBase> Game::getHomeEvents()
{
    return homeEvents;
}

QList<EventBase> Game::getAwayEvents()
{
    return awayEvents;
}

QString Game::printScore()
{
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

QString Game::printTime()
{
    return gameTime.toString("dd/MM/yyyy - hh:mm");
}
bool Game::parseTime(QString time, QString format)
{
    gameTime = QDateTime::fromString(time, format);
    if (gameTime.date().month()<8) gameTime = gameTime.addYears(1);
    return gameTime.isValid();
}


void Game::loadFlashScoreInformation()
{
    QString Link = QString("http://www.flashscore.ae/match/%1/#match-summary").arg(ID);
    qDebug() << "opening : " << Link;
    FlashScoreWebEngine->load(Link);
}


void Game::on_FlashScoreWebEngine_loadFinished(bool ok)
{
    qDebug() << "Game has been Loaded" << ok;


    FlashScoreWebEngine->toHtml([this](const QString& result) mutable
    {
        int init = 0, fine = 0;
        int i=0;

        // Focus on tabel only

        QString Table;
        init = result.indexOf("<table id=\"parts", fine) + QString("<table id=\"parts").length();
        fine = result.indexOf("</table>", init);
        if (fine >= 0  && init >= 0  && fine > init )
            Table = result.mid(init, fine-init);


        qDebug() << result;

        int initHome = 0;
        int fineHome = 0;

        int initAway= 0;
        int fineAway = 0;

      //  ui->listWidget_eventAway->clear();
     //   ui->listWidget_eventHome->clear();
        homeEvents.clear();
        awayEvents.clear();


        while (initHome >= 0 && fineHome >= 0 && i < 30)
        {


            {
                QString GameEvent;

                initHome = Table.indexOf("<td class=\"summary-vertical fl", fineAway);
                fineHome = Table.indexOf("</td>", initHome + QString("<td class=\"summary-vertical fl").length());
                if (fineHome >= 0  && initHome >= 0  && fineHome > initHome )
                    GameEvent = Table.mid(initHome, fineHome-initHome);

                //    qDebug() << "GameEvent Home" << GameEvent;

                if (initHome < 0 || fineHome < 0)
                    break;

                QString wrapper;
                {
                    int init_wrapper = GameEvent.indexOf("<div class=\"wrapper\">") + QString("<div class=\"wrapper\">").length();
                    int fine_wrapper = GameEvent.indexOf("</div>", init_wrapper);
                    if (fine_wrapper >= 0  && init_wrapper >= 0  && fine_wrapper > init_wrapper )
                        wrapper = GameEvent.mid(init_wrapper, fine_wrapper-init_wrapper);
                }

                EventBase event;

                QString TimeEvent;
                if (wrapper.length() > 20)
                {
                    int init_timebox = GameEvent.indexOf("<div class=\"time-box") + QString("<div class=\"time-box").length();
                    init_timebox = GameEvent.indexOf(">", init_timebox) +1;
                    int fine_timebox = GameEvent.indexOf("'</div>", init_timebox);
                    if (fine_timebox >= 0  && init_timebox >= 0  && fine_timebox > init_timebox )
                    {
                        TimeEvent = GameEvent.mid(init_timebox, fine_timebox-init_timebox);
                        int temp, timeMin, timeExtra = 0;

                        QStringList comp = TimeEvent.split("+");

                        if (comp.size() > 1) timeExtra = comp.at(1).toInt();

                        timeMin = comp.at(0).toInt();
                        if ( timeMin <= 45)
                        {
                            temp = 1;
                            timeMin = timeMin + timeExtra;
                        }
                        else
                        {
                            temp = 2;
                            timeMin = timeMin + timeExtra - 45;
                        }
                        event.setTime(QTime(temp, timeMin));
                    }
                }


                QString TypeEvent;
                if (wrapper.length() > 20)
                {
                    int init_typeevent = GameEvent.indexOf("<span class=\"icon ") + QString("<span class=\"icon ").length();
                    int fine_typeevent = GameEvent.indexOf("\">&nbsp;</span>", init_typeevent);
                    if (fine_typeevent >= 0  && init_typeevent >= 0  && fine_typeevent > init_typeevent )
                    {
                        TypeEvent = GameEvent.mid(init_typeevent, fine_typeevent-init_typeevent);
                        event.setDescription(TypeEvent);
                    }
                }
                homeEvents.append(event);

                    qDebug() << "Home Event Grabbed : " << event.prettyPrint();
                //ui->listWidget_eventHome->addItem(event.prettyPrint());


            }

            {


                QString GameEvent;

                initAway = Table.indexOf("<td class=\"summary-vertical fr", fineHome);
                fineAway = Table.indexOf("</td>", initAway + QString("<td class=\"summary-vertical fr").length());
                if (fineAway >= 0  && initAway >= 0  && fineAway > initAway )
                    GameEvent = Table.mid(initAway, fineAway-initAway);


                if (initAway < 0 || fineAway < 0)
                    break;


                QString wrapper;
                {
                    int init_wrapper = GameEvent.indexOf("<div class=\"wrapper\">") + QString("<div class=\"wrapper\">").length();
                    int fine_wrapper = GameEvent.indexOf("</div>", init_wrapper);
                    if (fine_wrapper >= 0  && init_wrapper >= 0  && fine_wrapper > init_wrapper )
                        wrapper = GameEvent.mid(init_wrapper, fine_wrapper-init_wrapper);
                }

                EventBase event;

                QString TimeEvent;
                if (wrapper.length() > 20)
                {
                    int init_timebox = GameEvent.indexOf("<div class=\"time-box") + QString("<div class=\"time-box").length();
                    init_timebox = GameEvent.indexOf(">", init_timebox) +1;
                    int fine_timebox = GameEvent.indexOf("'</div>", init_timebox);
                    if (fine_timebox >= 0  && init_timebox >= 0  && fine_timebox > init_timebox )
                    {
                        TimeEvent = GameEvent.mid(init_timebox, fine_timebox-init_timebox);
                        int temp, timeMin, timeExtra = 0;

                        QStringList comp = TimeEvent.split("+");

                        if (comp.size() > 1)
                            timeExtra = comp.at(1).toInt();

                        timeMin = comp.at(0).toInt();
                        if ( timeMin <= 45)
                        {
                            temp = 1;
                            timeMin = timeMin + timeExtra;
                        }
                        else
                        {
                            temp = 2;
                            timeMin = timeMin + timeExtra - 45;
                        }
                        event.setTime(QTime(temp, timeMin));
                    }
                }

                QString TypeEvent;
                if (wrapper.length() > 20)
                {
                    int init_typeevent = GameEvent.indexOf("<span class=\"icon ") + QString("<span class=\"icon ").length();
                    int fine_typeevent = GameEvent.indexOf("\">&nbsp;</span>", init_typeevent);
                    if (fine_typeevent >= 0  && init_typeevent >= 0  && fine_typeevent > init_typeevent )
                    {
                        TypeEvent = GameEvent.mid(init_typeevent, fine_typeevent-init_typeevent);
                        event.setDescription(TypeEvent);
                    }
                }


                awayEvents.append(event);

                qDebug() << "Away Event Grabbed : " << event.prettyPrint();
//                ui->listWidget_eventAway->addItem(event.prettyPrint());

            }

            i++;
        }
        emit newDataGrabbed();
    });


    return;

}
