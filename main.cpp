#include "CommandLineInput.h"
#include "MainWindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("linae");
    app.setApplicationVersion("0.1");

    linae::CommandLineInput input;
    input.process(app);

    linae::MainWindow window;

    QString file = input.providedFilePath();
    if (!file.isEmpty()) {
        QTimer::singleShot(0, [&]() {
            window.openFile(file);
        });
    }

    window.resize(640, 480);
    window.show();

    return app.exec();
}
