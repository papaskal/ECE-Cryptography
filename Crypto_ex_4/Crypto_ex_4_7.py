# This code provides an example of the Schnorr signature scheme
# using a Schnorr group
#
# In particular, we select a safe prime P, meaning P = 2Q + 1, 
# where Q is prime. 
# We work on a subgroup of order Q of the group (mod P).
#
# I should point out that I use a safe prime for the sake of simplicity.
# In reality, P and Q can be replaced with any primes so long that
# P = rQ + 1, where r is any integer
#
# Since the exercise is done for educational purposes, we use relatively small primes P, Q


import random, hashlib

P = 1000000007         
Q = (P-1)//2

def powerModulo(a, n, m):
    x = a % m
    y = 1
    while n > 0:
        if n % 2 != 0:
            y = (y * x) % m
        x = (x ** 2) % m
        n = n // 2
    return y

def groupGen():
    r = (P-1) // Q
    g = random.randint(2,P-1)
    while powerModulo(g, r, P) == 1:
        g = random.randint(2,P)
    return g

def publicKey(x):
    return powerModulo(G, x, P)

def signMsg(m, x):
    k = random.randint(2, Q-1)
    r = powerModulo(G, k, P)
    binR = str(bin(r))[2:]
    h = hashlib.sha256()
    h.update(binR.encode("utf8"))
    h.update(m.encode("utf8"))
    e = int(h.hexdigest(), 16) % (P-1)
    s = (k - x*e) % (P-1)
    return (s, e)

def verify(m, y, s, e):
    rv = (powerModulo(G, s, P) * powerModulo(y, e, P)) % P
    binRv = str(bin(rv))[2:]
    hv = hashlib.sha256()
    hv.update(binRv.encode("utf8"))
    hv.update(m.encode("utf8"))
    ev = int(hv.hexdigest(), 16) % (P-1)
    if ev == e:
        return True
    return False

if __name__ == "__main__":
    G = groupGen()
    x = random.randint(2, Q)
    y = publicKey(x)
    print("g =", G, "x =", x, "y =", y)
    m = "This is my message\n"
    
    (s, e) = signMsg(m, x)
    if verify(m, y, s, e):
        print("Verification successful")
    else:
        print("Verification failed")
    