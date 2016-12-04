#include "Highlighter.h"

#include <QDebug>

namespace linae {

Highlighter::Highlighter(QTextDocument *parent)
    :QSyntaxHighlighter(parent)
{
    setUpMarkerStyles();
}

void Highlighter::addMarked(const QString &text, int style) {
    if (style < 0 || style >= markerStyles.size()) {
        qWarning() << "Nonexisting style requested (" << style << ") for " << text;
    }

    markers[style].append(text);

    rehighlight();
}

void Highlighter::setSelected(const QString &text) {
    if (text != selected) {
        selected = text;
        rehighlight();
    }
}

void Highlighter::highlightBlock(const QString &text) {
    for (int style = 0; style < markers.size(); ++style) {
        for (const auto& mark : markers[style]) {
            int markLength = mark.size();
            int index = text.indexOf(mark, 0, Qt::CaseInsensitive);
            while (index >= 0) {
                setFormat(index, markLength, markerStyles[style]);
                index = text.indexOf(mark, index + markLength, Qt::CaseInsensitive);
            }
        }
    }

    if (not selected.isNull() and not selected.isEmpty()) {
        int markLength = selected.size();
        int index = text.indexOf(selected, 0, Qt::CaseInsensitive);
        while (index >= 0) {
            setFormat(index, markLength, selectedStyle);
            index = text.indexOf(selected, index + markLength, Qt::CaseInsensitive);
        }
    }
}

void Highlighter::setUpMarkerStyles() {
    for (const auto& col : {Qt::cyan, Qt::yellow, Qt::green, Qt::magenta}) {
        QTextCharFormat format;
        format.setBackground(col);
        markerStyles.push_back(format);
        markers.push_back(QVector<QString>());
    }

    selectedStyle.setBackground(QColor(80,255,160));
}

}
