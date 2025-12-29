
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class Token {
public:
    string srcIP;
    string destIP;
    string message;
    bool isFree;
    bool hasArrived;
    vector<string> history;

    Token() : isFree(true), hasArrived(false) {}
};

class Computer {
public:
    string ip;
    vector<string> buffer;

    Computer(string ip) : ip(ip) {}

    string getBuffer() const {
        if (buffer.empty()) return "null";
        string result;
        for (const string& msg : buffer) {
            if (!result.empty()) result += " ; ";
            result += msg;
        }
        return result;
    }
};

string generateRandomIP() {
    return to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
}

void initializeNetwork(vector<Computer>& network) {
    for (int i = 0; i < 10; ++i) {
        string ip;
        bool unique;
        do {
            unique = true;
            ip = generateRandomIP();
            for (const auto& computer : network) {
                if (computer.ip == ip) {
                    unique = false;
                    break;
                }
            }
        } while (!unique);
        network.push_back(Computer(ip));
    }
}

int findComputerIndex(const vector<Computer>& network, const string& ip) {
    for (int i = 0; i < network.size(); ++i) {
        if (network[i].ip == ip) {
            return i;
        }
    }
    return -1;
}

void displayNetwork(const vector<Computer>& network) {
    for (int i = 0; i < network.size(); ++i) {
        cout << "C" << i << "(" << network[i].ip << ") -> " << network[i].getBuffer() << endl;
    }
}


void displayHistory(const vector<Computer>& network, const vector<string>& history) {
    cout << "History: ";
    for (const string& ip : history) {
        int index = findComputerIndex(network, ip);
        cout << "C" << index << " ; ";
    }
    cout << endl;
}

void simulateTokenRing(vector<Computer>& network, Token& token, bool clockwise, int& currentIndex) {
    int steps = network.size();
    int destIndex = findComputerIndex(network, token.destIP);

    for (int step = 0; step < steps * 2; ++step) {
        if (clockwise) {
            currentIndex = (currentIndex + 1) % network.size();
        }
        else {
            currentIndex = (currentIndex - 1 + network.size()) % network.size();
        }

        token.history.push_back(network[currentIndex].ip);

        if (network[currentIndex].ip == token.srcIP && token.isFree) {
            token.isFree = false;
            cout << "C" << currentIndex << ": A preluat jetonul" << endl;
            cout << "C" << currentIndex << ": Muta jetonul" << endl;
        }
        else if (network[currentIndex].ip == token.destIP && !token.isFree) {
            network[currentIndex].buffer.push_back(token.message);
            token.hasArrived = true;
            cout << "C" << currentIndex << ": Am ajuns la destinatie" << endl;
            cout << "C" << currentIndex << ": Muta jetonul" << endl;
        }
        else if (network[currentIndex].ip == token.srcIP && token.hasArrived) {
            token.isFree = true;
            token.hasArrived = false;
            cout << "C" << currentIndex << ": Am revenit la sursa" << endl;
            cout << "C" << currentIndex << ": Token reset" << endl;
            break;
        }
        else {
            cout << "C" << currentIndex << ": Muta jetonul" << endl;
        }
    }
}

vector<string> readMessagesFromFile(const string& filename) {
    vector<string> messages;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        messages.push_back(line);
    }
    return messages;
}

int main() {
    srand(time(0));
    vector<Computer> network;
    initializeNetwork(network);

    vector<string> messages = readMessagesFromFile("message.txt");

    int currentIndex = 0;
    bool firstIteration = true;

    Token token;
    for (int i = 0; i < 2; ++i) {

        token.srcIP = network[rand() % network.size()].ip;
        do {
            token.destIP = network[rand() % network.size()].ip;
        } while (token.srcIP == token.destIP);
        token.message = messages[rand() % messages.size()];

        int srcIndex = findComputerIndex(network, token.srcIP);
        int destIndex = findComputerIndex(network, token.destIP);

        cout << "Sursa: C" << srcIndex << "(" << token.srcIP << ") Destinatie: C" << destIndex << "(" << token.destIP << ")" << endl;

        bool clockwise;
        cout << "Enter direction (1 for clockwise, 0 for counterclockwise): ";
        cin >> clockwise;



        displayNetwork(network);
        if (!firstIteration) {
            cout << "C" << currentIndex << ": Muta jetonul" << endl;
        }
        if (firstIteration)
        {
            cout << "C" << currentIndex << ": Muta jetonul" << endl;

        }
        firstIteration = false;
        simulateTokenRing(network, token, clockwise, currentIndex);
        displayNetwork(network);
        cout << endl;

        displayHistory(network, token.history);
        cout << endl;
    }

    //displayHistory(network, token.history);
    return 0;
}