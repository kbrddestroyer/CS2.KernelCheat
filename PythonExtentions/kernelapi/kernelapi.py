# Placeholder file for C library development replacement
# C library is enabled in runtime
import ctypes


def init():
    """Initialize kernelapi"""
    pass


def get_client():
    """Get driver address as number from C"""
    pass


def read(address, size):
    """Read data from process memory"""
    pass


def write(address, value, size):
    """Write data to process memory"""
    pass
