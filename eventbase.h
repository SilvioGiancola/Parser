#ifndef EVENTBASE_H
#define EVENTBASE_H

#include <QTime>
#include <team.h>



class EventBase
{
public:
    EventBase();
    EventBase(QTime t);
    EventBase(QTime t, QString desc);

    bool operator==(const EventBase &rhs)const;

    // Time handling
    void setTime(QTime value);
    void setTime(double value);
    QTime getTime();

    QTime getStartTime();
    QTime getEndTime();
    double getStartTimeInSeconds();
    double getEndTimeInSeconds();

    bool setStartTime(QTime value);
    bool setEndTime(QTime value);
    bool setStartTime(double value);
    bool setEndTime(double value);

    // Description handling
    void setDescription(QString value);
    QString getDescription();

    // Label handling
    void setLabel(QString value);
    QString getLabel();


    // Pretty PRint
    QString prettyPrint();


    bool isHomeTeam();
    bool isAwayTeam();
    void setHomeTeam();
    void setAwayTeam();

    bool isEmpty();
protected:
    QTime StartTime;
    QTime EndTime;
    QString description;
    QString label;

    int team; //0-Home / 1-Away

  //  float startTime;
  //  float endTime;

};

#endif // EVENTBASE_H
