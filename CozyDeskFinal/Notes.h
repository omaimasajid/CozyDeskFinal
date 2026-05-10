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
    explicit Notes(QListWidget* list, QTextEdit* editor, QLineEdit* titleEdit, QObject* parent = nullptr);

    void setup();

public slots:
    void newNote();
    void saveCurrentNote();
    void deleteCurrentNote();
    void exportAsPdf();
    void exportAsTxt();
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

    QListWidget* m_list;
    QTextEdit* m_editor;
    QLineEdit* m_titleEdit;
    QString      m_currentNote;
    QMap<QString, QString> m_notes;
};