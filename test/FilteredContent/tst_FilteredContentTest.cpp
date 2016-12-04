#include <QString>
#include <QtTest>

class FilteredContentTest : public QObject
{
    Q_OBJECT

public:
    FilteredContentTest();

private Q_SLOTS:
    void testCase1();
};

FilteredContentTest::FilteredContentTest()
{
}

void FilteredContentTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(FilteredContentTest)

#include "tst_FilteredContentTest.moc"
