class Handler(object):
    g_handler = None

    def __init__(self):
        if Handler.g_handler:
            raise RuntimeError('Handler already created')
        Handler.g_handler = self
