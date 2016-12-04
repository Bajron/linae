#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QVector>

namespace linae {

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument* parent = nullptr);

public slots:
    void addMarked(const QString& text, int style);
    void setSelected(const QString& text);

protected:
    void highlightBlock(const QString &text) override;

private:
    QVector<QTextCharFormat> markerStyles;
    QVector<QVector<QString>> markers;

    QString selected;
    QTextCharFormat selectedStyle;

    void setUpMarkerStyles();
};

}

#endif // HIGHLIGHTER_H
