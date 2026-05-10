#pragma once
#include <QObject>
#include <QTimer>
#include <QString>

class PomodoroTimer : public QObject
{
	Q_OBJECT
public:
	PomodoroTimer(QObject* ptr = nullptr);
	void startTimer(int mins, int secs);
	void pauseTimer();
	void resetTimer();
signals:
	void changeTime(QString time);
	void playSound();
private slots:
	void onTick();
private:
	QTimer* T;
	int minutes, seconds;
};

