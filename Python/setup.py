#setup.py
import sys, os
from cx_Freeze import setup, Executable

__version__ = "1.1.0"

include_files = ['config.txt','readme.txt']
excludes = ["tkinter"]
packages = ["os", "sys", "time","networktables","serial","ast","serial.tools.list_ports"]

setup(
    name = "appname",
    description='App Description',
    version=__version__,
    options = {"build_exe": {
    'packages': packages,
    'include_files': include_files,
    'excludes': excludes,
    'include_msvcr': True,
}},
executables = [Executable("Listener.py",base="Win32GUI")]
)
