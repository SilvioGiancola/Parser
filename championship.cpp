#include "championship.h"

Championship::Championship()
{
    FlashScoreTeamsListWebEngine = new QWebEnginePage();
    connect(FlashScoreTeamsListWebEngine, SIGNAL(loadFinished(bool)),
            this, SLOT(on_teams_loadFinished(bool)));

    FlashScoreWebEngine = new QWebEnginePage();
    connect(FlashScoreWebEngine, SIGNAL(loadFinished(bool)),
            this, SLOT(on_FlashScoreWebEngine_loadFinished(bool)));
}



QString Championship::getLeague()
{
    return League;
}

int Championship::getYear()
{
    return Year;
}

void Championship::setLeague(QString value)
{
    League = value;
}

void Championship::setYear(int value)
{
    Year = value;
}





void Championship::printListofTeams()
{
    qDebug() << "print all Teams FullName: " << Teams.count();
    foreach (Team *t, Teams)
        qDebug() << t->getFullName();
}

QString Championship::printChampionship()
{
    return QString("%1-%2-%3").arg(League).arg(Year).arg(Year+1);
}

bool Championship::addTeam(Team *team)
{
    if (hasTeam(team)) return true;

    Teams.append(team);

    emit newTeamAdded();

    foreach (Team * oldTeam, Teams)
    {
        if (oldTeam != team)
        {
            Games.append(new Game(oldTeam, team));
            Games.append(new Game(team, oldTeam));
        }
    }

    return false;
}

bool Championship::hasGame(Game *thisgame)
{
    foreach (Game *game, Games)
        if ((*game) == (*thisgame))
            return true;
    return false;
}
bool Championship::hasTeam(Team *thisteam)
{
    foreach (Team *team, Teams)
        if ((*team) == (*thisteam))
            return true;
    return false;
}


/*

bool Championship::fillGame(Game *newgame)
{
    if (!hasGame(newgame))
        return true;

   // Game * currentGame = this->getGame(newgame);
   // currentGame = newgame;
    //  Games.append(newgame);
    emit gameEdited(newgame);
    return false;
}
*/

QStringList Championship::getListOfTeamsName()
{
    QStringList listName;
    foreach (Team *team, Teams)
        listName.append(team->getFullName());
    return listName;
}


QList<Team*> Championship::getAllTeams()
{
    return Teams;
}

QList<Game*> Championship::getAllGames()
{
    return Games;
}

int Championship::countGames()
{
    return Games.count();
}




Game* Championship::getGame(QString homeTeam, QString awayTeam)
{
    return getGame(new Team(homeTeam), new Team(awayTeam));
}


Game* Championship::getGame(Team * homeTeam, Team * awayTeam)
{
    return getGame(new Game(homeTeam, awayTeam));
}


Game* Championship::getGame(Game * thisgame)
{
    foreach(Game * game, Games)
    {
        if ((*game) == *thisgame)
        {
            qDebug() << "I have found your game : " << game->printScoreWithTeamNames();
            return game;
        }
    }
    return new Game();
}

int Championship::countTeams()
{
    return Teams.count();
}




void Championship::loadTeams()
{
    QString teamsURL = QString("http://www.flashscore.ae/football/%1/teams/")
            .arg(printChampionship());
    FlashScoreTeamsListWebEngine->load(QUrl(teamsURL));

    return;
}

void Championship::loadGames()
{
    QString resultsURL = QString("http://www.flashscore.ae/football/%1/results/")
            .arg(printChampionship());
    FlashScoreWebEngine->load(QUrl(resultsURL));

    return;
}

void Championship::launchJS()
{
    FlashScoreWebEngine->runJavaScript(QString("loadMoreGames()"),[this](const QVariant &v){qDebug() << "launched JavaScript for More Games  " << v.toString();});
}

void Championship::loadMoreGames()
{
    on_FlashScoreWebEngine_loadFinished(true);
    return;
}

