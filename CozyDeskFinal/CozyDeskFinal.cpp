#include "CozyDeskFinal.h"
#include "MusicPlayer.h"
#include "Pet.h"
#include <QTimer>

CozyDeskFinal::CozyDeskFinal(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //music player stuffs

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
            ui.playButton->setText("ⅠⅠ");
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
        ui.playerSlider->blockSignals(true);
        ui.playerSlider->setValue(p * 1000 / MP->getDuration());
        ui.playerSlider->blockSignals(false);
        });

    connect(MP, &MusicPlayer::durationChange, this, [this](qint64 d) {
        ui.playerSlider->setMaximum(1000);
        });

    connect(MP, &MusicPlayer::songChange, this, [this](QString n) {
        ui.songLabel->setText(n);
        });


    // Below this is the stuff for the pet

    pet = new petwidget(ui.petFrame);
    pet->setGeometry(0, 0, ui.petFrame->width(), ui.petFrame->height());

    connect(MP, &MusicPlayer::isplaying, pet, &petwidget::isPetListening);
}




CozyDeskFinal::~CozyDeskFinal()
{}

