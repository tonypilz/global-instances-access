// Example program

#include "tests/TestMain.h"

//#include "tools/SingleFileTester.h"
//#include <tools/SingleFileGenerator.h>

#include <iostream>

int main(int argc, char* argv[])
{
    global::tests::TestMain{argc, argv};

    std::cout<<__FILE__<<" finished\n";

    return 0;
}

