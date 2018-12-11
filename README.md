[![Build Status](https://travis-ci.com/ruffchain/RuffVM.svg?branch=master)](https://travis-ci.com/ruffchain/RuffVM)

# RuffVM

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

### Class: Script
#### new Script(code)
* `code` \<string> code to compile and evaluate in VM
* Returns: \<Script>
#### script.setUserCode(userCode)
* `userCode` \<string> script to run in VM after `code`
* Returns: \<Script>
#### script.setOption(options)
* `options`
    * `cpuCount` \<number> cpu count to limit
    * `memSizeKB` \<number> memory size in KB
* Returns: \<Script>
#### script.setSandbox(sandbox)
* `sandbox` \<Object>  will be the global object in vm
* Returns: \<Script>
#### script.runAsync()
* Returns: \<Promise>
  * `resolve` return value from script
  * `reject` error status (boolean)

### createScript(code)
* `code` \<string> code to compile and evaluate in VM
* Returns: \<Script>

**Example**

Host return no `Promised` value to VM


```javascript
const assert = require('assert')
const { createScript } = require('ruffvm')

const code = `
  function helloFun(parameterString) {
    var buf = new Uint8Array(20)
    buf[0] = 1
    buf[1] = 2
    return hello(buf.buffer)
  }`

let isTriggered = false
const sandbox = {
  hello: function(resolve, param) {
    isTriggered = true
    const u8 = new Uint8Array(param, 0, param.byteLength)
    assert(u8.length === 20)
    assert((u8[0] = 1 && u8[1] === 2))
    return true
  }
}

;(async () => {
  const res = await createScript(code)
    .setUserCode(`helloFun("ruffVM")`)
    .setSandbox(sandbox)
    .setOption({ cpuCount: 1, memSizeKB: 200 })
    .runAsync()
  assert(isTriggered)
  assert(res === true)
})()
```

Host return Resolved `Promised` value to VM


```javascript
const assert = require('assert')
const vm = require('ruffvm')

function bufferToArrayBuffer(b ){
  return b.buffer.slice(b.byteOffset, b.byteOffset + b.byteLength);
}

const code = `
  function helloFun(parameterString) {
    var buf = new Uint8Array(20)
    buf[0] = 1
    buf[1] = 2
    return hello(buf.buffer)
  }`

const sandbox = {
  hello: function(vmResolve, name) {
    return new Promise(function(resolve) {
      let ab1 = bufferToArrayBuffer(Buffer.from('this is hostapi test'))
      const u8 = new Uint8Array(ab1, 0, ab1.byteLength)
      setTimeout(() => {
        vmResolve(u8) // send resolved value to VM
        resolve()
      }, 20)
    })
  }
}

;(async () => {
  const res = await createScript(code)
    .setUserCode(`helloFun("ruffVM")`)
    .setSandbox(sandbox)
    .setOption({ cpuCount: 1, memSizeKB: 200 })
    .runAsync()

  const expectBuffer = bufferToArrayBuffer(Buffer.from('this is hostapi test'))
  const expectU8 = new Uint8Array(expectBuffer, 0, expectBuffer.byteLength)
  const resU8 = new Uint8Array(res, 0, res.byteLength)
  assert.deepStrictEqual(resU8, expectU8)
})()
```
For more example please refer to test/basic.test.js

## Function export to VM from Node.js
* function hostApi(vmResolve, parmFromVM)
  * vmResolve: resolve handler to resolve *Resolved* Value on Node.js, just ignore this handler if the return value is not **promise**
  * paramFromVM: parameter specified from VM

## Main usage

`ruffchain` use ruffvm as its smart contract execute engine, provide the ability for user develop their smart contract by JavaScript.

## Difference from Node's vm

Following code will escape node vm `sandbox` and do exit on host

```JavaScript
const vm = require('vm');
vm.runInNewContext('this.constructor.constructor("return process")().exit()');
console.log('Never gets executed.');
```

## Difference from duktape.node

ruffvm-node support set limit on CPU and Memory comparing to duktape.node

## To do list

* ~~Enable build on Linux~~
* ~~Add chain style API~~
* Enable build on Windows
* Add SharedArrayBuffer support (ES2017)
* Evaluate `XS` JavaScript Engine

## Known improvements
Does not support multiple instance of vm run simultaneously. Communication between VM and Host is not optimized for heavily usage scenario.