void Championship::on_teams_loadFinished(bool ok)
{
    qDebug () << "on_teams_loadFinished " << ok;

    FlashScoreTeamsListWebEngine->toHtml([this](const QString& result) mutable
    {
        Games.clear();
        Teams.clear();

        int init=0, fine=0;

        while (init >= 0 && fine >= 0)
        {
            init = result.indexOf("<tr id=\"participant_", fine);
            init = result.indexOf("<a href=", init);
            init = result.indexOf(">", init) +1;
            fine = result.indexOf("</a>", init);

            QString thisTeamName = result.mid(init, fine-init);

            if (!thisTeamName.isEmpty())
            {
                Team * thisTeam =   new Team(thisTeamName);
                this->addTeam(thisTeam);
            }
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



            if (GameInfo.length() < 20)
                continue;


            QString GameID;
            init = GameInfo.indexOf("<tr id=\"g_1_") + QString("<tr id=\"g_1_").length();
            fine = GameInfo.indexOf("\" class=\"", init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameID = GameInfo.mid(init, fine-init);




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

/*
            Team *homeTeam = new Team();
            for (int i = 0; i < Teams.count(); i++)
                if (Teams.at(i)->getFullName() == GameHomeTeam)
                    homeTeam = Teams.at(i);

            Team *awayTeam = new Team();
            for (int i = 0; i < Teams.count(); i++)
                if (Teams.at(i)->getFullName() == GameAwayTeam)
                    awayTeam = Teams.at(i);
*/


            //Game *thisGame = new Game(GameHomeTeam, GameAwayTeam);
            Game* thisGame = this->getGame(GameHomeTeam, GameAwayTeam);
            thisGame->setflashScoreID(GameID);



            QString GameTime;
            init = GameInfo.indexOf(QString("<td class=\"cell_ad time"));
            init = GameInfo.indexOf(QString(">"),init) + 1;
            fine = GameInfo.indexOf(QString("</td>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameTime = GameInfo.mid(init, fine-init);

            GameTime = QString("%1.%2").arg(Year).arg(GameTime);
            qDebug() << "GameTime : " << GameTime;
            thisGame->parseTime(GameTime, QString("yyyy.dd.MM. hh:mm"));
            //ui->label_Time->setText(GameTime);





            QString GameScore;
            init = GameInfo.indexOf(QString("<td class=\"cell_sa score"));
            init = GameInfo.indexOf(QString(">"),init) + 1;
            fine = GameInfo.indexOf(QString("</td>"), init);
            if (fine >= 0  && init >= 0  && fine > init )
                GameScore = GameInfo.mid(init, fine-init).replace("&nbsp;"," ");
            // qDebug() << "GameScore" << GameScore;


            thisGame->parseScore(GameScore);

            emit gameEdited(thisGame);

          //  this->fillGame(thisGame);
        }


     //   qDebug() << "I have parsed " << countGames() << "games";
     //   qDebug() << "I have parsed " << countTeams() << "teams";

        //<a href="#" onclick="loadMoreGames(); return false;">Show more matches</a>



        if (result.contains("Show more matches"))
        {

            qDebug()<< "JS found!";

            launchJS();
        }

        qDebug() << "";

    });


    return;
}



void Championship::saveJSON(QString filename)
{
    qDebug() << "saveJSON :" << filename;
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QJsonObject Dataset;


        QJsonObject Championship;
        foreach (Game *game, getAllGames())
            Championship[game->getFlashScoreID()] = game->exportJSonObject();
        Dataset["Championship"] = Championship;


        QJsonObject Teams;
        foreach (Team *team, getAllTeams())
            Teams[team->getShortName()] = team->getFullName();
        Dataset["Teams"] = Teams;

        QJsonDocument saveDoc(Dataset);
        file.write(saveDoc.toJson());
    }
}

bool Championship::loadJSON(QString filename)
{
    qDebug() << "loadJSON :" << filename;

    QFile file(filename);

    if ( !file.open(QIODevice::ReadOnly) )
    {
        qDebug() << "cannot open " << filename;
        return true;
    }


    QByteArray saveData = file.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject Dataset = loadDoc.object();


    Teams.clear();
    Games.clear();

    QJsonObject Teams = Dataset["Teams"].toObject();
    foreach(QString ShortName, Teams.keys())
    {
        addTeam(new Team(Teams[ShortName].toString()));
    }



    QJsonObject Championship = Dataset["Championship"].toObject();
    foreach(QString gameKeyID, Championship.keys())
    {


       // qDebug() << "filling" << gameKeyID;
        QJsonObject GameJson = Championship[gameKeyID].toObject();

        Game *myGame = getGame(
                    GameJson["gameHomeTeam"].toString(),
                GameJson["gameAwayTeam"].toString());


          myGame->importJSonObject(GameJson, gameKeyID);


          emit gameEdited(myGame);
       // fillGame(myGame);


        //  Game * myGame = myChampionship->getGame(new Team(teamHomeName), new Team(teamAwayName));

    }



    return false;
}
