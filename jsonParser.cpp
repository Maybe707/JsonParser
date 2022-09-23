#include "jsonParser.hpp"
#include "Vector.hpp"
#include <chrono>
#include <ostream>
#include <pthread.h>
#include <cassert>

namespace GLVM::Core
{    

    void CJsonParser::ReadFile(const char* _filePath) {
        const char* _pJsonFilePath = _filePath;
        std::ifstream jsonFileInputStream;
        std::stringstream jsonFileOutputStream;
        
        jsonFileInputStream.open(_pJsonFilePath);
        if(jsonFileInputStream.good()) {

            jsonFileOutputStream << jsonFileInputStream.rdbuf();
            jsonFileInputStream.close();
            sJsonFileData_ = jsonFileOutputStream.str();
        } else {
            std::cout << "Error of reading json file" << std::endl;
            return;
        }

        pJsonFileData_ = sJsonFileData_.c_str();
    }

    void CJsonParser::Parse() {
		while (currentChar_ != '\0') {
			if (searchKeyFlag)
				lastKey_ = StringParse();
			
			NextInitialValueToken();

			while (1) {
				if (currentChar_ == ' ' || currentChar_ == '\n') {
					++globalFileCounter_;
					currentChar_ = pJsonFileData_[globalFileCounter_];
					continue;
				} else
					break;
			}

			if (currentChar_ == ',') {
				++globalFileCounter_;
				continue;
			} else if (currentChar_ == '}' || currentChar_ == ']') {
				// stackOfJsonValues_.Pop();

				// if (stackOfJsonValues_.GetHead().type == JSON_OBJECT)
				// 	searchKeyFlag = true;
				// else
				// 	searchKeyFlag = false;

				++globalFileCounter_;
				
				while (1) {
					currentChar_ = pJsonFileData_[globalFileCounter_];
					std::cout << currentChar_ << std::endl;
					if (currentChar_ == '}' || currentChar_ == ']') {
						// stackOfJsonValues_.Pop();
						// if (stackOfJsonValues_.GetHead().type == JSON_OBJECT)
						// 	searchKeyFlag = true;
						// else
						// 	searchKeyFlag = false;
						
						++globalFileCounter_;
						continue;
					} else if (currentChar_ == ',') {
						++globalFileCounter_;
						break;
					}

					++globalFileCounter_;
					continue;
				}

				continue;
			} else {
				searchKeyFlag = false;
				++globalFileCounter_;
			}
		}
	}

