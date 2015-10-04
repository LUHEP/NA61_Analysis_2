#!/bin/bash

source ./runjob.sh

COUNTFILES=0
CLINE=''
export CLINE
rm -f /tmp/resubmitlist.txt
cat filelist.txt | while read line
do
  let "COUNTFILES=COUNTFILES+1"
  if [ "$COUNTFILES" -gt "$FILESPERJOB" ]
  then
#    echo "$CLINE"
    ARGS=$CLINE
    MD5ARGS=`echo $ARGS | md5sum | cut -b-32`
    echo -e  "\nVALIDATING JOB ""$MD5ARGS"" for ""$ARGS""\n"
    LSOUPTUT=`ls -d $OUTPUTBASE/$MD5ARGS | tee  /tmp/lsout.txt`
    LSOUTPUT=`cat /tmp/lsout.txt`
    rm  /tmp/lsout.txt

    if [ -z $LSOUTPUT ]; then
      echo -e  "\nJOB FILED: " $MD5ARGS "\n";
      echo "for" $ARGS \n;
      echo $ARGS >> /tmp/resubmitlist.txt
    else
      echo -e  "\nJOB IS SUCCESSFUL: " $MD5ARGS "\n \n";
    fi
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
touch /tmp/resubmitlist.txt
MD5ARGS=`echo $ARGS | md5sum | cut -b-32`
echo -e  \n"VALIDATING JOB ""$MD5ARGS"" for ""$ARGS""\n"
LSOUPTUT=`ls -d $OUTPUTBASE/$MD5ARGS | tee  /tmp/lsout.txt`
LSOUTPUT=`cat /tmp/lsout.txt`
rm  /tmp/lsout.txt
if [ -z $LSOUTPUT ]; then
  echo -e  "\nJOB FILED: " $MD5ARGS "\n";
  echo -e  "for" $ARGS \n;
  echo $ARGS >> /tmp/resubmitlist.txt
else
  echo -e  "\nJOB IS SUCCESSFUL: " $MD5ARGS "\n \n";
fi
#bsub -q $QUEUE -M $MEMORY -W $TTL -J $MD5ARGS -Jd \'"$ARGS"\'  $RUNJOB $ARGS
LSTRY=`cat /tmp/resubmitlist.txt`
if [[ -z "$LSTRY" ]]; then
  echo "ALL DONE..."
  echo "You can merge..."
  rm -f /tmp/resubmitlist.txt
else
  echo "I will resubmit the following jobs in 5 seconds..."
  cat /tmp/resubmitlist.txt
  sleep 5
  DATAPATH=""
  ./submit.sh /tmp/resubmitlist.txt
  rm -f /tmp/resubmitlist.txt
fi
