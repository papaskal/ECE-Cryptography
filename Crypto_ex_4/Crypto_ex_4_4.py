# This code tests the loc attack on an RSA encryption system
#
# We generate 100 random messages (in the form of an integer <N), 
# we encrypt them and then attack the resulting ciphertexts 
# using an oracle for the loc function, whereas loc(c) = (decrypt(c) > N/2)
#
# Since the exercise is done for educational purposes, we use relatively small primes P, Q (around 200 bits each)
# In particular we use the RSA-120 semiprime for N = P * Q
# One can replace P, Q with any prime and the resulting code will work (albeit maybe take a lot longer) 


import random, math

P = 327414555693498015751146303749141488063642403240171463406883         
Q = 693342667110830181197325401899700641361965863127336680673013
N = P*Q                     
fN = (P-1)*(Q-1)            
E = 0           
D = 0        


def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m


def powerModulo(a, n, m):
    x = a % m
    y = 1
    while n > 0:
        if n % 2 != 0:
            y = (y * x) % m
        x = (x ** 2) % m
        n = n // 2
    return y


def rsaEnc(m):
    return powerModulo(m, E, N)

def rsaDec(c):
    return powerModulo(c, D, N)

def loc(c):
    if rsaDec(c) > N/2:
        return True
    return False


def locAttack(c, d=10000):
    a = 0
    b = N-1
    ctemp = c
    
    while (b - a > d):                  #Binary search with loc. Stop when you are close enough to the target
        if loc(ctemp):                  #(binary search with loc has issues when the interval gets too small)
            a = (a + b)//2
        else:
            b = (a + b)//2 -1
        ctemp *= rsaEnc(2)

    ae = (a + b)//2
    be = (a + b)//2
    while (1):                      #Once you are close enough to the target, search serially with enc
        for m in range (a, ae):   
            if rsaEnc(m) == c:
                return m
        for m in range (be, b+1): 
            if rsaEnc(m) == c:
                return m
        ae = a
        be = b
        a -= d                      #If we don't find the right plaintext, that means the interval was too narrow
        b += d                      #so we increase its size
    

if __name__ == "__main__":
    print("N =", N)
    E = 2
    while (math.gcd(E,fN) != 1):
        E = random.randint(3, fN-1)
    D = modinv(E, fN)
    print("E =", E)
    print("D =", D)

    for i in range (0,100):                     #Test the attack on 100 random messages
        m = random.randint(0, N-1)      
        c = rsaEnc(m)
        res = locAttack(c)
        if (res == m):                          #Due to the nature of the locAttack function, the result will always be successful,
            print("Test", i+1, ":\tAttack successful: ", res)   #but some cases might take longer than others
        else:
            print("Test", i+1, ":\tAttack failed: ", m)        
    
    
