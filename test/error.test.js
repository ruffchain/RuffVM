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
})
