#pragma once
#include <iostream>
#include <iomanip>
#include <vector>

class GooglePageRank
{
	private:
		static constexpr int ASCII_OFFSET = 65;
		static constexpr double p = 0.85;

	public:
		GooglePageRank();
		std::vector<std::vector<double>> ApplyImportance(std::vector<std::vector<double>> matrix);
		std::vector<std::vector<double>> ApplyProbability(std::vector<std::vector<double>> matrix);
		std::vector<std::vector<double>> ApplyAdjust(std::vector<std::vector<double>> matrix);
		std::vector<std::vector<double>> ApplyTransition(std::vector<std::vector<double>> matrix);
		std::vector<std::vector<double>> initMatrixQ(int size);
		std::vector<std::vector<double>> initMatrixM(int size);
		std::vector<std::vector<double>> prepareS(std::vector<std::vector<double>> matrixS, double p);
		std::vector<std::vector<double>> prepareQ(std::vector<std::vector<double>> matrixS, double p);
		std::vector<double> initRank(std::vector<double> matrixRank);
		std::vector<double> getRank(std::vector<std::vector<double>> matrix, std::vector<double> matrixRank);
		std::vector<double> getPageRank(std::vector<double> matrix);
		void showMatrix(std::vector<std::vector<double>> matrix);
		void showRank(std::vector<double> matrix);
};