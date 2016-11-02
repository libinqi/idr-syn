var addon = require('../bin/SynIDCard.node');
var synIDCard = new addon.SynIDCard();

exports.readCard = function (port) {
    port = port || 1001;
    var result = synIDCard.ReadCard(port);
    if (result) {
        return true;
    }
    return false;
};

exports.startRealReadCard = function (port, realCallback, errorCallback) {
    port = port || 1001;
    var result = synIDCard.StartRealReadCard(port, realCallback, errorCallback);
    if (result) {
        return true;
    }
    return false;
};

exports.stopRealReadCard = function () {
    var result = synIDCard.StopRealReadCard();
    if (result) {
        return true;
    }
    return false;
};