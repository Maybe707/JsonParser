#ifndef JSON_PARSER
#define JSON_PARSER

#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Vector.hpp"
#include "hashMap.hpp"
#include <cmath>
#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>

namespace GLVM::Core
{
    enum JsonType
    {
		JSON_INVALID_VALUE,
        JSON_OBJECT,
        JSON_FLOAT_NUMBER,
		JSON_INTEGER_NUMBER,
        JSON_STRING,
        JSON_BOOLEAN,
        JSON_NULL,
        JSON_ARRAY
    };
	
	struct JsonValue;
	
    union JsonVariant
    {
        std::string string;
        float fNumber;
		int iNumber;
        bool boolean;
        void* null;
        GLVM::Core::Vector<JsonValue> array;
        HashMap<JsonValue> object;
        JsonVariant() {}
        ~JsonVariant() {}
    };

    struct JsonValue
    {
        JsonVariant value;
        JsonType type;

		JsonValue() { type = JSON_INVALID_VALUE; }
		JsonValue(std::string _string) {
			type = JSON_STRING;
			new (&value.string) std::string{_string};
		}
		JsonValue(float _float) {
			type = JSON_FLOAT_NUMBER;
			value.fNumber = _float;
		}
		JsonValue(int _int) {
			type = JSON_INTEGER_NUMBER;
			value.iNumber = _int;
		}
		JsonValue(bool _bool) {
			type = JSON_BOOLEAN;
			value.boolean = _bool;
		}
		// JsonValue(void* _null) {
		// 	type = JSON_NULL;
		// 	value.null = _null;
		// }
		
		JsonValue(const JsonValue& _value) {
			type = JSON_INVALID_VALUE;
			
			switch (_value.type) {
			case JSON_OBJECT:
				new (&value.object) HashMap<JsonValue>{_value.value.object};
				break;
			case JSON_INTEGER_NUMBER:
				value.iNumber = _value.value.iNumber;
				break;
			case JSON_FLOAT_NUMBER:
				value.fNumber = _value.value.fNumber;
				break;
			case JSON_STRING:
				new (&value.string) std::string{_value.value.string};
				break;
			case JSON_BOOLEAN:
				value.boolean = _value.value.boolean;
				break;
			case JSON_NULL:
				value.null = _value.value.null;
				break;
			case JSON_ARRAY:
				new (&value.array) GLVM::Core::Vector<JsonValue>{_value.value.array};
				break;
			default:
				break;
			}
			type = _value.type;
		}

		~JsonValue() {
			switch (type) {
			case JSON_INVALID_VALUE:
				break;
			case JSON_OBJECT:
				value.object.~HashMap();
				break;
			case JSON_INTEGER_NUMBER:
				break;
			case JSON_FLOAT_NUMBER:
				break;
			case JSON_STRING:
				value.string.~basic_string();
				break;
			case JSON_BOOLEAN:
				break;
			case JSON_NULL:
				break;
			case JSON_ARRAY:
				value.array.~Vector();
				break;
			}
		}
		
		void operator=(const JsonValue& _value) {
			switch (type) {
			case JSON_INVALID_VALUE:
				break;
			case JSON_OBJECT:
				value.object.~HashMap();
				break;
			case JSON_INTEGER_NUMBER:
				break;
			case JSON_FLOAT_NUMBER:
				break;
			case JSON_STRING:
				value.string.~basic_string();
				break;
			case JSON_BOOLEAN:
				break;
			case JSON_NULL:
				break;
			case JSON_ARRAY:
				value.array.~Vector();
				break;
			}

			switch (_value.type) {
			case JSON_OBJECT:
				new (&value.object) HashMap<JsonValue>{_value.value.object};
				break;
			case JSON_INTEGER_NUMBER:
				value.iNumber = _value.value.iNumber;
				break;
			case JSON_FLOAT_NUMBER:
				value.fNumber = _value.value.fNumber;
				break;
			case JSON_STRING:
				new (&value.string) std::string{_value.value.string};
				break;
			case JSON_BOOLEAN:
				value.boolean = _value.value.boolean;
				break;
			case JSON_NULL:
				value.null = _value.value.null;
				break;
			case JSON_ARRAY:
				new (&value.array) GLVM::Core::Vector<JsonValue>{_value.value.array};
				break;
			default:
				break;
			}
			type = _value.type;
		}
    };
        
    class CJsonParser
    {
        std::string sJsonFileData_;
        const char* pJsonFileData_;
		char currentChar_;
        unsigned int globalFileCounter_ = 0;
		
		Vector<JsonValue*> stackOfJsonValues_;
		JsonValue* root_;
		bool keyFlag = true;
	    std::string lastKey_ = "";
		std::string bufferString_ = "";

    public:
		JsonValue* GetRoot() { return root_; }
        void ReadFile(const char* _filePath);
        void Parse();
		JsonValue* CreateJsonHashMap();
		JsonValue* CreateJsonArray();
		std::string BoolOrNullParse();
		bool IsContainChar(std::string _string, char _char);
		std::string NumberAsStringParse();
		std::string StringParse();
		GLVM::Core::Vector<char> StringToVectorOfChars(std::string _string);
		int ParseInteger(GLVM::Core::Vector<char> _word);
		float ParseFloating(GLVM::Core::Vector<char> _word);
    };
}

#endif
