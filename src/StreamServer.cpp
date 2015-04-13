#include "StreamServer.hpp"

#include <chrono>
#include <vlc/libvlc_vlm.h>
#include <vlc/vlc.h>
#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>

StreamServer::StreamServer() {
	Ice::CommunicatorPtr ic;
	try {
		ic = Ice::initialize();
		Ice::ObjectPrx obj = ic->stringToProxy("IceStorm/TopicManager:tcp -h 80.240.129.188 -p 9999");
		IceStorm::TopicManagerPrx topicManager = IceStorm::TopicManagerPrx::checkedCast(obj);
		IceStorm::TopicPrx topic;
		while (!topic) {
			try {
				topic = topicManager->retrieve("StreamPlayerNotifs");
			} catch (const IceStorm::NoSuchTopic&) {
				try {
					topic = topicManager->create("StreamPlayerNotifs");
				} catch(const IceStorm::TopicExists&) {
				}
			}
		}
		Ice::ObjectPrx pub = topic->getPublisher()->ice_twoway();
		monitor = MonitorPrx::uncheckedCast(pub);
	} catch (const Ice::Exception& e) {
		std::cerr << e << '\n';
	}
	vlc = libvlc_new(0, NULL);
}

std::string StreamServer::selectSong(const Song& s, const Ice::Current& c) {
	std::cout << "Selecting song (" << s.artist << ", " << s.title << ")\n";
	if(vlc == nullptr)
		return "";
	Ice::IPConnectionInfo* ipCoInfo = dynamic_cast<Ice::IPConnectionInfo*>(c.con->getInfo().get());
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	std::string token = ipCoInfo->remoteAddress.substr(7) + "." + std::to_string(ipCoInfo->remotePort) + "." + std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
	std::string ssout = "#transcode{acodec=mp3,ab=128,channels=2," \
						 "samplerate=44100}:http{dst=:8090/"+token+".mp3}";
	const char* sout = ssout.c_str();
	std::string path = "songs/"+s.path;
	libvlc_vlm_add_broadcast(vlc, token.c_str(), path.c_str(), sout, 0, nullptr, true, false);
	std::cout << "Selected song (" << token << ")\n";
	return token;
}

void StreamServer::playSong(const std::string& token, const Ice::Current&) {
	std::cout << "Playing song (" << token << ")\n";
	if(vlc == nullptr)
		return;
	libvlc_vlm_play_media(vlc, token.c_str());
}

void StreamServer::stopSong(const std::string& token, const Ice::Current&) {
	std::cout << "Stopping song (" << token << ")\n";
	if(vlc == nullptr)
		return;
	libvlc_vlm_stop_media(vlc, token.c_str());
}

void StreamServer::addSong(const Song& s, const Ice::Current&) {
	std::cout << "Adding song (" << s.artist << ", " << s.title << ")\n";
	songs.push_back(s);
	monitor->report("add", s);
}

void StreamServer::removeSong(const Song& s, const Ice::Current&) {
	std::cout << "Removing song (" << s.artist << ", " << s.title << ")\n";
	for(int i = 0; i < songs.size(); ++i) {
		if(songs[i].artist == s.artist && songs[i].title == s.title)
			songs.erase(songs.begin()+(i--));
	}
	monitor->report("del", s);
}

std::vector<Song> StreamServer::searchSong(const std::string& artist, const std::string& title, const Ice::Current&) {
	std::cout << "Searching song (" << artist << ", " << title << ")\n";
	std::vector<Song> output;
	for(int i = 0; i < songs.size(); ++i) {
		if(artist.empty() && title.empty()) {
			output.push_back(songs[i]);
			std::cout << "Found song (" << songs[i].artist << ", " << songs[i].title << ")\n";
		} else if(artist.empty() || title.empty()) {
			if((title.empty() && songs[i].artist == artist)
					|| (artist.empty() && songs[i].title == title)) {
				output.push_back(songs[i]);
				std::cout << "Found song (" << songs[i].artist << ", " << songs[i].title << ")\n";
			}
		} else if(songs[i].artist == artist && songs[i].title == title) {
			output.push_back(songs[i]);
			std::cout << "Found song (" << songs[i].artist << ", " << songs[i].title << ")\n";
		}
	}
	std::cout << "Search finished\n";
	return output;
}

void StreamServer::uploadFile(const std::string& name, const ByteSeq& data, const Ice::Current& c) {
	FILE* file;
	std::string path = "../songs/" + name;
	file = fopen(path.c_str(), "a+");
	fseek(file, 0, SEEK_END);
	fwrite(&data[0], 1, data.size(), file);
	fclose(file);
}
