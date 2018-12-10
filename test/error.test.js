const assert = require('assert');
const { Script, createScript } = require('../index.js')

describe('Context', function() {
  it('could throw error in context', (done) => {
      createScript()
      .setUserCode('throw "error"')
      .setOption({ cpuCount: 1, memSizeKB: 200 })
      .runAsync()
      .then()
      .catch((err) => {
        assert.equal(err, 'Error: error', 'VM should throw error')
        done()
        })
    })
  it('could throw error of UTF-8 in context', (done) => {
      createScript()
      .setUserCode('throw "错误"')
      .setOption({ cpuCount: 1, memSizeKB: 200 })
      .runAsync()
      .then()
      .catch((err) => {
        done(assert.equal(err, 'Error: 错误', 'VM should throw error'))
        })
    })

})
