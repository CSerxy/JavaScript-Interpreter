var a = 1;
var b = [];
b[0] = 2;

function foo(myarray) { myarray[0]++; }

function bar(myvalue) { myvalue++; }

foo(b);
bar(a);
print(a);
print(b[0]);
