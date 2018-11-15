'use strict'

/* global */
var global = new Function('return this;')();
Object.defineProperty(global, 'global', {
	value: global,
	writable: true,
	enumerable: false,
	configurable: true
});


