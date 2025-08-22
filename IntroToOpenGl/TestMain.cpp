#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <array>

#pragma region  ARRAYS

void PassArry(int a[], int length);
void PassArry_2(const int* a, int length);

int main()
{
	int arr[5] = { 1, 2, 3, 4, 5 };
	PassArry(arr, 5);
	PassArry_2(arr, 5);

	// Allocated a size for 5 integers
	int example[5]; // Stack Allocated

	int* anotherExample = new int[5]; // Heap Allocated
	delete[] anotherExample; // Heap deallocation

	// Initialize the values of the array
	example[0] = 1;
	example[1] = 2;
	example[2] = 3;
	example[3] = 4;
	example[4] = 5;

	// Accessing and modifying/initializing the array using a loop
	// sizeof(exaple) gives us the memory size
	// sizeof(example[0]) gives us the memory size of a single element
	// we divide to get the number of elements
	// THIS ONLY WORKS IF THE ARRAY IS ALLOCATED ON THE STACK
	// When we pass it into a function it will be converted to a pointer so it will be hard to access the count
	for (int i = 0; i < sizeof(example) / sizeof(example[0]); i++)
	{
		example[i] = i;
		std::cout << "Example: " << example[i] << std::endl;
	}

	// Pointer to the array
	int * ptr = example;

	std::cout << "Pointer 1 to example: " << *(ptr) << std::endl;
	// Pointer increment to traverse the array
	std::cout << "Pointer increment to example: " << *(ptr + 1) << std::endl;
	std::cout << "Pointer increment to example: " << *(ptr + 3) << std::endl;

	// Change the value of the pointer makes changes to the array
	*(ptr + 4) = 60;
	std::cout << "Pointer value change to example: " << *(ptr + 4) << std::endl;

	// Prints the value of the array
	std::cout << "Example: " << example[0] << std::endl;
	// Prints the memory address of the array
	std::cout << "Example: " << example << std::endl;

	// We can initialize the array in one line
	int example2[5] = { 1,2,3,4,5 };

	// The array will be initialized with 0s as the rest of the elements are not initialized
	int example3[5] = { 1,2 };

	std::cout << "Example3 Element 3: " << example3[2] << std::endl;

	// Multidimensional array
	// X is the number of rows
	// Y is the number of columns
	// Multidimensional arrays are basically an array of pointers that points to the first element 
	// of each array/row stored somewhere in memory
	int example4[4][4] = { {1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16} };
	// This refers to n amount of rows but each row has 4 elements
	int (*ptr2)[4] = example4;
	// Adding to the pointer we increment the row and the [] use to access the column
	std::cout << "LAST: " << *(ptr2[3] + 1) << std::endl;

	// Standard array
	std::array<int, 6> example5 = { 1,2,3,4,5, 10 };
	// Accessing the array
	std::cout << "Example5 Element 3: " << example5[2] << std::endl;
	// Accessing the size of the array
	std::cout << "Example5 Size: " << example5.size() << std::endl;

	return 0;
}

// int a[] is basically int *a, which is a pointer to the first element of the array
// we only have the pointer to the array so we cannot access its length
void PassArry(int a[], int length)
{
	float count = sizeof(a) / sizeof(a[0]);

	std::cout << "size of a: " << sizeof(a) << std::endl; // Size of pointer that is 8 bytes (64 bit machine)
	std::cout << "size of first element of a: " << sizeof(a[0]) << std::endl; // Size of int which is 4 bytes
	std::cout << "INCORRECT COUNT: " << count << std::endl; // Count is 2 which is incorrect

	for (int i = 0; i < length; i++)
	{
		std::cout << a[i] << std::endl;
	}
}

// Basically the parameter looks like
// As we are passing a pointer then we are passing by reference and not by value
// So we will be working on the original array
// Its a good practice to pass array as const so that we don't modify the actual array by mistake
void PassArry_2(const int* a, int length)
{
	for (int i = 0; i < length; i++)
	{
		std::cout << "Pass array 2: " << a[i] << std::endl;
	}
}

#pragma endregion


