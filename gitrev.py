import os
import sys
import subprocess
from subprocess import call
import click

Import("env")


revision = subprocess.check_output(["git", "rev-parse", "--short" , "HEAD"]).strip()
branch = subprocess.check_output(["git", "rev-parse", "--abbrev-ref" , "HEAD"]).strip()
st = "\\\"%s_%s\\\"" % (branch,revision)
print ("\n\n****\nbuilding GITVERSION %s \n****\n\n " % (st))


#print "-DPIO_SRC_REV=%s" % revision
env.Append(CPPDEFINES=[("GITVERSION", st)])

