#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->comboBox_season->clear();
    //  for (int year = 2010; year < 2017; year++)
    //      ui->comboBox_season->addItem(QString("%1-%2").arg(year).arg(year+1));
    for (int year = 2010; year < 2017; year++)
        ui->comboBox_season->addItem(QString("%1").arg(year));






    /*results_championship = new QWebEnginePage();
    teams = new QWebEnginePage();
    results_game = new QWebEnginePage();
*/
    currentGame = new Game();

    myChampionship = new Championship();


    connect(myChampionship, SIGNAL(newTeamAdded()), this, SLOT(on_myChampionship_newTeamAdded()));
    connect(myChampionship, SIGNAL(newGameAdded(Game*)), this, SLOT(on_myChampionship_newGameAdded(Game*)));


    //  connect(teams, SIGNAL(loadFinished(bool)),  this, SLOT(on_teams_loadFinished(bool)));
    // connect(teams, SIGNAL(loadStarted()),       this, SLOT(on_teams_loadStarted()));


    // connect(results_game, SIGNAL(loadFinished(bool)), this, SLOT(on_results_game_loadFinished(bool)));


    //    on_pushButton_LoadChampionship_clicked();
}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_myChampionship_newTeamAdded()
{
    qDebug() << "on_myChampionship_newTeamAdded";

    QStringList listofTeam = myChampionship->getListOfTeamsName();

    ui->listWidget_HomeTeams->clear();
    ui->listWidget_VisitorsTeams->clear();
    ui->listWidget_HomeTeams->addItems(listofTeam);
    ui->listWidget_VisitorsTeams->addItems(listofTeam);


    ui->tableWidget_Season->setColumnCount(listofTeam.count());
    ui->tableWidget_Season->setRowCount(listofTeam.count());

    ui->tableWidget_Season->setHorizontalHeaderLabels(listofTeam);
    ui->tableWidget_Season->setVerticalHeaderLabels(listofTeam);

    ui->tableWidget_Season->horizontalHeader()->resizeSections(QHeaderView::Stretch);

    ui->dockWidget_Teams->setEnabled(listofTeam.count() > 0);
}

void MainWindow::on_myChampionship_newGameAdded(Game * newGame)
{

    qDebug() << "newGameAdded : " << newGame->printScoreWithTeamNames();
    int col = 0, row = 0;

    for ( int i = 0; i < ui->tableWidget_Season->columnCount(); i++)
        if (newGame->awayTeam->getFullName() == ui->tableWidget_Season->horizontalHeaderItem(i)->text())
            col = i;

    for ( int j = 0; j < ui->tableWidget_Season->rowCount(); j++)
        if (newGame->homeTeam->getFullName() == ui->tableWidget_Season->verticalHeaderItem(j)->text())
            row = j;

    QTableWidgetItem *setdes = new QTableWidgetItem();
    setdes->setText(newGame->printScoreWithTeamNames());
    // qDebug() << "putting in (row:" << row << ", col:" << col<<")";
    ui->tableWidget_Season->setItem(row,col,setdes);

}



void MainWindow::on_pushButton_LoadTeams_clicked()
{
    myChampionship->loadTeams(ui->comboBox_league->currentText(), ui->comboBox_season->currentText().toInt());
}

void MainWindow::on_pushButton_LoadGames_clicked()
{
    myChampionship->loadGames(ui->comboBox_league->currentText(), ui->comboBox_season->currentText().toInt());
}

void MainWindow::on_pushButton_LoadMore_clicked()
{
    myChampionship->loadMoreGames();
}
/*
void MainWindow::on_results_championship_loadFinished(bool)
{
    //  isLoaded = true;
    qDebug() << "on_results_championship_loadFinished";

    //parseGames

}
*/
/*
void MainWindow::on_teams_loadStarted()
{
    qDebug() << "on_teams_loadStarted";
    ui->dockWidget_Teams->setEnabled(false);
}*/
/*
void MainWindow::on_teams_loadFinished(bool ok)
{
    qDebug() << "on_teams_loadFinished" << ok;
    if (ok) on_pushButton_GetTeams_clicked();
}*/

/*
void MainWindow::on_results_game_loadFinished(bool ok )
{
    //isLoaded = true;
    qDebug() << "on_results_game_loadFinished" << ok;
    on_pushButton_GetGameDetails_clicked();
}*/


