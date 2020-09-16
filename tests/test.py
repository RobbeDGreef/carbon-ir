"""
DISCLAIMER: the code here is pretty trash, I just quickly wiped it
            up to have something to easily test with.

@todo: Create a better test suite
"""
import os
import sys
import subprocess
import re

C_RED = "\033[31;1m"
C_YELLOW = "\033[33;1m"
C_GREEN = "\033[32;1m"
C_BLUE = "\033[34;1m"
C_END = "\033[0m"

testfolder = os.path.join(os.path.dirname(os.path.abspath(__file__)), "testfiles")
executable = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "carbon-ir")

results = []

def add_result(name, success, reason, returncode, expected=0):
    results.append((name, success, reason, returncode, expected))

def extract_expected(path):
    with open(path, "r") as f:
        x = f.readline()[2:]
        return int(x)
    
def rununit(path):
    name = os.path.split(path)[-1]
    buildcommand = [executable, "-o",  "tmp", path]
    ret = subprocess.run(buildcommand).returncode
    if ret:
        add_result(name, False, "compilation error", ret, 0)
        return

    expected = extract_expected(path)
    runcommand = ["./tmp"]
    ret = subprocess.run(runcommand).returncode
    
    if ret != expected:
        add_result(name, False, "binary did not yield expected return value", ret, expected)
        return

    add_result(name, True, "success", ret, expected)
    

def format_results():
    for item in results:
        if isinstance(item, str):
            print(C_BLUE + "[ Folder ] " + C_END + " " + item)
            continue
    
        print(item[0] + ": ", end="")

        if item[1]:
            print(C_GREEN + "[ SUCCEEDED ] " + C_END + "test yielded the correct return value, " + str(item[3]))
        
        else:
            print(C_RED + "[ FAILED    ] " + C_END + item[2] + ", expected: " + str(item[4]) + " got: " + str(item[3]))
            

def str_to_int(string):
    if string.isdigit():
        return int(string)
    
    return string

def natural_keys(file):
    return [ (str_to_int(c)) for c in re.split(r'(\d+)', file)]


def run_all():
    for dirpath, dirnames, filenames in os.walk(testfolder):
        results.append(dirpath)

        filenames = sorted(filenames, key=natural_keys)
        for name in filenames:
            rununit(os.path.join(dirpath, name))

        format_results()

if (len(sys.argv) > 1):
    rununit(os.path.join(testfolder, sys.argv[1] + ".ir"))
    format_results()
else:
    run_all()
