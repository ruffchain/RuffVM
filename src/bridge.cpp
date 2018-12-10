#include <assert.h>
#include <iostream>
#include "bridge.h"

namespace bridge {

PacketType VMPacket::fromJerryError(jerry_value_t error_value) {
    jerry_value_t stack_str = jerry_create_string ((const jerry_char_t *) "stack");
    jerry_value_t backtrace_val = jerry_get_property (error_value, stack_str);
    jerry_release_value (stack_str);

    if (!jerry_value_is_error (backtrace_val) && jerry_value_is_array (backtrace_val)) {
        uint32_t length = jerry_get_array_length (backtrace_val);

        /* This length should be enough. */
        if (length > 32) {
            length = 32;
        }

        for (uint32_t i = 0; i < length; i++) {
            jerry_value_t item_val = jerry_get_property_by_index (backtrace_val, i);

            if (!jerry_value_is_error (item_val) && jerry_value_is_string (item_val)) {
                jerry_size_t str_size = jerry_get_string_size (item_val);

                if (str_size >= m_capacity) {
                    std::cout << "Backtrace string too long]\n" << std::endl;
                } else {
                    jerry_size_t string_end = jerry_string_to_char_buffer (item_val, m_pData, str_size);
                    assert (string_end == str_size);
                    m_pData[string_end] = 0;
                }
            }

            jerry_release_value (item_val);
        }
    }
    jerry_release_value (backtrace_val);

    jerry_value_t err_str_val = jerry_value_to_string (error_value);
    jerry_size_t err_str_size = jerry_get_string_size (err_str_val);

    if (err_str_size >= m_capacity) {
        const char msg[] = "[Error message too long]";
        err_str_size = sizeof (msg) / sizeof (char) - 1;
        memcpy (m_pData, msg, err_str_size + 1);
    } else {
        jerry_size_t string_end = jerry_string_to_char_buffer (err_str_val, m_pData, err_str_size);
        assert (string_end == err_str_size);
        m_pData[string_end] = 0;
        jerry_release_value (err_str_val);
    }
    return VMError;
}
PacketType VMPacket::fromJerryString(jerry_value_t value) {
    jerry_size_t req_sz;

    req_sz = jerry_get_utf8_string_size(value);
    if (req_sz > m_capacity) {
        //ToDo
        std::cout << "To handle this error" << std::endl;
        return PacketFailToParse;
    } else {
        jerry_string_to_utf8_char_buffer(value, (jerry_char_t*)m_pData, req_sz);
        m_length = req_sz;
        m_type = VMString;
    }
    return VMString;
}

PacketType VMPacket::from(jerry_value_t value) {
    jerry_size_t req_sz;
    jerry_value_t stringified;

    m_type = PacketFailToParse;
    m_length = 0;

    if (jerry_value_is_number(value)) {
        m_variantValue.number = jerry_get_number_value(value);
        m_type = VMNumber;
    } else if (jerry_value_is_boolean(value)) {
        m_variantValue.isTrue = jerry_get_boolean_value(value);
        m_type = VMBool;
    } else if (jerry_value_is_string(value)) {
        m_type = fromJerryString(value);
    } else if (jerry_value_is_arraybuffer(value)) {
        req_sz = jerry_get_arraybuffer_byte_length(value);
        if (req_sz > m_capacity) {
            //toDo
            std::cout << "To handle this error" << std::endl;
            goto out;
        }
        jerry_arraybuffer_read(value, 0, m_pData, req_sz);
        m_length = req_sz;
        m_type = VMArrayBuffer;
    } else if (jerry_value_is_object(value)) {
        stringified = jerry_json_stringfy (value);
        if (jerry_value_is_error(stringified)) {
            std::cout << "error when serializeData" << std::endl;
            goto out;
        }
        m_type = fromJerryString(stringified);
        jerry_release_value(stringified);
    } else if (jerry_value_is_error(value)) {
        stringified = jerry_get_value_from_error(value, false);
        m_type = fromJerryError(stringified);
        jerry_release_value(stringified);
    } else if (jerry_value_is_undefined(value)) {
        m_type = VMUndefined;
    } else {
        std::cout << "Unhandle type in packet from" << std::endl;
    }
out:
    return m_type;
}

PacketType VMPacket::from(v8::Local<v8::Value> value) {
    // use the caller's scope i.e v8::HandleScope scope;
    m_type = PacketFailToParse;
    m_length = 0;

    if (value->IsNumber()) {
        m_variantValue.number = value->NumberValue();
        m_type = HostNumber;
    } else if (value->IsBoolean()) {
        m_variantValue.isTrue = value->BooleanValue();
        m_type = HostBool;
    } else if (!value.IsEmpty() && value->IsString()) {
        v8::Local<v8::String> string = value->ToString();
        if (!string.IsEmpty()) {
            size_t len = 3 * string->Length() + 1;
            if (len > m_capacity) {
                //ToDo
                std::cout << "to handle this error" << std::endl;
                goto out;
            }
            const int flags = v8::String::NO_NULL_TERMINATION | v8::String::REPLACE_INVALID_UTF8;
            m_length = string->WriteUtf8((char*)m_pData, static_cast<int>(len), 0, flags);
            ((char*)m_pData)[m_length] = '\0';
            m_type = HostString;
        }
    } else if (value->IsArrayBufferView()) {
        auto view = v8::Local<v8::ArrayBufferView>::Cast(value);
        auto ab = view->Buffer();
        auto contents = ab->GetContents();
        if (view->ByteLength() > m_capacity) {
            //ToDo
            std::cout << "to handle this error" << std::endl;
            goto out;
        }
        auto srcData = reinterpret_cast<uint8_t*>(contents.Data()) + view->ByteOffset();
        memcpy((void*)m_pData, (const void*)srcData, view->ByteLength());
        m_length = view->ByteLength();
        m_type = HostArrayBufferView;
    } else if (value->IsUndefined()) {
        m_type = HostUndefined;
    } else {
        std::cout << "Unsupported type" << std::endl;
    }
out:
    return m_type;
}

jerry_value_t VMPacket::toJerryValue() const {
    jerry_value_t ret;

    if (m_type == HostNumber) {
        ret = jerry_create_number(m_variantValue.number);
    } else if (m_type == HostBool) {
        ret = jerry_create_boolean(m_variantValue.isTrue);
    } else if (m_type == HostString || m_type == HostError) {
        ret = jerry_create_string_from_utf8((const jerry_char_t*)m_pData);
    } else if (m_type == HostArrayBufferView) {
        ret = jerry_create_arraybuffer(m_length);
        jerry_arraybuffer_write(ret, 0, (const uint8_t*)m_pData, m_length);
    } else {
        ret = jerry_create_undefined();
    }

    return ret;
}

v8::Local<v8::Value> VMPacket::toV8Value(v8::Isolate *isolate) const {
    v8::Local<v8::Value> ret;

    if (m_type == bridge::VMArrayBuffer) {
        v8::Local<v8::ArrayBuffer> ab = v8::ArrayBuffer::New(isolate, m_length);
        memcpy(ab->GetContents().Data(), m_pData, m_length);
        return ab;
    }

    if (m_type == bridge::VMString) {
        ret = Nan::New((const char*)m_pData, m_length).ToLocalChecked();
    } else if (m_type == bridge::VMNumber) {
        ret = Nan::New(m_variantValue.number);
    } else if (m_type == bridge::VMBool) {
        ret = Nan::New(m_variantValue.isTrue);
    } else if (m_type == VMError) {
        ret = Nan::Error((const char*)m_pData);
    } else {
        ret = Nan::Undefined();
    }

    return ret;
}
}
