#ifndef SIMPLESTYLEMARKER_H
#define SIMPLESTYLEMARKER_H

#include <QObject>

class QTextEdit;


namespace linae {

class Highlighter;

class SimpleStyleMarker : public QObject
{
    Q_OBJECT
public:
    explicit SimpleStyleMarker(QObject *parent = nullptr);

    void setHighlighter(Highlighter* highlighter);

    void observe(QTextEdit* text);

signals:

public slots:
    void Style0Selected() { return StyleSelected(0); }
    void Style1Selected() { return StyleSelected(1); }
    void Style2Selected() { return StyleSelected(2); }
    void Style3Selected() { return StyleSelected(3); }

    void StyleSelected(int style);

private:
    Highlighter* highlighter;
    QTextEdit* observedText;
};

}

#endif // SIMPLESTYLEMARKER_H
