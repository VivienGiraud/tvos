#ifndef QXSREXAMPLE_H
#define QXSREXAMPLE_H

#include <QMainWindow>

#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vlc/vlc.h>
#include <unistd.h>

#include <QDesktopWidget>
#include <QXmlStreamReader>

namespace Ui {
class QXSRExample;
}

class EPG : public QMainWindow
{
    Q_OBJECT

public:
    explicit EPG(QWidget *parent = 0);
    ~EPG();

protected:
        void keyPressEvent(QKeyEvent *event);
private:
    Ui::QXSRExample *ui;

    void addchaToUI(QList< QMap<QString,QString> >& cha);
    QMap<QString, QString> parse(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,QMap<QString, QString>& map) const;
    void addToUI(QList< QMap<QString,QString> >& cha);
    void setSize();
    void printDate();
    void updateLabel();
    void buildingWidgetVideo();

    int comparateur;
    libvlc_media_t *m;
    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;

    QDesktopWidget screen2;
    QRect screenSize2;

    bool EPGCOMPARE;
    bool sound;
    int sound_value;
private slots:
    void parseXML();
    void showTime();

};

#endif // QXSREXAMPLE_H
