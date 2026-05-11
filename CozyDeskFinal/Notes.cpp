#include "notes.h"
#include <QDir> //folder
#include <QFile> //files (html)
#include <QTextStream> //writing and reading files
#include <QFileDialog> //save as windows
#include <QPrinter> //pdf export
#include <QStandardPaths> //locator to find the app data (safe place o save)
#include <QTextCharFormat> 
#include <QTextListFormat>
#include <QTextCursor>
#include <QFont>
#include <QRegularExpression> //for santizing

Notes::Notes(QListWidget* list, QTextEdit* editor, QLineEdit* titleEdit, QObject* parent) //constructor taking pointors and then storing them in variable
    : QObject(parent), list(list), editor(editor), titleEdit(titleEdit) {
}

void Notes::setup() {
    connect(list, &QListWidget::currentItemChanged, this, &Notes::onNoteSelected); //connects a signal from original class from qt to a slot
    connect(titleEdit, &QLineEdit::editingFinished, this, &Notes::onTitleChanged);

    loadFromDisk();
    if (notes.isEmpty()) newNote();
}

// function to locate a suitable directory for storing notes, creates it if it doesn't exist, and returns the path
QString Notes::notesDir() const {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/notes"; //this is a function from qt that helps us find a safe direction such as documents or downloads
    QDir().mkpath(path); //actually writes the pahs
    return path;
}

// function to sanitize note names for safe use as filenames by replacing invalid characters with underscores
QString Notes::sanitize(const QString& name) const {
    QString s = name;
	s.replace(QRegularExpression("[\\\\/:*?\"<>|]"), "_"); //replaces invalid filename characters with underscores
    return s;
}



void Notes::newNote() {
    saveCurrentNote(); //saves the previous notes
    int idx = 1;
    QString name;

    do {
        name = QString("Note %1").arg(idx++);  // qt string formatting, the %1 is just a plceholder, the .arg() function tells that whatever is in %1 replace it with the argument in the closed brackets
    } while (notes.contains(name));

    notes[name] = "";

    //the list block
    list->addItem(name); //built in qt function, adds a new list entry to the widget qlist that we added
    list->setCurrentRow(list->count() - 1); //upar wali row set kardo
    currentNote = name;

    //the title edit block
    titleEdit->setText(name); //this is what actually sets in the name

    // the text edit block
    editor->clear(); //when new note is clicked, it clears away everything in the box
    editor->setFocus(); //this solves issue of the cursor disappearing
}


// saving

void Notes::saveCurrentNote() {
    if (currentNote.isEmpty()) return;

    QString html = editor->toHtml();//this turns whatever text is in the text edit block into an html

    notes[currentNote] = html;

    saveToDisk(currentNote, html);//calling function that we built
}

//deleting

void Notes::deleteCurrentNote() {
    if (currentNote.isEmpty()) return;

    notes.remove(currentNote);
    deleteFromDisk(currentNote);

    int row = list->currentRow();

    list->blockSignals(true);
    delete list->currentItem();
    list->blockSignals(false);

    currentNote.clear();
    editor->clear();
    titleEdit->clear();

    if (list->count() > 0) {
        int nextRow = qMin(row, list->count() - 1);
        list->setCurrentRow(nextRow);
        QListWidgetItem* item = list->currentItem(); //another pointer that points to the lists current item
        if (item) { //checks if it doenst point to an empty thing
            //then continues setting the current notes to the item stuff
            currentNote = item->text();
            titleEdit->setText(currentNote);
            editor->setHtml(notes.value(currentNote));
        }
    }

    editor->setFocus(); //forces cursor into text block
}


// switching notes

void Notes::onNoteSelected(QListWidgetItem* current, QListWidgetItem* previous) {

    Q_UNUSED(previous);//tells compiler not to give warning about old note because it was not being used, its a safe fail, prevents bugs

    if (!current) return; //if current doesnt exist, stop here

    saveCurrentNote(); //saves current note

    //actual switching happens below

    currentNote = current->text(); 
    titleEdit->setText(currentNote);
    editor->setHtml(notes.value(currentNote));
    editor->setFocus();
}


//uses QFile Rename and renames the file name

void Notes::onTitleChanged() {
    QString newName = titleEdit->text().trimmed(); //this takes what we edited into the title edit and trims access
    if (newName.isEmpty() || newName == currentNote) return; //failsafe

    QString oldPath = notesDir() + "/" + sanitize(currentNote) + ".html";
    QString newPath = notesDir() + "/" + sanitize(newName) + ".html";

    //actual renaming process
    if (QFile::rename(oldPath, newPath)) {
        QString html = notes.take(currentNote);
        notes[newName] = html;

		//changes stuff in the list widget
        if (list->currentItem())
            list->currentItem()->setText(newName);
        currentNote = newName;
    }
    editor->setFocus();
}



void Notes::toggleBold() {
    QTextCharFormat fmt; //this is a format holder
    fmt.setFontWeight(editor->fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    editor->textCursor().mergeCharFormat(fmt); //whatever highlighted by cursort
    editor->setFocus();
}

void Notes::insertHeading() {
    QTextCursor cursor = editor->textCursor(); //making a vairable that holds the cursor's postion

    // this makes it so the entire line remains selected when we click the ehaidng buton
    cursor.beginEditBlock();

    cursor.movePosition(QTextCursor::StartOfBlock); //wherever the cursor might be it tells the cursor to move to the start of the line

    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);//anchor thats like holding shift button

    QTextCharFormat fmt = cursor.charFormat();

    if (fmt.fontPointSize() == 22 && fmt.fontWeight() == QFont::Bold) {
        QTextCharFormat normal;
        normal.setFontPointSize(10);
        normal.setFontWeight(QFont::Normal);
        cursor.mergeCharFormat(normal);
    }
    else {
        QTextCharFormat heading;
        heading.setFontPointSize(22);
        heading.setFontWeight(QFont::Bold);
        cursor.mergeCharFormat(heading);
    }

    cursor.endEditBlock();

    editor->setTextCursor(cursor);
    editor->setFocus();
}

void Notes::insertBullet() {
    QTextCursor cursor = editor->textCursor();
    QTextListFormat fmt;
    fmt.setStyle(QTextListFormat::ListDisc);
    cursor.insertList(fmt);
    editor->setTextCursor(cursor);
    editor->setFocus();
}



void Notes::saveToDisk(const QString& name, const QString& html) {
    QFile file(notesDir() + "/" + sanitize(name) + ".html");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) //tells the computer we also want to write text into the file, and taht the file will be text
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
        name.chop(5); //removes the .html (5 letters)
        notes[name] = QTextStream(&file).readAll();
        list->addItem(name);
    }
    if (!notes.isEmpty()) {
        list->setCurrentRow(0);
        currentNote = list->item(0)->text();
        titleEdit->setText(currentNote);
        editor->setHtml(notes[currentNote]);
        editor->setFocus();
    }
}

void Notes::exportAsPdf() {
    if (currentNote.isEmpty()) return;

    QString path = QFileDialog::getSaveFileName(nullptr, "Export as PDF",
        currentNote + ".pdf", "PDF files (*.pdf)");

    //if user closes window
    if (path.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    editor->document()->print(&printer);
    editor->setFocus();
}