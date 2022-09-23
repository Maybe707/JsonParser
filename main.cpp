#include "Vector.hpp"
#include "jsonParser.hpp"

struct Govno
{
	int value;
};

int main(int argc, char* argv[])
{
    GLVM::Core::CJsonParser parser;
    parser.ReadFile("/home/cyber-demon/cyberDemonCode/C++/jsonParser/untitled.gltf");
    parser.Parse();

	// HashMap<GLVM::Core::JsonValue> map1;
	// GLVM::Core::JsonValue jsonValue;
	// jsonValue.type = GLVM::Core::JSON_INTEGER_NUMBER;
	// jsonValue.value.iNumber = 1000;

	// std::cout << jsonValue.value.iNumber << std::endl;
	
	// map1["wer"] = jsonValue;

	// std::cout << map1["wer"].value.iNumber << std::endl;
	
	// HashMap<GLVM::Core::JsonValue> map0(map1);

	// std::cout << map0["wer"].value.iNumber << std::endl;
	
    // unsigned int counter = 0;
    // while(parser.pJsonFileData[counter] != '\0') {
    //     std::cout << parser.pJsonFileData[counter] << std::endl;
    //     ++counter;
    // }
   
    return 0;
}
