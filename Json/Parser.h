#pragma once

#include "Value.h"

#include <cctype>
#include <algorithm>
#include <string>
#include <utility>

namespace ls
{
    namespace json
    {
        namespace detail
        {
            inline bool isWhitespace(char c)
            {
                return std::isspace(static_cast<unsigned char>(c));
            }
            inline bool isControl(char c)
            {
                return std::iscntrl(static_cast<unsigned char>(c));
            }
            inline bool isDigit(char c)
            {
                return std::isdigit(static_cast<unsigned char>(c));
            }

            struct InputStream
            {
            public:
                struct Location
                {
                    int line;
                    int character;
                };

                InputStream(const std::string& str) :
                    m_str(str),
                    m_ptr(m_str.begin())
                {

                }
                InputStream(std::string&& str) :
                    m_str(std::move(str)),
                    m_ptr(m_str.begin())
                {

                }

                char current() const
                {
                    return *m_ptr;
                }

                void advance(size_t n = 1)
                {
                    m_ptr += std::min(n, static_cast<size_t>(m_str.end() - m_ptr));
                }

                void eatWhitespaces()
                {
                    while (m_ptr != m_str.end() && isWhitespace(*m_ptr))
                    {
                        ++m_ptr;
                    }
                }

                bool isOnEnd()
                {
                    return m_ptr == m_str.end();
                }

                double readDouble()
                {
                    const char* begin = &*m_ptr;
                    char* end;
                    const double result = strtod(begin, &end);
                    const size_t length = end - begin;
                    advance(length);

                    return result;
                }

                int64_t readInt64()
                {
                    const char* begin = &*m_ptr;
                    char* end;
                    const int64_t result = static_cast<int64_t>(strtoll(begin, &end, 10));
                    const size_t length = end - begin;
                    advance(length);

                    return result;
                }

                std::string readString()
                {
                    std::string result;

                    advance(); // '\"'
                    while (m_ptr != m_str.end())
                    {
                        const char currentChar = *m_ptr;
                        advance();
                        char outputChar = currentChar;

                        if (iscntrl(currentChar))
                        {
                            throw std::runtime_error("No control characters allowed inside strings");
                        }

                        if (currentChar == '\"')
                        {
                            break;
                        }

                        if (currentChar == '\\')
                        {
                            const char nextChar = *m_ptr;
                            advance();
                            switch (nextChar)
                            {
                            case '\"': outputChar = '\"'; break;
                            case '\\': outputChar = '\\'; break;
                            case 'b': outputChar = '\b'; break;
                            case 'f': outputChar = '\f'; break;
                            case 'n': outputChar = '\n'; break;
                            case 'r': outputChar = '\r'; break;
                            case 't': outputChar = '\t'; break;
                            case '/': outputChar = '/'; break;
                            default: throw std::runtime_error("Invalid escape sequence");
                            }
                        }

                        result += outputChar;
                    }

                    return result;
                }

                bool readBool()
                {
                    const char currentChar = *m_ptr;
                    switch (currentChar)
                    {
                    case 't':
                        advance(4);
                        return true;
                    case 'f':
                        advance(5);
                        return false;
                    default:
                        throw std::runtime_error("Invalid boolean constant");
                    }
                }

                void readNull()
                {
                    advance(4);
                }

                Location currentLocation() const
                {
                    int line = 1;
                    int character = 1;
                    for (auto iter = m_str.begin(); iter != m_ptr; ++iter)
                    {
                        if (*iter == '\n')
                        {
                            ++line;
                            character = 1;
                        }
                        else ++character;
                    }
                    return { line, character };
                }

            private:
                std::string m_str;
                std::string::iterator m_ptr;
            };
        }

        struct DocumentParser
        {
        public:
            struct ParsingError : public std::runtime_error
            {
            public:
                ParsingError(const std::string& message, const detail::InputStream& stream) :
                    std::runtime_error(generateMessage(message, stream))
                {

                }

