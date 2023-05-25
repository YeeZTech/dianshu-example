import random
import sys


def gen_ordered_data(filename):
    f = open(filename, 'w')
    gap = 10
    count = 1000
    n = 0
    for i in range(count):
        n += int(10 * random.random() + 1)
        f.write('{}\n'.format(n))
    f.close()


def gen_unordered_data(filename):
    f = open(filename, 'w')
    count = 1000
    for i in range(count):
        f.write('{}\n'.format(int(count * random.random())))
    f.close()


def main():
    gen_unordered_data(sys.argv[1])


if __name__ == '__main__':
    main()
