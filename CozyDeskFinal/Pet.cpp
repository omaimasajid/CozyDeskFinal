#pragma once

#include "Pet.h"
#include <QPixmap>
#include <QLabel>
#include <QDebug>

petwidget::petwidget(QWidget* parent): QLabel(parent){
	idlePix = QPixmap("petsprites/idle.png");
	hoverPix = QPixmap("petsprites/hover.png");
	listeningPix = QPixmap("petsprites/music.png");
	currentState = Idle;
	setPixmap(idlePix);
	setScaledContents(true);
    }

void petwidget::enterEvent(QEnterEvent* event){
	if (currentState != Listening) {
		setPixmap(hoverPix);
		currentState = Hover;
	}
}

void petwidget::leaveEvent(QEvent* event) {
	if(currentState != Listening) {
		setPixmap(idlePix);
		currentState = Idle;
	}
}

void petwidget::isPetListening(bool listening) {
	if (listening == true) {
		setPixmap(listeningPix);
		currentState = Listening;

	}
	if(listening == false) {
		setPixmap(idlePix);
		currentState = Idle;
	}
}