/*
void MainWindow::on_pushButton_GetTeams_clicked()
{
    qDebug () << "on_pushButton_GetTeams_clicked";

    teams->toHtml([this](const QString& result) mutable
    {
        ui->listWidget_HomeTeams->clear();
        ui->listWidget_VisitorsTeams->clear();
        int init=0, fine=0;
        QStringList listofTeam;

        myChampionship->clearTeams();

        while (init >= 0 && fine >= 0)
        {
            init = result.indexOf("<tr id=\"participant_", fine);
            init = result.indexOf("<a href=", init);
            init = result.indexOf(">", init) +1;
            fine = result.indexOf("</a>", init);


            QString thisTeam = result.mid(init, fine-init);

           // if (!thisTeam.isEmpty())
             //   myChampionship->addTeam(Team(thisTeam));
            //  qDebug() << "Find from" << init << "to" << fine << " : "<< thisTeam;



        }

        ui->listWidget_HomeTeams->addItems(myChampionship->getListOfTeamsName());
        ui->listWidget_VisitorsTeams->addItems(myChampionship->getListOfTeamsName());

        ui->tableWidget_Season->setColumnCount(myChampionship->countTeams());
        ui->tableWidget_Season->setRowCount(myChampionship->countTeams());

        ui->tableWidget_Season->setHorizontalHeaderLabels(myChampionship->getListOfTeamsName());
        ui->tableWidget_Season->setVerticalHeaderLabels(myChampionship->getListOfTeamsName());

        ui->tableWidget_Season->horizontalHeader()->resizeSections(QHeaderView::Stretch);


        ui->dockWidget_Teams->setEnabled(true);

    });

}
*/
void MainWindow::on_pushButton_GetGameID_clicked()
{
    qDebug() << "on_pushButton_GetGameID_clicked";

    Team *Home = new Team();
    Team *Away = new Team();
    if (ui->tabWidget->currentIndex() == 0) // 2 column
    {
        Home = new Team(ui->listWidget_HomeTeams->currentItem()->text());
        Away = new Team(ui->listWidget_VisitorsTeams->currentItem()->text());
    }
    else if (ui->tabWidget->currentIndex() ==1) // Table
    {
        int row = ui->tableWidget_Season->currentRow();
        int col = ui->tableWidget_Season->currentColumn();
        Home = new Team(ui->tableWidget_Season->horizontalHeaderItem(row)->text());
        Away = new Team(ui->tableWidget_Season->verticalHeaderItem(col)->text());
    }


    currentGame = new Game();
    currentGame = myChampionship->getGame(Home, Away);
    connect(currentGame, SIGNAL(newDataGrabbed()), this, SLOT(on_currentGame_newDataGrabbed()));

    ui->label_HomeTeam->setText(currentGame->homeTeam->getFullName());
    ui->label_AwayTeam->setText(currentGame->awayTeam->getFullName());

    ui->label_Score->setText(currentGame->printScore());
    ui->label_Time->setText(currentGame->printTime());
    //   ui->label_ID->setText(currentGame->getTime());

    //ui->label_Score->setText(currentGame->getAwayTeam()->getFullName());

    QString hyperlink =
            QString("<a href=\"http://www.flashscore.ae/match/%1/#matchsummary\">%1</a>")
            .arg(currentGame->getFlashScoreID());

    //     ui->label_ID->setName(GameID);
    ui->label_ID->setText(hyperlink);
    ui->label_ID->setTextFormat(Qt::RichText);
    ui->label_ID->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_ID->setOpenExternalLinks(true);


    /*
    results_championship->toHtml([this](const QString& result) mutable
    {
        int init = 0, fine = 0;
        QString homeTeam = ui->listWidget_HomeTeams->currentItem()->text();
        QString visitorTeam = ui->listWidget_VisitorsTeams->currentItem()->text();


        QString GameInfo;
        bool match_found = false;
        while (init >= 0 && fine >= 0)
        {
            init = result.indexOf("<tr id=\"g_1_", fine);
            fine = result.indexOf("</tr>", init);

            QString thisGame = result.mid(init, fine-init);
            int i_homeTeam = thisGame.indexOf(homeTeam);
            int i_visitorTeam = thisGame.indexOf(visitorTeam);
            // qDebug () << i_homeTeam << i_visitorTeam;
            if (i_homeTeam < i_visitorTeam && i_homeTeam >= 0 && i_visitorTeam >= 0)
            {
                GameInfo = thisGame;
                match_found = true;
                break;
            }

        }

        if (!match_found)
        {
            qDebug () << "Match not found";
            results_championship->runJavaScript(QString("loadMoreGames()"),[](const QVariant &v) { qDebug() << v.toString(); });
            myGameID = "";
        }
        //   qDebug() << GameInfo;



        //  QString GameID;
        init = GameInfo.indexOf("<tr id=\"g_1_") + QString("<tr id=\"g_1_").length();
        fine = GameInfo.indexOf("\" class=\"", init);
        if (fine >= 0  && init >= 0  && fine > init )
            myGameID = GameInfo.mid(init, fine-init);
        //   qDebug() << "GameID" << myGameID;

        QString hyperlink =
                QString("<a href=\"http://www.flashscore.ae/match/%1/#matchsummary\">%1</a>")
                .arg(myGameID);

        //     ui->label_ID->setName(GameID);
        ui->label_ID->setText(hyperlink);
        ui->label_ID->setTextFormat(Qt::RichText);
        ui->label_ID->setTextInteractionFlags(Qt::TextBrowserInteraction);
        ui->label_ID->setOpenExternalLinks(true);



        QString GameHomeTeam;
        init = GameInfo.indexOf(QString("<td class=\"cell_ab team-home"));
        init = GameInfo.indexOf(QString(">"),init) + 1;
        fine = GameInfo.indexOf(QString("</td>"), init);
        if (fine >= 0  && init >= 0  && fine > init )
            GameHomeTeam = GameInfo.mid(init, fine-init);

        init = GameHomeTeam.indexOf(QString("<span class=\"padr"));
        init = GameHomeTeam.indexOf(QString(">"),init) + 1;
        fine = GameHomeTeam.indexOf(QString("</span>"), init);
        if (fine >= 0  && init >= 0  && fine > init )
            GameHomeTeam = GameHomeTeam.mid(init, fine-init);

        //   qDebug() << "GameHomeTeam" << GameHomeTeam;
        ui->label_HomeTeam->setText(GameHomeTeam);




        QString GameAwayTeam;
        init = GameInfo.indexOf(QString("<td class=\"cell_ac team-away"));
        init = GameInfo.indexOf(QString(">"),init) + 1;
        fine = GameInfo.indexOf(QString("</td>"), init);
        if (fine >= 0  && init >= 0  && fine > init )
            GameAwayTeam = GameInfo.mid(init, fine-init);

        init = GameAwayTeam.indexOf(QString("<span class=\"padr"));
        init = GameAwayTeam.indexOf(QString(">"),init) + 1;
        fine = GameAwayTeam.indexOf(QString("</span>"), init);
        if (fine >= 0  && init >= 0  && fine > init )
            GameAwayTeam = GameAwayTeam.mid(init, fine-init);

        //    qDebug() << "GameAwayTeam" << GameAwayTeam;
        ui->label_AwayTeam->setText(GameAwayTeam);





        QString GameTime;
        init = GameInfo.indexOf(QString("<td class=\"cell_ad time"));
        init = GameInfo.indexOf(QString(">"),init) + 1;
        fine = GameInfo.indexOf(QString("</td>"), init);
        if (fine >= 0  && init >= 0  && fine > init )
            GameTime = GameInfo.mid(init, fine-init);
        //   qDebug() << "GameTime" << GameTime;
        ui->label_Time->setText(GameTime);





        QString GameScore;
        init = GameInfo.indexOf(QString("<td class=\"cell_sa score"));
        init = GameInfo.indexOf(QString(">"),init) + 1;
        fine = GameInfo.indexOf(QString("</td>"), init);
        if (fine >= 0  && init >= 0  && fine > init )
            GameScore = GameInfo.mid(init, fine-init).replace("&nbsp;"," ");
        //   qDebug() << "GameScore" << GameScore;
        ui->label_Score->setText(GameScore);




        on_pushButton_LoadGame_clicked();
    });
*/

}

