#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->hide();
    mux = new QMutex();



    // setup my Championship
    myChampionship = new Championship();



    QStringList Leagues = {
        "italy/serie-a",
        "germany/bundesliga",
        "spain/laliga",
        "england/epl",
        "france/ligue-1",
        "europe/uefa-champions-league"
    };


    ui->comboBox_league->clear();
    ui->listWidget_BatchLeagues->clear();

    foreach (QString league, Leagues)
    {
        ui->comboBox_league->addItem(league);
        QListWidgetItem* item = new QListWidgetItem(league, ui->listWidget_BatchLeagues);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Unchecked); // AND initialize check state
    }




    connect(myChampionship, SIGNAL(newTeamAdded()), this, SLOT(on_myChampionship_newTeamAdded()));
    connect(myChampionship, SIGNAL(gameEdited(Game*)), this, SLOT(on_myChampionship_gameEdited(Game*)));


    myChampionship->setLeague(ui->comboBox_league->currentText());
    myChampionship->setYear(ui->spinBox_year->value());
    updateChampionshipLinks(myChampionship->getLeague(), myChampionship->getYear());



    //setup selected Game
    currentGame = new Game();



    currentGameWebEngine = new QWebEnginePage();



    player = new QMediaPlayer(this);

    player->setVideoOutput(ui->videoWidget);
    player->setMedia(QUrl::fromLocalFile("/home/giancos/2015-04-12 Hellas Verona vs Inter Milan 1st.mkv"));



}




MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_spinBox_year_valueChanged(int arg1)
{
    myChampionship->setYear(arg1);
    updateChampionshipLinks(myChampionship->getLeague(), arg1);
}

void MainWindow::on_comboBox_league_currentTextChanged(const QString &arg1)
{
    myChampionship->setLeague(arg1);
    updateChampionshipLinks(arg1, myChampionship->getYear());
}

void MainWindow::updateChampionshipLinks(QString league, int year)
{
    QString ChampionshipName = QString("%1-%2-%3")
            .arg(league).arg(year).arg(year + 1);


    QString teamsLink;
    teamsLink = QString("http://www.flashscore.ae/football/%1/teams/")
            .arg(ChampionshipName);

    ui->label_linkTeams->setText(QString("<a href=\"%1\">%1</a>").arg(teamsLink));
    ui->label_linkTeams->setTextFormat(Qt::RichText);
    ui->label_linkTeams->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_linkTeams->setOpenExternalLinks(true);


    QString resultsLink;
    resultsLink = QString("http://www.flashscore.ae/football/%1/results/")
            .arg(ChampionshipName);

    ui->label_linkResults->setText(QString("<a href=\"%1\">%1</a>").arg(resultsLink));
    ui->label_linkResults->setTextFormat(Qt::RichText);
    ui->label_linkResults->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_linkResults->setOpenExternalLinks(true);

    return;
}






void MainWindow::on_myChampionship_newTeamAdded()
{
    QMutexLocker muxlocker(mux);

    qDebug() << "on_myChampionship_newTeamAdded";

    QStringList listofTeam = myChampionship->getListOfTeamsName();

    // handle list
    ui->listWidget_HomeTeams->clear();
    ui->listWidget_AwayTeams->clear();
    ui->listWidget_HomeTeams->addItems(listofTeam);
    ui->listWidget_AwayTeams->addItems(listofTeam);


    // handle table
    ui->tableWidget_Season->setColumnCount(listofTeam.count());
    ui->tableWidget_Season->setRowCount(listofTeam.count());

    ui->tableWidget_Season->setHorizontalHeaderLabels(listofTeam);
    ui->tableWidget_Season->setVerticalHeaderLabels(listofTeam);

    ui->tableWidget_Season->horizontalHeader()->resizeSections(QHeaderView::Stretch);


    //handle list of Gaems
    ui->listWidget_Games->clear();


    ui->dockWidget_Teams->setEnabled(listofTeam.count() > 0);
}

