// function-closure
var a = 40;

function closure() {
  var a = 39;
  return function() { return a*10; };
}

var b = closure(); 
print(b());
