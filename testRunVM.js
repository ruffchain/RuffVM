const vm = require('./index.js');
const fs = require('fs');
function bufferToArrayBuffer(b ){
    return b.buffer.slice(b.byteOffset, b.byteOffset + b.byteLength);
}

let jsonCode = fs.readFileSync('test/jsonTest.txt').toString();
code = jsonCode;
const contextAB = bufferToArrayBuffer(Buffer.from(jsonCode));
const contextU8Buf = new Uint8Array(contextAB, 0, contextAB.byteLength);

var loopCode=
    `var num =0;
     while(1) {
         var buf = new Uint8Array(1024);
         buf[num] = new  Uint8Array(1024);
         num += 1;
         apiTest();
        // print(buf);
    }`;
function hostTest() {
    console.log('in hosttest');
    return 1;
}

let resolveCode = `
    apiConsole('hi i am in');
    ret = apiResolveTimeoutTest();
    apiConsole(ret);
`;

let throwCode = `
    apiThrowTest();
`;
function resolveHelper(resolve) {
    let resolved = false;
    let timeOut = setTimeout(() => {
        //resolve(1);
        resolved = true;
        resolve(new Error());
    }, 100);
    return (value) => {
        if (!resolved) {
            clearTimeout(timeOut);
            resolve(value);
            resolved = true;
        }
    };
}
function hostResolveTest(vmResolve) {
    let vmResolver = vm.resolveHelper(vmResolve);
    return new Promise(function(resolve) {
        setTimeout(() => {
            resolve(vmResolver('hi ruffvm'));
        },10)
    });
}

function hostResolveTimeoutTest(vmResolve) {
    let vmResolver = vm.resolveHelper(vmResolve);
    return new Promise(function(resolve) {
        setTimeout(() => {
            resolve(vmResolver('hi ruffvm'));
        },200);
    });
}

function hostConsole(resolve, info) {
    console.log(info);
}

function hostThrowTest(resolve, info) {
    console.log('in hostThrowTest');
    throw new Error('test');
}
let apiTable = {
    apiResolveTest: hostResolveTest,
    apiResolveTimeoutTest: hostResolveTimeoutTest,
    apiTest: hostTest,
    apiConsole: hostConsole,
    apiThrowTest: hostThrowTest,
};

vm.run(throwCode, null, apiTable, { cpuCount: 256, memSizeKB: 256}, (err, ret) => {
    if (err) {
        console.log('err is', err);
    }
});

