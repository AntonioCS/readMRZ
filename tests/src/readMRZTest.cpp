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
			"IDBEL590335801485120100200<<<<8512017F0901015BEL<<<<<<<<<<<7REINARTZ<<ULRIKE<KATIA<E<<<<<<"
		},
		{			
			"test_data/mrz_example_02.jpg",
			"IDFRABERTHIER<<<<<<<<<<<<<<<<<<<<<<<9409923102854CORINNE<<<<<<<6512068F4"
		},
		{			
			"test_data/mrz_example_03.jpg",
			"IOHRV000000000000000000000<<<<0000000M0000000HRV<<<<<<<<<<<0UZORAK<<SPECIMEN<<<<<<<<<<<<<<"
		},
		{
			"test_data/mrz_example_04.jpg",
			"IDAUT10000999<6<<<<<<<<<<<<<<<7109094F1112315AUT<<<<<<<<<<<6MUSTERFRAU<<ISOLDE<<<<<<<<<<<<"
		},
		{
			"test_data/mrz_example_05.jpg",
			"P<GBRJENNINGS<<PAUL<MICHAEL<<<<<<<<<<<<<<<<<0123456784GBR5011025M0810050<<<<<<<<<<<<<<00"
		},
		{
			"test_data/mrz_example_06.jpg",
			"P<HRVSPECIMEN<<SPECIMEN<<<<<<<<<<<<<<<<<<<<<10070070071HRV8212258F1407019<<<<<<<<<<<<<<06"
		},
		{
			"test_data/mrz_example_07.jpg",
			"P<KGZLOMAKO<<ALEKSEY<<<<<<<<<<<<<<<<<<<<<<<<AC17494593KGZ8301174M25102952170119831554749"
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