void MainWindow::on_myChampionship_gameEdited(Game * newGame)
{    
    qDebug() << "on_myChampionship_gameEdited : " << newGame->printScoreWithTeamNames();

    QMutexLocker muxlocker(mux);


    // Setting cross games
    int col = 0, row = 0;

    for ( int i = 0; i < ui->tableWidget_Season->columnCount(); i++)
        if (newGame->getAwayTeam()->getFullName() == ui->tableWidget_Season->horizontalHeaderItem(i)->text())
            col = i;

    for ( int j = 0; j < ui->tableWidget_Season->rowCount(); j++)
        if (newGame->getHomeTeam()->getFullName() == ui->tableWidget_Season->verticalHeaderItem(j)->text())
            row = j;

    QTableWidgetItem *setdes = new QTableWidgetItem();
    setdes->setText(newGame->printScore());
    if (newGame->hasVideo())    setdes->setBackgroundColor(QColor(Qt::green));
    else                        setdes->setBackgroundColor(QColor(255,100,100));
    ui->tableWidget_Season->setItem(row,col,setdes);


    // Setting list of game
    ui->listWidget_Games->addItem(newGame->printScoreWithTeamNames());
}



/******************
 * Wrapper functions for flashscore.ae
 **************************/


/////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButton_LoadTeams_clicked
/// Loads the team from the flashscore.ea teams link
void MainWindow::on_pushButton_LoadTeams_clicked()
{
    myChampionship->loadTeams();
}

///////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButton_LoadGames_clicked
/// Loads 100 games from the flashscore.ea results link
void MainWindow::on_pushButton_LoadGames_clicked()
{
    myChampionship->loadGames();
}

////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButton_LoadMore_clicked
/// Loads an extra 100 games from the flashscore.ea results link
void MainWindow::on_pushButton_LoadMore_clicked()
{
    myChampionship->loadMoreGames();
}

//////////////////////////////////////////////////////////////
/// \brief MainWindow::on_pushButton_GetGameDetails_clicked
/// Loads details for the current game form the flashscore.ea match link
void MainWindow::on_pushButton_GetGameDetails_clicked()
{
    qDebug() << "on_pushButton_GetGameDetails_clicked";
    currentGameWebEngine->toHtml([this](const QString& result) mutable
    {

        on_pushButton_GetGameID_clicked();

        int init = 0, fine = 0;
        int i=0;

        // Focus on tabel only
        QString Table;
        init = result.indexOf("<table id=\"parts", fine) + QString("<table id=\"parts").length();
        fine = result.indexOf("</table>", init);
        if (fine >= 0  && init >= 0  && fine > init )
            Table = result.mid(init, fine-init);


        // qDebug() << result;

        int initHome = 0;
        int fineHome = 0;

        int initAway= 0;
        int fineAway = 0;

        ui->listWidget_eventHome->clear();
        ui->listWidget_eventAway->clear();


        currentGame->clearEvents();

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
                event.setHomeTeam();

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
                        event.setLabel(TypeEvent);
                    }
                }
                //  homeEvents.append(event);

                qDebug() << "Home Event Grabbed : " << event.prettyPrint();
                ui->listWidget_eventHome->addItem(event.prettyPrint());
                if(!event.isEmpty())
                    currentGame->addEvent(event);

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
                event.setAwayTeam();

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
                        event.setLabel(TypeEvent);
                    }
                }


                //awayEvents.append(event);

                qDebug() << "Away Event Grabbed : " << event.prettyPrint();
                ui->listWidget_eventAway->addItem(event.prettyPrint());
                if(!event.isEmpty())
                    currentGame->addEvent(event);
            }

            i++;
        }
        //emit newDataGrabbed();
    });

}





void MainWindow::on_listWidget_HomeTeams_clicked(const QModelIndex &index)
{
    qDebug() << "on_listWidget_HomeTeams_clicked : " << ui->listWidget_HomeTeams->currentRow() << ui->listWidget_AwayTeams->currentRow();
    if ( (ui->listWidget_AwayTeams->currentRow() > 0) && (index.row() > 0) )
    {
        Team * Home = new Team(ui->listWidget_HomeTeams->currentItem()->text());
        Team * Away = new Team(ui->listWidget_AwayTeams->currentItem()->text());
        currentGame = myChampionship->getGame(Home, Away);

        on_pushButton_GetGameID_clicked();
        loadFlashScoreInformation();
    }
}

void MainWindow::on_listWidget_AwayTeams_clicked(const QModelIndex &index)
{
    qDebug() << "on_listWidget_AwayTeams_clicked : " << ui->listWidget_HomeTeams->currentRow() << ui->listWidget_AwayTeams->currentRow();
    if ( (ui->listWidget_HomeTeams->currentRow() > 0) && (index.row() > 0) )
    {
        Team * Home = new Team(ui->listWidget_HomeTeams->currentItem()->text());
        Team * Away = new Team(ui->listWidget_AwayTeams->currentItem()->text());
        currentGame = myChampionship->getGame(Home, Away);

        on_pushButton_GetGameID_clicked();
        loadFlashScoreInformation();
    }
}


