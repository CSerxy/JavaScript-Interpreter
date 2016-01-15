function Person(x) {
  this.money = x;
  this.work = function(gain) { this.money += gain; };
}

var a = new Person(100);
a.work(10);
print(a.money);