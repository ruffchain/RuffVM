const assert = require('assert');
const vm = require('../index.js');
const fs = require('fs');

function bufferToArrayBuffer(b ){
    return b.buffer.slice(b.byteOffset, b.byteOffset + b.byteLength);
}

describe('ruff vm basic', function() {
  describe('export', function() {
    it('should export run and resolve', function() {
        var exportProperty = Object.getOwnPropertyNames(vm);
        assert(typeof(vm.run) === 'function');
    });
  });

  describe('run', function() {
    it('should run simple script with empty context', (done) => {
        vm.run('({ret: "HI"})', null, null, null, (err, ret) => {
            assert(err === null);
            done(assert(ret === '{"ret":"HI"}'));
        });
    });

    it('should twice run simple script with empty context', (done) => {
        vm.run('({ret: "HI"})', null, null, null, (err, ret) => {
            assert(err === null);
            assert(ret === '{"ret":"HI"}');
            setImmediate(() => {
                vm.run('({ret: "HELLO"})', null, null, null, (err, ret) => {
                    assert(err === null);
                    done(assert(ret === '{"ret":"HELLO"}'));
                });
            });
        });
    });

    it('should not crash when do JSON', (done) => {
        let jsonCode = fs.readFileSync('test/jsonTest.txt').toString();
        code = jsonCode;
        const contextAB = bufferToArrayBuffer(Buffer.from(jsonCode));
        const contextU8Buf = new Uint8Array(contextAB, 0, contextAB.byteLength);

        vm.run(null, contextU8Buf, null, null, (err, ret) => {
            console.log(err);
            if (err) {
                done();
            }
        });
    });

    it('should not crash when do Array', (done) => {

        let code = `
            function a (n) {
              var array = new Int32Array(n)
              var array2 = new Int32Array(array)
              array2.set(new Uint8Array(array.buffer, 34))
            }
            a(10)
        `;
        vm.run(code, null, null, null, (err, ret) => {
            done(assert(err===null));
        });

    });

    it('should exit run infinite loop when set cpu limit', (done) => {

        function hostTest() {
            console.log('in hosttest');
            return 1;
        }
        let apiTable = {
            apiTest: hostTest
        };
        vm.run(`var num =0;
                while(1) {
                    var buf = new Uint8Array(1024);
                    buf[num] = new  Uint8Array(1024);
                    num += 1;
                    apiTest();
                    //print(buf);
                }`,
            null, apiTable, { cpuCount: 1000, memSizeKB: 256}, (err, ret) => {
            done(assert.equal(err, 'Error: {"error": "Abort script"}', 'expected error in VM'));
        });
    });

    it('should get value in script from context', (done) => {
        var context = 'var ruff = {value: "hello"};';
        var ab = bufferToArrayBuffer(Buffer.from(context))
        const u8Buffer = new Uint8Array(ab, 0, ab.byteLength);

        vm.run('({ "ret": ruff.value })', u8Buffer, null, null, (err, ret) => {
            done(assert(ret === '{"ret":"hello"}'));
        })
    });
  });

  describe('vm resolve', function() {
    it('should resolve with ArrayBuffer', (done) => {
        let isTriggered = false;

        function hostTest(vmResolve, param) {
            return new Promise(function(resolve){
                isTriggered = true;
                let ab1 = bufferToArrayBuffer(Buffer.from(`this is hostapi ${param}`));
                const u8 = new Uint8Array(ab1, 0, ab1.byteLength);
                setTimeout(() => {
                    resolve(vmResolve(u8));
                }, 100);
            });
        }
        let apiTable = {
            test: hostTest
        };

        vm.run('test("test");', null, apiTable, null, (err, ret) => {
            assert(!err);
            assert(isTriggered);
            var expectBuffer = bufferToArrayBuffer(Buffer.from('this is hostapi test'));
            var expectU8 = new Uint8Array(expectBuffer, 0, expectBuffer.byteLength);
            var retU8 = new Uint8Array(ret, 0, ret.byteLength);
            done(assert.deepStrictEqual(retU8, expectU8));
        });
    });
  });

  describe('bridge VM and Host', function() {

    it('Host could get string from VM', (done) => {
        let isTriggered = false;
        function hostTest(vmResolve, param) {
            assert(param === 'hi from ruffvm');
            isTriggered = true;
            return true;
        }

        let apiTable = {
            test: hostTest
        };

        vm.run('test("hi from ruffvm");', null, apiTable, null, (err, ret) => {
            assert(!err);
            assert(isTriggered);
            done();
        });
    });

    it('Host could get utf8 string from VM', (done) => {
        let isTriggered = false;
        function hostTest(vmResolve, param) {
            assert(param === 1);
            isTriggered = true;
            return true;
        }

        let apiTable = {
            test: hostTest
        };

        vm.run('test(1);', null, apiTable, null, (err, ret) => {
            assert(!err);
            assert(isTriggered);
            done();
        });
    });

    it('Host could get number from VM', (done) => {
        let isTriggered = false;
        function hostTest(vmResolve, param) {
            assert(param === 2.5);
            isTriggered = true;
        }

        let apiTable = {
            test: hostTest
        };

        vm.run('test(2.5);', null, apiTable, null, (err, ret) => {
            assert(!err);
            assert(isTriggered);
            done();
        });
    });

    it('VM could get number from Host', (done) => {
        let isTriggered = false;
        function hostTest(vmResolve, param) {
            isTriggered = true;
            return 3.6;
        }

        let apiTable = {
            test: hostTest
        };

        vm.run('var ret = test(); if (ret === 3.6) {ret} else {while(1);}', null, apiTable, null, (err, ret) => {
            assert(!err);
            assert(isTriggered);
            done();
        });
    });


    it('Host could get arraybuffer from VM', (done) => {
        let isTriggered = false;
        function hostTest(vmResolve, param) {
            isTriggered = true;
            const u8 = new Uint8Array(param, 0, param.byteLength);
            assert(u8.length === 20);
            assert(u8[0] = 12 && u8[1] === 234);
        }

        let apiTable = {
            test: hostTest
        };

        let script = 'var buf = new Uint8Array(20); buf[0] = 12;buf[1]=234;test(buf.buffer);'
        vm.run(script, null, apiTable, null, (err, ret) => {
            assert(!err);
            assert(isTriggered);
            done();
        });
    });

    it('Host could get Bool from VM', (done) => {
        let isTriggered = false;

        let script = 'false;'
        vm.run(script, null, null, null, (err, ret) => {
            assert(!err);
            assert(ret === false);
            done();
        });
    });

    it('VM could get int number from Host', (done) => {
        let isTriggered = false;
        function hostTest(vmResolve, param) {
            isTriggered = true;
            return 3;
        }

        let apiTable = {
            test: hostTest
        };

        vm.run('var ret = test(); if (ret === 3) {ret} else {while(1);}', null, apiTable, null, (err, ret) => {
            assert(!err);
            assert(isTriggered);
            done();
        });
    });
  });

  describe('Context', function() {
      it ('could define function in context', (done) => {
        let isTriggered = false;

        function apiFunction(resolve, param) {
            isTriggered = true;
            const u8 = new Uint8Array(param, 0, param.byteLength);
            assert(u8.length === 20);
            assert(u8[0] = 1 && u8[1] === 2);
            return true;
        }
        var contextStr = " \
        function helloFun(parameterString) {  \
          var buf = new Uint8Array(20);        \
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
            done(assert(err === null && ret === true));
        });
    });

      it ('could pass more than 1 param to Host API', (done) => {
          let isTriggered = false;

          function apiFunction(resolve, param1, param2) {
              isTriggered = true;
              const u8 = new Uint8Array(param1, 0, param1.byteLength);
              assert(u8.length === 20);
              assert(u8[0] = 1 && u8[1] === 2);

              assert(param2 === 1234);
              return true;
          }
          var contextStr =`
            function helloFun(parameterString) {
              var buf = new Uint8Array(20);
              buf[0] = 1;
              buf[1] = 2;
              return hello(buf.buffer, 1234);
            }
        `;
          var apiObject = {
              hello: apiFunction
          };

          const contextAB = bufferToArrayBuffer(Buffer.from(contextStr));
          const contextU8Buf = new Uint8Array(contextAB, 0, contextAB.byteLength);
          vm.run('helloFun("ruffVM")', contextU8Buf, apiObject,  {cpuCount:1, memSizeKB:200}, (err, ret) => {
              assert(isTriggered);
              done(assert(err === null && ret === true));
          });
      });

  });
});
