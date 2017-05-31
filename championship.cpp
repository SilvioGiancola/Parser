#include "championship.h"

Championship::Championship()
{
 /*   FlashScoreWebEngine = new QWebEnginePage();
    FlashScoreTeamsListWebEngine = new QWebEnginePage();

    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)), this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));
*/

    FlashScoreTeamsListWebEngine = new QWebEnginePage();
    connect(FlashScoreTeamsListWebEngine, SIGNAL(loadFinished(bool)),
            this, SLOT(on_teams_loadFinished(bool)));


    FlashScoreWebEngine = new QWebEnginePage();
    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)),
            this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));

}
/*
Championship::Championship(QString Name, int Year)
{
    FlashScoreTeamsListWebEngine = new QWebEnginePage();
    connect(FlashScoreTeamsListWebEngine, SIGNAL(loadFinished(bool)),
            this, SLOT(on_teams_loadFinished(bool)));


    FlashScoreWebEngine = new QWebEnginePage();
    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)),
            this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));

*/
/*
    FlashScoreWebEngine = new QWebEnginePage();
    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)),
            this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));

    QString resultsURL = QString("http://www.flashscore.ae/football/%1-%2-%3/results/")
            .arg(Name).arg(Year).arg(Year+1);
    FlashScoreWebEngine->load(QUrl(resultsURL));
*/
//}

void Championship::printListofTeams()
{
    qDebug() << "print all Teams FullName: " << Teams.count();
    foreach (Team *t, Teams)
        qDebug() << t->getFullName();
}

bool Championship::addTeam(Team *team)
{
    if (Teams.contains(team))
        return true;
    Teams.append(team);
    emit newTeamAdded();
    return false;
}


bool Championship::addGame(Game *newgame)
{
    foreach (Game *game, Games)
        if ((*game) == (*newgame))
            return true;

    Games.append(newgame);
    emit newGameAdded(Games.last());
    return false;
}


QStringList Championship::getListOfTeamsName()
{
    QStringList listName;
    foreach (Team *team, Teams)
        listName.append(team->getFullName());
    return listName;
}


QList<Game*> Championship::getAllGames()
{
    return Games;
}

int Championship::countGames()
{
    return Games.count();
}

Game* Championship::getGame(Team * homeTeam, Team * awayTeam)
{
    foreach(Game * game, Games)
    {
        qDebug () << "comparing " << game->homeTeam->getFullName() << "with" << homeTeam->getFullName();
        qDebug () << "comparing " << game->awayTeam->getFullName() << "with" << awayTeam->getFullName();
        if ( ((*game->homeTeam) == (*homeTeam)) && ((*game->awayTeam) == (*awayTeam)) )
     //   if (game == new Game(homeTeam,awayTeam))
        {
            qDebug() << "I have found your game";
            qDebug() << "results:" << game->printScoreWithTeamNames();
            return game;
        }
    }
    return new Game();
}

int Championship::countTeams()
{
    return Teams.count();
}

void Championship::clearTeams()
{
    Teams.clear();
    return;
}

bool Championship::setName(QString Name)
{
    this->Name = Name;
    return false;
}



void Championship::loadTeams(QString Name, int Year)
{
    this->Name = Name;
    this->Year = Year;
    QString teamsURL = QString("http://www.flashscore.ae/football/%1-%2-%3/teams/")
            .arg(Name).arg(Year).arg(Year+1);
    FlashScoreTeamsListWebEngine->load(QUrl(teamsURL));

    return;
}

void Championship::loadGames(QString Name, int Year)
{
    this->Name = Name;
    this->Year = Year;
    QString resultsURL = QString("http://www.flashscore.ae/football/%1-%2-%3/results/")
            .arg(Name).arg(Year).arg(Year+1);
    FlashScoreWebEngine->load(QUrl(resultsURL));

    return;
}

void Championship::launchJS()
{
    FlashScoreWebEngine->runJavaScript(QString("loadMoreGames()"),[](const QVariant &v){});
}

