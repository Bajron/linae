#ifndef FILTEREDCONTENT_H
#define FILTEREDCONTENT_H

#include <QObject>
#include <QVector>

#include <functional>

class QTextStream;

namespace linae {

class FilteredContent : public QObject
{
    Q_OBJECT
public:
    explicit FilteredContent(QObject *parent = 0);

    void initializeContent(QTextStream &stream);

    const QString& getContent() {
        return content;
    }

    int getAbsoluteCursor() {
        return cursor.absolute;
    }

    enum class FilterType {
        HideNotContaining = 0,
        ShowContaining,
        HideContaining,
        Count
    };

signals:

public slots:
    void resetFilters(int absCursor = 0);

    void addFilter(FilterType type, const QString& str, int absCursor = 0);

    void addHideNotContainingFilter(const QString& str, int absCursor) {
        return addFilter(FilterType::HideNotContaining, str, absCursor);
    }

    void addHideContainingFilter(const QString& str, int absCursor) {
        return addFilter(FilterType::HideContaining, str, absCursor);
    }

    void addShowContainingFilter(const QString& str, int absCursor) {
        return addFilter(FilterType::ShowContaining, str, absCursor);
    }
private:
    QString content;

    struct Cursor {
        int absolute = 0;
        int inLine = 0;
        int line = 0;
    };

    Cursor cursor;

    void calculateCursor(int absCursor);
    void updateCursor();

    struct LineIndex {
        int number;
        int start;
        QString content;
    };

    QVector<LineIndex> index;
    QVector<bool> status;

    struct Filter {
        FilterType type;
        QString arg;
    };

    QVector<Filter> filters;

    QVector<std::function<bool(const QString&, const QString&, bool)>> filterMap;

    void updateContent();

    void applyFilter();
    void applyLastFilter();

    bool filter(const QString& line) const;
};

}

#endif // FILTEREDCONTENT_H
