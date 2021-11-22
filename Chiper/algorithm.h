#pragma once
#include <iostream>
#include <string>
#include <fstream>

namespace algorithm

{	

	std::ifstream& CaesarAlgoritm(std::ifstream& s);
	std::ifstream& DeCaesarAlgoritm(std::ifstream& s);
	std::ifstream& DESAlgoritm(std::ifstream& s);
	std::ifstream& DeDESAlgoritm(std::ifstream& s);
	std::ifstream& AESAlgoritm(std::ifstream& s);
	std::ifstream& DeAESAlgoritm(std::ifstream& s);
	std::ifstream& PlayfairAlgoritm(std::ifstream& s);
	std::ifstream& DePlayfairAlgoritm(std::ifstream& s);
	std::ifstream& ElgamalAlgoritm(std::ifstream& s);
	std::ifstream& DeElgamalAlgoritm(std::ifstream& s);

};

