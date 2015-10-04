#!/bin/bash

source ./runjob.sh
OUTPUTPATH=$OUTPUTBASE/out
mkdir -p $OUTPUTPATH/tmp
COUNTFILES=0
CLINE=''
export CLINE
cat filelist.txt | while read line
do
  let "COUNTFILES=COUNTFILES+1"
  if [ "$COUNTFILES" -gt "$FILESPERJOB" ]
  then
#    echo "$CLINE"
    ARGS=$CLINE
    MD5ARGS=`echo $ARGS | md5sum | cut -b-32`
    echo -e  "\nTAKING JOB OUTPUT OF ""$MD5ARGS"" for ""$ARGS""\n"
    curr=`echo $OUTPUTBASE/$MD5ARGS/*.root`
    arrr=($curr)
    arr0=${arrr[0]}
    ln -s -T  $arr0 $OUTPUTPATH/tmp/$MD5ARGS.root
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
echo -e  "\nTAKING JOB OUTPUT OF ""$MD5ARGS"" for ""$ARGS""\n"
curr=`echo $OUTPUTBASE/$MD5ARGS/*.root`
arrr=($curr)
arr0=${arrr[0]}
ln -s -T  $arr0 $OUTPUTPATH/tmp/$MD5ARGS.root
cp mergeScript.C $OUTPUTPATH/tmp/
cd $OUTPUTPATH/tmp/
root -b -q mergeScript.C
mv MergedOutput.root ../
cd ..
rm -rf $OUTPUTPATH/tmp
