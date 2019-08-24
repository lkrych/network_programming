#!/bin/bash
# inspect the main function in the binary executable simple
# with the intel assembly syntax
objdump -x86-asm-syntax intel -D simple | grep _main: -A 20