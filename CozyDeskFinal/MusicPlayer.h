#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>

class MusicPlayer : public QObject
{
	Q_OBJECT
public:
	explicit MusicPlayer(QObject* ptr = nullptr);
	void loadfiles(QStringList file);
	void play();
	void pause();
	void nextSong();
	void prevSong();
	void setVolume(int v);
	void setMusicSlider(int v);
signals:
	void positionChange(qint64 p);
	void durationChange(qint64 d);
	void songChange(QString name);
private:
	QMediaPlayer* mp;
	QAudioOutput* ao;
	QStringList playlist;
	int songIndex;
};

