const assert = require('assert')
const { Script, createScript, resolveHelper } = require('../index.js')

function bufferToArrayBuffer(b) {
  return b.buffer.slice(b.byteOffset, b.byteOffset + b.byteLength)
}

describe('ruff vm basic', function() {
  describe('export', function() {
    it('should export Script', function() {
      assert(typeof Script === 'function')
    })
  })

  it('function createScript could creat a script', async () => {
    const script = createScript()
    assert(script instanceof Script)
  })

  describe('run', function() {
    it('should run simple script with empty context', async () => {
      const res = await createScript()
        .setUserCode('({ret: "HI"})')
        .runAsync()
      assert(res === '{"ret":"HI"}')
    })
  })

  it('should twice run simple script with empty context', async () => {
    const script = createScript()
    const res1 = await script.setUserCode('({ret: "HI"})').runAsync()
    assert(res1 === '{"ret":"HI"}')
    const res2 = await script.setUserCode('({ret: "HELLO"})').runAsync()
    assert(res2 === '{"ret":"HELLO"}')
  })

  it('should exit run infinite loop when set cpu limit', (done) => {
    createScript()
        .setUserCode('while(1){};')
        .setOption({ cpuCount: 200, memSizeKB: 256 })
        .runAsync()
        .then()
        .catch((err) => {
            assert.equal(err, 'Error: {"error": "Abort script"}', 'expected error in VM')
            done()
        })
  })
  it('should get value in script from context', async () => {
    var code = 'var ruff = {value: "hello"};'
    const res = await createScript(code)
      .setUserCode('({ "ret": ruff.value })')
      .runAsync()
    assert(res === '{"ret":"hello"}')
  })
})

describe('vm resolve', function() {
  it('should resolve with ArrayBuffer', async () => {
    let isTriggered = false

    function hostTest(vmResolve, param) {
      return new Promise(function(resolve) {
        isTriggered = true
        let ab1 = bufferToArrayBuffer(Buffer.from(`this is hostapi ${param}`))
        const u8 = new Uint8Array(ab1, 0, ab1.byteLength)
        setTimeout(() => {
          resolve(vmResolve(u8))
        }, 100)
      })
    }
    let apiTable = {
      test: hostTest
    }

    const res = await createScript()
      .setUserCode('test("test");')
      .setSandbox(apiTable)
      .runAsync()

    assert(isTriggered)
    let expectBuffer = bufferToArrayBuffer(Buffer.from('this is hostapi test'))
    let expectU8 = new Uint8Array(expectBuffer, 0, expectBuffer.byteLength)
    let retU8 = new Uint8Array(res, 0, res.byteLength)
    assert.deepStrictEqual(retU8, expectU8)
  })
})