#pragma region  MAPS
//struct CityRecord
//{
//	std::string cityName;
//	unsigned int population;
//	double latitude, longitude;
//
//	CityRecord() 
//	{
//		cityName = "";
//		population = 0;
//		latitude = 0;
//		longitude = 0;
//	}
//
//	CityRecord(const std::string& name, unsigned int pop, double lat, double lon)
//	{
//		cityName = name;
//		population = pop;
//		latitude = lat;
//		longitude = lon;
//	}
//
//	// Overload the << operator for CityRecord
//	friend std::ostream& operator<<(std::ostream& os, const CityRecord& city)
//	{
//		os << "City: " << city.cityName
//			<< ", Population: " << city.population
//			<< ", Latitude: " << city.latitude
//			<< ", Longitude: " << city.longitude;
//		return os;
//	}
//
//	bool operator==(const CityRecord& other) const
//	{
//		return cityName == other.cityName; 
//	}
//
//	// Used for sorting if we have a map rather than an unordered map. Especially if we have a custom type that we are hashing
//	// This is also responsible for the unique key in the map
//	bool operator<(const CityRecord& other) const
//	{
//		return population < other.population; // This will just order them as per the population
//	}
//};
//
//// Hash function for a custom type this time its CityRecord
//namespace std
//{
//	// Template specialization for CityRecord
//	template<>
//	struct hash<CityRecord>
//	{
//		// Call operator that returns a size_t
//		size_t operator()(const CityRecord& key) const
//		{
//			// hash is a template class and we are also calling the operator(). So techincally its like constructing it and then calling it
//			return hash<std::string>()(key.cityName);
//		}
//	};
//}
//
//int main()
//{
//	std::unordered_map <CityRecord, unsigned int> foundedYear;
//
//	foundedYear[CityRecord("New York", 2000, 2.5, 5.6)] = 2000;
//
//	std::unordered_map<std::string, CityRecord> cityMap;
//
//	cityMap["New York"] = CityRecord("New York", 2000, 2.5, 5.6);
//	cityMap["Shimla"] = CityRecord("Shimla", 6000, 7.88, 99.2);
//	cityMap["Solan"] = CityRecord("Solan", 8000, 592.5, 992.1);
//	cityMap["Delhi"] = CityRecord("Delhi", 8000, 54.2, 45.3);
//
//	// Fetch information
//	CityRecord shimlaData = cityMap["Shimla"];
//	std::cout << "Population of Shimla: " << shimlaData.population << std::endl;
//	std::cout << "Shimla Data: " << shimlaData << std::endl;
//
//	// Adds new record
//	CityRecord chandigarhCity = cityMap["Chandigarh"]; // This creates a copy and map stays unchanged
//	chandigarhCity.population = 100000;
//
//	std::cout << "Chandigarh Data: " << chandigarhCity.population << std::endl;
//	std::cout << "Chandigarh Data: " << cityMap["Chandigarh"].population << std::endl;
//
//	// If you don't want to add a new record with [] operator then use .at()
//	if (cityMap.find("rohru") != cityMap.end()) // Check if the key exists
//	{
//		CityRecord& rohruCity = cityMap.at("rohru"); // as the key doesn't exist it throws an exception
//		std::cout << "Rohru found" << std::endl;
//	}
//
//	// Use it as a reference to update the map as well
//	CityRecord& chandigarhCityRef = cityMap["Chandigarh"];
//	chandigarhCityRef.population = 200000;
//
//	std::cout << "Chandigarh Data: " << chandigarhCityRef.population << std::endl;
//	std::cout << "Chandigarh Data: " << cityMap["Chandigarh"].population << std::endl;
//
//	// Iterate
//	for (auto it = cityMap.begin(); it != cityMap.end(); it++)
//	{
//		std::cout << "Key: " << it->first << ", Value: " << it -> second << std::endl;
//		std::cout << &(*it) << std::endl; // Address of the key-value pair
//	}
//
//	//Erase
//	cityMap.erase("Shimla");
//
//	// C++ 17 approach to iterating
//	for (auto& [key, value] : cityMap)
//	{
//		std::cout << "Key: " << key << ", Value: " << value << std::endl;
//	}
//
//	return 0;
//}

#pragma endregion