void MainWindow::on_tableWidget_Season_cellClicked(int row, int column)
{
    qDebug() << "on_tableWidget_Season_itemActivated";
    currentGame = myChampionship->getGame(
                ui->tableWidget_Season->horizontalHeaderItem(row)->text(),
                ui->tableWidget_Season->verticalHeaderItem(column)->text()
                );

  //  currentGame = new Game();
   // connect(currentGame, SIGNAL(newDataGrabbed()), this, SLOT(on_currentGame_newDataGrabbed()));
  //  currentGame = game;

    on_pushButton_GetGameID_clicked();
    loadFlashScoreInformation();
}


void MainWindow::on_listWidget_Games_clicked(const QModelIndex &index)
{
    qDebug() << "on_listWidget_Games_clicked";
    foreach (Game * myGame, myChampionship->getAllGames())
    {
        if (QString::compare(index.data().toString(), myGame->printScoreWithTeamNames()) == 0)
        {
            currentGame = myGame;
            break;
        }
    }

    on_pushButton_GetGameID_clicked();

    loadFlashScoreInformation();
}



void MainWindow::on_pushButton_GetGameID_clicked()
{
    qDebug() << "on_pushButton_GetGameID_clicked";


   // currentGame = new Game();
   // currentGame = game;
 //   connect(currentGame, SIGNAL(newDataGrabbed()), this, SLOT(on_currentGame_newDataGrabbed()));


    ui->label_HomeTeam->setText(currentGame->getHomeTeam()->getFullName());
    ui->label_AwayTeam->setText(currentGame->getAwayTeam()->getFullName());

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


    QString fulllink = QString("http://www.flashscore.ae/match/%1/#matchsummary").arg(currentGame->getFlashScoreID());
    ui->label_linkGame->setText(QString("<a href=\"%1\">%1</a>").arg(fulllink));
    ui->label_linkGame->setTextFormat(Qt::RichText);
    ui->label_linkGame->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_linkGame->setOpenExternalLinks(true);


    ui->listWidget_eventHome->clear();
    ui->listWidget_eventAway->clear();
    foreach (EventBase event, currentGame->getEvents())
    {
        if (event.isHomeTeam())
        {
            qDebug() << "Event added to Home list : " << event.prettyPrint();
            ui->listWidget_eventHome->addItem(event.prettyPrint());
            ui->listWidget_eventAway->addItem(EventBase().prettyPrint());
        }
        else if (event.isAwayTeam())
        {
            qDebug() << "Event added to Away list : " << event.prettyPrint();
            ui->listWidget_eventHome->addItem(EventBase().prettyPrint());
            ui->listWidget_eventAway->addItem(event.prettyPrint());
        }
    }

    ui->lineEdit_VideoPath->setText(currentGame->getLocalVideoURL().toString());

    player->setMedia(currentGame->getLocalVideoURL());


 /*   if (currentGame->isEmpty())
        myChampionship->loadMoreGames();
    else*/

}






void MainWindow::on_pushButton_FindYoutubeVideo_clicked()
{
    QString link = QString("https://www.youtube.com/results?search_query=%1+%2+%3+full+game&sp=EgIYAg%253D%253D")
            .arg(currentGame->getHomeTeam()->getFullName())
            .arg(currentGame->getAwayTeam()->getFullName())
            .arg(currentGame->getDateTime().toString("yyyy+MM+dd"));

    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_pushButton_ExportJSON_clicked()
{
    //  QString filename =  QFileDialog::getSaveFileName(this, tr("Saving path"), QDir::homePath(),"*.json");

    // if (!filename.isEmpty())
    myChampionship->saveJSON(QDir::homePath() + "/FootballActivity/" + myChampionship->printChampionship().replace("/","_") + ".json");
}


void MainWindow::on_pushButton_OpenJSON_clicked()
{
    myChampionship->loadJSON(QDir::homePath() + "/FootballActivity/" + myChampionship->printChampionship().replace("/","_") + ".json");
}


void MainWindow::on_pushButton_launchJS_clicked()
{
    myChampionship->launchJS();
}





void MainWindow::loadFlashScoreInformation()
{
    qDebug() << "loadFlashScoreInformation : ";
    //  currentGameWebEngine->load(Link);

    QEventLoop loop;
    connect(currentGameWebEngine,SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));

    ui->pushButton_GetGameDetails->setEnabled(false);

    QString Link = QString("http://www.flashscore.ae/match/%1/#match-summary").arg(currentGame->getFlashScoreID());
    currentGameWebEngine->load(Link);

    qDebug() << "loading ...";
    loop.exec();
    qDebug() << "... loaded";

    // QThread::msleep(1000);
    ui->pushButton_GetGameDetails->setEnabled(true);
}




