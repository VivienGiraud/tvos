#include "FenCodeGenere.h"
#include "ui_qxsrexample.h"

#include <QDesktopWidget>
#include <QKeyEvent>
#include <QtGui>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vlc/vlc.h>
#include <unistd.h>

EPG::EPG(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QXSRExample)
{
    ui->setupUi(this);

    /* --- Initialization --- */
    EPGCOMPARE = true;
    sound = true;
    sound_value = 50;

    /* --- Function calls --- */
    buildingWidgetVideo();
    parseXML();
    setSize();
    printDate();
}

EPG::~EPG()
{
    delete ui;
}

void EPG::buildingWidgetVideo()
{
#ifdef __x86_64__
    const char * const x86_64_vlc_args[] =
    {
        "-I", "dummy", /* Don't use any interface */
        "--ignore-config", /* Don't use VLC's config */
    };
    /* Load the VLC engine */
    inst = libvlc_new (sizeof(x86_64_vlc_args) / sizeof(x86_64_vlc_args[0]), x86_64_vlc_args);
    /* Create a new item */
    m = libvlc_media_new_path (inst, "test.mp4");
#elif __ARM_ARCH_7A__
    const char * const arm_vlc_args[] =
    {
        "--intf=dummy",
        "--no-media-library",
        "--no-one-instance",
        "--no-plugins-cache",
        "--no-stats",
        "--no-osd",
        "--no-loop",
        "--no-video-title-show",
        "--no-keyboard-events",
        "--no-mouse-events",
        "--demux=ffmpeg",
        "--codec=cedar",
        "--vout=cedarfb"
    };
    const char * const arm_dvb_vlc_args[] =
    {
        "--intf=dummy",
        "--no-media-library",
        "--no-one-instance",
        "--no-plugins-cache",
        "--no-stats",
        "--no-osd",
        "--no-loop",
        "--no-video-title-show",
        "--no-keyboard-events",
        "--no-mouse-events",
        "--demux=ffmpeg",
        "--codec=cedar",
        "--vout=cedarfb",
        "dvb-t://frequency=586000000:bandwidth=8:program=1026:adapter=0"
    };
    //inst = libvlc_new (sizeof(dvb_arm_vlc_args) / sizeof(dvb_arm_vlc_args[0]), dvb_arm_vlc_args);
    inst = libvlc_new (sizeof(arm_vlc_args) / sizeof(arm_vlc_args[0]), arm_vlc_args);
    /* Create a new item */
    m = libvlc_media_new_path (inst, "/opt/test.mp4");
#else
#error Platform not supported!
#endif

/* Create a media player playing environement */
mp = libvlc_media_player_new_from_media (m);
libvlc_audio_set_volume(mp,sound_value);
libvlc_media_player_set_xwindow(mp, ui->video->winId());

/* play the media_player */
libvlc_media_player_play (mp);

screenSize2 = screen2.screenGeometry();
int a = screenSize2.width();
int b = screenSize2.height();
ui->video->setGeometry(0,0,a,b);
ui->video->setFocus();
}

void EPG::parseXML()
{
    ui->labelPic->setPixmap(QPixmap(QApplication::applicationDirPath() + "/logo/22.jpg").scaled(ui->labelPic->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->labelCsa->setPixmap(QPixmap(QApplication::applicationDirPath() + "/CSA/18.gif").scaled(ui->labelCsa->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    comparateur = 10;
    ui->channel_list->clear();

    /* Open File */
    QFile* file = new QFile("epg.xml");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)){ exit(1); }

    QXmlStreamReader xml(file);
    QList< QMap<QString,QString> > cha;

    /* We'll parse the XML until we reach end of it.*/
    while(!xml.atEnd() &&!xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();

        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement)
        {
                cha.append(this->parse(xml));
        }
    }

    /* Error handling. */
    if(xml.hasError())
    {
        exit(12);
    }

    xml.clear();

    this->addToUI(cha);
}

QMap<QString, QString> EPG::parse(QXmlStreamReader& xml)
{
    QMap<QString, QString> chaa;
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "channel") { return chaa; }
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "programme") { return chaa; }
    QXmlStreamAttributes attributes = xml.attributes();

    /* Let's check that cha has a attribute. */
    if(attributes.hasAttribute("id")) { chaa["id"] = attributes.value("id").toString();}
    if(attributes.hasAttribute("channel")) { chaa["channel"] = attributes.value("channel").toString(); }
    if(attributes.hasAttribute("start")) { chaa["start"] = attributes.value("start").toString();}
    if(attributes.hasAttribute("stop")) { chaa["end"] = attributes.value("stop").toString();}
    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement))
    {
        if(xml.tokenType() == QXmlStreamReader::StartElement)
        {
            /* We've found display-name / title. */
            if(xml.name() == "display-name") { this->addElementDataToMap(xml, chaa); }
            if(xml.name() == "title") { this->addElementDataToMap(xml, chaa); }
            if(xml.name() == "desc") { this->addElementDataToMap(xml, chaa); }
        }
        /* ...and next... */
        xml.readNext();
    }

    return chaa;
}