//void MainWindow::on_pushButton_LoadChampionship_clicked()
//{
/*  qDebug() << "on_pushButton_LoadChampionship_clicked";

    QString ChampionshipName = QString("%1-%2")
            .arg(ui->comboBox_league->currentText())
            .arg(ui->comboBox_season->currentText());

    myChampionship.setName(ChampionshipName);

    QString URL;
    URL = QString("http://www.flashscore.ae/football/%1/teams/")
            .arg(ChampionshipName);

    //isLoaded = false;
    teams->load(QUrl(URL));


    URL = QString("http://www.flashscore.ae/football/%1/results/")
            .arg(ChampionshipName);

    // isLoaded = false;
    results_championship->load(QUrl(URL));*/
//}

/*
void MainWindow::on_pushButton_LoadGame_clicked()
{
    QString URL;
    URL = QString("http://www.flashscore.ae/match/%1/#matchsummary")
            .arg(myGameID);

    qDebug() << URL;
    //isLoaded = false;
    results_game->load(QUrl(URL));

}
*/

void MainWindow::on_pushButton_GetGameDetails_clicked()
{
    qDebug() << "on_pushButton_GetGameDetails_clicked";

    currentGame->loadFlashScoreInformation();

    /* results_game->toHtml([this](const QString& result) mutable
    {
        int init = 0, fine = 0;
        int i=0;

        // Focus on tabel only

        QString Table;
        init = result.indexOf("<table id=\"parts", fine) + QString("<table id=\"parts").length();
        fine = result.indexOf("</table>", init);
        if (fine >= 0  && init >= 0  && fine > init )
            Table = result.mid(init, fine-init);


        int initHome = 0;
        int fineHome = 0;

        int initAway= 0;
        int fineAway = 0;

        ui->listWidget_eventAway->clear();
        ui->listWidget_eventHome->clear();

        QString EventString;


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

                ui->listWidget_eventHome->addItem(event.prettyPrint());


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



                ui->listWidget_eventAway->addItem(event.prettyPrint());

            }

            i++;
        }
    });*/
}


