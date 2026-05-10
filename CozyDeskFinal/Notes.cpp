#include "notes.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QPrinter>
#include <QStandardPaths>
#include <QTextCharFormat>
#include <QTextListFormat>
#include <QTextCursor>
#include <QFont>
#include <QRegularExpression>

Notes::Notes(QListWidget* list, QTextEdit* editor, QLineEdit* titleEdit, QObject* parent)
    : QObject(parent), m_list(list), m_editor(editor), m_titleEdit(titleEdit) {
}

void Notes::setup() {
    connect(m_list, &QListWidget::currentItemChanged, this, &Notes::onNoteSelected);
    connect(m_titleEdit, &QLineEdit::editingFinished, this, &Notes::onTitleChanged);

    loadFromDisk();
    if (m_notes.isEmpty()) newNote();
}

// ── Helpers ───────────────────────────────

QString Notes::notesDir() const {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/notes";
    QDir().mkpath(path);
    return path;
}

QString Notes::sanitize(const QString& name) const {
    QString s = name;
    s.replace(QRegularExpression("[\\\\/:*?\"<>|]"), "_");
    return s;
}

// ── Note CRUD ─────────────────────────────

void Notes::newNote() {
    saveCurrentNote();
    int idx = 1;
    QString name;
    do { name = QString("Note %1").arg(idx++); } while (m_notes.contains(name));
    m_notes[name] = "";
    m_list->addItem(name);
    m_list->setCurrentRow(m_list->count() - 1);
    m_currentNote = name;
    m_titleEdit->setText(name);
    m_editor->clear();
    m_editor->setFocus();
}

void Notes::saveCurrentNote() {
    if (m_currentNote.isEmpty()) return;
    QString html = m_editor->toHtml();
    m_notes[m_currentNote] = html;
    saveToDisk(m_currentNote, html);
}

void Notes::deleteCurrentNote() {
    if (m_currentNote.isEmpty()) return;
    m_notes.remove(m_currentNote);
    deleteFromDisk(m_currentNote);
    delete m_list->currentItem();
    m_currentNote.clear();
    m_editor->clear();
    m_titleEdit->clear();
}

void Notes::onNoteSelected(QListWidgetItem* current, QListWidgetItem* previous) {
    Q_UNUSED(previous);
    if (!current) return;
    saveCurrentNote();
    m_currentNote = current->text();
    m_titleEdit->setText(m_currentNote);
    m_editor->setHtml(m_notes.value(m_currentNote));
}

void Notes::onTitleChanged() {
    QString newName = m_titleEdit->text().trimmed();
    if (newName.isEmpty() || newName == m_currentNote) return;
    QString html = m_notes.take(m_currentNote);
    deleteFromDisk(m_currentNote);
    m_notes[newName] = html;
    if (m_list->currentItem())
        m_list->currentItem()->setText(newName);
    m_currentNote = newName;
    saveToDisk(newName, html);
}

// ── Formatting ────────────────────────────

void Notes::toggleBold() {
    QTextCharFormat fmt;
    fmt.setFontWeight(m_editor->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    m_editor->textCursor().mergeCharFormat(fmt);
}

void Notes::insertHeading() {
    QTextCursor cursor = m_editor->textCursor();
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

    QTextCharFormat fmt = cursor.charFormat();

    if (fmt.fontPointSize() == 22 && fmt.fontWeight() == QFont::Bold) {
        // already a heading, revert to normal
        QTextCharFormat normal;
        normal.setFontPointSize(10);
        normal.setFontWeight(QFont::Normal);
        cursor.mergeCharFormat(normal);
    }
    else {
        // apply heading
        QTextCharFormat heading;
        heading.setFontPointSize(22);
        heading.setFontWeight(QFont::Bold);
        cursor.mergeCharFormat(heading);
    }

    cursor.endEditBlock();
    m_editor->setTextCursor(cursor);
}

void Notes::insertBullet() {
    QTextCursor cursor = m_editor->textCursor();
    QTextListFormat fmt;
    fmt.setStyle(QTextListFormat::ListDisc);
    cursor.insertList(fmt);
    m_editor->setTextCursor(cursor);
}

// ── Disk I/O ──────────────────────────────

void Notes::saveToDisk(const QString& name, const QString& html) {
    QFile file(notesDir() + "/" + sanitize(name) + ".html");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        QTextStream(&file) << html;
}

void Notes::deleteFromDisk(const QString& name) {
    QFile::remove(notesDir() + "/" + sanitize(name) + ".html");
}

void Notes::loadFromDisk() {
    QDir dir(notesDir());
    for (const QString& filename : dir.entryList({ "*.html" }, QDir::Files)) {
        QFile file(dir.filePath(filename));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) continue;
        QString name = filename;
        name.chop(5);
        m_notes[name] = QTextStream(&file).readAll();
        m_list->addItem(name);
    }
    if (!m_notes.isEmpty()) {
        m_list->setCurrentRow(0);
        m_currentNote = m_list->item(0)->text();
        m_titleEdit->setText(m_currentNote);
        m_editor->setHtml(m_notes[m_currentNote]);
    }
}

void Notes::exportAsPdf() {
    if (m_currentNote.isEmpty()) return;
    QString path = QFileDialog::getSaveFileName(nullptr, "Export as PDF",
        m_currentNote + ".pdf", "PDF files (*.pdf)");
    if (path.isEmpty()) return;
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    m_editor->document()->print(&printer);
}

void Notes::exportAsTxt() {
    if (m_currentNote.isEmpty()) return;
    QString path = QFileDialog::getSaveFileName(nullptr, "Export as Text",
        m_currentNote + ".txt", "Text files (*.txt)");
    if (path.isEmpty()) return;
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        QTextStream(&file) << m_editor->toPlainText();
}