void EPG::addElementDataToMap(QXmlStreamReader& xml,QMap<QString, QString>& map) const
{
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement)
    {
        return;
    }

    /* Let's read the name... */
    QString elementName = xml.name().toString();

    /* ...go to the next. */
    xml.readNext();

    if(xml.tokenType() != QXmlStreamReader::Characters)
    {
        return;
    }

    /* Now we can add it to the map.*/
    map.insert(elementName, xml.text().toString());
}

void EPG::addToUI(QList< QMap<QString,QString> >& cha)
{
    int i = 0;
    while (!cha.isEmpty())
    {
            QMap<QString,QString> chaa = cha.takeAt(0);
            if(chaa["display-name"] != 0){ ui->channel_list->addItem(chaa["display-name"]); } //TO DO : saut de ligne
            if(chaa["display-name"] != 0){ ui->textEdit_2->insertPlainText(chaa["display-name"]); } //TO DO : saut de ligne
            if(chaa["title"] != 0){ ui->textEdit->append("Nom du programme : "+chaa["title"]); }
            if(chaa["desc"] != 0){ ui->textEdit_5->append("Desc : "+chaa["desc"]); }
            if(chaa["start"] != 0){ ui->textEdit_5->insertPlainText("\nHeure de debut : "+chaa["start"]+"\n"); }
            if(chaa["end"] != 0){ ui->textEdit_5->insertPlainText("Heure de fin : "+chaa["end"]+"\n"); }
            i++;
    }
}

void EPG::setSize()
{
    int x = screenSize2.width();
    int y = screenSize2.height();

    /* --- Channel list --- */
    ui->channel_list->setGeometry(x/50,y/1.8,x/6.8,y/2.5);
    ui->channel_list->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->channel_list->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    /* --- Time programs --- */
    ui->time_programs->setGeometry(x/5,y/1.8,x/1.3,y/2.5);
    ui->time_programs->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->time_programs->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->Time_programslabel1->setGeometry(x/5,y/3.1,x/1.3,y/2.5);
    ui->Time_programslabel1_2->setGeometry(x/3.5,y/3.1,x/1.3,y/2.5);
    ui->Time_programslabel1_3->setGeometry(x/2.7,y/3.1,x/1.3,y/2.5);
    ui->Time_programslabel1_4->setGeometry(x/2.2,y/3.1,x/1.3,y/2.5);
    ui->label_dynamictime->setGeometry(x/50,y/3.5,x/1.3,y/2.5);

    /* --- Label available --- */
    ui->label_available->setGeometry(x/50,y/3.1,x/1.3,y/2.5);


    ui->textEdit_5->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_5->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_2->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_2->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
}

void EPG::printDate()
{
    /* --- Dynamic clock --- */
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
     timer->start(1000);
     showTime();

     /* --- Dynamic labels --- */
     QTime time_label = QTime::currentTime();
     QString text_label = time_label.toString("hh:00");
     ui->Time_programslabel1->setText(text_label);
     time_label = time_label.addSecs(3600);
     text_label = time_label.toString("hh:00");
     ui->Time_programslabel1_2->setText(text_label);
     time_label = time_label.addSecs(3600);
     text_label = time_label.toString("hh:00");
     ui->Time_programslabel1_3->setText(text_label);
     time_label = time_label.addSecs(3600);
     text_label = time_label.toString("hh:00");
     ui->Time_programslabel1_4->setText(text_label);
}
void EPG::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    ui->label_dynamictime->setText(text);
}

void EPG::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_E)
    {
        if(EPGCOMPARE == true)
        {
            screenSize2 = screen2.screenGeometry();
            int a = screenSize2.width();
            int b = screenSize2.height();
            ui->video->setGeometry(15,15,a*0.375,b*0.375);
            EPGCOMPARE = false;
        }
        else
        {
            screenSize2 = screen2.screenGeometry();
            int a = screenSize2.width();
            int b = screenSize2.height();
            ui->video->setGeometry(0,0,a,b);
            EPGCOMPARE = true;
        }
    }
    if(event->key() == Qt::Key_Escape)
    {
        libvlc_media_player_stop (mp); // Stop playing
        /*Free the media_player */
        libvlc_media_player_release (mp);
        libvlc_release (inst);
        this->close();
    }

    if(event->key() == Qt::Key_M)
    {
        if (sound == true)
        {
            libvlc_audio_set_volume(mp,0);
            sound = false;
        }
        else
        {
            libvlc_audio_set_volume(mp,sound_value);
            sound = true;
        }
    }
    if(event->key() == Qt::Key_L)
    {
        libvlc_video_set_logo_string(mp, 1, "test.png");
        libvlc_video_set_logo_int(mp, libvlc_logo_x, 505);
        libvlc_video_set_logo_int(mp, libvlc_logo_y, 305);
        libvlc_video_set_logo_int(mp, libvlc_logo_opacity, 255);
        libvlc_video_set_logo_int(mp, libvlc_logo_enable, 1);
    }
}
