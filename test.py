import sys
import subprocess


def test(p_name):
    with open('test_cases', 'r') as file:
        for line in file.readlines():
            subprocess.call('echo' + ' ' + line + ' | ./source/build/' + p_name +
                            ' --even', shell=True)


if __name__ == '__main__':
    test(sys.argv[1])
