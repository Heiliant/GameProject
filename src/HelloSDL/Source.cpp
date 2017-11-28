#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"+
#include <iostream>
#include <sstream>
#include <string>

void main() {
	rapidxml::xml_document<> doc;
	std::ifstream file("../../res/files/BookStore.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> *pRoot = doc.first_node();
	for (rapidxml::xml_node<> *pNode = pRoot->first_node("book"); pNode; pNode = pNode->next_sibling()) {
		std::cout << pNode->name() << ':' << '\n';

		for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI->next_sibling()) {
			std::cout << pNodeI->name() << ':' << pNodeI->value() << '\n';
		}
		std::cout << std::endl;
	}
}