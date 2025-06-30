#include "kernel/types.h"

#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"
void mkfile(char *filename) {
    int fd = open(filename, O_CREATE | O_RDWR);
    write(fd, "hi", 3);
    close(fd);
}

void mkd(char *dirname) {
    if (mkdir(dirname) < 0) {
        fprintf(2, "mkdir %s failed.", dirname);
        exit(1);
    }
}
void test0() {
    mkd("os2025");

    mkd("os2025/d1");
    mkd("os2025/d2");
    mkd("os2025/d3");

    mkd("os2025/d2/a");
    mkd("os2025/d2/b");
    mkfile("os2025/d2/c");

    mkd("os2025/d3/a");
    mkfile("os2025/d3/b");
}

void test1() {
    mkd("a");
    mkd("a/0");
    mkd("a/1");
    mkd("a/2");
    mkd("a/3");
    mkd("a/4");
    mkd("a/5");
    mkd("a/6");
    mkd("a/7");
    mkd("a/8");
    mkd("a/9");
    mkd("a/10");
}

void test2() {
    mkd("x_x_x_XXXX");
    mkfile("x_x_x_XXXX/xf0");
    mkd("x_x_x_XXXX/x1");
    mkd("x_x_x_XXXX/x2");
    mkd("x_x_x_XXXX/x3");
    mkd("x_x_x_XXXX/x4");
    mkfile("x_x_x_XXXX/x1/f1x");
    mkfile("x_x_x_XXXX/x1/f2x");
    mkfile("x_x_x_XXXX/x2/f3x");
    mkfile("x_x_x_XXXX/x2/f4x");
    mkfile("x_x_x_XXXX/x2/f5x");
    mkfile("x_x_x_XXXX/x3/f6x");
}

void test3() {
    mkd("qazzaqlopt");

    mkd("qazzaqlopt/qtrrtqqttq");
    mkd("qazzaqlopt/qtyytqqttq");
    mkd("qazzaqlopt/qtkktqqttq");
    mkd("qazzaqlopt/qtqqtqqttq");

    mkd("qazzaqlopt/qtrrtqqttq/fqffqffqff");
    mkd("qazzaqlopt/qtrrtqqttq/frffrffrff");

    mkd("qazzaqlopt/qtyytqqttq/fqffqffqff");
    mkd("qazzaqlopt/qtyytqqttq/frffrffrff");

    mkd("qazzaqlopt/qtkktqqttq/qqqqqqqqqq");
    mkd("qazzaqlopt/qtkktqqttq/qazzaqlopt");

    mkd("qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ");
    mkd("qazzaqlopt/qtqqtqqttq/qQqQqQqQqQ");

    mkd("qazzaqlopt/qtrrtqqttq/fqffqffqff/quoouqquo0");
    mkd("qazzaqlopt/qtkktqqttq/qazzaqlopt/q00q00q00q");
    mkd("qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0tooez");  
    mkd("qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0noeza");

    mkfile("qazzaqlopt/qqqq44qqqq");
    mkfile("qazzaqlopt/qtkktqqttq/qazzaqlopt/q00q00q00q/rqqqqqqqqr");
    mkfile("qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0tooez/q123q246q7");
    mkdir("qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0noeza/nomoretest");
}

void test4() {
    mkdir("D");

    mkdir("D/da");
    mkdir("D/db");
    mkdir("D/dc");
    mkdir("D/de");
    mkdir("D/df");
    mkdir("D/dg");
    mkdir("D/dh");
    mkdir("D/di");

    mkdir("D/da/daa");
    mkdir("D/da/dab");
    mkdir("D/da/dac");
    mkdir("D/da/dad");
    mkdir("D/da/dae");
    mkdir("D/da/daf");
    
    mkdir("D/da/dae/daea");
    mkdir("D/da/dae/daeb");
    mkdir("D/da/dae/daec");
    mkdir("D/da/dae/daed");

    mkdir("D/da/dae/daeb/daeba");
    mkdir("D/da/dae/daeb/daebb");
}

int main(int argc, char *argv[]) {
    test0();
    test1();
    test2();
    test3();
    test4();
    exit(0);
}