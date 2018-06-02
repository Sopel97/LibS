#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <string>

namespace ls
{
    namespace json
    {
        struct Value
        {
        private:
            static constexpr bool useUnorderedMap = false;

        public:
            using Array = std::vector<Value>;
            using Object = typename std::conditional_t<useUnorderedMap, std::unordered_map<std::string, Value>, std::map<std::string, Value, std::less<>>>;

            Value() : m_valueType(Type::Empty) {}
            Value(const Value& other) : m_valueType(other.m_valueType)
            {
                if (other.m_value != nullptr)
                {
                    m_value = std::make_unique<ValueUnion>(*other.m_value);
                }
                else
                {
                    m_value = nullptr;
                }
            }
            Value(Value&&) noexcept = default;
            Value& operator=(const Value& other)
            {
                if (other.m_value != nullptr)
                {
                    m_value = std::make_unique<ValueUnion>(*other.m_value);
                }
                else
                {
                    m_value = nullptr;
                }
                m_valueType = other.m_valueType;

                return *this;
            }
            Value& operator=(Value&&) noexcept = default;
            explicit Value(const char* str) : m_value(std::make_unique<ValueUnion>(std::string(str))), m_valueType(Type::String) {}
            explicit Value(const std::string& str) : m_value(std::make_unique<ValueUnion>(str)), m_valueType(Type::String) {}
            explicit Value(std::string&& str) : m_value(std::make_unique<ValueUnion>(std::move(str))), m_valueType(Type::String) {}
            explicit Value(double d) : m_value(std::make_unique<ValueUnion>(d)), m_valueType(Type::Float) {}
            explicit Value(int64_t i) : m_value(std::make_unique<ValueUnion>(i)), m_valueType(Type::Int) {}
            explicit Value(Object&& obj) : m_value(std::make_unique<ValueUnion>(std::move(obj))), m_valueType(Type::Object) {}
            explicit Value(Array&& arr) : m_value(std::make_unique<ValueUnion>(std::move(arr))), m_valueType(Type::Array) {}
            explicit Value(bool b) : m_value(std::make_unique<ValueUnion>(static_cast<int64_t>(b))), m_valueType(Type::Bool) {}
            explicit Value(std::nullptr_t) : m_valueType(Type::Null) {}

            bool exists() const { return m_valueType != Type::Empty; }
            bool isString() const { return m_valueType == Type::String; }
            bool isNumber() const { return m_valueType == Type::Float || m_valueType == Type::Int; }
            bool isFloat() const { return m_valueType == Type::Float; }
            bool isInt() const { return m_valueType == Type::Int; }
            bool isObject() const { return m_valueType == Type::Object; }
            bool isArray() const { return m_valueType == Type::Array; }
            bool isBool() const { return m_valueType == Type::Bool; }
            bool isNull() const { return m_valueType == Type::Null; }

            bool isEmpty() const
            {
                if (isArray()) return getArray().size() == 0;
                if (isObject()) return getObject().size() == 0;
                else return false;
            }

