a = "foo bar stuff";
print(a);
r = Math.rand();
print(r);

parsed = Integer.parseInt("42");
print(parsed);

a_Str = "ABCD";
aChar = a_Str.charAt(0);
print(aChar);

obj1 = new Object();
obj1.food = "cake";
obj1.desert = "pie";
print(obj1.food+obj1.desert);

obj2 = obj1.clone();
obj2.food = "kittens";
print(obj2.food);

a = "a1bv1d1hfg1ewrt1sadfg1";
b = a.split('1');
for (i=0;i<b.length;i++) print(b[i]);