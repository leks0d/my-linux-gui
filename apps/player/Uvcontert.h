
namespace mango
{

	class Uvcontert{
	public:
		Uvcontert();
		~Uvcontert();
		void addItem(const char* str);
		void doContert();
		int getItem(int pos,CString& str);
		uint32_t getpossibleEncodings(const char *entry);
	private:
		CStringArray entry;
		CStringArray result;
		uint32_t encode;
	};

};
