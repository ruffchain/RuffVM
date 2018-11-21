const assert = require('assert')
const { createScript } = require('../../../index.js')
const BigNumber = require('bignumber.js')

describe('Script Test', function() {
  it('could ction in context', async () => {
    let isTriggered = false
    const sandbox = {
      helloInHost: function(resolve, param) {
        isTriggered = true
        const number = BigNumber(param)
        const expect = BigNumber(123.4567)
        assert(number.isEqualTo(expect))
        return true
      }
    }

    const code = require('fs').readFileSync('dist/index.js', 'utf-8')
    const res = await createScript(code)
      .setUserCode(`helloInVm("hi");`)
      .setSandbox(sandbox)
      .setOption({ cpuCount: 1, memSizeKB: 200 })
      .runAsync()

    assert(isTriggered)
    assert(res === true)
  })
})