	void CJsonParser::NextInitialValueToken() {
		while (1) {
			currentChar_ = pJsonFileData_[globalFileCounter_];
			if (currentChar_ == '{') {
				std::cout << "{" << std::endl;
				
				// JsonValue* jsonObject = new JsonValue;
				// jsonObject->type = JSON_OBJECT;
				// new(&jsonObject->value.object) HashMap<JsonValue>;

				// if (stackOfJsonValues_.GetSize() == 0)
				// 	stackOfJsonValues_.Push(*jsonObject);
				// else {
				// 	stackOfJsonValues_.Push(*jsonObject);
				// 	if (searchKeyFlag)
				// 		stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()] = *jsonObject;
				// 	else
				// 		stackOfJsonValues_.GetHead().value.array.Push(*jsonObject);
				// }

				// if (searchKeyFlag)
				// 	std::cout << "stack object value: " << stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()].value.fNumber << std::endl;
				// else
				// 	std::cout << "stack object value: " << stackOfJsonValues_.GetHead().value.array.GetHead().value. << std::endl;
					
				searchKeyFlag = true;
				++globalFileCounter_;
				return;
			} else if (currentChar_ == '[') {
				std::cout << "[" << std::endl;

				// JsonValue* jsonArray = new JsonValue;
				// jsonArray->type = JSON_ARRAY;
				// new(&jsonArray->value.array) Vector<JsonValue>;

				// if (stackOfJsonValues_.GetSize() == 0)
				// 	stackOfJsonValues_.Push(*jsonArray);
				// else {
				// 	stackOfJsonValues_.Push(*jsonArray);
				// 	if (searchKeyFlag)
				// 		stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()] = *jsonArray;
				// 	else
				// 		stackOfJsonValues_.GetHead().value.array.Push(*jsonArray);
				// }

//				std::cout << "stack array value: " << stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()].value.fNumber << std::endl;
				
				searchKeyFlag = false;
				++globalFileCounter_;
				return;
			} else if (currentChar_ == '"') {
				++globalFileCounter_;
				std::string stringValue = StringParse();

				// JsonValue* jsonString = new JsonValue;
				// jsonString->type = JSON_STRING;
				// new(&jsonString->value.string) std::string{stringValue};

				// if (searchKeyFlag && stackOfJsonValues_.GetSize() > 0) {
				// 	assert(stackOfJsonValues_.GetHead().type == JSON_OBJECT);
				// 	stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()] = *jsonString;
				// } else if (!searchKeyFlag && stackOfJsonValues_.GetSize() > 0) {
				// 	assert(stackOfJsonValues_.GetHead().type == JSON_ARRAY);
				// 	stackOfJsonValues_.GetHead().value.array.Push(*jsonString);
				// }
				
				std::cout << stringValue << std::endl;
				return;
			} else if ((currentChar_ >= '0' && currentChar_ <= '9') ||
				       currentChar_ == '+' || currentChar_ == '-') {
				std::string numberAsString = NumberAsStringParse();
				GLVM::Core::Vector<char> vector = StringToVectorOfChars(numberAsString);
				std::cout << numberAsString << std::endl;
				float fNumber = 0.0f;
				int iNumber = 0;
				if (IsContainChar(numberAsString, '.')) {
					fNumber = ParseFloating(vector);
// 					JsonValue* jsonFnumber = new JsonValue;
// 					jsonFnumber->type = JSON_FLOAT_NUMBER;
// 					jsonFnumber->value.fNumber = fNumber;
					std::cout << "Float: " << fNumber << std::endl;
// 					if (searchKeyFlag && stackOfJsonValues_.GetSize() > 0)
// 						stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()] = *jsonFnumber;
// 					else if (!searchKeyFlag && stackOfJsonValues_.GetSize() > 0)
// 						stackOfJsonValues_.GetHead().value.array.Push(*jsonFnumber);
// 					std::cout << "stack float value: " << stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()].value.fNumber << std::endl;
				} else {
// 					iNumber = ParseInteger(vector);
// 					JsonValue* jsonInumber = new JsonValue;
// 					jsonInumber->type = JSON_INTEGER_NUMBER;
// 					jsonInumber->value.iNumber = iNumber;
					std::cout << "Int: " << iNumber<< std::endl;
// 					if (searchKeyFlag && stackOfJsonValues_.GetSize() > 0)
// 						stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()] = *jsonInumber;
// 					else if (!searchKeyFlag && stackOfJsonValues_.GetSize() > 0)
// 						stackOfJsonValues_.GetHead().value.array.Push(*jsonInumber);
// 					std::cout << "stack int value: " << stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()].value.iNumber << std::endl;
				}
				
				return;
			} else if (currentChar_ == 't' ||
				       currentChar_ == 'f' ||
				       currentChar_ == 'n') {
				std::string boolOrNullString = BoolOrNullParse();
				JsonValue* jsonBoolOrNull = new JsonValue;
				
				// if (boolOrNullString == "true") {
				// 	jsonBoolOrNull->type = JSON_BOOLEAN;
				// 	jsonBoolOrNull->value.boolean = true;
				// } else if (boolOrNullString == "false") {
				// 	jsonBoolOrNull->type = JSON_BOOLEAN;
				// 	jsonBoolOrNull->value.boolean = false;
				// } else if (boolOrNullString == "null") {
				// 	jsonBoolOrNull->type = JSON_NULL;
				// 	jsonBoolOrNull->value.null = NULL;
				// }

				// if (searchKeyFlag && stackOfJsonValues_.GetSize() > 0)
				// 	stackOfJsonValues_.GetHead().value.object[lastKey_.c_str()] = *jsonBoolOrNull;
				// else if (!searchKeyFlag && stackOfJsonValues_.GetSize() > 0)
				// 	stackOfJsonValues_.GetHead().value.array.Push(*jsonBoolOrNull);
				
				std::cout << boolOrNullString << std::endl;
				return;
			} else if (currentChar_ == '\0') {
				return;
			} else {
				++globalFileCounter_;
			}
		}
	}

	std::string CJsonParser::BoolOrNullParse() {
		std::string boolOrNullString = "";
		while (1) {
			currentChar_ = pJsonFileData_[globalFileCounter_];
			if (currentChar_ >= 'a' && currentChar_ <= 'z') {
				boolOrNullString.push_back(currentChar_);
				++globalFileCounter_;
			} else
				return boolOrNullString;
		}
	}
	
