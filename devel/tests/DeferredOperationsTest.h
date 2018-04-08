#ifndef CONDITIONALSINGLESHOTOPERATIONSTEST_H
#define CONDITIONALSINGLESHOTOPERATIONSTEST_H

#include <QObject>
#include <QtTest/QtTest>

class DeferredOperationsTest : public QObject
{
    Q_OBJECT
public:
    explicit DeferredOperationsTest(QObject *parent = nullptr);

    struct A{};

signals:

private slots:

    void argumentIsPassedToEachElement();
    void falseConditionsAreRexecuted();
    void trueConditionsAreExecutedOnce();
    void recursiveExecutionWorks();
    void recursiveExecutionWorks1();

    void instanceBeforeIsPassedCorrectly();

    void operatorNewNotUsedOnFinishedFunctions();



};

#endif // CONDITIONALSINGLESHOTOPERATIONSTEST_H
