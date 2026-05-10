#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QObject* p) : QObject(p) {
	mp = new QMediaPlayer(this);
	ao = new QAudioOutput(this);
	mp->setAudioOutput(ao);
	songIndex = 0;
	check = false;
	isRepeat = false;


	connect(mp, &QMediaPlayer::positionChanged, this, &MusicPlayer::positionChange);
	connect(mp, &QMediaPlayer::durationChanged, this, &MusicPlayer::durationChange);
	connect(mp, &QMediaPlayer::sourceChanged, this, [this](const QUrl& url) {
		emit songChange(url.fileName());
		});

	connect(mp, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
		if (status == QMediaPlayer::EndOfMedia && isRepeat) {
			mp->setPosition(0);
			mp->play();
		}
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
	if (check == true) {
		emit isplaying(true);
	}
}

void MusicPlayer::pauseSong() {
	mp->pause();
	emit isplaying(false);
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

void MusicPlayer::toggleRepeat() {
	isRepeat = !isRepeat;
}

void MusicPlayer::loadfiles(QStringList f) {
	if (!f.isEmpty()) {
		playlist = f;
		songIndex = 0;
		mp->setSource(QUrl::fromLocalFile(playlist[songIndex]));
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
