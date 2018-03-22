#include "Assignment.hpp"

#include <iostream>

int main(void)
{
	GooglePageRank test;
	return 0;
}

GooglePageRank::GooglePageRank()
{

	std::vector<double> listW;
	std::vector< std::vector<double> > matrixG;
	std::vector< std::vector<double> > matrixS;

	std::vector< std::vector<double> > matrixM;
	std::vector<double> matrixRank;

	std::string fileName = "connec-tivity.txt";

	//Get matrix data from text file 
	printf("Initial input Data:\n");
	listW = getData(fileName);

	//Make matrix by vector from listW
	matrixG = getMatrix(listW);

	printf("Initial input Data on Matrix:\n");
	showMatrix(matrixG);

	//Apply importance baced on each columns  
	matrixS = ApplyImportance(matrixG);

	printf("ApplyImportance:\n");
	showMatrix(matrixS);

	//Apply Probability baced on each columns 
	matrixS = ApplyProbability(matrixS);

	printf("ApplyProbability:\n");
	showMatrix(matrixS);

	//Apply Transition baced on each columns 
	printf("ApplyTransition:\n");
	matrixM = ApplyTransition(matrixS);

	//initialize matrixRank
	matrixRank.resize(matrixM.size());
	matrixRank = initRank(matrixRank);

	//Calcurate matrixM multiply by matrixRank
	matrixRank = getRank(matrixM, matrixRank);

	printf("Page Rank before divide by sum:\n");
	showRank(matrixRank);
	
	//Calcurate PageRank from matrixRank
	matrixRank = getPageRank(matrixRank);
	printf("Page Rank:\n");
	showRank(matrixRank);
}

//Get matrix data from text file and show it as line from
std::vector<double> GooglePageRank::getData(std::string fileName)
{
	std::string str;
	std::vector<double> list;

	std::ifstream ifs(fileName);

	while(std::getline(ifs, str))
	{
		std::string tmp;
		std::istringstream stream(str);

		std::cout << " ";
		while (getline(stream, tmp, ' '))
		{
			list.push_back(std::stod(tmp));

			std:: cout << tmp << " ";
		}
	}
	std::cout << "\n\n";
	return list;
}

//Make matrix by vector from listW
std::vector< std::vector<double> > GooglePageRank::getMatrix(std::vector<double> list)
{
	std::string str;

	double sqrtNumList = std::sqrt(list.size());

	std::vector< std::vector<double> > matrix;

	matrix.resize(sqrtNumList);
	for (int i = 0; i < sqrtNumList; i++) {
		matrix[i].resize(sqrtNumList);
	}

	int count = 0;
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix.size(); j++)
		{

			matrix[i][j] = list[count++];
		}
	}

	return matrix;
}

//Apply importance baced on each columns. 
//Each part which has connection will get number od 1 / number of connection
std::vector< std::vector<double> > GooglePageRank::ApplyImportance(std::vector< std::vector<double> > matrix)
{
	int count;
	double importance;

	for (int i = 0; i < matrix.size(); i++)
	{
		count = 0;
		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[j][i] == 1)
			{
				count++;
			}
		}

		importance = 1 / (double)count;

		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[j][i] == 1)
			{
				matrix[j][i] = importance;
			}
		}
	}

	return matrix;
}

//Apply Probability baced on each columns 
//Each part which has connection will get number od 1 / number of pages
std::vector< std::vector<double> > GooglePageRank::ApplyProbability(std::vector< std::vector<double> > matrix)
{
	double sum;
	double probability;

	for (int i = 0; i < matrix.size(); i++)
	{
		sum = 0;
		for (int j = 0; j < matrix.size(); j++)
		{
			sum += matrix[j][i];
		}


		if (sum == 0)
		{
			probability = (1 - sum) / (double)matrix.size();

			for (int j = 0; j < matrix.size(); j++)
			{
				matrix[j][i] += probability;
			}
		}
	}

	return matrix;
}

