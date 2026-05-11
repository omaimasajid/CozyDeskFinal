#pragma once
#include <QObject>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QMap>
#include <QString>

class Notes : public QObject {
    Q_OBJECT
public:
    //constructor
    explicit Notes(QListWidget* list, QTextEdit* editor, QLineEdit* titleEdit, QObject* parent = nullptr);
    
    // a setup function that would run when program runs
    void setup();

public slots:
    void newNote();
    void saveCurrentNote();
    void deleteCurrentNote();
    void exportAsPdf();
    void toggleBold();
    void insertHeading();
    void insertBullet();

private slots:
    void onNoteSelected(QListWidgetItem* current, QListWidgetItem* previous);
    void onTitleChanged();

private:
    void loadFromDisk();
    void saveToDisk(const QString& name, const QString& html);
    void deleteFromDisk(const QString& name);
    QString notesDir() const;
    QString sanitize(const QString& name) const;

    QListWidget* list;
    QTextEdit* editor;
    QLineEdit* titleEdit;
    QString currentNote;
    QMap<QString, QString> notes; //special that holds 2 pair, first is the key which is the title, second string is the html file that is saved
};