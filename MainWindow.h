#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace linae {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

};

} // namespace linae

#endif // MAINWINDOW_H
