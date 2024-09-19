class Instance(object):
    def __init__(self):
        self.definitions = {}
        self.private_name = 'instance'

    def __get__(self, instance, owner):
        return getattr(instance, self.private_name)

    def __set__(self, instance, value):
        setattr(instance, self.private_name, value)

    def __set_name__(self, owner, name):
        self.private_name = owner + '_' + name


class Singleton(object):
    def __call__(self, original_class):
        assert original_class
        original_init = original_class.__init__

        setattr(original_class, 'instance', Instance())

        def __init__(instance, *args, **kwargs):
            instance.instance = instance
            original_init(instance, args, kwargs)

        original_class.__init__ = __init__
        return original_class
