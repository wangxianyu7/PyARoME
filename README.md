# PyARoME
Python interface to the ARoME library (http://www.astro.up.pt/resources/arome/). The ARoME library is described in the following paper: https://ui.adsabs.harvard.edu/#abs/2013A&A...550A..53B/abstract


Installation
------------

### Install ARoME from http://www.astro.up.pt/resources/arome/
```bash
wget http://www.astro.up.pt/resources/arome/files/arome-1.0.0.tar.gz
tar zxvf arome-1.0.0.tar.gz
cd arome-1.0.0
#execute the following statements to install your library in the default directory  /usr/local 
./configure --disable-shared
make 
make check
make install

```

### compile the module simply do:

```python setup.py install```

