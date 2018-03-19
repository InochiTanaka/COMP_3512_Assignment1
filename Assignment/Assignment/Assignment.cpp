#include "Assignment.hpp"

#include <iostream>

int main(void)
{
	GooglePageRank test;
	return 0;
}

GooglePageRank::GooglePageRank()
{

	std::vector< std::vector<double> > matrixW;
	std::vector< std::vector<double> > matrixG;
	std::vector< std::vector<double> > matrixS;

	std::vector< std::vector<double> > matrixM;

	std::vector<double> matrixRank;


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

	//matrixG = {
	//	{0, 1, 1, 1, 1, 1, 1},
	//	{0, 0, 0, 1, 0, 1, 1},
	//	{0, 1, 0, 1, 1, 1, 1},
	//	{1, 1, 1, 0, 0, 1, 1},
	//	{1, 1, 0, 0, 0, 1, 1},
	//	{0, 0, 1, 0, 1, 0, 1},
	//	{1, 1, 0, 1, 0, 1, 0}
	//};

	printf("Initial input Data:\n");
	showMatrix(matrixG);

	matrixS = ApplyImportance(matrixG);

	//ApplyImportance
	printf("ApplyImportance:\n");
	showMatrix(matrixS);

	//matrixS should be like:
	//matrixS = {
	//	{ 0, 0.5, 0.5, 0 },
	//	{ 0.5, 0, 0.5, 0 },
	//	{ 0.5, 0.5, 0, 0 },
	//	{ 0, 0, 0, 0 }
	//	};

	matrixS = ApplyProbability(matrixS);

	printf("ApplyProbability:\n");
	showMatrix(matrixS);

	//matrixS = ApplyAdjust(matrixS);

	//matrixS should be like:
	//matrixS = {
	//	{ 0, 0.5, 0.5, 0.25 },
	//	{ 0.5, 0, 0.5, 0.25 },
	//	{ 0.5, 0.5, 0, 0.25 },
	//	{ 0, 0, 0, 0.25 }
	//	};

	printf("ApplyTransition:\n");

	matrixM = ApplyTransition(matrixS);

	matrixRank.resize(matrixM.size());
	matrixRank = initRank(matrixRank);
	matrixRank = getRank(matrixM, matrixRank);

	printf("Sum of Row:\n");
	showRank(matrixRank);
	
	matrixRank = getPageRank(matrixRank);
	printf("Page Rank:\n");
	showRank(matrixRank);
}

std::vector< std::vector<double> > GooglePageRank::ApplyImportance(std::vector< std::vector<double> > matrix)
{
	int count;
	double importance;

	for (int i = 0; i < matrix.size(); i++)
	{
		count = 0;
		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[i][j] == 1)
			{
				count++;
			}
		}

		importance = 1 / (double)count;

		for (int j = 0; j < matrix.size(); j++)
		{
			if (matrix[i][j] == 1)
			{
				matrix[i][j] = importance;
			}
		}
	}

	return matrix;
}

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


		if (sum != 1)
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

std::vector< std::vector<double> > GooglePageRank::ApplyAdjust(std::vector< std::vector<double> > matrix)
{
	double sum;
	double magnif;

	for (int i = 0; i < matrix.size(); i++)
	{
		sum = 0;
		
		for (int j = 0; j < matrix.size(); j++)
		{
			sum += matrix[i][j];
		}

		magnif = 1 / sum;

		for (int j = 0; j < matrix.size(); j++)
		{
			matrix[i][j] *= magnif;
		}
	}

	return matrix;
}

std::vector< std::vector<double> > GooglePageRank::ApplyTransition(std::vector< std::vector<double> > matrix)
{
	std::vector< std::vector<double> > matrixS = matrix;
	std::vector< std::vector<double> > matrixQ = initMatrixQ((int)matrix.size());
	std::vector< std::vector<double> > matrixM = initMatrixM((int)matrix.size());;

	//M = p * S + (1 - p) * Q;
	matrixS = prepareS(matrixS, p);// Get S'
	matrixQ = prepareQ(matrixQ, p);// Get Q'

	printf("matrixS:\n");
	showMatrix(matrixS);
	printf("matrixQ:\n");
	showMatrix(matrixQ);

	//M = S' + Q'
	for (int i = 0; i < matrixM.size(); i++)
	{
		for (int j = 0; j < matrixM.size(); j++)
		{
			matrixM[i][j] = matrixS[i][j] + matrixQ[i][j];
		}
	}

	printf("matrixM ( M = %.2f * S + (1 - %.2f) * Q ) :\n", p, p);
	showMatrix(matrixM);

	return matrixM;
}

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

	//showMatrix(matrix);

	return matrix;
}

std::vector< std::vector<double> > GooglePageRank::initMatrixM(int size)
{
	std::vector< std::vector<double> > matrix;

	matrix.resize(size);

	for (int i = 0; i<size; i++) {
		matrix[i].resize(size);
	}

	return matrix;
}

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

std::vector<double> GooglePageRank::initRank(std::vector<double>matrixRank)
{
	for (int i = 0; i < matrixRank.size(); i++)
	{

		matrixRank[i] = 1;

	}

	return matrixRank;
}

std::vector<double> GooglePageRank::getRank(std::vector< std::vector<double> > matrixM, std::vector<double>matrixRank)
{
	double sum;

	for (int i = 0; i < matrixRank.size(); i++)
	{
		sum = 0;

		for (int j = 0; j < matrixM.size(); j++)
		{
			sum += matrixM[i][j];
		}

		matrixRank[i] *= sum;

	}

	return matrixRank;

}

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

void GooglePageRank::showMatrix(std::vector< std::vector<double> > matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		std::cout << " ";
		for (int j = 0; j < matrix.size(); j++)
		{
			std::cout << std::setw(8) << std::left << matrix[i][j];
		}
		std::cout << "\n";
	}

	std::cout << "\n";
}

void GooglePageRank::showRank(std::vector<double> matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		std::cout << " ";
		std::cout<< (char)(i + ASCII_OFFSET) << " : " << matrix[i] << "\n";
	}

	std::cout << "\n";
}