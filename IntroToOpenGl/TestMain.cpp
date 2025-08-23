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

	// Standard array
	std::array<int, 6> example5 = { 1,2,3,4,5, 10 };
	// Accessing the array
	std::cout << "Example5 Element 3: " << example5[2] << std::endl;
	// Accessing the size of the array
	std::cout << "Example5 Size: " << example5.size() << std::endl;

	// Multidimensional array
	// X is the number of rows
	// Y is the number of columns
	// Multidimensional arrays are basically an array of pointers that points to the first element 
	// of each array/row stored somewhere in memory
	// But in memory they are stored contiguously
	// We access them like
	// example4[row][column] and is converted to example4[row * width + column]
	int example4[4][4] = { {1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16} };
	// This refers to n amount of rows but each row has 4 elements
	int (*ptr2)[4] = example4;
	// This is a pointer to the first element of the first row
	int *ptr3 = example4[0];

	// Adding to the pointer we increment the column and the [] use to access the row
	// ptr2[3] -> Gets the row  (returns int* pointing to that row)
	// and ptr2[3] + 1 -> + 1 Gets the second element in the row as it moves the pointer forward
	std::cout << "Example 4[3][1]: " << *(ptr2[3] + 1) << std::endl;
	// This is just a pointer that jumps by row * width + column
	// You can write the above pointer like this
	std::cout << "Example 4[3][1] different : " << *((int*)ptr2 + (3 * 4 + 1)) << std::endl;
	// As it is contiguiously stored in memory so this should be the same as above
	std::cout << "Example 4[3][1] pointer : " << *(ptr3 + (3 * 4 + 1)) << std::endl;

    // Allocation of a 2D array on the heap
    int (*heapArray)[3] = new int[4][3];
	// Access heapArray[1][2] still converts to *(heapArray + 1*3 + 2)

	// ========== ONLY FOR UNDERSTANDING ===========

	// Below is a dynamic allocation of a 2D array
	// It does not give good performance because of cache misses but has a few advantages
	// 1. Variable Row Sizes
	//     arr2d[0] = new int[3];     // Row 0: 3 elements
	//	   arr2d[1] = new int[7];     // Row 1: 7 elements  
	//	   arr2d[2] = new int[2];     // Row 2: 2 elements
	//	   arr2d[3] = new int[10];    // Row 3: 10 elements
	// 2. Dynamic Row Addition/Removal
	// 3. Memory Efficiency for Sparse Data
	//    If many rows are empty or very short, you don't waste memory:
	// 4. Row Swapping Without Data Movement. Swap rows by just swapping pointers
	
	// Not actual implemntation in C++
	
	// Here we create an array of 5 pointers
	// and arr2d is a pointer to the first element
	// There is no integer or data as of now. There are just pointers pointing to nothing.
	// This is an array that contains memory addresses of 5 other arrays
	int ** arr2d = new int*[5]; // Allocating 8 * 5 = 40 bytes in the heap
	// this is basically
	arr2d[0] = nullptr; // Same for all the other elements

	// Populating this with array of integers
	// So now the first pointer points to an array of integers (basically the first element)
	arr2d[0] = new int[5]; // All these can be of different sizes
	// Same for the other pointers
	arr2d[1] = new int[5]; // All these can be of different sizes
	arr2d[2] = new int[5]; // All these can be of different sizes
	arr2d[3] = new int[5]; // All these can be of different sizes
	arr2d[4] = new int[5]; // All these can be of different sizes

	// Accessing elements
	// arr2d[row] gives us the pointer
	// arr2d[row][column] gives us the value
	arr2d[0][0] = 1; // First row, first column
	arr2d[0][1] = 2; // First row, second column and so on
	arr2d[0][2] = 3;
	arr2d[0][3] = 4;
	arr2d[0][4] = 5;
	
	arr2d[1][0] = 6; // Second row, first column
	arr2d[1][1] = 7; // Second row, second column and so on
	arr2d[1][2] = 8;
	arr2d[1][3] = 9;
	arr2d[1][4] = 10;

	// Deleting the 2D array
	for (int i = 0; i < 5; i++)
	{
		delete[] arr2d[i];
	}
	delete[] arr2d;

	// 3D array
	int *** arr3d = new int**[5];
	for (int i = 0; i < 5; i++)
	{
		arr3d[i] = new int*[5];
		for (int j = 0; j < 5; j++)
		{
			// arr3d[i] -> dereferencing the first part
			// arr3d[i][j] -> dereferencing the second part
			arr3d[i][j] = new int[5];
		}
	}
	// Accessing Elements
	arr3d[0][0][0] = 1;
	arr3d[0][0][1] = 2;

	// Deleting the 3D array
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			delete[] arr3d[i][j];
		}
		delete[] arr3d[i];
	}
	delete[] arr3d;

	// Memory allocation of 2D array
	// As 2D arrays are basically an array of pointers that points to the first element of each array/row stored somewhere in memory
	// these are not stored together in a contiguos block
	// So whenever we swtich a row or move to the next pointer to read/write data then it can lead to cache miss
	// This is pretty slow use single dimensional array mostly

	// Alertnative approach. Actually how it works in C++
	// Stored contiguously so it is faster
	int * arr2d_single = new int[5 * 5];
	for(int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			arr2d_single[i * 5 + j] = i * 5 + j;
		}
	}

	delete[] arr2d_single;
	
	// =========================================

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
