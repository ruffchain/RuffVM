#include "./lib/jerryscript/jerry-core/api/jerry-debugger-transport.c"
#include "./lib/jerryscript/jerry-core/api/jerry-debugger.c"
#include "./lib/jerryscript/jerry-core/api/jerry-snapshot.c"
#include "./lib/jerryscript/jerry-core/api/jerry.c"
#include "./lib/jerryscript/jerry-core/debugger/debugger.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-alloc.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-gc.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers-conversion.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers-errol.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers-external-pointers.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers-number.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers-string.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers-value.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers-values-collection.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-helpers.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-init-finalize.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-lcache.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-literal-storage.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-module.c"
#include "./lib/jerryscript/jerry-core/ecma/base/ecma-property-hashmap.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array-iterator-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-arraybuffer.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-boolean-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-boolean.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-dataview-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-dataview.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-date-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-date.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-error-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-error.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-evalerror.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-function-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-function.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-global.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-date.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-error.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-json.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers-sort.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-helpers.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-iterator-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-json.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-map-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-map.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-math.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-number-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-number.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-object-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-object.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-promise-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-promise.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-rangeerror.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-referenceerror.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-regexp-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-regexp.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-set-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-set.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-string-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-string.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-symbol-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-symbol.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-syntaxerror.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-type-error-thrower.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-typeerror.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-urierror-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtin-urierror.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/ecma-builtins.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float32array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-float64array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int16array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int32array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-int8array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-helpers.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-typedarray.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint16array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint32array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8array.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray-prototype.c"
#include "./lib/jerryscript/jerry-core/ecma/builtin-objects/typedarray/ecma-builtin-uint8clampedarray.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-array-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-arraybuffer-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-boolean-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-comparison.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-container-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-conversion.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-dataview-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-eval.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-exceptions.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-function-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-get-put-value.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-iterator-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-jobqueue.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-lex-env.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-number-arithmetic.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-number-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-objects-arguments.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-objects-general.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-objects.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-promise-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-reference.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-regexp-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-string-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-symbol-object.c"
#include "./lib/jerryscript/jerry-core/ecma/operations/ecma-typedarray-object.c"
#include "./lib/jerryscript/jerry-core/jcontext/jcontext.c"
#include "./lib/jerryscript/jerry-core/jmem/jmem-allocator.c"
#include "./lib/jerryscript/jerry-core/jmem/jmem-heap.c"
#include "./lib/jerryscript/jerry-core/jmem/jmem-poolman.c"
#include "./lib/jerryscript/jerry-core/jrt/jrt-fatals.c"
#include "./lib/jerryscript/jerry-core/lit/lit-char-helpers.c"
#include "./lib/jerryscript/jerry-core/lit/lit-magic-strings.c"
#include "./lib/jerryscript/jerry-core/lit/lit-strings.c"
#include "./lib/jerryscript/jerry-core/parser/js/byte-code.c"
#include "./lib/jerryscript/jerry-core/parser/js/common.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-lexer.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-parser-expr.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-parser-mem.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-parser-module.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-parser-scanner.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-parser-statm.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-parser-util.c"
#include "./lib/jerryscript/jerry-core/parser/js/js-parser.c"
#include "./lib/jerryscript/jerry-core/parser/regexp/re-bytecode.c"
#include "./lib/jerryscript/jerry-core/parser/regexp/re-compiler.c"
#include "./lib/jerryscript/jerry-core/parser/regexp/re-parser.c"
#include "./lib/jerryscript/jerry-core/vm/opcodes-ecma-arithmetics.c"
#include "./lib/jerryscript/jerry-core/vm/opcodes-ecma-bitwise.c"
#include "./lib/jerryscript/jerry-core/vm/opcodes-ecma-relational-equality.c"
#include "./lib/jerryscript/jerry-core/vm/opcodes.c"
#include "./lib/jerryscript/jerry-core/vm/vm-stack.c"
#include "./lib/jerryscript/jerry-core/vm/vm-utils.c"
#include "./lib/jerryscript/jerry-core/vm/vm.c"
#include "./lib/jerryscript/jerry-ext/handler/handler-print.c"
#include "./lib/jerryscript/jerry-ext/handler/handler-register.c"
#include "./lib/jerryscript/jerry-ext/handler/handler-assert.c"
#include "./lib/jerryscript/jerry-port/default/default-external-context.c"
#include "./lib/jerryscript/jerry-port/default/default-io.c"
#include "./lib/jerryscript/jerry-port/default/default-fatal.c"
