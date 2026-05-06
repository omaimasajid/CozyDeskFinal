#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CozyDeskFinal.h"
#include "MusicPlayer.h"

class CozyDeskFinal : public QMainWindow
{
    Q_OBJECT

public:
    CozyDeskFinal(QWidget *parent = nullptr);
    ~CozyDeskFinal();

private:
    Ui::CozyDeskFinalClass ui;
    MusicPlayer* MP;
};