	bool CJsonParser::IsContainChar(std::string _string, char _char) {
		for (int i = 0; i < _string.size(); ++i) {
			if (_string[i] == _char)
				return true;
		}

		return false;
	}
			
	std::string CJsonParser::NumberAsStringParse() {
		std::string numberAsString = "";
		while (1) {
			currentChar_ = pJsonFileData_[globalFileCounter_];
			if ((currentChar_ >= '0' && currentChar_ <= '9') ||
				currentChar_ == '+' || currentChar_ == '-') {
				numberAsString.push_back(currentChar_);
				++globalFileCounter_;
			} else if (currentChar_ == '.') {
				numberAsString.push_back(currentChar_);
				++globalFileCounter_;
			} else
				return numberAsString;
		}
	}
	
	void CJsonParser::NextFinalValueToken() {
		while (1) {
			currentChar_ = pJsonFileData_[globalFileCounter_];
			if (currentChar_ == ']') {
				
			} else if (currentChar_ == '}') {
				
			}
		}
	}
	
 	std::string CJsonParser::StringParse() {
		std::string localBuffer = "";
		while (1) {
			currentChar_ = pJsonFileData_[globalFileCounter_];
			if (currentChar_ == '"') {
				++globalFileCounter_;
				return localBuffer;
			} else {
				localBuffer.push_back(currentChar_);
				++globalFileCounter_;
			}
		}
	}
	
	GLVM::Core::Vector<char> CJsonParser::StringToVectorOfChars(std::string _string) {
		GLVM::Core::Vector<char> vectorWithChars;
		for (int i = 0; i < _string.size(); ++i) {
			vectorWithChars.Push(_string[i]);
		}

		return vectorWithChars;
	}
	
	int CJsonParser::ParseInteger(GLVM::Core::Vector<char> _word) {
        GLVM::Core::Vector<int> baseContainer;

        for (int i = 0; i < _word.GetSize(); ++i)
            baseContainer.Push(_word[i] - 48);

        int iResult = 0;
        bool negateFlag = false;

        unsigned int baseContainerSize = baseContainer.GetSize();
        for (int i = 0; i < baseContainerSize; ++i) {
            if (baseContainer[i] == -3 && i == 0) {
				negateFlag = true;
                continue;
            } else if (baseContainer[i] == -5 && i == 0)
                continue;

            iResult += baseContainer[i] * std::pow(10, (baseContainerSize - 1) - i);
        }

		if (negateFlag)
			iResult *= -1;
		
        return iResult;
    }
    
    float CJsonParser::ParseFloating(GLVM::Core::Vector<char> _word) {
        GLVM::Core::Vector<int> baseContainer;

        for (int i = 0; i < _word.GetSize(); ++i)
            baseContainer.Push(_word[i] - 48);

        int integerPart = 0;
        float floatingPart = 0;
        GLVM::Core::Vector<int> integerPartContainer;
        GLVM::Core::Vector<int> floatingPartContainer;
        bool dotFlag = false;
        bool negateFlag = false;
        unsigned int baseContainerSize = baseContainer.GetSize();

        if (baseContainer[0] == -3)
            negateFlag = true;

        for (int i = 0; i < baseContainerSize; ++i) {
            if (negateFlag && i == 0)
                continue;
            else if (baseContainer[i] == -5 && i == 0)
                continue;
            else if (baseContainer[i] == -2) {
                dotFlag = true;
                continue;
            }

            if (baseContainer[i] >= 0 && baseContainer[i] <= 9) {
                if (dotFlag)
                    floatingPartContainer.Push(baseContainer[i]);
                else
                    integerPartContainer.Push(baseContainer[i]);
            } else {
                std::cout << "Element is not a number" << std::endl;
                return NAN;
            }
        }

        unsigned int integerPartContainerSize = integerPartContainer.GetSize();
        for (int i = 0; i < integerPartContainerSize; ++i)
            integerPart += integerPartContainer[i] * std::pow(10, (integerPartContainerSize - 1) - i);

        unsigned int floatingPartContainerSize = floatingPartContainer.GetSize();
        for (int i = 0; i < floatingPartContainerSize; ++i)
            floatingPart += floatingPartContainer[i] / std::pow(10, i + 1);

        float result = 0;
        result = (float)(integerPart + floatingPart);
        
        if (negateFlag)
            result *= -1.0f;

        return result;
    }
}
