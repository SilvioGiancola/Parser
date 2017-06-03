#include "eventbase.h"

EventBase::EventBase()
{
    StartTime = QTime(0,0);
    EndTime = QTime(0,0);
    description = QString("");
    label = QString("");
}

EventBase::EventBase(QTime t)
{
    this->setTime(t);
    description = QString("");
    label = QString("");
}

EventBase::EventBase(QTime t, QString desc)
{
    this->setTime(t);
    description = desc;
    label = desc;
}




bool EventBase::operator==(const EventBase &rhs)const
{
    return ( ( (this->StartTime) == (rhs.StartTime)) &&
             ( (this->EndTime)   == (rhs.EndTime))   &&
             ( QString::compare(this->label, rhs.label) == 0) &&
             ( QString::compare(this->description, rhs.description) == 0 ));
}

bool EventBase::isEmpty()
{
    return ((*this) == EventBase());
}



QString EventBase::prettyPrint()
{
    if (EndTime.hour() <= 0)
        return QString(" - ");
    else
        return QString("[%1 - %2] : %3")
                .arg(StartTime.toString("h - mm:ss"))
                .arg(EndTime.toString("mm:ss"))
                .arg(description);
}




void EventBase::setTime(QTime value)
{
    setEndTime( value  );
    setStartTime(getEndTime().addSecs(-60));
    return;
}

void EventBase::setTime(double value)
{
  //  EndTime = value;
    setEndTime( value  );
    setStartTime(getEndTimeInSeconds() - 60);
    return;
}
/*
QTime EventBase::getTime()
{
    return time;
}*/

QTime EventBase::getStartTime()
{
    return StartTime;
}

double EventBase::getStartTimeInSeconds()
{
    return QTime(0,0,0).secsTo(StartTime);
}

QTime EventBase::getEndTime()
{
    return EndTime;
}

double EventBase::getEndTimeInSeconds()
{
    return QTime(0,0,0).secsTo(EndTime);
}



bool EventBase::setStartTime(QTime value)
{
    StartTime = value;
    return false;
}

bool EventBase::setStartTime(double value)
{
    int value_int = value*1000;
    int msec = value_int%1000; value_int = (value_int - msec)/1000;
    int sec = value_int%60;  value_int = (value_int - sec)/60;
    int min = value_int%60;  value_int = (value_int - min)/60;
    int hour = value_int%60; value_int = (value_int - hour)/60;
    if (value_int > 0) return true;
    StartTime.setHMS(hour,min,sec,msec);
    return false;
}

bool EventBase::setEndTime(QTime value)
{
    EndTime = value;
    return false;
}

bool EventBase::setEndTime(double value)
{
    int value_int = value*1000;
    int msec = value_int%1000; value_int = (value_int - msec)/1000;
    int sec = value_int%60;  value_int = (value_int - sec)/60;
    int min = value_int%60;  value_int = (value_int - min)/60;
    int hour = value_int%60; value_int = (value_int - hour)/60;
    if (value_int > 0) return true;
    EndTime.setHMS(hour,min,sec);
    return false;
}





void EventBase::setDescription(QString value)
{
    description = value;
    return;
}

QString EventBase::getDescription()
{
    return description;
}





// Label handling
void EventBase::setLabel(QString value)
{
    label = value;
    return;
}

QString EventBase::getLabel()
{
    return label;
}



bool EventBase::isHomeTeam()
{
    return (team == 0);
}

bool EventBase::isAwayTeam()
{
    return (team == 1);
}

void EventBase::setHomeTeam()
{
    team = 0;
}

void EventBase::setAwayTeam()
{
    team = 1;
}
