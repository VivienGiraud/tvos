#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vlc/vlc.h>
#include <unistd.h>

#include <QMainWindow>

namespace Ui {
class player;
}

class player : public QMainWindow
{
    Q_OBJECT

public:
    explicit player(QWidget *parent = 0);
    ~player();
    void Create_Player();

protected:
    void keyPressEvent(QKeyEvent *event);
    libvlc_media_t *m;
    libvlc_instance_t * inst;
    libvlc_media_player_t *mp;

private:
    Ui::player *ui;
    bool sound;
    int sound_value;
};

#endif // PLAYER_H
