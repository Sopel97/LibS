#pragma once

#include "Writer.h"
#include "Value.h"
#include "Parser.h"

#include <fstream>
#include <string>
#include <utility>

namespace ls
{
    namespace json
    {
        struct Document : public Value
        {
        public:
            friend struct DocumentParser;

            static Document fromString(const std::string& str)
            {
                return Document(str);
            }
            static Document fromString(std::string&& str)
            {
                return Document(std::move(str));
            }
            static Document fromFile(const std::string& path)
            {
                std::fstream file(path, std::ios::in);
                if (file)
                {
                    std::string contents;
                    file.seekg(0, std::ios::end);
                    contents.resize(static_cast<size_t>(file.tellg()));
                    file.seekg(0, std::ios::beg);
                    file.read(contents.data(), contents.size());
                    file.close();
                    return contents;
                }
                else throw std::runtime_error("File not found: " + path);
            }
            static Document emptyObject()
            {
                return Document(Value(Value::Object{}));
            }
            static Document emptyArray()
            {
                return Document(Value(Value::Array{}));
            }
            static Document singleValue(const Value& val)
            {
                return Document(Value(val));
            }
            static Document singleValue(Value&& val)
            {
                return Document(std::move(val));
            }

            std::string stringify(const WriterParams& params = WriterParams::pretty())
            {
                return Writer(*this, params).result();
            }

        private:
            Document(const std::string& str) :
                Value(DocumentParser(str).parse())
            {

            }

            Document(std::string&& str) :
                Value(DocumentParser(std::move(str)).parse())
            {

            }

            Document(Value&& root) noexcept :
                Value(std::move(root))
            {

            }
        };
    }
}