void Championship::loadMoreGames()
{
   // qDebug() << "loading More Games ...";
   // FlashScoreWebEngine->runJavaScript(QString("loadMoreGames()"),[this](const QVariant &v)
    //{
    //    qDebug() << v.toString();
    //    qDebug() << "Im inside the Javascript call";
    //    qDebug() << "tying to load more ...";
        on_FlashScoreWebEngine_loadFinished(true);
   //     qDebug() << " ...Finished !";
   // });

    return;
}


void Championship::on_teams_loadFinished(bool ok)
{
    qDebug () << "on_teams_loadFinished " << ok;

    FlashScoreTeamsListWebEngine->toHtml([this](const QString& result) mutable
    {
        this->clearTeams();

        int init=0, fine=0;

        while (init >= 0 && fine >= 0)
        {
            init = result.indexOf("<tr id=\"participant_", fine);
            init = result.indexOf("<a href=", init);
            init = result.indexOf(">", init) +1;
            fine = result.indexOf("</a>", init);

            QString thisTeam = result.mid(init, fine-init);

            if (!thisTeam.isEmpty())
                this->addTeam(new Team(thisTeam));
        }

    });

}

void Championship::on_FlashScoreWebEngine_loadFinished(bool ok)
{
    qDebug () << "on_FlashScoreWebEngine_loadFinished" << ok;
    FlashScoreWebEngine->toHtml([this](const QString& result) mutable
    {


        int init = 0, fine = 0;

        int initGame = 0;
        int fineGame = 0;

        while (initGame >= 0 && fineGame >= 0)
        {
            initGame = result.indexOf("<tr id=\"g_1_", fineGame);
            fineGame = result.indexOf("</tr>", initGame);

            QString GameInfo = result.mid(initGame, fineGame-initGame);


            GameInfo = GameInfo.replace("<span class=\"racard racard1\">&nbsp;</span>", "");
            GameInfo = GameInfo.replace("<span class=\"racard racard2\">&nbsp;</span>", "");
            GameInfo = GameInfo.replace("<span class=\"racard racard3\">&nbsp;</span>", "");


          //  qDebug() << "parsing: ";
          //  qDebug() << GameInfo;

            if (GameInfo.length() < 20)
                continue;

          /*  int i_homeTeam = thisGame.indexOf(homeTeam);
            int i_visitorTeam = thisGame.indexOf(visitorTeam);
            // qDebug () << i_homeTeam << i_visitorTeam;
            if (i_homeTeam < i_visitorTeam && i_homeTeam >= 0 && i_visitorTeam >= 0)
            {
                GameInfo = thisGame;
                match_found = true;
                break;
            }*/


            /* if (!match_found)
        {
            qDebug () << "Match not found";
            results_championship->runJavaScript(QString("loadMoreGames()"),[](const QVariant &v) { qDebug() << v.toString(); });
            myGameID = "";
        }*/
            //   qDebug() << GameInfo;


            QString GameID;
            init = GameInfo.indexOf("<tr id=\"g_1_") + QString("<tr id=\"g_1_").length();
            fine = GameInfo.indexOf("\" class=\"", init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameID = GameInfo.mid(init, fine-init);

            //   qDebug() << "GameID" << myGameID;
/*
            QString hyperlink =
                    QString("<a href=\"http://www.flashscore.ae/match/%1/#matchsummary\">%1</a>")
                    .arg(myGameID);*/

            //     ui->label_ID->setName(GameID);
          /*  ui->label_ID->setText(hyperlink);
            ui->label_ID->setTextFormat(Qt::RichText);
            ui->label_ID->setTextInteractionFlags(Qt::TextBrowserInteraction);
            ui->label_ID->setOpenExternalLinks(true);*/



            QString GameHomeTeam;
            init = GameInfo.indexOf(QString("<td class=\"cell_ab team-home"));
            init = GameInfo.indexOf(QString(">"),init) + 1;
            fine = GameInfo.indexOf(QString("</td>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameHomeTeam = GameInfo.mid(init, fine-init);

            init = GameHomeTeam.indexOf(QString("<span class=\"pad"));
            init = GameHomeTeam.indexOf(QString(">"),init) + 1;
            fine = GameHomeTeam.indexOf(QString("</span>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameHomeTeam = GameHomeTeam.mid(init, fine-init);


           // this->addTeam(new Team(GameHomeTeam));
       //     thisGame.setAwayTeam(Teams.at(Teams.indexOf(Team(GameHomeTeam))));
            //   qDebug() << "GameHomeTeam" << GameHomeTeam;
         //   ui->label_HomeTeam->setText(GameHomeTeam);




            QString GameAwayTeam;
            init = GameInfo.indexOf(QString("<td class=\"cell_ac team-away"));
            init = GameInfo.indexOf(QString(">"),init) + 1;
            fine = GameInfo.indexOf(QString("</td>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameAwayTeam = GameInfo.mid(init, fine-init);

            init = GameAwayTeam.indexOf(QString("<span class=\"pad"));
            init = GameAwayTeam.indexOf(QString(">"),init) + 1;
            fine = GameAwayTeam.indexOf(QString("</span>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameAwayTeam = GameAwayTeam.mid(init, fine-init);

          //  this->addTeam(new Team(GameAwayTeam));
          //  thisGame.setAwayTeam(Teams.at(Teams.indexOf(Team(GameAwayTeam))));
            //    qDebug() << "GameAwayTeam" << GameAwayTeam;
            //ui->label_AwayTeam->setText(GameAwayTeam);

            Team *homeTeam = new Team();
            for (int i = 0; i < Teams.count(); i++)
                if (Teams.at(i)->getFullName() == GameHomeTeam)
                    homeTeam = Teams.at(i);

            Team *awayTeam = new Team();
            for (int i = 0; i < Teams.count(); i++)
                if (Teams.at(i)->getFullName() == GameAwayTeam)
                    awayTeam = Teams.at(i);


           // int i_home = Teams.indexOf(new Team(GameHomeTeam));
         //   Team *homeTeam = Teams.takeAt(i_home);
          //  int i_away = Teams.indexOf(new Team(GameAwayTeam));
         //   Team *awayTeam = Teams.takeAt(i_away);

            Game *thisGame = new Game(homeTeam, awayTeam);
            thisGame->setflashScoreID(GameID);

         //   Game *thisGame = new Game(GameID);



            QString GameTime;
            init = GameInfo.indexOf(QString("<td class=\"cell_ad time"));
            init = GameInfo.indexOf(QString(">"),init) + 1;
            fine = GameInfo.indexOf(QString("</td>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameTime = GameInfo.mid(init, fine-init);

            //   qDebug() << "GameTime : " << GameTime;
               thisGame->parseTime(QString("%1.%2").arg(Year).arg(GameTime), QString("YYYY.dd.MM. hh:mm"));
            //ui->label_Time->setText(GameTime);





            QString GameScore;
            init = GameInfo.indexOf(QString("<td class=\"cell_sa score"));
            init = GameInfo.indexOf(QString(">"),init) + 1;
            fine = GameInfo.indexOf(QString("</td>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameScore = GameInfo.mid(init, fine-init).replace("&nbsp;"," ");
           // qDebug() << "GameScore" << GameScore;

            thisGame->setHomeTeamScore(GameScore.mid(0,1).toInt());
            thisGame->setAwayTeamScore(GameScore.mid(4,1).toInt());


            this->addGame(thisGame);
        }


        qDebug() << "I have parsed " << countGames() << "games";
        qDebug() << "I have parsed " << countTeams() << "teams";

//<a href="#" onclick="loadMoreGames(); return false;">Show more matches</a>

        if (result.contains("Show more matches"))
            qDebug()<< "JS found!";

        qDebug() << "";

    });


    return;
}
