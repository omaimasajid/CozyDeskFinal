#pragma once

#include "Pet.h"
#include <QPixmap>
#include <QLabel>
#include <QDebug>

petwidget::petwidget(QWidget* parent): QLabel(parent){
	idlePix = QPixmap("petsprites/idle.png");
	hoverPix = QPixmap("petsprites/hover.png");
	listeningPix = QPixmap("petsprites/listening.png");
	currentState = Idle;
	setPixmap(idlePix);
	setScaledContents(true);
    }

void petwidget::enterEvent(QEnterEvent* event){
	setPixmap(hoverPix);
	currentState= Hover;
}

void petwidget::leaveEvent(QEvent* event) {
	setPixmap(idlePix);
	currentState = Idle;
}

void petwidget::isPetListening(bool listening) {

}

