#ifndef COMMANDLINEINPUT_H
#define COMMANDLINEINPUT_H

#include <QCommandLineParser>

namespace linae {

class CommandLineInput
{
public:
    CommandLineInput();

    void process(QCoreApplication& app) {
        parser.process(app);
    }

    QString providedFilePath();

private:
    QCommandLineParser parser;
};

}

#endif // COMMANDLINEINPUT_H
