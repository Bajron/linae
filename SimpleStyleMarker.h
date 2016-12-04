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
    void style0Selected() { return styleSelected(0); }
    void style1Selected() { return styleSelected(1); }
    void style2Selected() { return styleSelected(2); }
    void style3Selected() { return styleSelected(3); }

    void selected();

    void styleSelected(int style);

private:
    Highlighter* highlighter;
    QTextEdit* observedText;
};

}

#endif // SIMPLESTYLEMARKER_H
