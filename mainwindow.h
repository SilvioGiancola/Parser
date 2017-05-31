#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
//#include <QXmlSimpleReader>
#include <QString>
#include <QThread>
#include <QDesktopServices>

#include <eventgoal.h>
#include <championship.h>
#include <game.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

private slots:


    void on_myChampionship_newTeamAdded();
    void on_myChampionship_newGameAdded(Game *newGame);
    void on_currentGame_newDataGrabbed();

    void on_pushButton_LoadMore_clicked();
 //   void on_results_game_loadFinished(bool);
 //   void on_results_championship_loadFinished(bool);

  //  void on_teams_loadFinished(bool);
 //   void on_teams_loadStarted();


   // void on_pushButton_GetTeams_clicked();

    void on_pushButton_GetGameID_clicked();



 //   void on_pushButton_LoadChampionship_clicked();

    void on_pushButton_GetGameDetails_clicked();

  //  void on_pushButton_LoadGame_clicked();

    void on_comboBox_league_currentIndexChanged(const QString &arg1);

    void on_comboBox_season_currentIndexChanged(const QString &arg1);


    void on_pushButton_FindYoutubeVideo_clicked();

    void on_pushButton_ExportJSON_clicked();

    void on_pushButton_LoadTeams_clicked();

    void on_pushButton_LoadGames_clicked();

    void on_pushButton_launchJS_clicked();

private:
    Ui::MainWindow *ui;


    Championship *myChampionship;

    Game *currentGame;

    void updateChampionshipLinks();
  /*  QWebEnginePage *results_game;
    QWebEnginePage *results_championship;
    QWebEnginePage *teams;
*/

  //  QString myGameID;


};

#endif // MAINWINDOW_H
