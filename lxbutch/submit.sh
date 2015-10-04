#!/bin/bash


source ./runjob.sh ""

RUNJOB=`pwd`/runjob.sh

# For resubmitting:
if [ -z "$@" ]; then
  FILELIST=filelist.txt
else
  FILELIST=$@
  FILESPERJOB=1
fi

# If we have DATAPATH we will generate filelists, otherwise we will use the one from filelist.txt
if [ -z "$DATAPATH" ]; then
  echo "FILE LIST pre-defined:"
  cat $FILELIST
else
  ./preparefilelist.sh $DATAPATH
  echo "FILE LIST generated:"
  cat $FILELIST
fi




MEMORY=$MEMORY'200'
COUNTFILES=0
CLINE=''
export CLINE
cat $FILELIST | while read line
do
  let "COUNTFILES=COUNTFILES+1"
  if [ "$COUNTFILES" -gt "$FILESPERJOB" ]
  then
    ARGS=$CLINE
    MD5ARGS=`echo $ARGS | md5sum | cut -b-32`
    echo -e "\nSTARTING JOB ""$MD5ARGS"" for ""$ARGS""\n"
    bsub -q $QUEUE -M $MEMORY -W $TTL -J $MD5ARGS -Jd \'"$ARGS"\'  $RUNJOB $ARGS
    CLINE=''
    COUNTFILES=1
  fi
  if [ -z "$CLINE" ]; then
    CLINE=$line
  else
    CLINE=$CLINE' '$line
  fi


  
  echo $CLINE > /tmp/clinesubmit.txt  
done
ARGS=`cat /tmp/clinesubmit.txt`
rm -f /tmp/clinesubmit.txt
MD5ARGS=`echo $ARGS | md5sum | cut -b-32`
echo -e "\nSTARTING JOB ""$MD5ARGS"" for ""$ARGS""\n"
bsub -q $QUEUE -M $MEMORY -W $TTL -J $MD5ARGS -Jd \'"$ARGS"\'  $RUNJOB $ARGS
#-v 4000000
