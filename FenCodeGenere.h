#ifndef QXSREXAMPLE_H
#define QXSREXAMPLE_H

#include <QMainWindow>

#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

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

private:
    Ui::QXSRExample *ui;

    void addchaToUI(QList< QMap<QString,QString> >& cha);
    QMap<QString, QString> parse(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,QMap<QString, QString>& map) const;
    void addToUI(QList< QMap<QString,QString> >& cha);
    void setSize();
    void printDate();
    void updateLabel();

    int comparateur;

private slots:
    void parseXML();

};

#endif // QXSREXAMPLE_H
