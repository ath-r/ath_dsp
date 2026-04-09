import numpy as np
from numpy import sin, pi, floor

import matplotlib.pyplot as plt

a = 3
kernel_size = a*2 + 1

def lanczos(x):
    if np.abs(x) < 1e-12: return 1.0
    if np.abs(x) > a: return 0.0

    num = a * sin(pi * x) * sin(pi * x / a)
    den = (pi * x)**2

    return num/den

class ResamplerState:
    def __init__(self):
        self.delay = np.zeros(kernel_size)
        self.offset = 0

def resample(in_buffer, in_sr, out_buffer, out_sr, state):
    _ratio = in_sr / out_sr

    for i in range(out_buffer.size):

        norm = 0
        out_buffer[i] = 0

        x_in_center = i * _ratio + state.offset - kernel_size

        for j in range(kernel_size):

            j_centered = j - a

            x_in = x_in_center + j 
            x_in_i = int(floor(x_in))
            x_in_offset = x_in - x_in_i

            x_lan = j_centered - x_in_offset
            y_lan = lanczos(x_lan)

            norm += y_lan

            sample = 0
            if x_in_i < 0: sample = state.delay[x_in_i + kernel_size]
            elif x_in_i < in_buffer.size: sample = in_buffer[x_in_i]
            else: print('LOOK OVER HERE IT\'S HAPPENING')

            out_buffer[i] += sample * y_lan

        out_buffer[i] /= norm
    state.offset = x_in_offset
    state.delay = in_buffer[-kernel_size:]

sr1 = 48000
sr2 = 32000
ratio = sr2 / sr1

x1 = np.arange(50)
y1 = np.sin(np.sin(x1/4))

plt.plot(x1, y1, marker='.')



in_samples = 10
out_samples = in_samples * ratio

out_buffer = np.ndarray(int(out_samples))

state = ResamplerState()
for i in range(5):
    input = y1[in_samples * i: in_samples]
    resample(, sr1, out_buffer, sr2, state)




x2 = np.linspace(0, out_samples / ratio, int(out_samples))
plt.plot(x2, out_buffer, marker='.')