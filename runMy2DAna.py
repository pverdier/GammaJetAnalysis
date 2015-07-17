#!/usr/bin/env python
#
#==============================================================================#
# Import Modules                                                               #
#==============================================================================#

import sys, os, string, shutil

outRoot ="Plots"
outLog  ="Plots"
exe = "./runMy2DAna"

fileListDir = "filelists"

mkdir="mkdir "+outRoot
os.system(mkdir)

copy = "cp My2DAna.cpp "+outRoot+"/."
os.system(copy)

filelist = os.popen("ls  "+fileListDir+"/*.list").readlines()

for aFile in filelist:
    #if aFile.count("htaumu") == 0:
    #    continue
    
    outRoo1 = outRoot+os.sep+string.split(string.strip(aFile),"/")[-1][:-5]+".root"
    outLog1 = outLog+os.sep+string.split(string.strip(aFile),"/")[-1][:-5]+".log"
    
    option  = " -doraw"
    #option  = " -doraw -skim"
    #option  = " "
    if aFile.count("MC") == 1:
        option = option+" -mc -domc "

    cmd = "nohup "+exe+" -filelist "+string.strip(aFile)+" -out "+outRoo1+" "+option+" >&  "+outLog1 +"  & "
    #cmd = exe+" -filelist "+string.strip(aFile)+" -out "+outRoo1+" "+option+"  >&  "+outLog1 
    
    print cmd
    os.system(cmd)
    
sys.exit()
