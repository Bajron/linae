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
            textView->setText(file.readAll());
        }
    }
}

void MainWindow::noop() {

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

void MainWindow::setUpToolbar()
{
    QToolBar *fileToolbar = addToolBar(tr("File actions"));
    fileToolbar->addAction(tr("&Open"), this, SLOT(openFile()));

    QToolBar *filtering = addToolBar(tr("Filtering actions"));
    filtering->addAction(tr("Show"), this, SLOT(noop()));
    filtering->addAction(tr("Leave"), this, SLOT(noop()));
    filtering->addAction(tr("Hide"), this, SLOT(noop()));
    filtering->addAction(tr("Reset"), this, SLOT(noop()));

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
