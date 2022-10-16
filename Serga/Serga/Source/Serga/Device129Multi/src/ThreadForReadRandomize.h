#pragma once

#include <QThread>

class AbstractAltera;

class ThreadForReadRandomize : public QThread
{
public:
    ThreadForReadRandomize(AbstractAltera *_altera);
    void run();

public slots:
    void StartAltera(bool _first);

private:
    AbstractAltera *altera;
};