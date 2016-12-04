#ifndef FILTEREDCONTENT_H
#define FILTEREDCONTENT_H

#include <QObject>

class FilteredContent : public QObject
{
    Q_OBJECT
public:
    explicit FilteredContent(QObject *parent = 0);

signals:

public slots:
};

#endif // FILTEREDCONTENT_H