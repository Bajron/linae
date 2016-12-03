#include "Highlighter.h"
#include "MainWindow.h"
#include "SimpleStyleMarker.h"

#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>
#include <QFileDialog>
#include <QTextEdit>
#include <QApplication>
#include <QToolBar>
#include <QTextStream>
#include <QDebug>

namespace linae {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    textView(nullptr),
    styleMarker(nullptr)
{
    setUpViewer();
    setUpHighlighter();
    setUpStyleMarker();

    setUpFileMenu();
    setUpToolbar();

    setCentralWidget(textView);
    setWindowTitle(tr("linae"));
}

MainWindow::~MainWindow() {

}

void MainWindow::openFile(const QString &filePath) {
    QString fileToOpen = filePath;

    if (fileToOpen.isNull()) {
        fileToOpen = QFileDialog::getOpenFileName(this, tr("Choose file"), "", "Log files (*.log)");
    }

    if (!fileToOpen.isEmpty()) {
        QFile file(fileToOpen);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            setWindowTitle(tr("linae") + tr(" -- ") + fileToOpen);
            setOriginalContent(file.readAll());
        }
    }
}

void MainWindow::noop() {

}

void MainWindow::filterSelected() {
    QString selected = textView->textCursor().selectedText();

    if (selected.isNull() || selected.isEmpty()) {
        qDebug() << "Selection is empty for filtering.";
        return;
    }

    int cur = textView->textCursor().selectionStart();
    int newCur = 0;

    qDebug() << "Previous cursor was " << cur;

    QTextStream stream(&content, QIODevice::ReadOnly);
    QString filtered;
    QTextStream out(&filtered);

    auto inPos = stream.pos();
    QString line = stream.readLine();

    while (!line.isEmpty()) {
        if (line.contains(selected)) {
            out << line << '\n';
        }

        if (inPos <= cur && cur < stream.pos()) {
            out.flush();
            auto outPos = filtered.size() - line.size();

            qDebug() << inPos << " < " << cur << " < " << stream.pos();
            newCur = outPos + cur - inPos - 1;
            qDebug() << "New cursor found " << newCur << " outPos: " << outPos;
        }

        inPos = stream.pos();
        line = stream.readLine();
    }
    setContent(*out.string());

    auto c = textView->textCursor();
    c.setPosition(newCur);
    textView->setTextCursor(c);
}

void MainWindow::resetFiltering() {
    setContent(original);
}

void MainWindow::setOriginalContent(const QString &c) {
    original = c;
    setContent(original);
}

void MainWindow::setContent(const QString &c) {
    content = c;
    textView->setText(content);
}

void MainWindow::setUpFileMenu() {
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(tr("&Open"), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), QApplication::instance(), SLOT(quit()), QKeySequence::Quit);

    menuBar()->addMenu(fileMenu);
}

void MainWindow::setUpViewer() {
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    textView = new QTextEdit(this);
    textView->setFont(font);
    textView->setPlainText("Open log...");
    textView->setReadOnly(true);
    textView->setTextInteractionFlags(textView->textInteractionFlags() | Qt::TextSelectableByKeyboard);
}

void MainWindow::setUpToolbar() {
    QToolBar *fileToolbar = addToolBar(tr("File actions"));
    fileToolbar->addAction(tr("&Open"), this, SLOT(openFile()))
            ->setShortcut(QKeySequence::Open);

    QToolBar *filtering = addToolBar(tr("Filtering actions"));
    filtering->addAction(tr("Show"), this, SLOT(noop()))
            ->setShortcut(QKeySequence("Ctrl+Alt+G"));
    filtering->addAction(tr("Leave"), this, SLOT(filterSelected()))
            ->setShortcut(QKeySequence("Ctrl+Alt+H"));
    filtering->addAction(tr("Hide"), this, SLOT(noop()))
            ->setShortcut(QKeySequence("Ctrl+Alt+J"));
    filtering->addAction(tr("Reset"), this, SLOT(resetFiltering()))
            ->setShortcut(QKeySequence("Ctrl+Alt+0"));

    QToolBar *marking = addToolBar(tr("Marking actions"));
    marking->addAction(tr("Color 1"), styleMarker, SLOT(Style0Selected()));
    marking->addAction(tr("Color 2"), styleMarker, SLOT(Style1Selected()));
    marking->addAction(tr("Color 3"), styleMarker, SLOT(Style2Selected()));
    marking->addAction(tr("Color 4"), styleMarker, SLOT(Style3Selected()));
    marking->addAction(tr("Toggle bookmark"), this, SLOT(noop()));
}

void MainWindow::setUpHighlighter() {
    highlighter = new Highlighter(textView->document());
}

void MainWindow::setUpStyleMarker() {
    styleMarker = new SimpleStyleMarker(this);
    styleMarker->setHighlighter(highlighter);
    styleMarker->observe(textView);
}

} // namespace linae
