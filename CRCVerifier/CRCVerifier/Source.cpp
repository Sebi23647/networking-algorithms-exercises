
#include <iostream>
#include <string>

bool isBinary(const std::string& s) {
    for (char c : s) {
        if (c != '0' && c != '1') {
            return false;
        }
    }
    return true;
}

std::string trimLeadingZeros(const std::string& s) {
    size_t pos = s.find_first_not_of('0');
    if (pos == std::string::npos) {
        return "0";
    }
    return s.substr(pos);
}

std::string xorOperation(const std::string& a, const std::string& b) {
    std::string result = a;
    for (size_t i = 0; i < b.size(); ++i) {
        result[i] = (a[i] == b[i]) ? '0' : '1';
    }
    return result;
}

int main() {
    std::string mesaj, polinom;

    std::cout << "Introduceti mesajul binar: ";
    std::cin >> mesaj;
    std::cout << "Introduceti polinomul generator (coeficienti binari): ";
    std::cin >> polinom;

    if (!isBinary(mesaj)) {
        std::cout << "Eroare: Mesajul introdus nu este binar." << std::endl;
        return 1;
    }
    if (!isBinary(polinom)) {
        std::cout << "Eroare: Polinomul generator nu este binar." << std::endl;
        return 1;
    }

    std::string polinomTrimat = trimLeadingZeros(polinom);

    if (polinomTrimat == "0") {
        std::cout << "Eroare: Polinomul generator nu poate fi format doar din zerouri." << std::endl;
        return 1;
    }

    if (mesaj.size() <= polinomTrimat.size()) {
        std::cout << "Eroare: Lungimea mesajului trebuie sa fie mai mare decat numarul de coeficienti ai polinomului generator (dup? eliminarea zerourilor de la început)." << std::endl;
        return 1;
    }

    std::cout << "Input valid! Mesajul si polinomul sunt corecte." << std::endl;

    int grad = polinomTrimat.size() - 1;
    std::string mesajExtins = mesaj;
    mesajExtins.append(grad, '0');

    std::cout << "Mesajul extins: " << mesajExtins << std::endl;

    std::string rest = mesajExtins;
    while (true) {
        std::cout << "Before XOR: " << rest << std::endl;

        std::string restTrimmed = trimLeadingZeros(rest);

        if (restTrimmed.size() < polinomTrimat.size()) {
            break;
        }

        std::string temp = xorOperation(restTrimmed.substr(0, polinomTrimat.size()), polinomTrimat);

        rest.replace(rest.size() - restTrimmed.size(), polinomTrimat.size(), temp);

        std::cout << "After XOR: " << rest << std::endl;
    }

    rest = trimLeadingZeros(rest);
    std::cout << "Restul final: " << rest << std::endl;


    std::string rezultatFinal = mesajExtins;
    int offset = rezultatFinal.size() - rest.size();
    for (size_t i = 0; i < rest.size(); ++i) {
        rezultatFinal[offset + i] = (rezultatFinal[offset + i] == rest[i]) ? '0' : '1';
    }

    std::cout << "Rezultatul final: " << rezultatFinal << std::endl;

    return 0;
}

