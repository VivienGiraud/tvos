#include "FenCodeGenere.h"
#include "ui_qxsrexample.h"
#include <time.h>
#include <QDesktopWidget>
#include "player.h"



EPG::EPG(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QXSRExample)
{
    player *media = new player();
    media->showFullScreen();

    ui->setupUi(this);

    //parseXML();
    setSize();
    printDate();

    ui->labelPic->setPixmap(QPixmap(QApplication::applicationDirPath() + "/logo/22.jpg").scaled(ui->labelPic->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->labelCsa->setPixmap(QPixmap(QApplication::applicationDirPath() + "/CSA/18.gif").scaled(ui->labelCsa->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

EPG::~EPG()
{
    delete ui;
}

void EPG::parseXML()
{
    comparateur = 10;
    ui->listWidget->clear();

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
            if(chaa["display-name"] != 0){ ui->listWidget->addItem(chaa["display-name"]); } //TO DO : saut de ligne
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
    QDesktopWidget screen; // (Retrieve the size of the office + Geometric placement)
    QRect screenSize = screen.screenGeometry();
    int x = screenSize.width();
    int y = screenSize.height();
    ui->listWidget->setFixedHeight(y/2.5);
    ui->textEdit_4->setFixedHeight(y/2.5);
    ui->textEdit_4->setFixedWidth(x/1.27);


    ui->label_2->setFixedHeight(y/30);
    ui->label_4->setFixedHeight(y/30);
    ui->label_5->setFixedHeight(y/30);
    ui->label_6->setFixedHeight(y/30);

    ui->listWidget->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->listWidget->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_4->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_4->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_5->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_5->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_2->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->textEdit_2->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
}

void EPG::printDate()
{
    time_t structTime;
    struct tm *epg_clock;

     if (time (&structTime) == -1)
     {
        exit(100);
     }

     epg_clock = localtime(&structTime);
     ui->label_3->setText(QString::number(epg_clock->tm_hour)+":"+QString::number(epg_clock->tm_min)+":"+QString::number(epg_clock->tm_sec));

     int currentTime = epg_clock->tm_hour;
     ui->label_2->setText(QString::number(currentTime)+":00");

     currentTime = epg_clock->tm_hour+1;
     ui->label_4->setText(QString::number(currentTime)+":00");

     currentTime = epg_clock->tm_hour+2;
     ui->label_5->setText(QString::number(currentTime)+":00");

     currentTime = epg_clock->tm_hour+3;
     ui->label_6->setText(QString::number(currentTime)+":00");
}
