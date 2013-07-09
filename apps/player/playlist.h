
namespace mango
{
	class PlaylistItem : public Object
	{
	public:
		PlaylistItem(void);
		PlaylistItem(TCHAR* path);
		virtual ~PlaylistItem(void);

	public:
		LIST_HEAD  mList;

		TCHAR   mPath[MAX_PATH];
	}; 


	class Playlist : public Object
	{
	public:
		Playlist(void);
		virtual ~Playlist(void);

	public:
		
		int setSource(TCHAR* path);
		int dsetSource(char* path);
		int prepare();
		int start();
		int pause();
		int stop();
		bool isPlaying();


		int play(TCHAR* path);
		int dplay(char* path);

		int next();
		int prev();

		int clearAllSong();
		PlaylistItem* addSong(TCHAR* path);
		PlaylistItem* getCurrentSong(void);

		PlaylistItem* getFirstSong(void);
		PlaylistItem* getNextSong(PlaylistItem* item);

		PlaylistItem* mCurrentSong;

	private:
		int play(PlaylistItem* item);

	public:
		LIST_HEAD   mSongHead;
	};

	extern Playlist* gPlaylist;
};
