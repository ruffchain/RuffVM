'use strict'

function Console () {
}

Console.prototype.log = function () {
  for (var i = 0; i < arguments.length; i++) {
    if (typeof arguments[i] === 'object' && typeof arguments[i].inspect === 'function') {
      arguments[i] = arguments[i].inspect()
    }
  }

  print.apply(this, arguments)
}

Console.prototype.error = Console.prototype.error

Console.prototype.debug = Console.prototype.error

global.console = new Console()
