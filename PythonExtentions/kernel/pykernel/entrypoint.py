from abc import abstractmethod


class Handler(object):
    """
    Handler class, updates cheats from entrypoint
    """

    g_handler = None

    def __init__(self):
        if Handler.g_handler:
            raise RuntimeError('Handler already created')
        Handler.g_handler = self

        self.__cheats = []

    def add(self, cheat):
        self.__cheats.append(cheat)

    def update(self):
        for cheat in self.__cheats:
            cheat.update()

    def destroy(self):
        for cheat in self.__cheats:
            cheat.destroy()
        Handler.g_handler = None


class Cheat(object):
    """
    Base cheat class, handled automatically
    """

    def __init__(self):
        Handler.g_handler.add(self)

    @abstractmethod
    def update(self):
        pass

    @abstractmethod
    def destroy(self):
        pass

