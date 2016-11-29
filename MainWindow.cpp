#include "MainWindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>
#include <QFileDialog>
#include <QTextEdit>
#include <QApplication>

namespace linae {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), textView(nullptr)
{
    setUpFileMenu();
    setUpViewer();

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
}

} // namespace linae
