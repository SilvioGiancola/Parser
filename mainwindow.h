#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebEnginePage>
//#include <QXmlSimpleReader>
#include <QString>
#include <QThread>
#include <QDesktopServices>
#include <QTimer>

#include <QMutexLocker>

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
void loadFlashScoreInformation();
private:

private slots:


    void on_myChampionship_newTeamAdded();
    void on_myChampionship_gameEdited(Game *newGame);
   // void on_currentGame_newDataGrabbed();

    //void on_currentGameWebEngine_loadFinished(bool);

    void on_pushButton_LoadMore_clicked();


    void on_pushButton_GetGameID_clicked(Game *game);




    void on_pushButton_GetGameDetails_clicked();


    void on_comboBox_league_currentTextChanged(const QString &arg1);
    void on_spinBox_year_valueChanged(int arg1);


    void on_pushButton_FindYoutubeVideo_clicked();

    void on_pushButton_ExportJSON_clicked();

    void on_pushButton_LoadTeams_clicked();

    void on_pushButton_LoadGames_clicked();

    void on_pushButton_launchJS_clicked();

  //  void on_tableWidget_Season_itemActivated(QTableWidgetItem *item);

    void on_tableWidget_Season_cellClicked(int row, int column);

    void on_pushButton_BatchChampionship_clicked();

    void on_listWidget_HomeTeams_clicked(const QModelIndex &index);

    void on_listWidget_AwayTeams_clicked(const QModelIndex &index);

    void on_pushButton_OpenJSON_clicked();


    void on_pushButton_BatchAllChampionship_clicked();

    void on_listWidget_Games_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QMutex *mux;

    Championship *myChampionship;

    Game *currentGame;


    QWebEnginePage *currentGameWebEngine;

    void updateChampionshipLinks(QString league, int year);


};

#endif // MAINWINDOW_H
