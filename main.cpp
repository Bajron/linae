#include "MainWindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QTimer>

void setUpCommandlineParser(QCommandLineParser& parser);
QString fileFromCommandline(QCommandLineParser& parser);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("linae");
    app.setApplicationVersion("0.1");

    QCommandLineParser parser;
    setUpCommandlineParser(parser);

    parser.process(app);

    linae::MainWindow window;

    QString file = fileFromCommandline(parser);
    if (!file.isEmpty()) {
        QTimer::singleShot(0, [&]() {
            window.openFile(file);
        });
    }

    window.resize(640, 480);
    window.show();

    return app.exec();
}


void setUpCommandlineParser(QCommandLineParser& parser) {
    parser.addHelpOption();
    parser.addOption({
        {"f", "file"},
        QApplication::translate("cmd", "Log file to open."),
        QApplication::translate("cmd", "file")});
    parser.addPositionalArgument(
        "<file>",
        QApplication::translate("cmd", "Log file to open."),
        QApplication::translate("cmd", "<file>"));
}

QString fileFromCommandline(QCommandLineParser &parser) {
    if (parser.isSet("file")) {
        return parser.value("file");
    }
    if (!parser.positionalArguments().isEmpty()) {
        return parser.positionalArguments().front();
    }
    return QString();
}
