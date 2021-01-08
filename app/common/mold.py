from .kernel import Kernel


class Mold():


    def __init__(self, fs, N, n_ch, kernel=None, use_gpu=False):
        self.N = N
        self.fs = fs
        self.n_ch = n_ch
        self.resample_factor = 0
        self.mix_freq = []
        self.use_gpu = use_gpu

        if kernel:
            self.k = kernel
        else:
            self.k = Kernel(gpu=self.use_gpu)

        self.__compile_plan()


    def __exp(self, freq, N, fs):
        T = 1.0 / fs
        x = self.k.xp.linspace(0.0, N*T, N, endpoint=False)
        return self.k.xp.exp(freq * 1.j * 2.0 * self.k.xp.pi * x)


    def __fft(self, sig):
        T = 1.0 / self.pfs
        yf = self.k.xf.fft(sig * self.window)
        yf = 2.0 / self.pN * self.k.xp.abs(yf)
        xf = self.k.xf.fftfreq(self.pN, T)
        return yf, xf


    def __compile_plan(self):
        if not self.n_ch > 1:
            raise ValueError("Number of channels has to be larger than 1.")

        self.resample_factor = self.n_ch
        self.pN = self.N * self.resample_factor
        self.pfs = self.fs * self.resample_factor
        self.window = self.k.xs.blackman(self.pN)

        factor_map = [(self.n_ch - 1) - (2 * i) for i in range(self.n_ch)]

        for factor in self.k.xp.flip(factor_map):
            f = (self.fs / 2) * factor
            mix = self.__exp(f, self.pN, self.pfs)
            self.mix_freq.append(mix)


    def inject(self, chs):
        if len(chs) != self.n_ch:
            raise ValueError("The n_ch is different than initialization.")

        if len(chs[0]) != self.N:
            raise ValueError("The N is different than initialization.")

        product_sig = self.k.xp.zeros(self.pN, dtype=chs[0].dtype)

        for i, ch in enumerate(chs):
            product_sig += self.k.xs.resample(ch, self.pN) * self.mix_freq[i]

        return product_sig


    def fft(self, sig):
        return self.__fft(sig)


    def print(self):
        print("============ Mold Configuration =========")
        print(f"Sample-rate (fs): {self.fs}")
        print(f"Number of Samples (N): {self.N}")
        print(f"Number of Channels (n_ch): {self.n_ch}")
        print(f"Resample Factor: {self.resample_factor}")
        print(f"GPU-mode: {self.use_gpu}")
        print("=========================================")
