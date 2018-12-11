{

    "targets": [
        {
            "target_name": "ruff-vm",
            "dependencies": [
                './libjerryscript.gyp:jerryscript'
            ],
            "include_dirs": [
                "lib/jerryscript/jerry-core",
                "lib/jerryscript/jerry-core/ecma/",
                "lib/jerryscript/jerry-core/ecma/builtin-objects",
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
                "lib/jerryscript/jerry-core/jscontext",
                "lib/jerryscript/jerry-core/api",
                "lib/jerryscript/jerry-core/vm",
                "lib/jerryscript/jerry-port/default/include/",
                "lib/jerryscript/jerry-ext/include/",
                "lib/PolyM",
                "lib/incbin",
                "src",
                "<!(node -e \"require('nan')\")"
            ],
            "sources": [
                "src/ruffvm_main.cpp",
                "src/ruffvm.cpp",
                "src/snapshot.cpp",
                "src/callbackcache.cpp",
                "src/run_async.cpp",
                "src/bridge.cpp",
                "lib/PolyM/Msg.cpp",
                "lib/PolyM/Queue.cpp"
            ],
            "conditions": [
                ["OS=='mac'", {
                    "cflags_cc": [
                        "-O2",
                        "-std=c++1y",
                        "-Weffc++",
                        "-pedantic",
                        "-Wall",
                        "-fstrict-aliasing",
                        "-fomit-frame-pointer",
                    ],
                    "libraries": [
                        "-L/usr/local/lib"
                    ],
                    "xcode_settings": {
                        "MACOSX_DEPLOYMENT_TARGET": "10.13",
                        'OTHER_CFLAGS':[
                            '-std=c++1y',
                            '-stdlib=libc++'
                        ]
                    }
                },
                "OS=='linux'", {
                    "cflags_cc": [
                        "-O2",
                        "-std=c++1y",
                        "-pedantic",
                        "-Wall",
                        "-fstrict-aliasing",
                        "-fomit-frame-pointer"
                    ]
                }]
            ],
        },
    ],
}
