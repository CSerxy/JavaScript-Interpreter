var Father = {
 value : function() { return this.x + this.y; }
};

var a = { prototype: Father, x: 1, y: 2 };
var b = new Father(); 
b.x = 2;
b.y = 3;

print(a.value());
print(b.value());