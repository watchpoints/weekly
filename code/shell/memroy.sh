#!/bin/sh

if [ $# -ne 1 ]; then
  echo "Usage: `basename $0` process_name"
  exit 1
fi

APPNAME=$1
PROC="`ps -ef | grep "$APPNAME" | grep -v "grep" | grep -v "awk" | grep -v $0 | awk '{print $2}'`"

if [ -z $PROC ]; then
  echo "invalid process_name"
  exit 1
fi

SMAPS="/proc/$PROC/smaps"
STATUS="/proc/$PROC/status"
echo "proc ---$PROC----"
OLDHEAP="0"
while :
do
  #HEAP="`cat $STATUS | grep "VmData" | awk '{print $2}'`"
  HEAP=`cat $SMAPS | grep -A 5 "heap" | grep "Rss" | awk '{print $2}'`
  if [ $HEAP -lt $OLDHEAP ]; then
    echo "`date` HEAP -`expr $OLDHEAP - $HEAP` to $HEAP kb"
    OLDHEAP=$HEAP
  elif [ $HEAP -gt $OLDHEAP ]; then
    echo "`date` HEAP +`expr $HEAP - $OLDHEAP` to $HEAP kb"
    OLDHEAP=$HEAP
  fi
  sleep 1
done

