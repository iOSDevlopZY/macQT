#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include "functionhelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    FunctionHelper::sharedInstance()->startFunction(argv);
    return a.exec();
}
