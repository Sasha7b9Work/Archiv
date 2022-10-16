#include "ThreadForReadRandomize.h"

ThreadForReadRandomize::ThreadForReadRandomize(AbstractAltera *_altera) 
    : QThread(),
    altera(_altera)
{

}

void ThreadForReadRandomize::run()
{
    exec();
}