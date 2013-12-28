#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

int main(int argc, char *argv[])
{
	std::string filename = "";
	std::string symbol = "";
	std::string modversion = "";
	std::string output_filename = "";

	for(int i=1;i<argc;i++)
	{
		if("-f" == (std::string)argv[i])
		{
			i++;
			if(i >= argc)
			{
				std::cerr << "missing argument to -f" << std::endl;
				return -1;
			}
			filename = argv[i];
		}
		if("-s" == (std::string)argv[i])
		{
			i++;
			if(i >= argc)
			{
				std::cerr << "missing argument to -s" << std::endl;
				return -1;
			}
			symbol = argv[i];
		}
		if("-m" == (std::string)argv[i])
		{
			i++;
			if(i >= argc)
			{
				std::cerr << "missing argument to -m" << std::endl;
				return -1;
			}
			modversion = argv[i];
			if(modversion.length() != 8) // 4 byte
			{
				std::cerr << "invalid modversion, specify as hex. for example: DEADC0DE" << std::endl;
				return -1;
			}
		}
		if("-o" == (std::string)argv[i])
		{
			i++;
			if(i >= argc)
			{
				std::cerr << "missing argument to -o" << std::endl;
				return -1;
			}
			output_filename = argv[i];
		}
	}

	if("" == filename)
	{
		std::cerr << "no filename specified (-f)" << std::endl;
		return -1;
	}

	if("" == symbol)
	{
		std::cerr << "no symbol specified (-s)" << std::endl;
		return -1;
	}

	if("" == modversion)
	{
		std::cerr << "please specify the new modversion (-m)" << std::endl;
		return -1;
	}

	if("" == output_filename)
	{
		std::cerr << "please specify the output filename (-o)" << std::endl;
		return -1;
	}

	std::string file_data;
	std::ifstream in;
	in.open(filename.c_str(), std::ios::in|std::ios::binary);
	if(in.is_open())
	{
		
		for(;;)
		{
			char c = in.get();
			if(in.eof()) break;
			file_data += c;
		}
		in.close();
	}
	else
	{
		std::cout << "failed to open: " << filename << std::endl;
		return -3;
	}

	size_t pos = file_data.find(symbol);

	if(std::string::npos == pos)
	{
		std::cout << "could not find " << symbol << " in file " << filename << std::endl;
	}

	int firstbyte, secondbyte, thirdbyte, fourthbyte;
	std::stringstream ss;
	ss << std::hex << modversion.substr(0,2);
	ss >> firstbyte;
	ss.clear();
	ss << std::hex << modversion.substr(2,2);
	ss >> secondbyte;
	ss.clear();
	ss << std::hex << modversion.substr(4,2);
	ss >> thirdbyte;
	ss.clear();
	ss << std::hex << modversion.substr(6,2);
	ss >> fourthbyte;

	std::ofstream out;
	out.open(output_filename.c_str(), std::ios::out|std::ios::binary);
	if(out.is_open())
	{
		pos -= 4;
		for(size_t j=0;j<pos;j++)
		{
			out << file_data[j];
		}
		out << static_cast<char>(firstbyte);
		out << static_cast<char>(secondbyte);
		out << static_cast<char>(thirdbyte);
		out << static_cast<char>(fourthbyte);
		for(size_t j=pos+4;j<file_data.size();j++)
		{
			out << file_data[j];
		}
		out.close();
		std::cout << "patched." << std::endl;
	}
	else
	{
		std::cerr << "could not open output file: " << output_filename << std::endl;
	}

	return 0;
}

