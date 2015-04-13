module Player {
	struct Song {
		string artist;
		string title;
		string path;
	};

	sequence<Song> SongSeq;
	sequence<byte> ByteSeq;

	interface Server {
		string selectSong(Song s);
		void playSong(string token);
		void stopSong(string token);

		void addSong(Song s);
		void removeSong(Song s);
		SongSeq searchSong(string artist, string title);

		void uploadFile(string path, ByteSeq data);
	};

	interface Monitor {
		void report(string action, Song s);
	};
};
