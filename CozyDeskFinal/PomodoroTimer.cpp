#include "PomodoroTimer.h"

PomodoroTimer::PomodoroTimer(QObject* p) : QObject(p) {

	T = new QTimer(this);
	minutes = 0;
	seconds = 0;

	connect(T, &QTimer::timeout, this, &PomodoroTimer::tickTock);
}

void PomodoroTimer::tickTock() {
	seconds--;
	if (seconds < 0) {
		seconds = 59;
		minutes--;
	}
	if (minutes < 0) {
		T->stop();
		emit playSound();
		emit changeTime("00:00");
		return;
	}

	// formatting time
	QString time = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
	emit changeTime(time);
}

void PomodoroTimer::startTimer(int m, int s) {
	minutes = m;
	seconds = s;
	T->start(1000);
}

void PomodoroTimer::pauseTimer() {
	T->stop();
}

void PomodoroTimer::resetTimer() {
	T->stop();
	minutes = 0;
	seconds = 0;
	emit changeTime("00:00");
}

