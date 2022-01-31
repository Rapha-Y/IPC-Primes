from random import random

n = 1000
a = []
b = []

i = 0
while i < n:
    while True:
        x = (int)(random() * 10000)
        # print(x)
        if x not in a:
            i+=1
            a.append(x)
            break
    

i = 0
while i < n:
    while True:
        x = (int)(random() * 10000)
        if x not in b:
            i+=1
            b.append(x)
            break
    
print(n)
for x in a:
    print(x)
for x in b:
    print(x)
