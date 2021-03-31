#include <catch2/catch.hpp>
#include <readMRZ/readMRZ.h>
#include <vector>
#include <utility> //std::pair
#include <string>


TEST_CASE("Read MRZ from images", "[readMRZ]")
{
	std::vector<std::pair<std::string, std::string>> testData{
		{			
			"test_data/mrz_example_01.jpg",
			"I0BEL590335801485120100200<<<<8512017F0901015BEL<<<<<<<<<<<7REINARTZ<<ULRIKE<KATIA<E<<<<<<"			
		},
		{			
			"test_data/mrz_example_02.jpg",
			"IDFRABERTHIER<<<<<<<<<<<<<<<<<<<<<<<9409923102854CORINNE<<<<<<<6512068F4"
		},
		{			
			"test_data/mrz_example_03.jpg",
			"I0HRV000000000000000000000<<<<0000000MO000000HRY<<<<<<<<<<<OUZORAK<<SPECIMEN<<<<<<<<<<<<<<"
		}
	};
		
	readMRZ::readMRZ read_mrz{ "tessdata/" };

	for (const auto& pair : testData)
	{
		const auto& [imagePath, expectedResult] = pair;
		const auto result = read_mrz.processImage(imagePath);
		
		REQUIRE(expectedResult == result);
	}	
}