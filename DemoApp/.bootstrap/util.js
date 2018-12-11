exports.inherits = function (ctor, superCtor) {
    if (ctor === undefined || ctor === null) {
        throw new TypeError('The constructor to "inherits" must not be ' +
                                                'null or undefined');
    }

    if (superCtor === undefined || superCtor === null) {
        throw new TypeError('The super constructor to "inherits" must not ' +
                                                'be null or undefined');
    }

    if (superCtor.prototype === undefined) {
        throw new TypeError('The super constructor to "inherits" must ' +
                                                'have a prototype');
    }

    ctor.super_ = superCtor;
    ctor.prototype = Object.create(superCtor.prototype, {
        constructor: {
            value: ctor,
            enumerable: false,
            writable: true,
            configurable: true
        }
    });
};

exports.mixin = function (Constructor, objects) {
    var prototype = Constructor.prototype;

    for (var i = 0; i < objects.length; i++) {
        var object = objects[i];
        var keys = Object.keys(object);

        for (var j = 0; j < keys.length; j++) {
            var key = keys[j];
            prototype[key] = object[key];
        }
    }
};
