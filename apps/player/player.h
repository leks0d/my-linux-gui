
#include "mango.h"
#include "resource.h"
#include "mediaView.h"
#include "playlist.h"
#include "mediaprovider.h"
#include "playinglist.h"
#include "ArrayMediaInfo.h"
#include "playingView.h"

namespace mango
{	
	class Player: public Party
	{
	public:
		Player(void);
		virtual ~Player(void);

	public:
		int main();
		int showPlayingView();
		int showMediaView();

		int  getVolume(void);
		void setVolume(int volume);


	public:
		MediaView*  mMeidaView;
		PlayingView* mPlayingView;
		sqlite3 *db; 
	};

	extern Player  gPlayer;
};
