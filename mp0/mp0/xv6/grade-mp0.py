from gradelib import *

r = Runner()


@test(5, "mp0 command with public testcase 0")
def test_mp0_0():
    r.run_qemu(shell_script([
        'testgen',
        'mp0 os2202 a',
    ]))
    r.match(
        'os2202 [error opening dir]',
        '',
        '0 directories, 0 files',
    )


@test(5, "mp0 command with public testcase 1")
def test_mp0_1():
    r.run_qemu(shell_script([
        # 'testgen',
        'mp0 os2202/ o',
    ]))
    r.match(
        'os2202/ [error opening dir]',
        '',
        '0 directories, 0 files',
    )


@test(5, "mp0 command with public testcase 2")
def test_mp0_2():
    r.run_qemu(shell_script([
        # 'testgen',
        'mp0 os2025 o',
    ]))
    r.match(
        'os2025 1',
        'os2025/d1 1',
        'os2025/d2 1',
        'os2025/d2/a 1',
        'os2025/d2/b 1',
        'os2025/d2/c 1',
        'os2025/d3 1',
        'os2025/d3/a 1',
        'os2025/d3/b 1',
        '',
        '6 directories, 2 files',
    )


@test(5, "mp0 command with public testcase 3")
def test_mp0_3():
    r.run_qemu(shell_script([
        # 'testgen',
        'mp0 os2025/ o',
    ]))
    r.match(
        'os2025/ 1',
        'os2025//d1 1',
        'os2025//d2 1',
        'os2025//d2/a 1',
        'os2025//d2/b 1',
        'os2025//d2/c 1',
        'os2025//d3 1',
        'os2025//d3/a 1',
        'os2025//d3/b 1',
        '',
        '6 directories, 2 files',
    )


@test(10, "mp0 command with public testcase 4")
def test_mp0_4():
    r.run_qemu(shell_script([
        # 'testgen',
        'mp0 a a',
    ]))
    r.match(
        'a 1',
        'a/0 1',
        'a/1 1',
        'a/2 1',
        'a/3 1',
        'a/4 1',
        'a/5 1',
        'a/6 1',
        'a/7 1',
        'a/8 1',
        'a/9 1',
        'a/10 1',
        '',
        '11 directories, 0 files',
    )


@test(10, "mp0 command with public testcase 5")
def test_mp0_5():
    r.run_qemu(shell_script([
        # 'testgen',
        'mp0 a/ a',
    ]))
    r.match(
        'a/ 1',
        'a//0 1',
        'a//1 1',
        'a//2 1',
        'a//3 1',
        'a//4 1',
        'a//5 1',
        'a//6 1',
        'a//7 1',
        'a//8 1',
        'a//9 1',
        'a//10 1',
        '',
        '11 directories, 0 files',
    )


"""
################
PRIVATE TESTCASE
################
"""

@test(10, "mp0 command with private testcase 0")
def test_tree_private_0():
    r.run_qemu(shell_script(['mp0 count z']))
    r.match(
        'count [error opening dir]',
        '',
        '0 directories, 0 files',
    )


@test(15, "mp0 command with private testcase 1")
def test_tree_private_1():
    r.run_qemu(shell_script(['mp0 x_x_x_XXXX x']))
    r.match(
        "x_x_x_XXXX 3",
        "x_x_x_XXXX/xf0 4",
        "x_x_x_XXXX/x1 4",
        "x_x_x_XXXX/x1/f1x 5",
        "x_x_x_XXXX/x1/f2x 5",
        "x_x_x_XXXX/x2 4",
        "x_x_x_XXXX/x2/f3x 5",
        "x_x_x_XXXX/x2/f4x 5",
        "x_x_x_XXXX/x2/f5x 5",
        "x_x_x_XXXX/x3 4",
        "x_x_x_XXXX/x3/f6x 5",
        "x_x_x_XXXX/x4 4",
        "",
        "4 directories, 7 files",
    )

@test(15, "mp0 command with private testcase 2")
def test_tree_private_1():
    r.run_qemu(shell_script(['mp0 D a']))
    r.match(
        "D 0",
        "D/da 1",
        "D/da/daa 3",
        "D/da/dab 2",
        "D/da/dac 2",
        "D/da/dad 2",
        "D/da/dae 2",
        "D/da/dae/daea 4",
        "D/da/dae/daeb 3",
        "D/da/dae/daeb/daeba 5",
        "D/da/dae/daeb/daebb 4",
        "D/da/dae/daec 3",
        "D/da/dae/daed 3",
        "D/da/daf 2",
        "D/db 0",
        "D/dc 0",
        "D/de 0",
        "D/df 0",
        "D/dg 0",
        "D/dh 0",
        "D/di 0",
        "",
        "20 directories, 0 files"
    )


@test(20, "mp0 command with private testcase 3")
def test_tree_private_2():
    r.run_qemu(shell_script([
        # 'testgen',
        'mp0 qazzaqlopt q',
    ]))
    r.match(
        "qazzaqlopt 2",
        "qazzaqlopt/qtrrtqqttq 6",
        "qazzaqlopt/qtrrtqqttq/fqffqffqff 9",
        "qazzaqlopt/qtrrtqqttq/fqffqffqff/quoouqquo0 12",
        "qazzaqlopt/qtrrtqqttq/frffrffrff 6",
        "qazzaqlopt/qtyytqqttq 6",
        "qazzaqlopt/qtyytqqttq/fqffqffqff 9",
        "qazzaqlopt/qtyytqqttq/frffrffrff 6",
        "qazzaqlopt/qtkktqqttq 6",
        "qazzaqlopt/qtkktqqttq/qqqqqqqqqq 16",
        "qazzaqlopt/qtkktqqttq/qazzaqlopt 8",
        "qazzaqlopt/qtkktqqttq/qazzaqlopt/q00q00q00q 12",
        "qazzaqlopt/qtkktqqttq/qazzaqlopt/q00q00q00q/rqqqqqqqqr 20",
        "qazzaqlopt/qtqqtqqttq 8",
        "qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ 8",
        "qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0tooez 8",
        "qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0tooez/q123q246q7 11",
        "qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0noeza 8",
        "qazzaqlopt/qtqqtqqttq/QQQQQQQQQQ/osmp0noeza/nomoretest 8",
        "qazzaqlopt/qtqqtqqttq/qQqQqQqQqQ 13",
        "qazzaqlopt/qqqq44qqqq 10",
        "",
        "17 directories, 3 files",
    )


run_tests()