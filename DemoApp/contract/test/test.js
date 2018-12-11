const assert = require('assert')
const { createScript } = require('../../../index.js')
const BigNumber = require('bignumber.js')

describe('Contract Test', function() {
  it('could load and run contract', async () => {
    let isATriggered = false
    let isBTriggered = false
    const sandbox = {
      transferAInHost: function(vmResolve, to, value) {
        assert(to ==  '12nD5LgUnLZDbyncFnoFB43YxhSFsERcgQ')
        const number = BigNumber(value)
        const expect = BigNumber(123.4)
        assert(number.isEqualTo(expect))
        isATriggered = true
        return true
      },

      transferBInHost: function(vmResolve, to, value) {
        assert(to ==  '1LuwjNj8wkqo237N7Gh8nZSSvUa6TZ5ds4')
        const number = BigNumber(value)
        const expect = BigNumber(123.4)
        assert(number.isEqualTo(expect))
        isBTriggered = true
        return true
      }
    }

    let transferValueStr = BigNumber(246.8).toString()
    let method = 'doTransfer'
    const code = require('fs').readFileSync('dist/index.js', 'utf-8')
    const res = await createScript(code)
      .setUserCode(
        `var contract = new Contract("1EYLLvMtXGeiBJ7AZ6KJRP2BdAQ2Bof79");
        contract.${method}("1EYLLvMtXGeiBJ7AZ6KJRP2BdAQ2Bof79", ${transferValueStr});
        `)
      .setSandbox(sandbox)
      .setOption({ cpuCount: 1, memSizeKB: 200 })
      .runAsync()

    assert(isATriggered && isBTriggered)
    assert(res === true)
  })

  it('The contract method should throw error with invalid account for contruct', async ()=> {
    let transferValueStr = BigNumber(246.8).toString()
    let method = 'doTransfer'
    const code = require('fs').readFileSync('dist/index.js', 'utf-8')
    const sandbox = {}
    try {
        const res = await createScript(code)
          .setUserCode(
            `var contract = new Contract("1EYLLvMtXGeiBJ7AZ6KJRP2BdAQ2Bof71");
            contract.${method}("1EYLLvMtXGeiBJ7AZ6KJRP2BdAQ2Bof79", ${transferValueStr});
            `)
          .setSandbox(sandbox)
          .setOption({ cpuCount: 1, memSizeKB: 200 })
          .runAsync()
    } catch(err) {
        assert.equal(err, 'Error: AssertionError: invalid _receiver', 'invalid vm assert message')
    }
  });
})
