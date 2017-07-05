import numpy as np
import pyarome
from pylab import plot,show
import ajplanet

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

phase = ((t-tc + 0.5*per) % per)/per
rv_pred_noRM = ajplanet.pl_rv_array(t-tc+res[2],0, K, omega * np.pi / 180.0, ecc, 0, per)
jitter_PFS = 5.8 # m/s
rv_err = np.sqrt(rv_err**2 + jitter_PFS**2 + 2.0**2) # in m/s. The 2 comes from the error in the zeropoint
rv0 = 35.0 # m/s
rv = rv 
rv /= 1e3
rv_err /= 1e3


plot(phase,rv_pred_noRM)
plot(phase,rv_pred_noRM + res[1])
plot(phase,rv,'.')
show()

