# runs all files in the input folder

from subprocess import call
import subprocess
from pathlib import Path
import os

call(("make"))

outpath = 'output'
inpath = 'input'

infiles = os.listdir(inpath)
infiles = [f.split(".")[0] for f in infiles if ".txt" in f]

for filename in infiles:
    os.system(f"make run < {inpath}/{filename}.txt > {outpath}/{filename}_stdout.txt \n\
    dmesg | grep OS_scheduler > {outpath}/{filename}_dmesg.txt")
    # sleep(10)

