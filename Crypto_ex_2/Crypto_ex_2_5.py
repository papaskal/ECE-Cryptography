import random


def powerModulo(a, n, m):
    x = a % m
    y = 1
    while n > 0:
        if n % 2 != 0:
            y = (y * x) % m
        x = (x ** 2) % m
        n = n // 2
    return y



def fermatTest(p, a):
    if powerModulo(a, p-1, p) == 1: 
        return True
    return False


if __name__ == "__main__":
    numOfTests = 30
    numsToTest = [67280421310721, 170141183460469231731687303715884105721, (2**2281) - 1, (2**9941) - 1, (2**19939) - 1]
    k = -1
    for p in numsToTest:
        isPrime = True
        k += 1
        for i in range (0, numOfTests):
            a = random.randint(2, p-2)
            isPrime = fermatTest(p, a)
            if not isPrime:
                print("Number " + str(k) + " is not prime")
                break
        if isPrime:
            print("Number " + str(k) + " is (probably) prime")
        

#Number 0 is (probably) prime
#Number 1 is not prime
#Number 2 is (probably) prime
#Number 3 is (probably) prime
#Number 4 is not prime