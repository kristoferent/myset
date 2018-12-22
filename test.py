import random
f = open("text.txt", "w+")
n = 100000;
f.write(str(n))
f.write('\n')
for _ in range (n):
    x = random.randint(1, 100000)
    f.write(str(x))
    f.write(' ');
f.write('\n')
f.write(str(n))
f.write('\n')
for _ in range (n):
    x = random.randint(1, 100000)
    f.write(str(x))
    f.write(' ')
f.close()
