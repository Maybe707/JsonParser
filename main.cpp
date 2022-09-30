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

	GLVM::Core::JsonValue* root = parser.GetRoot();
	
	std::cout << root->value.object["govno"].value.iNumber << std::endl;
	std::cout << root->value.object["hrenata"].value.fNumber << std::endl;
	std::cout << root->value.object["kal"].value.string << std::endl;
	std::cout << root->value.object["scene"].value.iNumber << std::endl;

	std::cout << root->value.object["asset"].value.object["generator"].value.string << std::endl;
	std::cout << root->value.object["asset"].value.object["exkrement"].value.fNumber << std::endl;
	std::cout << root->value.object["asset"].value.object["version"].value.string << std::endl;

	std::cout << root->value.object["scenes"].value.array[0].value.object["nodes"].value.array[1].value.iNumber << std::endl;

	std::cout << root->value.object["buffers"].value.array[0].value.object["byteLength"].value.iNumber << std::endl;
	std::cout << root->value.object["buffers"].value.array[0].value.object["uri"].value.string << std::endl;

	std::cout << root->value.object["bufferViews"].value.array[3].value.object["buffer"].value.boolean << std::endl;

	std::cout << root->value.object["bufferViews"].value.array[3].value.object["byteLength"].value.boolean << std::endl;

	std::cout << root->value.object["bufferViews"].value.array[3].value.object["byteOffset"].value.null << std::endl;
	
	// HashMap<GLVM::Core::JsonValue> map1;
	// GLVM::Core::JsonValue jsonValue;
	// jsonValue.type = GLVM::Core::JSON_INTEGER_NUMBER;
	// jsonValue.value.iNumber = 1000;

	// std::cout << jsonValue.value.iNumber << std::endl;
	
	// map1["wer"] = jsonValue;

	// std::cout << map1["wer"].value.iNumber << std::endl;
	
	// HashMap<GLVM::Core::JsonValue> map0(map1);

	// std::cout << map0["wer"].value.iNumber << std::endl;

	// jsonValue.type = GLVM::Core::JSON_STRING;
	// new (&jsonValue.value.string) std::string{"string"};
	// std::cout << jsonValue.value.string << std::endl;
	// jsonValue.value.string = "new string";
	// std::cout << jsonValue.value.string << std::endl;
	
    // unsigned int counter = 0;
    // while(parser.pJsonFileData[counter] != '\0') {
    //     std::cout << parser.pJsonFileData[counter] << std::endl;
    //     ++counter;
    // }
   
    return 0;
}
