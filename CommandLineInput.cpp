#include "CommandLineInput.h"
#include <QApplication>

namespace linae {

CommandLineInput::CommandLineInput() {
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({
        {"f", "file"},
        QApplication::translate("cmd", "Log file to open."),
        QApplication::translate("cmd", "file")});
    parser.addPositionalArgument(
        "<file>",
        QApplication::translate("cmd", "Log file to open."),
        QApplication::translate("cmd", "<file>"));
}

QString CommandLineInput::providedFilePath() {
    if (parser.isSet("file")) {
        return parser.value("file");
    }
    if (!parser.positionalArguments().isEmpty()) {
        return parser.positionalArguments().front();
    }
    return QString();
}

} // linae
