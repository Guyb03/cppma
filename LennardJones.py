import numpy as np
import matplotlib.pyplot as plt

r = np.linspace(0.9, 4.0, 500)
U = 4 * ((1/r)**12 - (1/r)**6)

plt.figure()
plt.plot(r, U)
plt.axhline(0, color='k', lw=0.5)
plt.axvline(2**(1/6), color='r', lw=0.7, linestyle='--', label=r'$r^* = 2^{1/6}\sigma$')
plt.axvline(2.5, color='g', lw=0.7, linestyle='--', label=r'$r_{cut} = 2.5\sigma$')
plt.ylim(-2, 2)
plt.xlabel('r')
plt.ylabel('U(r)')
plt.title('Potentiel de Lennard-Jones')
plt.legend()
plt.savefig('./lab3-6/lennard_jones.png')
plt.show()