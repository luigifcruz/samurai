import importlib


class Kernel():


    def __init__(self, gpu=False):
        self.gpu = gpu

        self.__init_numpy()
        self.__init_scipy_signal()
        self.__init_scipy_fft()


    def __init_numpy(self):
        self.np = importlib.import_module("numpy")

        if self.gpu:
            self.xp = importlib.import_module("cupy")
        else:
            self.xp = self.np


    def __init_scipy_signal(self):
        self.ss = importlib.import_module("scipy.signal")

        if self.gpu:
            self.xs = importlib.import_module("cusignal")
        else:
            self.xs = self.ss


    def __init_scipy_fft(self):
        self.sf = importlib.import_module("scipy.fft")

        if self.gpu:
            self.xf = importlib.import_module("cusignal.fft")
        else:
            self.xf = self.sf

