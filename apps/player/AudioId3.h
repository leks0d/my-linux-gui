namespace mango
{


class AudioId3
{

public:
	AudioId3(char* file);
	~AudioId3(void);

public:
	int setFile(FILE *stream);
	int GetTags(int id3, char *data);
	int PicValid();
	int piclength;
 	char *picdata;
private:
	InterfaceID3 *mInterfaceID3;
	ID3INFO *mFFmepgId3;
};
};