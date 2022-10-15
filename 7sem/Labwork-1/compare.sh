#!/bin/bash

if diff $1 $2 > /dev/null
then
  printf "\x1b[1;32mResults are the same\x1b[0m\n"
else
  printf "\x1b[1;31mResult is not the same\x1b[0m\n"
fi
