#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;

namespace linae {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /// Opens file dialog for empty file path.
    void openFile(const QString& filePath = QString());

private:
    void setUpFileMenu();
    void setUpViewer();

    QTextEdit *textView;
};

} // namespace linae

#endif // MAINWINDOW_H
