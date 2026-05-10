#include "CozyDeskFinal.h"
#include "MusicPlayer.h"
#include "Pet.h"
#include "PomodoroTimer.h"
#include "Notes.h"
#include <QTime>


CozyDeskFinal::CozyDeskFinal(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    alarmPlayer = new QMediaPlayer(this);
    alarmOutput = new QAudioOutput(this);
    alarmPlayer->setAudioOutput(alarmOutput);

  //stuff for he buttons
    ui.btnNewNote->setText("New");
    ui.btnBold->setText("B");
    ui.btnHeading->setText("H");
    ui.btnBullet->setText("*");
    ui.btnDeleteNote->setText("Del");
    ui.btnSave->setText("Save");
    ui.btnExportPdf->setText("PDF");
    ui.btnExportTxt->setText("TXT");


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


    // timer stuffs

    PT = new PomodoroTimer(this);

    connect(ui.startTime, &QPushButton::clicked, this, [this]() {
        int mins = ui.timerSet->time().minute();
        int secs = ui.timerSet->time().second();
        if (mins == 0 && secs == 0) return;

        if (ui.workRadio->isChecked())
            ui.sessionLabel->setText("Working!");
        else if (ui.breakRadio->isChecked())
            ui.sessionLabel->setText("Break Time!");
        PT->startTimer(mins, secs);
        });

    connect(PT, &PomodoroTimer::playSound, this, [this]() {
        ui.sessionLabel->setText("Done!!");
        alarmPlayer->setSource(QUrl("qrc:/sound/alarm.wav"));
        alarmPlayer->play();
        });

    connect(ui.pauseTime, &QPushButton::clicked, this, [this]() {
        PT->pauseTimer();
        alarmPlayer->stop();
        });

    connect(ui.resetTime, &QPushButton::clicked, this, [this]() {
        PT->resetTimer();
        ui.sessionLabel->setText("");
        });

    connect(PT, &PomodoroTimer::changeTime, this, [this](QString t) {
        ui.timeLabel->setText(t);
        });

  


    //notes 

    m_notes = new Notes(ui.notesList, ui.noteEditor, ui.noteTitleEdit, this);
    m_notes->setup();

    connect(ui.btnNewNote, &QPushButton::clicked, m_notes, &Notes::newNote);
    connect(ui.btnSave, &QPushButton::clicked, m_notes, &Notes::saveCurrentNote);
    connect(ui.btnDeleteNote, &QPushButton::clicked, m_notes, &Notes::deleteCurrentNote);
    connect(ui.btnExportPdf, &QPushButton::clicked, m_notes, &Notes::exportAsPdf);
    connect(ui.btnExportTxt, &QPushButton::clicked, m_notes, &Notes::exportAsTxt);
    connect(ui.btnBold, &QPushButton::clicked, m_notes, &Notes::toggleBold);
    connect(ui.btnHeading, &QPushButton::clicked, m_notes, &Notes::insertHeading);
    connect(ui.btnBullet, &QPushButton::clicked, m_notes, &Notes::insertBullet);
}



CozyDeskFinal::~CozyDeskFinal()
{}

