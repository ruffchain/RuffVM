const BigNumber = require('bignumber.js');
const assert = require('assert');

function doTransfer(from, amount) {
    const accountA = '12nD5LgUnLZDbyncFnoFB43YxhSFsERcgQ';
    const accountB = '1LuwjNj8wkqo237N7Gh8nZSSvUa6TZ5ds4';

    assert.equal(this.getReceiver(), '1EYLLvMtXGeiBJ7AZ6KJRP2BdAQ2Bof79', 'invalid _receiver');
    var amountValue = new BigNumber(amount);
    let toAValue = amountValue.div(2).toString();
    let toBValue = amountValue.div(2).toString();

    transferAInHost(accountA, toAValue);
    transferBInHost(accountB, toBValue);
    return true;
}

function Contract(receiver) {
    var that = this;

    that._receiver = receiver;
}

Contract.prototype.getReceiver = function() {
    return this._receiver;
}

Contract.prototype.doTransfer = doTransfer;
global.Contract = Contract;
