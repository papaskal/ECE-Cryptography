def powerModulo(a, n, m):
    x = a % m
    y = 1
    while n > 0:
        if n % 2 != 0:
            y = (y * x) % m
        x = (x ** 2) % m
        n = n // 2
    return y


def euler(k):
    if k % 5 == 0:
        return (k//5) * 2
    elif k % 2 == 0:
        return k//2
    else:
        return 0


def upUp(a, b, k):
    if b == 0 :
        return 1
    if k == 1:
        return 0
    f = euler(k)
    return powerModulo(a, upUp(a, b-1, f), k)


if __name__ == "__main__":
    a, b = 1707, 1783
    print(upUp(a, b, 10**16))
    