            private:
                static std::string generateMessage(const std::string& message, const detail::InputStream& stream)
                {
                    const auto loc = stream.currentLocation();

                    return message + std::string(" at ") + std::to_string(loc.line) + std::string(":") + std::to_string(loc.character);
                }
            };

            DocumentParser(const std::string& str) :
                m_input(str)
            {

            }
            DocumentParser(std::string&& str) :
                m_input(std::move(str))
            {

            }

            Value parse()
            {
                return Value(parseValue());
            }

        private:
            detail::InputStream m_input;

            [[noreturn]] void parsingError(const char* msg)
            {
                throw ParsingError(std::string(msg), m_input);
            }

            Value parseValue()
            {
                m_input.eatWhitespaces();

                const char currentChar = m_input.current();

                if (currentChar == '{') return Value(parseObject());
                if (currentChar == '[') return Value(parseArray());
                if (currentChar == '\"') return Value(parseString());
                if (currentChar == 't' || currentChar == 'f') return Value(parseBool());
                if (currentChar == 'n') { parseNull(); return Value(nullptr); };
                if (detail::isDigit(currentChar) || currentChar == '-')
                {
                    const double number = parseDouble();
                    if (number == std::floor(number))
                    {
                        return Value(static_cast<int64_t>(number));
                    }
                    else
                    {
                        return Value(number);
                    }
                }

                parsingError("Unexpected character");
            }

            double parseDouble()
            {
                return m_input.readDouble();
            }

            void parseNull()
            {
                m_input.readNull();
            }

            bool parseBool()
            {
                return m_input.readBool();
            }

            std::string parseString()
            {
                std::string str = m_input.readString();
                if (m_input.isOnEnd()) parsingError("Unterminated string"); //string is either not terminated or ends just before '\0'
                return str;
            }

            Value::Object parseObject()
            {
                Value::Object obj;

                m_input.advance(); // '{'

                m_input.eatWhitespaces();
                if (m_input.isOnEnd()) parsingError("Unterminated object");
                else if (m_input.current() != '}')
                {
                    for (;;)
                    {
                        m_input.eatWhitespaces();
                        if (m_input.isOnEnd()) parsingError("Unterminated object");

                        if (m_input.current() != '\"') parsingError("Expected key name");

                        std::string key = m_input.readString();
                        m_input.eatWhitespaces();

                        if (m_input.isOnEnd()) parsingError("Unexpected end of stream");
                        if (m_input.current() != ':') parsingError("Expected ':' after key name");
                        m_input.advance(); // ':'

                        m_input.eatWhitespaces();
                        Value val = parseValue();
                        obj.try_emplace(std::move(key), std::move(val));

                        m_input.eatWhitespaces();
                        if (m_input.isOnEnd()) parsingError("Unterminated object");
                        else if (m_input.current() == '}') break;
                        else if (m_input.current() == ',')
                        {
                            m_input.advance(); // ','
                            continue;
                        }
                        else parsingError("Expected ',' or '}'");
                    }
                }
                m_input.advance(); // '}'

                return obj;
            }

            Value::Array parseArray()
            {
                Value::Array arr;
                arr.reserve(8);

                m_input.advance(); // '['

                m_input.eatWhitespaces();
                if (m_input.isOnEnd()) parsingError("Unterminated array");
                else if (m_input.current() != ']')
                {
                    for (;;)
                    {
                        m_input.eatWhitespaces();
                        if (m_input.isOnEnd()) parsingError("Unterminated array");

                        Value val = parseValue();
                        arr.emplace_back(std::move(val));

                        m_input.eatWhitespaces();
                        if (m_input.isOnEnd()) parsingError("Unterminated array");
                        else if (m_input.current() == ']') break;
                        else if (m_input.current() == ',')
                        {
                            m_input.advance(); // ','
                            continue;
                        }
                        else parsingError("Expected ',' or ']'");
                    }
                }
                m_input.advance(); // ']'

                return arr;
            }

        };
    }
}
