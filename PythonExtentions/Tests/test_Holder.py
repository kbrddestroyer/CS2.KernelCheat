import pytest
import os
import constants


constants.SUPRESS_MSG = True


def test_Holder():
    # Basic test, more as template. Not really useful
    import pyext_api
    pyext_api.invoke()

    assert pyext_api.Holder.g_holder
    with pytest.raises(RuntimeError):
        pyext_api.Holder()

    pyext_api.invoke()


@pytest.mark.parametrize(
    'path', [
        '..\\usermode\\',
        '..\\usermode\\src\\client_dll.hpp',
        '..\\kernelmode\\'
    ]
)
def test_project_files(path):
    # Checks project files that may require in development
    os.path.exists(path)
