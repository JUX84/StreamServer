#include "StreamServer.hpp"

#include <chrono>
#include <vlc/libvlc_vlm.h>
#include <vlc/vlc.h>
#include <Ice/Ice.h>

StreamServer::StreamServer() {
	vlc = libvlc_new(0, NULL);
}

std::string StreamServer::selectSong(const Song& s, const Ice::Current& c) {
	if(vlc == nullptr)
		return "";
	Ice::IPConnectionInfo* ipCoInfo = dynamic_cast<Ice::IPConnectionInfo*>(c.con->getInfo().get());
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	std::string token = ipCoInfo->remoteAddress.substr(7) + "." + std::to_string(ipCoInfo->remotePort) + "." + std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
	std::cout << token << std::endl;
	std::string ssout = "#transcode{acodec=mp3,ab=128,channels=2," \
						"samplerate=44100}:http{dst=:8090/"+token+".mp3}";
	const char* sout = ssout.c_str();

	libvlc_vlm_add_broadcast(vlc, token.c_str(), "test.mp3", sout, 0, nullptr, true, false);
	return token;
}

void StreamServer::playSong(const std::string& token, const Ice::Current&) {
	if(vlc == nullptr)
		return;
	libvlc_vlm_play_media(vlc, token.c_str());
}

void StreamServer::stopSong(const std::string& token, const Ice::Current&) {
}

void StreamServer::addSong(const Song& s, const Ice::Current&) {
}

void StreamServer::removeSong(const Song& s, const Ice::Current&) {
}

std::vector<Song> StreamServer::searchSong(const std::string& artist, const std::string& title, const Ice::Current&) {
	return std::vector<Song>();
}
