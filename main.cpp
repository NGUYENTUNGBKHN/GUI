#include <stdio.h>
#include "libcpp.h"
#include "JWin.h"


extern int JWIN_Framework(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    printf("GUI Start !! \n");
    JWIN_Framework(0, NULL);
    
    return 0;
}



