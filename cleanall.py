# removes all output text files from output folder

from subprocess import call
import subprocess
from pathlib import Path
import os

outpath = 'output'
inpath = 'input'

outfiles = os.listdir(outpath)
outfiles = [f for f in outfiles if ".txt" in f]

for filename in outfiles:
    os.system(f"rm {outpath}/{filename}")

call("make clean".split())