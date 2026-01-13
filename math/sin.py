import os
current_dir = os.path.dirname(__file__) + '/'




import sympy
from sympy import pi

x = sympy.symbols('x')
x2 = x * x

#sin(x)

num = -x * (-11511339840.0 + x2 * (1640635920.0 + x2 * (-52785432.0 + x2 * 479249.0)))
den = 11511339840.0 + x2 * (277920720.0 + x2 * (3177720.0 + x2 * 18361.0))
expression = num / den

pnum = sympy.Poly(num)
pden = sympy.poly(den)

print(-pnum/pnum.LC())
print(-pden/pnum.LC())

#sin(2pi * x)

new_expression = expression.subs(x, x * 2 * pi).evalf()
num, den = sympy.fraction(new_expression)

pnum = sympy.Poly(num)
pden = sympy.Poly(den)

print(-pnum/pnum.LC())
print(-pden/pnum.LC())

#sin(2pi * x)

new_expression = expression.subs(x, ((-x + 1) * 2 * pi)).evalf()
num, den = sympy.fraction(new_expression)

pnum = sympy.Poly(num)
pden = sympy.Poly(den)

print(-pnum/pnum.LC())
print(-pden/pnum.LC())