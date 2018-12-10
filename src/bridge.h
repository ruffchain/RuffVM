#pragma once
#include <iostream>
#include <nan.h>
#include "jerryscript.h"

namespace bridge {
    enum VMErrorType {
        InvalidScript
    };

    enum HostErrorType {
        HostException
    };

    enum PacketType {
        PacketFailToParse,
        VMError,
        VMString,
        VMOject,
        VMArrayBuffer,
        VMNumber,
        VMBool,
        VMUndefined,
        HostString,
        HostNumber,
        HostArrayBufferView,
        HostError,
        HostBool,
        HostUndefined
    };

    class VMPacket {
    public:
        VMPacket():
            m_pData(m_data), m_capacity(sizeof(m_data)), m_length(0),
            m_toDelete(false), m_type(PacketFailToParse) {
        };

        VMPacket(uint8_t* buffer, size_t length, bool toDelete=false):
            m_pData(buffer), m_capacity(length), m_length(0), m_toDelete(toDelete), m_type(PacketFailToParse) {
        };

        PacketType type() const {
            return m_type;
        }

        void* data() const {
            return m_pData;
        }

        PacketType from(VMErrorType errCode) {
            m_type = VMError;
            (void) errCode;
            m_length = sprintf((char*)m_pData, "{\"error\": \"InvalidScript\"}");
            return m_type;
        }

        PacketType from(HostErrorType errCode) {
            m_type = HostError;
            (void) errCode;
            m_length = sprintf((char*)m_pData, "{\"error\": \"Exception\"}");
            return m_type;
        }

        PacketType from(jerry_value_t value);
        PacketType from(v8::Local<v8::Value> value);

        jerry_value_t toJerryValue() const;

        v8::Local<v8::Value> toV8Value(v8::Isolate *isolate) const;

        size_t length() const {
            return m_length;
        };

        double getNumValue() const {
            return m_variantValue.number;
        };

        ~VMPacket() {
            if (m_pData != m_data && m_toDelete) {
                delete m_pData;
            }
        }
    private:
        PacketType fromJerryError(jerry_value_t error_value);
        PacketType fromJerryString(jerry_value_t value);

        uint8_t* m_pData;
        uint8_t m_data[1024];
        size_t m_capacity;
        size_t m_length;
        bool m_toDelete;
        PacketType m_type;
        union {
            double number;
            bool  isTrue;
        }m_variantValue;
    };

    typedef std::vector<bridge::VMPacket*> VMPackets;
}
