
#include "player.h"

#ifndef WIN32
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "MediaPlayerInterface.h"
#endif

namespace mango
{
#ifndef WIN32
	particle::MediaPlayerInterface*  gParticleplayer = NULL; // = particle::createMediaPlayer();
#endif

	PlaylistItem::PlaylistItem(void)
	{

	}


	PlaylistItem::PlaylistItem(TCHAR* path)
	{
		String::copy(mPath, path);
	}

	
	PlaylistItem::~PlaylistItem(void)
	{

	}


	Playlist::Playlist(void)
	{
		INIT_LIST_HEAD(&mSongHead);
#ifndef WIN32
		if (gParticleplayer == NULL)
			gParticleplayer = particle::createMediaPlayer();
#endif
	}


	Playlist::~Playlist(void)
	{

	}


	int Playlist::setSource(TCHAR* path)
	{
		char  utf8Path[MAX_PATH * 3];

		Charset::wideCharToMultiByte(CP_UTF8, path, String::lstrlen(path), utf8Path, MAX_PATH * 3);
		log_i("setSource utf8Path=%s\n",utf8Path);
#ifndef WIN32
		if (gParticleplayer->setSource(utf8Path))
			return 1;
		else
			return -1;
#else
		return 1;
#endif
	}

	int Playlist::dsetSource(char* path)
		{
			if (gParticleplayer->setSource(path))
				return 1;
			else
				return -1;
		}


	int Playlist::prepare()
	{
#ifndef WIN32
		if (gParticleplayer->prepare())
			return 1;
		else
			return -1;
#else
		return 1;
#endif
	}


	int Playlist::start()
	{
#ifndef WIN32
		if (gParticleplayer->start())
			return 1;
		else
			return -1;
#else
		return 1;
#endif
	}



	int Playlist::pause()
	{
#ifndef WIN32
		if (gParticleplayer->pause())
			return 1;
		else
			return -1;
#else
		return 1;
#endif
	}


	
	int Playlist::stop()
	{
#ifndef WIN32
		if (gParticleplayer->stop())
			return 1;
		else
			return -1;
#else
		return 1;
#endif
	}


	bool Playlist::isPlaying()
	{
#ifndef WIN32
		if (gParticleplayer->isPlaying())
			return true;
		else
			return false;
#else
		return true;
#endif
	}



	int Playlist::play(TCHAR* path)
	{
		PlaylistItem* item;
		
		item = addSong(path);
		play(item);
		mCurrentSong = item;
		return 0;
	}



	int Playlist::play(PlaylistItem* item)
	{
		if(stop() != 1){log_i("stop() fail!");return -1;}else{log_i("stop() success!");}
		if(setSource(item->mPath) != 1){log_i("setSource() fail!");return -1;}else{log_i("setSource() success!");}
		if(prepare() != 1){log_i("prepare() fail!");return -1;}else{log_i("prepare() success!");}
		if(start() != 1){log_i("start() fail!");return -1;}else{log_i("start() success!");}
		return 0;
	}
	int Playlist::dplay(char *path)
	{
		if(stop() != 1){log_i("stop() fail!");return -1;}else{log_i("stop() success!");}
		if(dsetSource(path) != 1){log_i("setSource() fail!");return -1;}else{log_i("setSource() success!");}
		if(prepare() != 1){log_i("prepare() fail!");return -1;}else{log_i("prepare() success!");}
		if(start() != 1){log_i("start() fail!");return -1;}else{log_i("start() success!");}
		return 0;
	}



	int Playlist::next()
	{
		return -1;
	}



	int Playlist::prev()
	{
		return -1;
	}


	int Playlist::clearAllSong()
	{
		PlaylistItem* item;
		stop();

		while(item = Playlist::getFirstSong()) {
			list_del(&item->mList);
			delete item;
		}

		return 0;
	}


	PlaylistItem* Playlist::addSong(TCHAR* path)
	{
		PlaylistItem* item = new PlaylistItem(path);

		list_add_tail(&item->mList, &mSongHead);  
		return item;
	}


	PlaylistItem* Playlist::getCurrentSong(void)
	{
		return mCurrentSong;
	}


	PlaylistItem* Playlist::getFirstSong(void)
	{
		if (list_empty(&mSongHead))
			return NULL;

		return list_entry(mSongHead.next, PlaylistItem, mList); 
	}


	PlaylistItem* Playlist::getNextSong(PlaylistItem* item)
	{
		if (item->mList.next == &mSongHead)
			return NULL;
		else
			return list_entry(item->mList.next, PlaylistItem, mList); 
	}

	
	Playlist* gPlaylist = NULL;

};


