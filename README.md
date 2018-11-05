RuffVM
==========

RuffVM is a light-weight VM environment designed for DApps(Decentralized applications). It provides secure, isolated execution environment as well as resource control (e.g Memory and CPU) for Dapps. It lowers the barrier of Blockchain application development by significant amount via JavaScript based abstraction (But not limited to JS).

RuffVM's build-in,plug-in mechanism allows developers to customize DApp runtime depends on their need, these modules will be completely independent to a public chain base and will be compatible with any Blockchain platforms(e.g EOS,Ethereum,ruffchain). 

## RoadMap
### Present
* 0.1 Implementation on Node.js

### In progress
* Build-in modules
* Standerlize modules build-in, plug-in mechanism

### Future release
* Verification and optimization on ruffchain blockchain
* Split out ruffvm-core into separate repo
* Port to EOS
* Port to Ethereum
* DApp Module registry

RuffVM-Node
=========

RuffVM integration for node.js, aim to provide usable, secure sandbox in order to run untrusted javascript code in nodejs,
Resource control abilities supported in this version (e.g. cpu time and memory).
RuffVM leverage jerryscript(a lightweight JavaScript engine) as javascript runtime, it is isolated with Node V8 engine naturally.
Refer some idea from [duktape.node](https://github.com/ndob/duktape-node).

## Building manually and running tests
> Currently enabled build on Macos only

build
```
git clone --recurse-submodules https://github.com/ruffchain/RuffVM.git
cd RuffVM
npm install
```

build manually
```
node-gyp configure
node-gyp build
```

## how to test
```shell
npm run test
```

## data type supported between VM and Host by `bridge`
* Undefined
* Boolean
* Number
* String
* ArrayBuffer

## API

### run(script, context, apiTable, resourceCtrl, callback)
**Description**

Runs a single script on ruffvm and returns error status and return value to callback.

**Parameters**

* script: a script to run in `context`
* context: to compile and evaluate `script` in
* apiTable: API table for script and context
  * properties:
    * key: name for function
    * value: function to call
* resourceCtrl: resource control params
  * properties:
    * cpuTime: cpu count to limit
    * memSizeKB: memory size in KB
* callback: function with signature `function(error, returnValue)`
  * error: error status (boolean)
  * returnValue: return value from script or error in case of an error

**Example**

Host return no `Promised` value to VM


```javascript

    const assert = require('assert');
    const vm = require('ruffvm');

    let isTriggered = false;
    function bufferToArrayBuffer(b ){
        return b.buffer.slice(b.byteOffset, b.byteOffset + b.byteLength);
    }

    function apiFunction(resolve, param) {
        isTriggered = true;
        const u8 = new Uint8Array(param, 0, param.byteLength);
        assert(u8.length === 20);
        assert(u8[0] = 1 && u8[1] === 2);
        return true;
    }
    var contextStr = " \
    function helloFun(parameterString) {  \
      var buf = new Uint8Array(20);       \
      buf[0] = 1;                         \
      buf[1] = 2;                         \
      return hello(buf.buffer);           \
    }                                     \
    ";
    var apiObject = {
        hello: apiFunction
    };

    const contextAB = bufferToArrayBuffer(Buffer.from(contextStr));
    const contextU8Buf = new Uint8Array(contextAB, 0, contextAB.byteLength);
    vm.run('helloFun("ruffVM")', contextU8Buf, apiObject,  {cpuCount:1, memSizeKB:200}, (err, ret) => {
        assert(isTriggered);
        assert(err === false && ret === true);
    });

```

Host return Resolved `Promised` value to VM


```javascript

    const assert = require('assert');
    const vm = require('ruffvm');

    var apiFunction = function(vmResolve, name) {
        var ret = new Promise(function(resolve){
            let ab1 = bufferToArrayBuffer(Buffer.from('this is hostapi test'));
            const u8 = new Uint8Array(ab1, 0, ab1.byteLength);
            setTimeout(() => {
                vmResolve(u8); // send resolved value to VM
                resolve();
            }, 20);
        });

        return ret;
    }

    var contextStr = " \
    function helloFun(parameterString) {  \
      var buf = new Uint8Array(20);       \
      buf[0] = 1;                         \
      buf[1] = 2;                         \
      return hello(buf.buffer);           \
    }                                     \
    ";
    var apiObject = {
        hello: apiFunction
    };

    const contextAB = bufferToArrayBuffer(Buffer.from(contextStr));
    const contextU8Buf = new Uint8Array(contextAB, 0, contextAB.byteLength);
    vm.run('helloFun("ruffVM")', contextU8Buf, apiObject,  {cpuCount:1, memSizeKB:200}, (err, ret) => {
        assert(isTriggered);
        assert(err === false);
        var expectBuffer = bufferToArrayBuffer(Buffer.from('this is hostapi test'));
        var expectU8 = new Uint8Array(expectBuffer, 0, expectBuffer.byteLength);
        var retU8 = new Uint8Array(ret, 0, ret.byteLength);
        assert.deepStrictEqual(retU8, expectU8);
    });

```
For more example please refer to test/basic.test.js

## Function export to VM from Node.js
* function hostApi(vmResolve, parmFromVM)
  * vmResolve: resolve handle to resolve *Resolved* Value on Node.js, just ignore this handle if the return value is not **promise**
  * paramFromVM: parameter specified from VM

## Who is using this project

`ruffchain` use ruffvm as its smart contract execute engine, provide the ability for user develop their smart contract by JavaScript.

## What is the difference between Node's vm and ruffvm-node

Following code will escape node vm `sandbox` and do exit on host

```JavaScript
const vm = require('vm');
vm.runInNewContext('this.constructor.constructor("return process")().exit()');
console.log('Never gets executed.');
```

## what is the different between duktape.node and ruffvm-node

ruffvm-node support set both CPU and Memory limitation whereas duktape.node do not support

## ToDo

* Enable build on Linux and Windows
* Add node.js `vm` module compatible API
* Add SharedArrayBuffer support (ES2017)
* Evaluate `XS` JavaScript Engine

## Known limitation
Does not support multiple instance of vm run simultaneously. Communication between VM and Host is not optimized for heavily usage scenario.
