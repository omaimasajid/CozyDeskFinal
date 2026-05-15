#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QObject* p) : QObject(p) {
	mp = new QMediaPlayer(this);
	ao = new QAudioOutput(this);
	mp->setAudioOutput(ao);
	songIndex = 0;
	check = false;
	isRepeat = false;

	// qt internally does smt > class reacts > passed to ui

	connect(mp, &QMediaPlayer::positionChanged, this, &MusicPlayer::positionChange); // song playing
	connect(mp, &QMediaPlayer::durationChanged, this, &MusicPlayer::durationChange); // new song uploads with total duration 
	connect(mp, &QMediaPlayer::sourceChanged, this, [this](const QUrl& url) {  // new song file 
		emit songChange(url.fileName());  //emit manually fires signaks
		});
	 
	
							//when song status is changed					//tells what changed
	connect(mp, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
							//when song ends		and repeat is enabled
		if (status == QMediaPlayer::EndOfMedia && isRepeat) {
			mp->setPosition(0);
			mp->play();
		}
		});
}

// duration() givs song length in millisec

void MusicPlayer::setMusicSlider(int v) {
	mp->setPosition(v * (mp->duration()/1000));
}

void MusicPlayer::setVolume(int v) {
	float value;
	value = v / 100.0;    // between 0.0 - 0.1
	ao->setVolume(value);
}

void MusicPlayer::playSong() {
	mp->play();

	//pet
	if (check == true) {
		emit isplaying(true);
	}
}

void MusicPlayer::pauseSong() {
	mp->pause();
	//pet
	emit isplaying(false);
}

void MusicPlayer::nextSong() {
	songIndex++;
	if (songIndex >= playlist.size()) {
		songIndex = 0;
	}

	//		file string conversion to qurl obj, get file path at index..
	mp->setSource(QUrl::fromLocalFile(playlist[songIndex]));
	mp->play();
}

void MusicPlayer::prevSong() {
	songIndex--;
	if (songIndex < 0) {
		songIndex = playlist.size()-1;
	}
	mp->setSource(QUrl::fromLocalFile(playlist[songIndex]));
	mp->play();
}

void MusicPlayer::toggleRepeat() {
	isRepeat = !isRepeat;
}

void MusicPlayer::loadfiles(QStringList f) {
	if (!f.isEmpty()) {
		playlist = f;
		songIndex = 0;
		mp->setSource(QUrl::fromLocalFile(playlist[songIndex]));
		//pet
		check = true;
	}
}

void MusicPlayer::playSongAt(int i) {
	songIndex = i;
	mp->setSource(QUrl::fromLocalFile(playlist[songIndex]));
	mp->play();
}

int MusicPlayer::getSongIndex() {
	return songIndex;
}

qint64 MusicPlayer::getDuration()
{
	return mp->duration();
}
