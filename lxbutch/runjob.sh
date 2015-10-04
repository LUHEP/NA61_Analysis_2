#!/bin/bash


### CONFIGURATION ###


#where the Makefile and analysis code is placed (it is read-only place):
RUNCODEPATH='/afs/cern.ch/work/a/aseryako/lxbutch/fromZorg' 

#where to place the output (absolute path is needed):
OUTPUTBASE='/afs/cern.ch/work/a/aseryako/lxbutch/fromZorg/output' 

#where is the data to analyze. Leave it empty, if you pre-defined filelist.txt
DATAPATH=''  
#DATAPATH='/afs/cern.ch/work/e/eandrono/NA61BATCH/MC'
FILESPERJOB=2
QUEUE='1nh' #default is 1nh
MEMORY='2000' # in MB
TTL='60' #runtime in minutes


### END OF CONFIGURATION ###



# This is executable script, which is submitted via lxbatch.
# parameters - list of shoe files to analyze $@
# absolute path on AFS (LXPLUS) or CASTOR
# example:


if [[ -z "$@" ]]; then
  pwd > /dev/null
else



#shinepro


RUNCOMMAND='NA61_LRC_2' #name of execuatble file.
MASK='*.root *.log *.txt *.err' #define an output of the job
TMPWORK='/tmp/'`date +'%Y%m%d%M%H%S%N'`
OUTPUTPATH=$OUTPUTBASE/`echo $@ | md5sum | cut -b-32`

cp -r $RUNCODEPATH  $TMPWORK
cd $TMPWORK
#make
./$RUNCOMMAND $@
mkdir -p $OUTPUTPATH
cp -r $MASK $OUTPUTPATH/
rm -rf $TMPWORK


fi

