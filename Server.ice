module Player {
	struct Song {
		string artist;
		string title;
		string path;
	};

	struct Token {
		double key;
	};

	sequence<Song> SongSeq;
	sequence<Token> TokenSeq;

	interface Server {
		string selectSong(Song s);
		void playSong(Token tok);
		void stopSong(Token tok);

		void addSong(Song s);
		void removeSong(Song s);
		SongSeq searchSong(string artist, string title);
	};
};
