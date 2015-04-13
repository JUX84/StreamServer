#include "Server.h"

#include <list>
#include <vector>
#include <vlc/libvlc.h>

using namespace Player;

class StreamServer : public Server {
	private:
		std::vector<Song> songs;
		libvlc_instance_t *vlc;
		MonitorPrx monitor;
	public:
		StreamServer();
		std::string selectSong(const Song& s, const Ice::Current&);
		void playSong(const std::string& token, const Ice::Current&);
		void stopSong(const std::string& token, const Ice::Current&);
		void addSong(const Song& s, const Ice::Current&);
		void removeSong(const Song& s, const Ice::Current&);
		SongSeq searchSong(const std::string& artist, const std::string& title, const Ice::Current&);
		void uploadFile(const std::string& name, const ByteSeq& data, const Ice::Current&);
};
