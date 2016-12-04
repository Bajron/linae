#include "Highlighter.h"
#include "MainWindow.h"
#include "SimpleStyleMarker.h"

#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>
#include <QFileDialog>
#include <QTextEdit>
#include <QApplication>
#include <QClipboard>
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

    connect(textView, &QTextEdit::selectionChanged, styleMarker, &SimpleStyleMarker::selected);

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

            {
                QTextStream stream(&file);
                content.initializeContent(stream);
            }

            updateContent();
        }
    }
}

void MainWindow::noop() {

}

void MainWindow::useFilter(FilteredContent::FilterType type) {
    content.addFilter(type, getFilter(), textView->textCursor().position());
    updateContent();
}

void MainWindow::resetFiltering() {
    int cur = textView->textCursor().selectionStart();

    content.resetFilters(cur);

    updateContent();
}

void MainWindow::updateContent() {
    textView->setText(content.getContent());

    auto c = textView->textCursor();
    c.setPosition(content.getAbsoluteCursor());
    textView->setTextCursor(c);
}

QString MainWindow::getFilter() const {
    QString fromClipboard = QApplication::clipboard()->text();

    if (not fromClipboard.isNull() and not fromClipboard.isEmpty()) {
        return fromClipboard;
    }
    qDebug() << "Clipboard was empty on getFilter.";

    QString fromSelection = textView->textCursor().selectedText();

    if (not fromSelection.isNull() and not fromSelection.isEmpty()) {
        return fromSelection;
    }
    qWarning() << "Selection was empty on getFilter,";

    return QString();
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

    filtering->addAction(tr("Show"), this, SLOT(showFilter()))
            ->setShortcut(QKeySequence("Ctrl+Shift+G"));

    filtering->addAction(tr("Leave"), this, SLOT(leaveFilter()))
            ->setShortcut(QKeySequence("Ctrl+Shift+H"));

    filtering->addAction(tr("Hide"), this, SLOT(hideFilter()))
            ->setShortcut(QKeySequence("Ctrl+Shift+J"));

    filtering->addAction(tr("Reset"), this, SLOT(resetFiltering()))
            ->setShortcut(QKeySequence("Ctrl+Alt+0"));

    QToolBar *marking = addToolBar(tr("Marking actions"));
    marking->addAction(tr("Color 1"), styleMarker, SLOT(style0Selected()));
    marking->addAction(tr("Color 2"), styleMarker, SLOT(style1Selected()));
    marking->addAction(tr("Color 3"), styleMarker, SLOT(style2Selected()));
    marking->addAction(tr("Color 4"), styleMarker, SLOT(style3Selected()));
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
