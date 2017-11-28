#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"+
#include <iostream>
#include <sstream>
#include <string>
#include <map>

struct enemy {
	int hp;
	int atk;
	int def;
	float freq;
	int exp;
};

std::map<std::string, enemy> myMap;

void main() {
	rapidxml::xml_document<> myDoc;
	std::ifstream file("../../res/xml/config.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	std::string content(buffer.str());
	myDoc.parse<0>(&content[0]);

	rapidxml::xml_node<> *pRoot = myDoc.first_node();

	for (rapidxml::xml_node<> *pNode = pRoot->first_node("enemy"); pNode; pNode = pNode->next_sibling()) {
		std::cout << pNode->name() << ": ";

		std::string temporal;
		for (rapidxml::xml_attribute<> *myAttribute = pNode->first_attribute(); myAttribute; myAttribute = myAttribute->next_attribute()) {
			std::cout << myAttribute->value();
			myMap[myAttribute->value()];
			temporal = myAttribute->value();
		}
		std::cout << std::endl;
		int control = 0;
		for (rapidxml::xml_node<> *myInnerNode = pNode->first_node(); myInnerNode; myInnerNode = myInnerNode->next_sibling()) {
			std::cout << "\t" << myInnerNode->name() << ": " << myInnerNode->value() << std::endl;
			switch (control++) {
			case 0: myMap[temporal].hp = atoi(myInnerNode->value());
				break;
			case 1: myMap[temporal].atk = atoi(myInnerNode->value());
				break;
			case 2:myMap[temporal].def = atoi(myInnerNode->value());
				break;
			case 3: myMap[temporal].freq = atof(myInnerNode->value());
				break;
			case 4: myMap[temporal].exp = atoi(myInnerNode->value());
				break;
			default:
				std::cout << "Too many arguments" << std::endl;
				break;
			}
		}
	}
	int a = 0;
}