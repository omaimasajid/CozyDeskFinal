#include "CozyDeskFinal.h"
#include "MusicPlayer.h"
#include "Pet.h"
#include "PomodoroTimer.h"
#include "Notes.h"
#include <QTime>
#include <Qlistwidget>
#include <QFileInfo>
#include <QFile>


CozyDeskFinal::CozyDeskFinal(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // this is for the fancy style
    QFile file("style.qss");
    file.open(QFile::ReadOnly);
    setStyleSheet(QString(file.readAll()));
    setFixedSize(1080, 720);

    // background

    QPalette palette;
    palette.setBrush(QPalette::Window, QPixmap("petsprites/bg.jpeg").scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setPalette(palette);

    // this is the alarm
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
        ui.songDropdown->blockSignals(true);
        ui.songDropdown->clear();
        for (QString file : files) {
            QFileInfo info(file);
            ui.songDropdown->addItem(info.fileName());
        }
        ui.songDropdown->blockSignals(false);
        });

    connect(ui.songDropdown, &QComboBox::currentIndexChanged, this, [this](int i) {
        MP->playSongAt(i);
        });

    connect(ui.songDropdown, &QComboBox::currentIndexChanged, this, [this](int index) {
        MP->playSongAt(index);
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
        ui.songDropdown->blockSignals(true);
        ui.songDropdown->setCurrentIndex(MP->getSongIndex());
        ui.songDropdown->blockSignals(false);
        });


    connect(ui.repeatButton, &QPushButton::clicked, this, [this]() {
        MP->toggleRepeat();
        repeatOn = !repeatOn;
        if (repeatOn) {
            ui.repeatButton->setStyleSheet(
                "QPushButton { background-color: none; border-radius: none; border: none; font-size: 20px; font-weight: bold; color: #5883A7; }"
                "QPushButton:hover { background-color: none; color: #486D9C; }"
                "QPushButton:pressed { background-color: none; color: black; }"
            );
        }
        else {
            ui.repeatButton->setStyleSheet(
                "QPushButton { background-color: none; border-radius: none; border: none; font-size: 20px; font-weight: bold; color: #173357; }"
                "QPushButton:hover { background-color: none; color: #486D9C; }"
                "QPushButton:pressed { background-color: none; color: black; }"
            );
        }
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



    // to do list

    connect(ui.addTask, &QPushButton::clicked, this, [this]() {
        QString task = ui.taskInput->text();
        if (task.isEmpty()) return;
        QListWidgetItem* item = new QListWidgetItem(task);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui.taskList->addItem(item);
        ui.taskInput->clear();
        });

    connect(ui.delTask, &QPushButton::clicked, this, [this]() {
        int row = ui.taskList->currentRow();
        if (row == -1) return; 
        ui.taskList->takeItem(row);
        });

    connect(ui.taskList, &QListWidget::itemChanged, this, [this](QListWidgetItem* item) {
        QFont font = item->font();
        font.setStrikeOut(item->checkState() == Qt::Checked);
        item->setFont(font);
        });
}



CozyDeskFinal::~CozyDeskFinal()
{}

