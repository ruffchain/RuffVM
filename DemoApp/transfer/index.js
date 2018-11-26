const BigNumber = require('bignumber.js');

function doTransfer(from, amount) {
    const accountA = '12nD5LgUnLZDbyncFnoFB43YxhSFsERcgQ';
    const accountB = '1LuwjNj8wkqo237N7Gh8nZSSvUa6TZ5ds4';

    var amountValue = new BigNumber(amount);
    let toAValue = amountValue.div(2).toString();
    let toBValue = amountValue.div(2).toString();


    transferAInHost(accountA, toAValue);
    transferBInHost(accountB, toBValue);
    return true;
}

global.doTransferInVm = doTransfer;
