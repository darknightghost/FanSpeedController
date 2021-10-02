#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import subprocess


def main():
    #Parse argument
    parser = argparse.ArgumentParser(description="Genetate .hex file.")
    parser.add_argument("input", type=str, help="Input file.")
    parser.add_argument("output", type=str, help="Output file.")

    args = parser.parse_args()

    # Run command.
    process = subprocess.run(("packihx", args.input), stdout=subprocess.PIPE)
    ret = process.returncode
    if ret != 0:
        return ret

    f = open(args.output, "wb")
    f.write(process.stdout)
    f.close()

    return 0


if __name__ == "__main__":
    exit(main())