            void setString(const std::string& str)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(str);
                m_valueType = Type::String;
            }
            void setString(std::string&& str)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(std::move(str));
                m_valueType = Type::String;
            }
            void setDouble(double d)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(d);
                m_valueType = Type::Float;
            }
            void setInt(int64_t i)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(i);
                m_valueType = Type::Int;
            }
            void setObject(const Object& obj)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(obj);
                m_valueType = Type::Object;
            }
            void setObject(Object&& obj)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(std::move(obj));
                m_valueType = Type::Object;
            }
            void setArray(const Array& arr)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(arr);
                m_valueType = Type::Array;
            }
            void setArray(Array&& arr)
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = std::make_unique<ValueUnion>(std::move(arr));
                m_valueType = Type::Array;
            }
            void setNull()
            {
                if (!exists()) onAttemptToModifyInexistingValue();
                m_value = nullptr;
                m_valueType = Type::Null;
            }

            Value& addMember(std::string key, Value val)
            {
                if (!isObject()) throw std::runtime_error("Value is not an object");
                auto& parent = getObject();
                return parent.try_emplace(std::move(key), std::move(val)).first->second;
            }
            Value& addMember(std::string key, Array arr)
            {
                if (!isObject()) throw std::runtime_error("Value is not an object");
                auto& parent = getObject();
                return parent.try_emplace(std::move(key), std::move(arr)).first->second;
            }
            Value& addMember(std::string key, Object obj)
            {
                if (!isObject()) throw std::runtime_error("Value is not an object");
                auto& parent = getObject();
                return parent.try_emplace(std::move(key), std::move(obj)).first->second;
            }
            Value& addValue(Value val)
            {
                if (!isArray()) throw std::runtime_error("Value is not an array");
                auto& parent = getArray();
                parent.emplace_back(std::move(val));
                return parent.back();
            }
            Value& addValue(Array arr)
            {
                if (!isArray()) throw std::runtime_error("Value is not an array");
                auto& parent = getArray();
                parent.emplace_back(std::move(arr));
                return parent.back();
            }
            Value& addValue(Object obj)
            {
                if (!isArray()) throw std::runtime_error("Value is not an array");
                auto& parent = getArray();
                parent.emplace_back(std::move(obj));
                return parent.back();
            }

            const std::string& getString() const { return m_value->getString(); }
            const std::string& getStringOr(const std::string& def) const
            {
                if (exists())
                {
                    return getString();
                }
                else
                {
                    return def;
                }
            }
            int64_t getInt() const { return m_value->getInt(); }
            int64_t getIntOr(int64_t def) const
            {
                if (exists())
                {
                    return getInt();
                }
                else
                {
                    return def;
                }
            }
            bool getBool() const { return static_cast<bool>(m_value->getInt()); }
            bool getBoolOr(bool def) const
            {
                if (exists())
                {
                    return getBool();
                }
                else
                {
                    return def;
                }
            }
            double getDouble() const
            {
                if (isFloat()) return m_value->getDouble();
                return static_cast<double>(m_value->getInt());
            }
            double getDoubleOr(double def) const
            {
                if (exists())
                {
                    return getDouble();
                }
                else
                {
                    return def;
                }
            }

            const Object& getObject() const { return m_value->getObject(); }
            Object& getObject() { return m_value->getObject(); }
            const Array& getArray() const { return m_value->getArray(); }
            Array& getArray() { return m_value->getArray(); }

            Value& operator[](int i)
            {
                if (isArray()) return getArray().at(i);
                else throw std::runtime_error("Value is not an array");
            }
            const Value& operator[](int i) const
            {
                if (isArray()) return getArray().at(i);
                else throw std::runtime_error("Value is not an array");
            }

            Value& operator[](const char* str)
            {
                if (isObject())
                {
                    auto& obj = getObject();
                    const auto iter = obj.find(str);
                    if (iter == obj.end()) return emptyValue();

                    return iter->second;
                }
                else throw std::runtime_error("Value is not an object");
            }
            const Value& operator[](const char* str) const
            {
                if (isObject())
                {
                    const auto& obj = getObject();
                    const auto iter = obj.find(str);
                    if (iter == obj.end()) return emptyValue();

                    return iter->second;
                }
                else throw std::runtime_error("Value is not an object");
            }

            Value& operator[](const std::string& str)
            {
                return operator[](str.c_str());
            }
            const Value& operator[](const std::string& str) const
            {
                return operator[](str.c_str());
            }

            size_t size() const
            {
                if (isArray()) return getArray().size();
                else throw std::runtime_error("Value is not an array");
            }

        private:
            struct ValueUnion
            {
            public:
                ValueUnion(const ValueUnion& other) :
                    m_valueType(Type::Empty)
                {
                    switch (other.m_valueType)
                    {
                    case Type::String:
                        initString(other.m_value.s);
                        break;
                    case Type::Double:
                        initDouble(other.m_value.d);
                        break;
                    case Type::Int:
                        initInt(other.m_value.i);
                        break;
                    case Type::Object:
                        initObject(other.m_value.obj);
                        break;
                    case Type::Array:
                        initArray(other.m_value.arr);
                        break;
                    default:
                        break;
                    }
                }

                ValueUnion(ValueUnion&& other) :
                    m_valueType(Type::Empty)
                {
                    switch (other.m_valueType)
                    {
                    case Type::String:
                        initString(std::move(other.m_value.s));
                        break;
                    case Type::Double:
                        initDouble(other.m_value.d);
                        break;
                    case Type::Int:
                        initInt(other.m_value.i);
                        break;
                    case Type::Object:
                        initObject(std::move(other.m_value.obj));
                        break;
                    case Type::Array:
                        initArray(std::move(other.m_value.arr));
                        break;
                    default:
                        break;
                    }

                    other.m_valueType = Type::Empty;
                }
                ValueUnion(const std::string& str)
                {
                    initString(str);
                }
                ValueUnion(std::string&& str)
                {
                    initString(std::move(str));
                }
                ValueUnion(double d)
                {
                    initDouble(d);
                }
                ValueUnion(int64_t i)
                {
                    initInt(i);
                }
                ValueUnion(const Object& obj)
                {
                    initObject(obj);
                }
                ValueUnion(Object&& obj)
                {
                    initObject(std::move(obj));
                }
                ValueUnion(const Array& arr)
                {
                    initArray(arr);
                }
                ValueUnion(Array&& arr)
                {
                    initArray(std::move(arr));
                }
                ~ValueUnion()
                {
                    destroy();
                }

                const std::string& getString() const
                {
                    return m_value.s;
                }
                std::string& getString()
                {
                    return m_value.s;
                }
                double getDouble() const
                {
                    return m_value.d;
                }
                int64_t getInt() const
                {
                    return m_value.i;
                }
                const Object& getObject() const
                {
                    return m_value.obj;
                }
                Object& getObject()
                {
                    return m_value.obj;
                }
                const Array& getArray() const
                {
                    return m_value.arr;
                }
                Array& getArray()
                {
                    return m_value.arr;
                }

            private:
                enum struct Type
                {
                    Empty,
                    String,
                    Double,
                    Int,
                    Object,
                    Array
                };

                union U
                {
                    std::string s;
                    double d;
                    int64_t i;
                    Object obj;
                    Array arr;

                    U() {}
                    ~U() {}
                } m_value;

                Type m_valueType;

                void destroy()
                {
                    if (m_valueType == Type::Empty) return;

                    switch (m_valueType)
                    {
                    case Type::String:
                        (&m_value.s)->std::string::~string();
                        break;
                    case Type::Object:
                        (&m_value.obj)->Object::~Object();
                        break;
                    case Type::Array:
                        (&m_value.arr)->Array::~Array();
                        break;
                    default:
                        break;
                    }

                    m_valueType = Type::Empty;
                }

                void initString(const std::string& str)
                {
                    new (&m_value.s) std::string(str);
                    m_valueType = Type::String;
                }
                void initString(std::string& str)
                {
                    new (&m_value.s) std::string(std::move(str));
                    m_valueType = Type::String;
                }
                void initDouble(double d)
                {
                    new (&m_value.d) double(d);
                    m_valueType = Type::Double;
                }
                void initInt(int64_t i)
                {
                    new (&m_value.i) int64_t(i);
                    m_valueType = Type::Int;
                }
                void initObject(const Object& obj)
                {
                    new (&m_value.obj) Object(obj);
                    m_valueType = Type::Object;
                }
                void initObject(Object&& obj)
                {
                    new (&m_value.obj) Object(std::move(obj));
                    m_valueType = Type::Object;
                }
                void initArray(const Array& arr)
                {
                    new (&m_value.arr) Array(arr);
                    m_valueType = Type::Array;
                }
                void initArray(Array&& arr)
                {
                    new (&m_value.arr) Array(std::move(arr));
                    m_valueType = Type::Array;
                }
            };

            enum struct Type
            {
                Empty,
                String,
                Float,
                Int,
                Object,
                Array,
                Bool,
                Null
            };

            std::unique_ptr<ValueUnion> m_value;
            Type m_valueType;

            static Value& emptyValue()
            {
                static Value empty{};
                return empty;
            }

            [[noreturn]] void onAttemptToModifyInexistingValue() const
            {
                throw std::runtime_error("Cannot modify inexisting value");
            }
        };
    }
}