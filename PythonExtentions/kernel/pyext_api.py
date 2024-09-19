import kernelapi
import ctypes
import constants

from offsets_json import JsonParser, get_json
from pykernel.entrypoint import Handler

# Entrypoint for python kernelAPI
# Here you can find lifecycle definition, methods, globals that fall directly on C level
# Never try to modify .cpp files of PythonAPI if it's not needed
# Try to use this library as framework for C-part of cheat
#
# KeyboardDestroyer: 08.09.24 23:00


g_processes = {}


def message_box(msg, title):
    if constants.DEBUG_MODE:
        ctypes.windll.user32.MessageBoxW(0, msg, title, 0)


def invoke():
    global g_processes
    if hasattr(invoke, 'initialized'):
        raise RuntimeError('Cannot re-initialize kernel')
    setattr(invoke, 'initialized', True)
    g_processes['client.dll'] = kernelapi.get_client()
    
    Handler()


def update():
    Handler.g_handler.update()


def destroy():
    Handler.g_handler.destroy()


if __name__ == '__main__':
    invoke()
