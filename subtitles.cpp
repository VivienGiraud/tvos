#include "subtitles.h"
#include "FenCodeGenere.h"
#include <vlc/vlc.h>
#include <QDebug>

subtitles::subtitles()
{
}

void subtitles::GetSubtitlesInfo(libvlc_media_player_t *player)
{
  int nbSub;
  /* Get numbers of subtitle available */
  nbSub = libvlc_video_get_spu_count(player);

  /* get subtitles list and description */
  sub_desc = libvlc_video_get_spu_description(player);

  /* Parse list of available subtitles and print their id */
  for (int i = 0; i < nbSub; i++)
  {
      qDebug() << "Available subs" << sub_desc->i_id;
      sub_desc = sub_desc->p_next;
  }
}

void subtitles::setSubtitlesNext(libvlc_media_player_t *player)
{
  int currentSub;

  /* Get id of current subtitle, -1 if no subtitle */
  currentSub = libvlc_video_get_spu(player);

  /* get subtitles list and description */
  sub_desc = libvlc_video_get_spu_description(player);

  /* If subtitle already on screen then disable it */
  if (currentSub != -1)
  {
    libvlc_video_set_spu(player, -1);
  }
  else /* If no subtitle then choose the first on ne list */
  {
      libvlc_video_set_spu(player, sub_desc->p_next->i_id);
  }
}

