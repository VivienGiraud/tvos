#ifndef SUBTITLES_H
#define SUBTITLES_H
#include <vlc/vlc.h>

class subtitles
{
public:
  subtitles();
  bool SetNextSubtitle(bool previous);
  void GetSubtitlesInfo(libvlc_media_player_t *player);
  void setSubtitlesNext(libvlc_media_player_t *player);

  libvlc_track_description_t *sub_desc;
};

#endif // SUBTITLES_H
