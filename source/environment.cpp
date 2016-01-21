
#include <fstream>
#include "environment.h"


bool LoadEnvironmentFile(Environment* env, const std::string& filename)
{
	std::ifstream ifs;
	ifs.open(filename.c_str(), std::ios::in | std::ios::binary);
	if(!ifs)
		return false;

	ifs.seekg(0, std::ios_base::end);
	const unsigned long length = ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);
	ifs.read((char*)env, length);
	ifs.close();

	return true;
}
