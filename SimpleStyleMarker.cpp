#include "SimpleStyleMarker.h"
#include "Highlighter.h"

#include <QTextEdit>
#include <QDebug>

namespace linae {

SimpleStyleMarker::SimpleStyleMarker(QObject *parent) : QObject(parent),
    highlighter(nullptr),
    observedText(nullptr)
{

}

void SimpleStyleMarker::setHighlighter(Highlighter *h) {
    highlighter = h;
}

void SimpleStyleMarker::observe(QTextEdit *te) {
    observedText = te;
}

void SimpleStyleMarker::selected() {
    highlighter->setSelected(observedText->textCursor().selectedText());
}

void SimpleStyleMarker::styleSelected(int style) {
    if (!highlighter) {
        qWarning() << "Style selected used when no highlighter is provided.";
        return;
    }

    if (!observedText) {
        qWarning() << "Style selected used when no text is observed.";
        return;
    }

    QString selected = observedText->textCursor().selectedText();

    if (selected.isNull() or selected.isEmpty()) {
        qDebug() << "Mark clicked for empty selection.";
        return;
    }

    highlighter->addMarked(selected, style);
}


}
