from typing import override

from pykernel.entrypoint import Cheat
from offsets_json import get_json


class UpdateLocalPlayer(Cheat):
    def __init__(self):
        super(UpdateLocalPlayer, self).__init__()
        self.offsets = get_json('buttons.json')

    def update(self):
        pass

    def destroy(self):
        pass