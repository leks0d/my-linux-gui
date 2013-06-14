
#include "mango.h"
#include "resource.h"
#include "mediaView.h"
#include "playingView.h"
#include "playlist.h"

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
	};

	extern Player  gPlayer;
};
