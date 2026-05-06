#include "CozyDeskFinal.h"
#include "MusicPlayer.h"
#include "Pet.h"

CozyDeskFinal::CozyDeskFinal(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    MP = new MusicPlayer(this);
    ui.volSlider->hide();
    ui.playButton->setText("▶");

    connect(ui.fileButton, &QPushButton::clicked, this, [this]() {
        QStringList files = QFileDialog::getOpenFileNames(this, "Select a song", "",
            "Audio Files (*.mp3 *.wav *.flac *.ogg)");
        MP->loadfiles(files);
        });

    connect(ui.playButton, &QPushButton::clicked, this, [this]() {
        if (ui.playButton->text() == "▶") {
            ui.playButton->setText("❚❚");
            MP->playSong();
        }
        else {
            ui.playButton->setText("▶");
            MP->pauseSong();
        }
        });

    connect(ui.nextButton, &QPushButton::clicked, this, [this]() {
        MP->nextSong();
        });
    connect(ui.prevButton, &QPushButton::clicked, this, [this]() {
        MP->prevSong();
        });

    connect(ui.volButton, &QPushButton::clicked, this, [this]() {
        ui.volSlider->setVisible(!ui.volSlider->isVisible());
        });

    connect(ui.volSlider, &QSlider::sliderMoved, this, [this](int v) {
        MP->setVolume(v);
        });

    connect(ui.playerSlider, &QSlider::sliderMoved, this, [this](int v) {
        MP->setMusicSlider(v);
        });

    connect(MP, &MusicPlayer::positionChange, this, [this](qint64 p) {
        ui.playerSlider->setValue(p * 1000 / MP->getDuration());
        });

    // Below this is the stuff for the pet

    pet = new petwidget(ui.petFrame);
    pet->setGeometry(0, 0, ui.petFrame->width(), ui.petFrame->height());
}




CozyDeskFinal::~CozyDeskFinal()
{}

