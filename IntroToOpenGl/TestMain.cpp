//#include <iostream>
//#include <map>
//#include <unordered_map>
//#include <vector>
//#include <string>
//
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