#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QObject* p) : QObject(p) {
	mp = new QMediaPlayer(this);
	ao = new QAudioOutput(this);
	mp->setAudioOutput(ao);
	songIndex = 0;

	connect(mp, &QMediaPlayer::positionChanged, this, &MusicPlayer::positionChange);
	connect(mp, &QMediaPlayer::durationChanged, this, &MusicPlayer::durationChange);
	connect(mp, &QMediaPlayer::sourceChanged, this, [this](const QUrl& url) {
		emit songChange(url.fileName());
		});
}

void MusicPlayer::setMusicSlider(int v) {
	mp->setPosition(v * (mp->duration()/1000));
}

void MusicPlayer::setVolume(int v) {
	float value;
	value = v / 100.0;
	ao->setVolume(value);
}

void MusicPlayer::playSong() {
	mp->play();
}

void MusicPlayer::pauseSong() {
	mp->pause();
}

void MusicPlayer::nextSong() {
	songIndex++;
	if (songIndex >= playlist.size()) {
		songIndex = 0;
	}
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

void MusicPlayer::loadfiles(QStringList f) {
	if (!f.isEmpty()) {
		playlist = f;
		songIndex = 0;
		mp->setSource(QUrl::fromLocalFile(playlist[songIndex]));
	}
}