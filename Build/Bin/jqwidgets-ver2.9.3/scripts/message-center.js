function MessageCenter() {

};


MessageCenter.prototype.processMsg = function (data) {
    sendMessageToCpp(data);
};