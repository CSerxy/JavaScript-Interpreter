var a = [];
a[0] = 1;
a[1] = 2;

b = a;
b[0] = -1;

print(a[0]);
print(a[1]);

result = 0;
for (i = 0; i < 2; i++) result = result + b[i];
print(result);