//Apply Transition which calcurate for matrinX
//After initializing matrixQ and, matrixM, calculate for S and Q, finally conbine these two as the result
std::vector< std::vector<double> > GooglePageRank::ApplyTransition(std::vector< std::vector<double> > matrix)
{
	std::vector< std::vector<double> > matrixS = matrix;
	std::vector< std::vector<double> > matrixQ = initMatrixQ((int)matrix.size());
	std::vector< std::vector<double> > matrixM = initMatrixM((int)matrix.size());;

	//M = p * S + (1 - p) * Q;
	matrixS = prepareS(matrixS, p);// Get S'
	matrixQ = prepareQ(matrixQ, p);// Get Q'

	printf("matrixS ( matrixS = %.2f * S ) :\n", p);
	showMatrix(matrixS);
	printf("matrixQ ( matrixQ = (1 - %.2f ) * Q) :\n", p);
	showMatrix(matrixQ);

	//M = S' + Q'
	for (int i = 0; i < matrixM.size(); i++)
	{
		for (int j = 0; j < matrixM.size(); j++)
		{
			matrixM[i][j] = matrixS[i][j] + matrixQ[i][j];
		}
	}

	printf("matrixM ( matrixM = matrixS + matrixQ ) :\n");
	showMatrix(matrixM);

	return matrixM;
}
//initializing matrixQ which filled up by 1 / (double)matrix.size();
std::vector< std::vector<double> > GooglePageRank::initMatrixQ(int size)
{
	std::vector< std::vector<double> > matrix;

	matrix.resize(size);

	for (int i = 0; i<size; i++) {
		matrix[i].resize(size);
	}

	for (int i = 0; i < matrix.size(); i++)
	{

		for (int j = 0; j < matrix.size(); j++)
		{
			matrix[i][j] = 1 / (double)matrix.size();
		}
	}

	printf("initial matrixQ:\n");
	showMatrix(matrix);

	return matrix;
}

//initializing matrixM which filled up by 0;
std::vector< std::vector<double> > GooglePageRank::initMatrixM(int size)
{
	std::vector< std::vector<double> > matrix;

	matrix.resize(size);

	for (int i = 0; i<size; i++) {
		matrix[i].resize(size);
	}

	printf("initial matrixM:\n");
	showMatrix(matrix);
	return matrix;
}

//calculate p * S and return the result
std::vector< std::vector<double> > GooglePageRank::prepareS(std::vector< std::vector<double> >matrixS, double p)
{
	// S' = p * S

	for (int i = 0; i < matrixS.size(); i++)
	{
		for (int j = 0; j < matrixS.size(); j++)
		{
			matrixS[i][j] *= p;
		}
	}

	return matrixS;
}

//calculate(1 - p) * Q and return the result
std::vector< std::vector<double> > GooglePageRank::prepareQ(std::vector< std::vector<double> >matrixQ, double p)
{
	// Q' = (1 - p) * Q;

	for (int i = 0; i < matrixQ.size(); i++)
	{
		for (int j = 0; j < matrixQ.size(); j++)
		{
			matrixQ[i][j] *= (1 - p);
		}
	}
	return matrixQ;
}

//initialize matrixRank fill 1 to each vector 
std::vector<double> GooglePageRank::initRank(std::vector<double>matrixRank)
{

	for (int i = 0; i < matrixRank.size(); i++)
	{
		matrixRank[i] = 1;
	}

	return matrixRank;
}

//Calcurate matrixM multiply by matrixRank
//And return the result
std::vector<double> GooglePageRank::getRank(std::vector< std::vector<double> > matrixM, std::vector<double>matrixRank)
{
	double sum;
	std::vector<double> tempMatrixRank;

	do {
		tempMatrixRank = matrixRank;

		for (int i = 0; i < matrixRank.size(); i++)
		{
			sum = 0;

			for (int j = 0; j < matrixM.size(); j++)
			{
				sum += tempMatrixRank[j] * matrixM[i][j];
			}

			matrixRank[i] = sum;
		}
	} while (matrixRank != tempMatrixRank);

	return matrixRank;

}

//Calcurate PageRank from matrixRank
//Devide each vector array by sum of total array
//This calculation will continue the result is not changing for converging.
std::vector<double> GooglePageRank::getPageRank(std::vector<double> matrix)
{
	double sum = 0;

	for (int i = 0; i < matrix.size(); i++)
	{
		sum += matrix[i];
	}

	for (int i = 0; i < matrix.size(); i++)
	{
		matrix[i] /= sum;
	}

	return matrix;
}

//Display Matrix
void GooglePageRank::showMatrix(std::vector< std::vector<double> > matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		std::cout << " ";
		for (int j = 0; j < matrix.size(); j++)
		{
			std::cout << std::setw(10) << std::left << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\n";
}

//Display RankList
void GooglePageRank::showRank(std::vector<double> matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		std::cout << " ";
		std::cout<< (char)(i + ASCII_OFFSET) << " : " << matrix[i] << "\n";
	}

	std::cout << "\n";
}