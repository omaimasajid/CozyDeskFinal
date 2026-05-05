#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(QObject* p) : QObject(p) {
	mp = new QMediaPlayer(this);
	ao = new QAudioOutput(this);

	mp->setAudioOutput(ao);

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

void MusicPlayer::loadfiles(QStringList f) {
	if (!f.isEmpty()) {
		playlist = f;
		songIndex = 0;
		mp->setSource(QUrl::fromLocalFile(playlist[songIndex]));
	}
}