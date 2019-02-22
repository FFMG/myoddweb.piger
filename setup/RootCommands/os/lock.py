import am;
import sys

# This is a test module for Python.  It looks in the standard
# places for various *.py files.  If these are moved, you must
# change this module too.

try:
    import ctypes
    
    am.say("All good.", 100, 2000 )
    ctypes.windll.user32.LockWorkStation();
except:
    am.say("""Could not import the standard "ctypes" module.Please check your PYTHONPATH environment variable.""", 100, 2000 )
    sys.exit(1)
