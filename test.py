import numpy as np
import pyarome

beta0  =  1.68
Vsini  = 5.3
sigma0 =  4.0
zeta   =  2.0
Rp     =  0.1135
Kmax = 4

per = 2.516729
tc  = 2455818.40790 - 2454000
sma = 8.61
Rp = 0.1135
inc = 88.7
ecc = 0.028
omega = 269

K = 196.8/1e3

Vsini=5.0
zeta=2.0
lam = 0

(t,rv,rv_err) = np.loadtxt('RVs_PFS.dat',unpack=True)

res = pyarome.RM(lam,t,per,inc,ecc,omega,tc,sma,0.69,0.0,beta0,Vsini,sigma0,zeta,Kmax,Rp)

print(res)
