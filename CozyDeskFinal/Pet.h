#pragma once
#include <QPixmap>
#include <QLabel>
#include <QEvent>
#include <qdebug.h>


class petwidget : public QLabel {
	Q_OBJECT

public:
	enum PetState { Idle, Hover, Listening };

	explicit petwidget(QWidget* parent = nullptr);

public slots:
	void isPetListening(bool listening);


protected:
	void enterEvent(QEnterEvent* event) override;
	void leaveEvent(QEvent* event) override;

private:
	PetState currentState;
	QPixmap idlePix;
	QPixmap hoverPix;
	QPixmap listeningPix;
};