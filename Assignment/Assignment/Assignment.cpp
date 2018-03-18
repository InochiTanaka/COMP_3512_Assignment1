#include "Assignment.hpp"

int main(void)
{
	GooglePageRank test;
	return 0;
}

GooglePageRank::GooglePageRank()
{
	std::vector< std::vector<int> > matrixW;
	std::vector< std::vector<int> > matrixG;

	matrixW = {
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
		};

	matrixG = { 
		{ 0, 1, 1, 0 }, 
		{ 1, 0, 1, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 0, 0 }
		};


}