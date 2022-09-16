import random

RADIUS = 127.5
DADIUS = 2 * RADIUS
SADIUS = RADIUS * RADIUS
TOTAL = 256 * 256    
circle = {}

def powerModulo(a, n, m):
    x = a % m
    y = 1
    while n > 0:
        if n % 2 != 0:
            y = (y * x) % m
        x = (x ** 2) % m
        n = n // 2
    return y


def calcInside() :
    dict = {}
    cnt = 0

    for i in range (0, 256):
        for j in range (0, 256):
            if i*i + j*j + SADIUS <= DADIUS * (i+j):
                dict[(i,j)] = True
                cnt += 1
            else:
                dict[(i,j)] = False
    
    print(cnt, (cnt * 4)/TOTAL)
    return dict


def calcS(s, n, lambdaN, i):
    pow = powerModulo(2, i, lambdaN)
    return powerModulo(s, pow, n)
        

def circPoint(k, x, y, ex, succ, approx):
    global circle
    cnt = k % 16

    if cnt < 8:
        #x = (x << 1) + ex
        x = x*2 + ex
    else:
        #y = (y << 1) + ex
        y = y*2 + ex
        if (cnt == 15):
        #cnt = -1
            if circle[(x,y)] == True:
                succ += 1
            x = 0
            y = 0
            #approx = 4 * succ/(k//16)

    #cnt += 1

    return (x, y, succ, approx)


def bbs(p, q, seed):
    global circle
    k = cnt = x = y = succ = approx = 0
    n = p*q
    print(n)
    
    seed %= n

    s = seed
    print(circle[217,199])

    while True:
        ex = s % 2
        #print(ex)
        k += 1
        s = (s*s) % n

        (x, y, succ, approx) = circPoint(k, x, y, ex, succ, approx)
       
        
        '''
        if cnt < 8:
            x = x * 2 + ex
        elif cnt < 16:
            y = y * 2 + ex
        else:
            cnt = -1
            if circle[(x,y)] == True:
                succ += 1
            x = y = 0
            approx = 4 * succ/(k//16)
        cnt += 1
        '''
        
        #if (k % 1000000 == 0):
        #    print(k, approx, succ)
        #if k < 10: print(s)
        if s == seed: break
        if (k == 10000000): break
    
    print(k, s, succ)
    '''
    print(k, s)
    l = ((p-1)*(q-1))//4
    print(l)
    print(powerModulo(2, k, l))
    '''

def run():
    p = 600239  #600011 #383
    q = 600983  #503
    n = p * q
    seed = 20749
    s1 = seed*seed % n
    ln = 180366116858
    lln = 45091228910

    global circle 
    circle = calcInside()
  
    if s1 == calcS(s1, n, ln, lln):
        print("period:", lln)
    
    print(calcS(seed, n, ln, 1000000000))

    bbs(p,q,seed)


if __name__ == "__main__":
    run()
    


#n       = 360733434937
#l(n)    = 180366116858
#l(l(n)) = 45091228910
