#include "FilteredContent.h"

#include <QTextStream>
#include <QDebug>

namespace linae {

FilteredContent::FilteredContent(QObject *parent) : QObject(parent),
    filterMap(static_cast<int>(FilterType::Count))
{
    filterMap[static_cast<int>(FilterType::HideNotContaining)] =
            [](const QString& line, const QString& arg, bool was) -> bool {
                return was && line.contains(arg);
            };
    filterMap[static_cast<int>(FilterType::ShowContaining)] =
            [](const QString& line, const QString& arg, bool was) -> bool {
                return was || line.contains(arg);
            };
    filterMap[static_cast<int>(FilterType::HideContaining)] =
            [](const QString& line, const QString& arg, bool was) -> bool {
                return was && (not line.contains(arg));
            };
}

void FilteredContent::initializeContent(QTextStream &stream) {
    index.clear();
    content.clear();
    status.clear();

    int lineNo = 0;
    int pos = 0;

    QTextStream out(&content);
    QString line = stream.readLine();
    while (!line.isNull()) {
        index.push_back(LineIndex{lineNo, pos, line});

        ++lineNo;
        pos += line.size();

        out << line << '\n';

        line = stream.readLine();
    }

    status.fill(true, index.size());
}

void FilteredContent::resetFilters(int absCursor) {
    calculateCursor(absCursor);

    filters.clear();

    applyFilter();
    updateContent();

    updateCursor();
}

void FilteredContent::addFilter(FilterType type, const QString &str, int absCursor) {
    if (str.isNull() || str.isEmpty()) {
        qWarning() << "Filter string is empty.";
        return;
    }

    calculateCursor(absCursor);

    filters.push_back(Filter{type, str});

    applyLastFilter();
    updateContent();

    updateCursor();
}

void FilteredContent::calculateCursor(int absCursor) {
    int cur = absCursor;

    qDebug() << "Absolute cursor received " << cur;

    for (const auto& line : index) {
        if (!status[line.number]) {
            continue;
        }
        if (cur < line.content.size()) {
            cursor = Cursor{ absCursor, cur, line.number };
            qDebug() << "Cursor is in line" << line.number << ", character " << cur;
            break;
        } else {
            cur -= line.content.size() + 1;
        }
    }
}

void FilteredContent::updateCursor() {
    if (!status[cursor.line]) {
        qDebug() << "Cursor line hidden.";
        cursor.inLine = 0;

        // Move up until 0 or visible line.
        int l = cursor.line;
        while (l > 0 && !status[l]) {
            --l;
        }

        cursor.line = l;
    }

    int newAbsCur = cursor.inLine;
    for (int l = cursor.line - 1; l >= 0; --l) {
        if (status[l]) {
            newAbsCur += index[l].content.size() + 1;
        }
    }

    cursor.absolute = newAbsCur;

    qDebug() << "Cursor after update " << cursor.absolute;
}

void FilteredContent::updateContent() {
    content.clear();
    QTextStream out(&content);

    for (const auto& line : index) {
        if (status[line.number]) {
            out << line.content << '\n';
        }
    }
}

void FilteredContent::applyFilter() {
    for (const auto& l : index) {
        status[l.number] = filter(l.content);
    }
}

void FilteredContent::applyLastFilter() {
    const auto& last = filters.back();

    for (const auto& line : index) {
        status[line.number] = filterMap[static_cast<int>(last.type)](line.content, last.arg, status[line.number]);
    }
}

bool FilteredContent::filter(const QString &line) const {
    bool result = true;
    for (const auto& f : filters) {
        result = filterMap[static_cast<int>(f.type)](line, f.arg, result);
    }
    return result;
}

}
