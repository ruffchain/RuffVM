{
     "targets": [
        {
            "target_name": "jerryscript",
            "product_prefix": "lib",
            "type": "static_library",
            "include_dirs": [
                "lib/jerryscript/jerry-core",
                "lib/jerryscript/jerry-core/ecma/",
                "lib/jerryscript/jerry-core/ecma/operations",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray",
                "lib/jerryscript/jerry-core/ecma/base/",
                "lib/jerryscript/jerry-core/jrt",
                "lib/jerryscript/jerry-core/jmem",
                "lib/jerryscript/jerry-core/include/",
                "lib/jerryscript/jerry-core/lit",
                "lib/jerryscript/jerry-core/debugger",
                "lib/jerryscript/jerry-core/profiles",
                "lib/jerryscript/jerry-core/parser",
                "lib/jerryscript/jerry-core/parser/regexp",
                "lib/jerryscript/jerry-core/parser/js",
                "lib/jerryscript/jerry-core/jcontext",
                "lib/jerryscript/jerry-core/api",
                "lib/jerryscript/jerry-core/vm",
                "lib/jerryscript/jerry-core/jcontext",
                "lib/jerryscript/jerry-port/default/include/",
                "lib/jerryscript/jerry-ext/include/",
            ],
            "sources": [
                "lib/jerryscript/jerry-core/api/jerry-debugger-transport.c",
                "lib/jerryscript/jerry-core/api/jerry-debugger.c",
                "lib/jerryscript/jerry-core/api/jerry-snapshot.c",
                "lib/jerryscript/jerry-core/api/jerry.c",
                "lib/jerryscript/jerry-core/debugger/debugger.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-alloc.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-gc.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers-conversion.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers-errol.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers-external-pointers.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers-number.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers-string.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers-value.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers-values-collection.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-helpers.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-init-finalize.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-lcache.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-literal-storage.c",
                "lib/jerryscript/jerry-core/ecma/base/ecma-property-hashmap.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-boolean-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-boolean.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-date-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-date.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-error-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-error.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-function-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-function.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-global.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-date.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-error.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-json.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-sort.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-json.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-map-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-map.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-math.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-number-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-number.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-object-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-object.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-promise-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-promise.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-regexp-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-regexp.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-string-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-string.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-type-error-thrower.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-urierror-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-urierror.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtins.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-helpers.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray-prototype.c",
                "lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-array-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-arraybuffer-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-boolean-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-comparison.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-conversion.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-eval.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-exceptions.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-function-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-get-put-value.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-jobqueue.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-lex-env.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-map-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-number-arithmetic.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-number-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-objects-arguments.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-objects-general.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-objects.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-promise-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-reference.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-regexp-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-string-object.c",
                "lib/jerryscript/jerry-core/ecma/operations/ecma-typedarray-object.c",
                "lib/jerryscript/jerry-core/jcontext/jcontext.c",
                "lib/jerryscript/jerry-core/jmem/jmem-allocator.c",
                "lib/jerryscript/jerry-core/jmem/jmem-heap.c",
                "lib/jerryscript/jerry-core/jmem/jmem-poolman.c",
                "lib/jerryscript/jerry-core/jrt/jrt-fatals.c",
                "lib/jerryscript/jerry-core/lit/lit-char-helpers.c",
                "lib/jerryscript/jerry-core/lit/lit-magic-strings.c",
                "lib/jerryscript/jerry-core/lit/lit-strings.c",
                "lib/jerryscript/jerry-core/parser/js/byte-code.c",
                "lib/jerryscript/jerry-core/parser/js/common.c",
                "lib/jerryscript/jerry-core/parser/js/js-lexer.c",
                "lib/jerryscript/jerry-core/parser/js/js-parser-expr.c",
                "lib/jerryscript/jerry-core/parser/js/js-parser-mem.c",
                "lib/jerryscript/jerry-core/parser/js/js-parser-scanner.c",
                "lib/jerryscript/jerry-core/parser/js/js-parser-statm.c",
                "lib/jerryscript/jerry-core/parser/js/js-parser-util.c",
                "lib/jerryscript/jerry-core/parser/js/js-parser.c",
                "lib/jerryscript/jerry-core/parser/regexp/re-bytecode.c",
                "lib/jerryscript/jerry-core/parser/regexp/re-compiler.c",
                "lib/jerryscript/jerry-core/parser/regexp/re-parser.c",
                "lib/jerryscript/jerry-core/vm/opcodes-ecma-arithmetics.c",
                "lib/jerryscript/jerry-core/vm/opcodes-ecma-bitwise.c",
                "lib/jerryscript/jerry-core/vm/opcodes-ecma-relational-equality.c",
                "lib/jerryscript/jerry-core/vm/opcodes.c",
                "lib/jerryscript/jerry-core/vm/vm-stack.c",
                "lib/jerryscript/jerry-core/vm/vm-utils.c",
                "lib/jerryscript/jerry-core/vm/vm.c",
                "lib/jerryscript/jerry-ext/handler/handler-print.c",
                "lib/jerryscript/jerry-ext/handler/handler-assert.c",
                "lib/jerryscript/jerry-port/default/default-external-context.c",
                "lib/jerryscript/jerry-ext/handler/handler-register.c",
                "lib/jerryscript/jerry-port/default/defaultx-handler.c",
            ],
            "defines": [
                "CONFIG_MEM_HEAP_AREA_SIZE=131072",
                "JERRY_ENABLE_EXTERNAL_CONTEXT",
                "JERRY_NDEBUG",
                "JERRY_VM_EXEC_STOP",
            ],
            "conditions": [
                ["OS=='mac'", {
                    "cflags": [
                        "-std=c99",
                        "-pedantic",
                        "-fno-builtin",
                        "-fno-stack-protector"
                        "-Wall",
                        "-Os",
                    ],
                    "xcode_settings": {
                        "MACOSX_DEPLOYMENT_TARGET": "10.13"
                    }
                }]
            ],
        },
    ],

}
