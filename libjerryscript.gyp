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
                "jerry-all-in.c"
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
                        "-fno-stack-protector",
                        "-Wall",
                        "-Os",
                    ],
                    "xcode_settings": {
                        "MACOSX_DEPLOYMENT_TARGET": "10.13"
                    }
                },
                "OS=='linux'", {
                    "cflags": [
                        "-std=c99",
                        "-pedantic",
                        "-fno-builtin",
                        "-fno-stack-protector",
                        "-Wall",
                        "-Os",
                    ],
                }
                ]
            ],
        },
    ],

}