void MainWindow::on_pushButton_BatchChampionship_clicked()
{

    foreach (Game *game, myChampionship->getAllGames())
    {

        currentGame = game;
        QString Link = QString("http://www.flashscore.ae/match/%1/#match-summary").arg(game->getFlashScoreID());
        qDebug() << "opening : " << Link;


        QEventLoop loop;
        connect(currentGameWebEngine,SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
        currentGameWebEngine->load(Link);
        qDebug() << "loading ...";
        loop.exec();
        qDebug() << "... loaded";



        QEventLoop loop2;
        QTimer::singleShot(2000, &loop2, SLOT(quit()));
        loop2.exec();
        qDebug() << "... doublechecked!";


        on_pushButton_GetGameDetails_clicked();



        qDebug() << "... wait again...";
        QEventLoop loop3;
        QTimer::singleShot(2000, &loop3, SLOT(quit()));
        loop3.exec();


        myChampionship->saveJSON(QDir::homePath() + "/FootballActivity/" + myChampionship->printChampionship().replace("/","_") + ".json");

    }

}


void MainWindow::on_pushButton_BatchAllChampionship_clicked()
{
    int A = ui->spinBox_BatchToYear->value();
    int B = ui->spinBox_BatchFromYear->value();

    int Max = A>B?A:B;
    int Min = A>B?B:A;

    for (int year = Max; year >= Min; year--)
    {
        for (int i_league = 0; i_league < ui->listWidget_BatchLeagues->count(); i_league++)
        {
            if (ui->listWidget_BatchLeagues->item(i_league)->checkState() == Qt::Checked)
            {
                ui->comboBox_league->setCurrentIndex(i_league);
                ui->spinBox_year->setValue(year);

                myChampionship->setLeague(ui->comboBox_league->itemText(i_league));
                myChampionship->setYear(year);

                QString filename = QDir::homePath() + "/FootballActivity/" + myChampionship->printChampionship().replace("/","_") + ".json";
                qDebug() << filename;
                myChampionship->loadJSON(filename);

                foreach (Game *game, myChampionship->getAllGames())
                {

                    if (!game->getFlashScoreID().isEmpty())
                    {
                        currentGame = game;
                        QString Link = QString("http://www.flashscore.ae/match/%1/#match-summary").arg(game->getFlashScoreID());
                        qDebug() << "opening : " << Link;


                        QEventLoop loop;
                        connect(currentGameWebEngine,SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
                        currentGameWebEngine->load(Link);
                        qDebug() << "loading ...";
                        loop.exec();
                        qDebug() << "... loaded";





                        QEventLoop loop2;
                        QTimer::singleShot(2000, &loop2, SLOT(quit()));
                        loop2.exec();
                        qDebug() << "... doublechecked!";


                        on_pushButton_GetGameDetails_clicked();



                        qDebug() << "... wait again...";
                        QEventLoop loop3;
                        QTimer::singleShot(2000, &loop3, SLOT(quit()));
                        loop3.exec();


                        myChampionship->saveJSON(QDir::homePath() + "/FootballActivity/" + myChampionship->printChampionship().replace("/","_") + ".json");
                    }
                }
            }
        }
    }
}

void MainWindow::on_pushButton_VideoPlay_clicked()
{
    qDebug() << "on_pushButton_VideoPlay_clicked";
    player->play();
}

void MainWindow::on_lineEdit_VideoPath_textChanged(const QString &arg1)
{
    qDebug() << "on_lineEdit_VideoPath_textChanged";
    currentGame->setLocalVideoURL(QUrl::fromUserInput(arg1));
    player->setMedia(QUrl::fromUserInput(arg1));
}

void MainWindow::on_pushButton_OpenVideoFile_clicked()
{
    qDebug() << "on_pushButton_OpenVideoFile_clicked";
    QString path = currentGame->getLocalVideoURL().toString();
    if (path.isEmpty())
       path = QString("/media/giancos/NewVolume/FootballAnalytics/");

    path = QFileDialog::getOpenFileName(this,tr("Open video file"),path);
    if (!path.isEmpty())
        ui->lineEdit_VideoPath->setText(path);
}
