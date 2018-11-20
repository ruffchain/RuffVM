var bindings = require('bindings')('ruff-vm.node')

module.exports.run = bindings.run
const vm = require('bindings')('ruff-vm.node')

function _stringToU8Buf(str) {
  let b = Buffer.from(str)
  b = b.buffer.slice(b.byteOffset, b.byteOffset + b.byteLength)
  const U8Buf = new Uint8Array(b, 0, b.byteLength)
  return U8Buf
}

let isVmBusy = false
class Script {
  constructor(code = null) {
    this._script = code ? _stringToU8Buf(`${code}`) : null
    this._option = null
    this._sandbox = null
    this._userCode = null
  }

  setSandbox(sandbox) {
    this._sandbox = sandbox
    return this
  }

  setOption(option) {
    this._option = option
    return this
  }

  setUserCode(str) {
    this._userCode = str
    return this
  }

  runAsync() {
    if (isVmBusy) {
      throw new Error('vm is busy now')
    }
    isVmBusy = true

    const { _userCode, _script, _sandbox, _option } = this
    return new Promise((resolve, reject) => {
      vm.run(_userCode, _script, _sandbox, _option, (err, res) => {
        setImmediate(() => {
          err ? reject(err) : resolve(res)
          isVmBusy = false
        })
      })
    })
  }
}

function createScript(code) {
  return new Script(code)
}

module.exports = {
  run: vm.run,
  Script,
  createScript
}