void MainWindow::on_currentGame_newDataGrabbed()
{
    // currentGame->

    qDebug() << "on_currentGame_newDataGrabbed";

    ui->listWidget_eventAway->clear();
    ui->listWidget_eventHome->clear();

    foreach (EventBase event, currentGame->getHomeEvents())
    {
        ui->listWidget_eventHome->addItem(event.prettyPrint());
    }

    foreach (EventBase event, currentGame->getAwayEvents())
    {
        ui->listWidget_eventAway->addItem(event.prettyPrint());
    }
}



void MainWindow::on_comboBox_league_currentIndexChanged(const QString &arg1)
{
    updateChampionshipLinks();
}

void MainWindow::on_comboBox_season_currentIndexChanged(const QString &arg1)
{
    updateChampionshipLinks();
}

void MainWindow::updateChampionshipLinks()
{

    QString ChampionshipName = QString("%1-%2-%3")
            .arg(ui->comboBox_league->currentText())
            .arg(ui->comboBox_season->currentText().toInt())
            .arg(ui->comboBox_season->currentText().toInt() + 1);

    QString teamsLink;
    teamsLink = QString("http://www.flashscore.ae/football/%1/teams/")
            .arg(ChampionshipName);

    {
        QString hyperlink =
                QString("<a href=\"%1\">%1</a>")
                .arg(teamsLink);

        ui->label_linkTeams->setText(hyperlink);
        ui->label_linkTeams->setTextFormat(Qt::RichText);
        ui->label_linkTeams->setTextInteractionFlags(Qt::TextBrowserInteraction);
        ui->label_linkTeams->setOpenExternalLinks(true);
    }


    QString resultsLink;
    resultsLink = QString("http://www.flashscore.ae/football/%1/results/")
            .arg(ChampionshipName);

    {
        QString hyperlink =
                QString("<a href=\"%1\">%1</a>")
                .arg(resultsLink);

        ui->label_linkResults->setText(hyperlink);
        ui->label_linkResults->setTextFormat(Qt::RichText);
        ui->label_linkResults->setTextInteractionFlags(Qt::TextBrowserInteraction);
        ui->label_linkResults->setOpenExternalLinks(true);
    }
    return;
}

void MainWindow::on_pushButton_FindYoutubeVideo_clicked()
{
    QString link = QString("https://www.youtube.com/results?search_query=%1+%2")
            .arg(currentGame->homeTeam->getFullName())
            .arg(currentGame->awayTeam->getFullName());
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_pushButton_ExportJSON_clicked()
{

}

void MainWindow::on_pushButton_launchJS_clicked()
{
    myChampionship->launchJS();
}
