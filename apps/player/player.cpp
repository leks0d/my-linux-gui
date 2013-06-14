
#include "player.h"


namespace mango
{
	Player::Player(void)
	{
		mMeidaView = NULL;
		mPlayingView = NULL;
	}


	Player::~Player(void)
	{
		safeDelete(mMeidaView);
	}

	int Player::main()
	{
		wakeLock();
		initialize();

		mMeidaView = new MediaView (TEXT("Media"), NULL, NULL, 0, SW_NORMAL);
		mMeidaView->onCreate();

		return messageLoop();
	}


	int Player::showPlayingView()
	{
		if (mPlayingView == NULL) {
			mPlayingView = new PlayingView(TEXT("Playing"), NULL, NULL, 0, SW_NORMAL);
			mPlayingView->onCreate();
		} else {
			gSession.mViewZAxis.bringViewToTop(mPlayingView);
		}

		if (mPlayingView) {
			mPlayingView->invalidateRect();
			mPlayingView->setFocus();
		}

		return 0;
	}


	int Player::showMediaView()
	{
		if (mMeidaView == NULL) {

		} else {
			gSession.mViewZAxis.bringViewToTop(mMeidaView);
		}

		if (mMeidaView)	{
			mMeidaView->invalidateRect();
			mMeidaView->setFocus();
		}

		return 0;
	}



	int  Player::getVolume(void)
	{
#ifndef WIN32
		FILE* file;
		char buffer[20];
		int currentVolume;
		int maxVolume;
		int minVolume;

		file  = fopen("/dev/codec_volume", "rt");
		if (file == NULL) {
			log_e("/dev/codec_volume");
			return -1;
		}

		fread(buffer, 1, 20, file);
		buffer[19] = '\0';
		log_i ("%s", buffer);
		sscanf(buffer, "%d,%d,%d\n", &currentVolume, &minVolume, &maxVolume);

		fclose(file);

		return currentVolume;
#endif
		return 0;
	}


	void Player::setVolume(int volume)
	{
#ifndef WIN32
		FILE* file;
		char buffer[20];

		file  = fopen("/dev/codec_volume", "wt");
		if (file == NULL) {
			log_e("/dev/codec_volume");
			return;
		}

		sprintf(buffer, "%d", volume);
		log_i ("%s", buffer);

		fwrite(buffer, 1, strlen(buffer) + 1, file);
		fclose(file);
#endif
	}




	Player  gPlayer;
};



int main (int argc, char* argv[])
{
#ifndef WIN32
	mango::Thread::sleep(1000 * 20);
#endif
	return mango::gPlayer.main();
}