import time

def savetime(func):
    def wrapper(*args, **kwargs):
        start = time.clock()
        func(*args, **kwargs)
        end = time.clock()
        try:
            args[0]._time += end - start
        except:
            args[0]._time = end - start
    return wrapper
