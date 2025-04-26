//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//int main()
//{
//	std::ifstream inFile;
//
//	inFile.open("file.txt"); // To open a file. Can use a file path as well
//
//	if (inFile.fail())
//	{
//		std::cout << "Failed to open file" << std::endl;
//		return 1;
//	}
//
//	//data in file is like
//	// 42 34 56
//
//	std::string line;
//
//	// inFile >> line; // This only prints out "This" as space " " is the delimiter. It stops reading the data at the first space
//
//	while (getline(inFile, line))
//	{
//		std::stringstream stream(line); // Constructor takes in a string and converts it to a stream
//
//		int value;
//		while (stream >> value)
//		{
//			std::cout << value << std::endl; // This will print out 42, 34, 56 in each line
//		}
//	}
//
//	inFile.close();
//
//	return 0;
//}

