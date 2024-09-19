import pytest
import os

from kernel import constants, pyext_api
from pykernel.entrypoint import Handler
from offsets_json import get_json

constants.SUPRESS_MSG = True


def test_Holder():
    # Basic test, more as template. Not really useful
    pyext_api.invoke()

    assert Handler.g_handler
    with pytest.raises(RuntimeError):
        Handler()
    with pytest.raises(RuntimeError):
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
  

def test_readJson():
    buttons = get_json('kernel/json/buttons.json')
    assert get_json('kernel/json/buttons.json')
    assert buttons['client.dll']
    assert buttons['client.dll']['attack']

    assert len(buttons) == 1
    assert len(buttons['client.dll']) == 16

    for k, v in buttons.items():
        assert buttons[k] == v
