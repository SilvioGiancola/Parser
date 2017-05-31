#ifndef EVENTBASE_H
#define EVENTBASE_H

#include <QTime>

class EventBase
{
public:
    EventBase();


    EventBase(QTime t) : _time(t){};


    // Time handling
    void setTime(QTime value) {_time = value; return;}
    QTime getTime() { return _time;}

    // Description handling
    void setDescription(QString value) {_description = value; return;}
    QString getDescription() { return _description;}


    // Pretty PRint
    QString prettyPrint()
    {
        if (_time.hour() == 0)
            return QString(" - ");
        else
            return QString("%1 : %2").arg(_time.toString("h - mm:ss")).arg(_description);
    }
protected:
    QTime _time;
    QString _description;

};

#endif // EVENTBASE_H