describe('bridge VM and Host', function() {
  it('Host could get string from VM', async () => {
    let isTriggered = false
    function hostTest(vmResolve, param) {
      assert(param === 'hi from ruffvm')
      isTriggered = true
      return true
    }

    let apiTable = {
      test: hostTest
    }

    await createScript()
      .setUserCode('test("hi from ruffvm");')
      .setSandbox(apiTable)
      .runAsync()

    assert(isTriggered)
  })

  it('Host could get utf8 string from VM', async () => {
    let isTriggered = false
    function hostTest(vmResolve, param) {
      assert(param === 1)
      isTriggered = true
      return true
    }

    let apiTable = {
      test: hostTest
    }

    await createScript()
      .setUserCode('test(1);')
      .setSandbox(apiTable)
      .runAsync()

    assert(isTriggered)
  })

  it('Host could get number from VM', async () => {
    let isTriggered = false
    function hostTest(vmResolve, param) {
      assert(param === 2.5)
      isTriggered = true
    }

    let apiTable = {
      test: hostTest
    }

    await createScript()
      .setUserCode('test(2.5);')
      .setSandbox(apiTable)
      .runAsync()

    assert(isTriggered)
  })

  it('VM could get number from Host', async () => {
    let isTriggered = false
    function hostTest(vmResolve, param) {
      isTriggered = true
      return 3.6
    }

    let apiTable = {
      test: hostTest
    }

    await createScript()
      .setUserCode('var ret = test(); if (ret === 3.6) {ret} else {while(1);}')
      .setSandbox(apiTable)
      .runAsync()

    assert(isTriggered)
  })

  it('Host could get arraybuffer from VM', async () => {
    let isTriggered = false
    function hostTest(vmResolve, param) {
      isTriggered = true
      const u8 = new Uint8Array(param, 0, param.byteLength)
      assert(u8.length === 20)
      assert((u8[0] = 12 && u8[1] === 234))
    }

    let apiTable = {
      test: hostTest
    }

    const script = 'var buf = new Uint8Array(20); buf[0] = 12;buf[1]=234;test(buf.buffer);'
    await createScript()
      .setUserCode(script)
      .setSandbox(apiTable)
      .runAsync()

    assert(isTriggered)
  })

  it('Host could get Bool from VM', async () => {
    let script = 'false;'
    const res = await createScript()
      .setUserCode(script)
      .runAsync()
    assert(res === false)
  })

  it('VM could get int number from Host', async () => {
    let isTriggered = false
    function hostTest(vmResolve, param) {
      isTriggered = true
      return 3
    }

    let apiTable = {
      test: hostTest
    }
    await createScript()
      .setUserCode('var ret = test(); if (ret === 3) {ret} else {while(1);}')
      .setSandbox(apiTable)
      .runAsync()

    assert(isTriggered)
  })
})

describe('Context', function() {
  it('could define function in context', async () => {
    let isTriggered = false

    function apiFunction(resolve, param) {
      isTriggered = true
      const u8 = new Uint8Array(param, 0, param.byteLength)
      assert(u8.length === 20)
      assert((u8[0] = 1 && u8[1] === 2))
      return true
    }

    var code = `
      function helloFun(parameterString) {
        var buf = new Uint8Array(20)
        buf[0] = 1
        buf[1] = 2
        return hello(buf.buffer)
      }
    `

    var apiObject = {
      hello: apiFunction
    }

    const res = await createScript(code)
      .setUserCode('helloFun("ruffVM")')
      .setSandbox(apiObject)
      .setOption({ cpuCount: 1, memSizeKB: 200 })
      .runAsync()

    assert(isTriggered)
    assert(res === true)
  })
})

describe('vm resolve helper', function() {
  let apiObject = {
    apiResolveTest: hostResolveTest,
    apiResolveTimeoutTest: hostResolveTimeoutTest,
    apiConsole: hostConsole,
  };

  function hostResolveTest(vmResolve) {
    let vmResolver = resolveHelper(vmResolve, 100);
    return new Promise(function(resolve) {
      setTimeout(() => {
        resolve(vmResolver('hi ruffvm'));
      },10)
    });
  }

  function hostResolveTimeoutTest(vmResolve) {
    let vmResolver = resolveHelper(vmResolve, 40);
    return new Promise(function(resolve) {
      setTimeout(() => {
        resolve(vmResolver('hi ruffvm'));
      },50);
    });
  }

  function hostConsole(resolve, info) {
    console.log(info);
  }

  it('resolve helper should be trggered when timeout', async() => {
    let code = `
        ret = apiResolveTimeoutTest();
    `;
    try {
      const res = await createScript()
        .setUserCode(code)
        .setSandbox(apiObject)
        .setOption({ cpuCount: 256, memSizeKB: 200 })
        .runAsync();
    } catch (err) {
      assert.equal(err, 'Error: {"error": "Exception"}', 'expected error in vm');
    }
  });

  it('resolve helper should resolve expected value in Host', async() => {
    let code = `
        ret = apiResolveTest();
    `;
    const res = await createScript()
      .setUserCode(code)
      .setSandbox(apiObject)
      .setOption({ cpuCount: 256, memSizeKB: 200 })
      .runAsync();

    assert.equal(res, 'hi ruffvm', 'invalid resolved value from host');
  });
})
