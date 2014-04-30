#include "player.h"
#include "ui_player.h"
#include "FenCodeGenere.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vlc/vlc.h>
#include <unistd.h>

#include <QKeyEvent>
#include <QPushButton>
#include <QFileDialog>

player::player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::player)
{
    ui->setupUi(this);

    /* Init */
    sound = true;
    sound_value = 50;
    Create_Player();
}

player::~player()
{
    delete ui;
}

void player::Create_Player()
{
#ifdef __x86_64__
    /* Load the VLC engine */
    inst = libvlc_new (0, NULL);
    /* Create a new item */
    m = libvlc_media_new_path (inst, "/home/miasma/Documents/test.mp4");
#elif __ARM_ARCH_7__
    const char * const vlc_args[] = {
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
      };;
    inst = libvlc_new (sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    /* Create a new item */
    m = libvlc_media_new_path (inst, "/opt/test.mp4");
#else
#error Platform not supported!
#endif
    /* Create a media player playing environement */
    mp = libvlc_media_player_new_from_media (m);
    libvlc_audio_set_volume(mp,sound_value);

    libvlc_media_player_set_xwindow(mp, this->winId());

    /* play the media_player */
    libvlc_media_player_play (mp);
}

void player::keyPressEvent(QKeyEvent *event)
{
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

    if(event->key() == Qt::Key_E)
    {
        EPG *fenetreCode = new EPG();
        fenetreCode->show();
    }
}
