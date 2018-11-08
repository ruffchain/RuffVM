const BigNumber = require('bignumber.js');

var x = new BigNumber(123.4567);
var y = BigNumber('123456.7e-3');
var z = new BigNumber(x);
console.log(x.isEqualTo(y) && y.isEqualTo(z) && x.isEqualTo(z));
console.log(z);

function hello() {
    console.log('hello world', z.toString());
    return helloInHost(z.toString());
}

global.helloInVm = hello;
