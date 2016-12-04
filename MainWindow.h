#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "FilteredContent.h"

#include <QMainWindow>

class QTextEdit;

namespace linae {

class Highlighter;
class SimpleStyleMarker;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /// Opens file dialog for empty file path.
    void openFile(const QString& filePath = QString());

    void noop();

    void leaveFilter() {
        useFilter(FilteredContent::FilterType::HideNotContaining);
    }

    void hideFilter() {
        useFilter(FilteredContent::FilterType::HideContaining);
    }

    void showFilter() {
        useFilter(FilteredContent::FilterType::ShowContaining);
    }

    void useFilter(FilteredContent::FilterType type);

    void resetFiltering();

protected:
    void updateContent();

    QString getFilter() const;

private:
    void setUpFileMenu();
    void setUpViewer();
    void setUpToolbar();

    FilteredContent content;

    QTextEdit *textView;

    void setUpHighlighter();
    void setUpStyleMarker();

    Highlighter* highlighter;
    SimpleStyleMarker* styleMarker;
};

} // namespace linae

#endif // MAINWINDOW_H
