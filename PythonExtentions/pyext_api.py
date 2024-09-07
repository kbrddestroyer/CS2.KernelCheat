import ctypes


def invoke():
    ctypes.windll.user32.MessageBoxW(0, u'Python kernelapi is initialized, this message is from interpreter', u'OK!', 0)
     