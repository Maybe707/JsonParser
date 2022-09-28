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
		currentChar_ = pJsonFileData_[globalFileCounter_];
		
		while (currentChar_ != '\0') {
			currentChar_ = pJsonFileData_[globalFileCounter_];

			if (currentChar_ == '"' && keyFlag) {
				lastKey_ = StringParse();
				while (currentChar_ == ' ' || currentChar_ == ':') {
				++globalFileCounter_;
				currentChar_ = pJsonFileData_[globalFileCounter_];
				}
//				keys_.Push(lastKey_);
				std::cout << "key " << lastKey_ << std::endl;
			}
			
			if (currentChar_ == '"') {
				bufferString_ = StringParse();
//				stringValues_.Push(bufferString_);
				std::cout << "value str " << bufferString_ << std::endl;
				std::cout << "Key: " << keyFlag << std::endl;
				if (keyFlag) {
					JsonValue jsonString(bufferString_);
					// jsonString.type = JSON_STRING;
					// new (&jsonString.value.string) std::string{bufferString_};
					stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = jsonString;
				} else {
 					JsonValue jsonString(bufferString_);
					// jsonString.type = JSON_STRING;
					// new (&jsonString.value.string) std::string{bufferString_};
					stackOfJsonValues_.GetHead()->value.array.Push(jsonString);
				}
			} else if ((currentChar_ >= '0' && currentChar_ <= '9') ||
					   currentChar_ == '+' || currentChar_ == '-') {
				bufferString_ = NumberAsStringParse();
				GLVM::Core::Vector<char> vector = StringToVectorOfChars(bufferString_);
				float fNumber = 0.0f;
				int iNumber = 0;
				if (IsContainChar(bufferString_, '.')) {
					fNumber = ParseFloating(vector);
//					floatValues_.Push(fNumber);

					if (keyFlag) {
						JsonValue jsonFloat(fNumber);
						// jsonFloat.type = JSON_FLOAT_NUMBER;
						// jsonFloat.value.fNumber = fNumber;
						stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = jsonFloat;
					} else {
						JsonValue jsonFloat(fNumber);
						// jsonFloat.type = JSON_FLOAT_NUMBER;
						// jsonFloat.value.fNumber = fNumber;
						stackOfJsonValues_.GetHead()->value.array.Push(jsonFloat);
					}
				} else {
 					iNumber = ParseInteger(vector);
//					intValues_.Push(iNumber);

					if (keyFlag) {
						JsonValue jsonInt(iNumber);
						// jsonInt.type = JSON_INTEGER_NUMBER;
						// jsonInt.value.iNumber = iNumber;
						stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = jsonInt;
					} else {
						JsonValue jsonInt(iNumber);
						// jsonInt.type = JSON_INTEGER_NUMBER;
						// jsonInt.value.iNumber = iNumber;
						stackOfJsonValues_.GetHead()->value.array.Push(jsonInt);
					}
				}
				std::cout << bufferString_ << std::endl;


			} else if (currentChar_ == 't' ||
				       currentChar_ == 'f' ||
				       currentChar_ == 'n') {
				std::string boolOrNullString = BoolOrNullParse();
//				boolOrNullValues_.Push(boolOrNullString);
				std::cout << boolOrNullString << std::endl;

				if (boolOrNullString == "true") {
					if (keyFlag) {
						JsonValue jsonTrue(true);
						// jsonTrue.type = JSON_BOOLEAN;
						// jsonTrue.value.boolean = true;
						stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = jsonTrue;
					} else {
						JsonValue jsonTrue(true);
						// jsonTrue.type = JSON_BOOLEAN;
						// jsonTrue.value.boolean = true;
						stackOfJsonValues_.GetHead()->value.array.Push(jsonTrue);
					}
				} else if (boolOrNullString == "false") {
					if (keyFlag) {
						JsonValue jsonFalse(false);
						// jsonFalse.type = JSON_BOOLEAN;
						// jsonFalse.value.boolean = false;
						stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = jsonFalse;
					} else {
						JsonValue jsonFalse(false);
						// jsonFalse.type = JSON_BOOLEAN;
						// jsonFalse.value.boolean = false;
						stackOfJsonValues_.GetHead()->value.array.Push(jsonFalse);
					}
				} else if (boolOrNullString == "null") {
					if (keyFlag) {
						JsonValue jsonNull;
						jsonNull.type = JSON_NULL;
						jsonNull.value.null = NULL;
						stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = jsonNull;
					} else {
						JsonValue jsonNull;
						jsonNull.type = JSON_NULL;
						jsonNull.value.null = NULL;
						stackOfJsonValues_.GetHead()->value.array.Push(jsonNull);
					}
				}
			} else if (currentChar_ == '{') {
				if (stackOfJsonValues_.GetSize() == 0) {
					root_ = new JsonValue;
					root_->type = JSON_OBJECT;
					new (&root_->value.object) HashMap<JsonValue>;
					stackOfJsonValues_.Push(root_);
				} else if (keyFlag) {
					JsonValue* jsonObject = new JsonValue;
					jsonObject->type = JSON_OBJECT;
					new (&jsonObject->value.object) HashMap<JsonValue>;
					stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = *jsonObject;
					stackOfJsonValues_.Push(&stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()]);
				} else if (!keyFlag) {
					JsonValue* jsonObject = new JsonValue;
					jsonObject->type = JSON_OBJECT;
					new (&jsonObject->value.object) HashMap<JsonValue>;
					stackOfJsonValues_.GetHead()->value.array.Push(*jsonObject);
					stackOfJsonValues_.Push(&stackOfJsonValues_.GetHead()->value.array.GetHead());
				}

				keyFlag = true;
				std::cout << currentChar_ << std::endl;
			} else if (currentChar_ == '[') {
				if (stackOfJsonValues_.GetSize() == 0) {
					root_ = new JsonValue;
					root_->type = JSON_ARRAY;
					new (&root_->value.array) Vector<JsonValue>;
					stackOfJsonValues_.Push(root_);
				} else if (keyFlag) {
					JsonValue* jsonArray = new JsonValue;
					jsonArray->type = JSON_ARRAY;
					new (&jsonArray->value.array) Vector<JsonValue>;
					stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()] = *jsonArray;
					stackOfJsonValues_.Push(&stackOfJsonValues_.GetHead()->value.object[lastKey_.c_str()]);
				} else if (!keyFlag) {
					JsonValue* jsonArray = new JsonValue;
					jsonArray->type = JSON_ARRAY;
					new (&jsonArray->value.array) Vector<JsonValue>;
					stackOfJsonValues_.GetHead()->value.array.Push(*jsonArray);
					stackOfJsonValues_.Push(&stackOfJsonValues_.GetHead()->value.array.GetHead());
				}

				keyFlag = false;
				std::cout << currentChar_ << std::endl;
			} else if (currentChar_ == '}') {
				stackOfJsonValues_.Pop();
				if (stackOfJsonValues_.GetSize() && stackOfJsonValues_.GetHead()->type == JSON_OBJECT)
					keyFlag = true;
				else
					keyFlag = false;
				std::cout << currentChar_ << std::endl;
			} else if (currentChar_ == ']') {
				stackOfJsonValues_.Pop();
				if (stackOfJsonValues_.GetSize() && stackOfJsonValues_.GetHead()->type == JSON_OBJECT)
					keyFlag = true;
				else
					keyFlag = false;
				std::cout << currentChar_ << std::endl;
			} 
			
			++globalFileCounter_;
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
		
 	std::string CJsonParser::StringParse() {
		++globalFileCounter_;
		std::string localBuffer = "";
		while (1) {
			currentChar_ = pJsonFileData_[globalFileCounter_];
			if (currentChar_ == '"') {
				++globalFileCounter_;
				currentChar_ = pJsonFileData_[globalFileCounter_];
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
