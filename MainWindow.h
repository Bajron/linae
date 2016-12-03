#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;

namespace linae {

class Highlighter;
class SimpleStyleMarker;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /// Opens file dialog for empty file path.
    void openFile(const QString& filePath = QString());

    void noop();

private:
    void setUpFileMenu();
    void setUpViewer();
    void setUpToolbar();

    QTextEdit *textView;

    void setUpHighlighter();
    void setUpStyleMarker();

    Highlighter* highlighter;
    SimpleStyleMarker* styleMarker;
};

} // namespace linae

#endif // MAINWINDOW_H
