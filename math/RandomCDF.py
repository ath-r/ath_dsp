import numpy as np
import matplotlib.pyplot as plt

SIZE = 20000

x = np.linspace(-1, 1, SIZE)
y = np.random.standard_normal(SIZE)
y /= np.max(y)

y.sort()

pol = np.polyfit(x, y, 13)

plt.plot(x)
plt.plot(y)
plt.plot(np.polyval(pol, x))

import os
plt.savefig(os.path.dirname(__file__) + '/RandomCDF.png')

print(repr(pol))
print(__file__)
