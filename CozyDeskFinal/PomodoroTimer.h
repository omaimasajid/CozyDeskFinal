#pragma once
#include <QObject>
#include <QTimer>
#include <QString>

class PomodoroTimer : public QObject
{
	Q_OBJECT
public:
	PomodoroTimer(QObject* ptr = nullptr);
	void startTimer();
	void pauseTimer();
	void resetTimer();
	void setDuration(int workMins, int breakMins);
signals:
	void changeTime(QString time);
	void changeSession(QString session);
private slots:
	void onTick();
private:
	QTimer* T;
	int minutes, seconds, sessionCount, setWorkMins, setBreakMins;
	bool session;

};

