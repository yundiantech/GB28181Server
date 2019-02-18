
#include "GB28181Server/GB28181Server.h"

#include <Windows.h>

int main(int argc, char *argv[])
{
    GB28181Server *server = new GB28181Server;
    server->start();

    while(1)
    {
        Sleep(1000);
    }

    return 0;
}
