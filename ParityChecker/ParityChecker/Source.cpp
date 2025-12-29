#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

bool IsBinaryString(const std::string& str) {
    for (char c : str) {
        if (c != '0' && c != '1') {
            return false;
        }
    }
    return true;
}

void DisplayInitialMatrix(const std::vector<std::vector<char>>& matrix) {
    for (const auto& row : matrix) {
        for (const char& c : row) {
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }
}

void DisplayParityMatrix(const std::vector<std::vector<char>>& matrix) {
    int numRows = matrix.size();
    int numCols = matrix[0].size();

    for (int i = 0; i < numRows - 1; ++i) {
        for (int j = 0; j < numCols - 1; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << "| " << matrix[i][numCols - 1] << '\n';
    }

    for (int j = 0; j < numCols - 1; ++j) {
        std::cout << "--";
    }
    std::cout << "-\n";

    for (int j = 0; j < numCols - 1; ++j) {
        std::cout << matrix[numRows - 1][j] << ' ';
    }
    std::cout << "| " << matrix[numRows - 1][numCols - 1] << '\n';
}

std::vector<std::vector<char>> CalculateParityBits(const std::vector<std::vector<char>>& matrix) {
    int numRows = matrix.size();
    int numCols = matrix[0].size();

    std::vector<std::vector<char>> parityMatrix(numRows + 1, std::vector<char>(numCols + 1, '0'));

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            parityMatrix[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < numRows; ++i) {
        int countOnes = 0;
        for (int j = 0; j < numCols; ++j) {
            if (matrix[i][j] == '1') countOnes++;
        }
        parityMatrix[i][numCols] = (countOnes % 2 == 0) ? '0' : '1';
    }

    for (int j = 0; j < numCols; ++j) {
        int countOnes = 0;
        for (int i = 0; i < numRows; ++i) {
            if (matrix[i][j] == '1') countOnes++;
        }
        parityMatrix[numRows][j] = (countOnes % 2 == 0) ? '0' : '1';
    }

    int countOnesOnRow = 0;
    int countOnesOnCol = 0;
    for (int i = 0; i < numRows; ++i) {
        if (parityMatrix[i][numCols] == '1') countOnesOnRow++;
    }
    for (int j = 0; j < numCols; ++j) {
        if (parityMatrix[numRows][j] == '1') countOnesOnCol++;
    }

    if (countOnesOnRow % 2 == 1 && countOnesOnCol % 2 == 1) {
        parityMatrix[numRows][numCols] = '1';
    }

    return parityMatrix;
}

void CorruptMessage(std::vector<std::vector<char>>& matrix) {
    int numRows = matrix.size();
    int numCols = matrix[0].size();

    int randomRow = rand() % numRows;
    int randomCol = rand() % numCols;

    matrix[randomRow][randomCol] = (matrix[randomRow][randomCol] == '1') ? '0' : '1';

    std::cout << "Bitul de pe pozitia (" << randomRow << ", " << randomCol << ") a fost corupt." << std::endl;
}

void IdentifyCorruptedBit(const std::vector<std::vector<char>>& originalMatrix,
    const std::vector<std::vector<char>>& calculatedParityCorruptMatrix) {

    int numRows = originalMatrix.size();
    int numCols = originalMatrix[0].size();

    int corruptedRow = -1;
    int corruptedCol = -1;

    for (int i = 0; i < numRows - 1; ++i) {
        if (originalMatrix[i][numCols - 1] != calculatedParityCorruptMatrix[i][numCols - 1]) {
            corruptedRow = i;
            break;
        }
    }

    for (int j = 0; j < numCols - 1; ++j) {
        if (originalMatrix[numRows - 1][j] != calculatedParityCorruptMatrix[numRows - 1][j]) {
            corruptedCol = j;
            break;
        }
    }

    if (corruptedRow != -1 && corruptedCol != -1) {
        std::cout << "Bitul corupt se afla pe pozitia (" << corruptedRow << ", " << corruptedCol << ")." << std::endl;
    }
    else {
        std::cout << "Nu a fost identificat niciun bit corupt." << std::endl;
    }
}

int main() {
    std::string input;
    srand(static_cast<unsigned int>(time(0)));

    std::cout << "Introduceti un sir format dintr-un numar multiplu de 7 caractere binare: ";
    std::cin >> input;

    if (input.length() % 7 != 0 || !IsBinaryString(input)) {
        std::cerr << "Eroare: sirul trebuie sa fie format dintr-un numar multiplu de 7 caractere binare." << std::endl;
        return 1;
    }

    int numRows = input.length() / 7;
    std::vector<std::vector<char>> matrix(numRows, std::vector<char>(7));

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < 7; ++j) {
            matrix[i][j] = input[i * 7 + j];
        }
    }

    std::cout << "Matricea initiala este:" << std::endl;
    DisplayInitialMatrix(matrix);

    std::vector<std::vector<char>> parityMatrix = CalculateParityBits(matrix);
    std::cout << "Matricea cu bitii de paritate este:" << std::endl;
    DisplayParityMatrix(parityMatrix);

    std::vector<std::vector<char>> corruptedMatrix = matrix;
    CorruptMessage(corruptedMatrix);

    std::vector<std::vector<char>> calculatedParityCorruptMatrix = CalculateParityBits(corruptedMatrix);
    std::cout << "Matricea corupta este:" << std::endl;
    DisplayParityMatrix(calculatedParityCorruptMatrix);

    IdentifyCorruptedBit(parityMatrix, calculatedParityCorruptMatrix);

    return 0;
}
