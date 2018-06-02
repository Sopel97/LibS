#pragma once

#include "Value.h"

namespace ls
{
    namespace json
    {
        struct WriterParams
        {
            enum struct Whitespace
            {
                Space,
                Tab
            };

            Whitespace indentType;
            int indentSize;

            int spacesAfterKey;
            int spacesAfterColon;

            int spacesAfterOpeningBracket;
            int spacesBeforeClosingBracket;
            int spacesAfterComma;

            bool newLineAfterOpeningBracket;
            bool newLineAfterComma;

            bool keepEmptyCompact;

            static WriterParams compact()
            {
                WriterParams params;

                params.indentType = Whitespace::Space;
                params.indentSize = 0;

                params.spacesAfterKey = 0;
                params.spacesAfterColon = 0;

                params.spacesAfterOpeningBracket = 0;
                params.spacesBeforeClosingBracket = 0;
                params.spacesAfterComma = 0;

                params.newLineAfterOpeningBracket = false;
                params.newLineAfterComma = false;

                params.keepEmptyCompact = true;

                return params;
            }

            static WriterParams pretty()
            {
                WriterParams params;

                params.indentType = Whitespace::Space;
                params.indentSize = 4;

                params.spacesAfterKey = 0;
                params.spacesAfterColon = 1;

                params.spacesAfterOpeningBracket = 0;
                params.spacesBeforeClosingBracket = 0;
                params.spacesAfterComma = 0;

                params.newLineAfterOpeningBracket = true;
                params.newLineAfterComma = true;

                params.keepEmptyCompact = true;

                return params;
            }
        };

        struct Writer
        {
        public:
            Writer(const Value& val, const WriterParams& params = WriterParams::pretty()) :
                m_params(params),
                m_currentIndent(0)
            {
                write(val);
            }

            const std::string& result() const
            {
                return m_result;
            }
            std::string& result()
            {
                return m_result;
            }
        private:
            std::string m_result;
            WriterParams m_params;
            int m_currentIndent;

            void write(const Value& val)
            {
                if (val.isArray()) writeArray(val.getArray());
                else if (val.isObject()) writeObject(val.getObject());
                else if (val.isString()) writeString(val.getString());
                else if (val.isBool()) writeBool(val.getBool());
                else if (val.isFloat()) writeDouble(val.getDouble());
                else if (val.isInt()) writeInt(val.getInt());
                else if (val.isNull()) writeNull();
            }

            void newLine()
            {
                m_result += '\n';
                const char indentChar = m_params.indentType == WriterParams::Whitespace::Space ? ' ' : '\t';
                m_result.append(m_params.indentSize * m_currentIndent, indentChar);
            }

            void afterOpeningBracket()
            {
                m_result.append(m_params.spacesAfterOpeningBracket, ' ');
                if (m_params.newLineAfterOpeningBracket)
                {
                    ++m_currentIndent;
                    newLine();
                }
            }

            void beforeClosingBracket()
            {
                if (m_params.newLineAfterOpeningBracket)
                {
                    --m_currentIndent;
                    newLine();
                }
                m_result.append(m_params.spacesBeforeClosingBracket, ' ');
            }

            void startObject()
            {
                m_result += '{';
                afterOpeningBracket();
            }
            void endObject()
            {
                beforeClosingBracket();
                m_result += '}';
            }
            void startArray()
            {
                m_result += '[';
                afterOpeningBracket();
            }
            void endArray()
            {
                beforeClosingBracket();
                m_result += ']';
            }
            void emptyObject()
            {
                if (m_params.keepEmptyCompact)
                {
                    m_result += "{}";
                }
                else
                {
                    startObject();
                    endObject();
                }
            }
            void emptyArray()
            {
                if (m_params.keepEmptyCompact)
                {
                    m_result += "[]";
                }
                else
                {
                    startArray();
                    endArray();
                }
            }
            void comma()
            {
                m_result += ',';
                m_result.append(m_params.spacesAfterComma, ' ');
                if (m_params.newLineAfterComma)
                {
                    newLine();
                }
            }
            void key(const std::string& key)
            {
                writeString(key);
                m_result.append(m_params.spacesAfterKey, ' ');
                m_result += ':';
                m_result.append(m_params.spacesAfterColon, ' ');
            }

            void writeBool(bool b)
            {
                if (b) m_result += "true";
                else m_result += "false";
            }
            void writeString(const std::string& str)
            {
                m_result += '\"';
                m_result += str;
                m_result += '\"';
            }
            void writeDouble(double d)
            {
                m_result += std::to_string(d);
            }
            void writeInt(int64_t i)
            {
                m_result += std::to_string(i);
            }
            void writeNull()
            {
                m_result += "null";
            }
            void writeObject(const Value::Object& obj)
            {
                const size_t size = obj.size();
                if (size == 0) emptyObject();
                else
                {
                    startObject();
                    size_t i = 0;
                    for (const auto& p : obj)
                    {
                        key(p.first);
                        write(p.second);

                        ++i;
                        if (i < size)
                        {
                            comma();
                        }
                    }
                    endObject();
                }
            }
            void writeArray(const Value::Array& arr)
            {
                const size_t size = arr.size();
                if (size == 0) emptyArray();
                else
                {
                    startArray();
                    size_t i = 0;
                    for (const auto& e : arr)
                    {
                        write(e);

                        ++i;
                        if (i < size)
                        {
                            comma();
                        }
                    }
                    endArray();
                }
            }
        };
    }
}
