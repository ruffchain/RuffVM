#include "incbin.h"
#include "jerryscript.h"
#include <iostream>

namespace ruffvm {

 INCBIN(Prelude, "prelude.js");
 INCBIN_EXTERN(Prelude);

int snapshotLoad() {
    int ret = 0;

    jerry_value_t scriptRet = jerry_eval((const jerry_char_t*)gPreludeData,
                                         gPreludeSize,
                                         JERRY_PARSE_STRICT_MODE);
    if (jerry_value_is_error(scriptRet)) {
        std::cout << "Failed to to Prelude" << std::endl;
        ret = -1;
    }
    jerry_release_value(scriptRet);
    return ret;
}

}
