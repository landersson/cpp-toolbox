
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

namespace toolbox {

char
get_key(bool block)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (!block)
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    if (!block)
        fcntl(STDIN_FILENO, F_SETFL, oldf);

    return ch;
}

bool
kb_hit()
{
    char ch = get_key(false);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

void
wait_key()
{
    while (!kb_hit()) {
        usleep(10000);
    }

    getchar(); // Flush key buffer for next stop
}

}
