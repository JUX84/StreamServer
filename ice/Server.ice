module Player {
	struct Song {
		string artist;
		string title;
		string path;
	};

	sequence<Song> SongSeq;

	interface Server {
		string selectSong(Song s);
		void playSong(string token);
		void stopSong(string token);

		void addSong(Song s);
		void removeSong(Song s);
		SongSeq searchSong(string artist, string title);
	};

	interface Monitor {
		void report(string notif);
	};
};
