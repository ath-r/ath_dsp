import os
current_dir = os.path.dirname(__file__) + '/'

import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import AAA

np.set_printoptions(precision=20, floatmode='fixed', suppress=True)

def zero_even(p):
    p_rev = p[::-1]
    p_rev[np.arange(p_rev.size) % 2 == 0] = 0
    return p_rev[::-1]

def zero_odd(p):
    p_rev = p[::-1]
    p_rev[np.arange(p_rev.size) % 2 == 1] = 0
    return p_rev[::-1]

def barycentric_to_pq(r):
    zs = r.support_points
    fs = r.support_values
    ws = r.weights
    m = len(zs)

    max_deg = m - 1

    p_coef = np.zeros(max_deg + 1)
    q_coef = np.zeros(max_deg + 1)

    for j in range(m):
        other_zs = np.delete(zs, j)
        poly_term = np.poly(other_zs)  # standard monomial basis

        # Multiply term into p and q
        # Numerator gets f_j * w_j, denominator gets w_j
        p_coef += ws[j] * fs[j] * poly_term
        q_coef += ws[j] * poly_term

    return (p_coef, q_coef)

def approximate_odd(x, y, max_terms=100):

    r = AAA(x, y, max_terms=max_terms)
    p, q = barycentric_to_pq(r)
    p = zero_even(p)
    q = zero_odd(q)
    q /= p[0]
    p /= p[0]

    return (p, q)

def juce_sin(x):
    x2 = x * x
    num = x * (24019.5385697205 + x2 * (-3423.34761261891 + x2 * (110.14197630042 - x2)))
    den = 24019.5385697205 + x2 * (579.908815667847 + x2 * (6.63062416405668 + x2 * 0.0383120256901944))
    return num / den

# sin(x) on [-pi:pi]
print('sin(x) on [-pi:pi]')

x = np.linspace(-np.pi, np.pi, 1000)
y = np.sin(x)

p, q = approximate_odd(x, y)
y_approx = np.polyval(p, x) / np.polyval(q, x)

print(p)
print(q)

plt.plot(x, y_approx, label = 'sin9(x) on [-pi, pi]')
plt.plot(x, (y - y_approx) * 1e12, label = 'error * 1e12')

p, q = approximate_odd(x, y, 8)
y_approx = np.polyval(p, x) / np.polyval(q, x)
plt.plot(x, y_approx, label = 'sin7(x) on [-pi, pi]')
plt.plot(x, (y - y_approx) * 1e8, label = 'error * 1e12')

# sin(2pix) on [-0.5:0.5]
print('sin(2pix) on [-0.5:0.5]')

x = np.linspace(-0.5, 0.5, 600)
y = np.sin(2 * np.pi * x)
p, q = approximate_odd(x, y)

y_approx = np.polyval(p, x) / np.polyval(q, x)

print(p)
print(q)

plt.plot(x, y_approx, label = 'sin(2pix) on [-0.5, 0.5]')
#plt.plot(x, (y - y_approx) * 1e12, label = 'error * 1e12')

plt.legend()
plt.savefig(current_dir + 'sin.png')
