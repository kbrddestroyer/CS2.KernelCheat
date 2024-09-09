import ctypes
import constants

from pykernel.entrypoint import Handler

# Entrypoint for python kernelAPI
# Here you can find lifecycle definition, methods, globals that fall directly on C level
# Never try to modify .cpp files of PythonAPI if it's not needed
# Try to use this library as framework for C-part of cheat
#
# KeyboardDestroyer: 08.09.24 23:00


def message_box(msg, title):
    if constants.DEBUG_MODE:
        ctypes.windll.user32.MessageBoxW(0, msg, title, 0)


def invoke():
    message_box('Hello from python!', 'OK!')
    Handler()


def update():
    pass


def destroy():
    pass
