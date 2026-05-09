#include "PomodoroTimer.h"

PomodoroTimer::PomodoroTimer(QObject* p) : QObject(p) {

	T = new QTimer(this);
	setWorkMins = 25;
	setBreakMins = 5;

	minutes = setWorkMins;
	seconds = 0;

	sessionCount = 0;
	session = true;

	connect(T, &QTimer::timeout, this, &PomodoroTimer::onTick);

}

void PomodoroTimer::onTick() {
	seconds--;
	if (seconds < 0) {
		seconds = 59;
		minutes--;
	}
	if (minutes < 0) {
		session = !session;
		if (session == true) {
			minutes = setWorkMins;
			sessionCount++;
			emit changeSession("Working!");
		}
		else {
			minutes = setBreakMins;
			emit changeSession("Break Time!");
		}
		seconds = 0;
	}

	QString time = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
		emit changeTime(time);
}

void PomodoroTimer::startTimer() {
	T->start(1000);
}

void PomodoroTimer::pauseTimer() {
	T->stop();
}

void PomodoroTimer::resetTimer() {
	T->stop();
	minutes = setWorkMins;
	seconds = 0;
	session = true;
	emit changeSession("Working!");
	QString time = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
	emit changeTime(time);
}

void PomodoroTimer::setDuration(int wM, int bM) {
	setWorkMins = wM;
	setBreakMins = bM;
	minutes = setBreakMins;
	seconds = 0;
}
