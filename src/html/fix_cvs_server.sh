#!/bin/sh

for i in `find . -path '*/CVS/Root'`
do
  echo $1 > $i
done
