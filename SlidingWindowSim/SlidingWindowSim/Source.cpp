
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <random>

using namespace std;

bool isLost(double probability = 0.3) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<> dis(0, 1);
    return dis(gen) < probability;
}

void printWindow(const vector<int>& window) {
    cout << "=== Fereastra curenta: [ ";
    for (int f : window) cout << f << " ";
    cout << "] ===\n";
}

int main() {
    int total_frames, window_size;
    cout << "Introdu numarul total de pachete (frames): ";
    cin >> total_frames;
    cout << "Introdu dimensiunea ferestrei: ";
    cin >> window_size;

    int next_frame = 1;
    vector<int> receiver_frames;
    vector<int> acks(total_frames + 1, 0);

    vector<int> window;
    for (int i = 0; i < window_size && next_frame <= total_frames; ++i) {
        window.push_back(next_frame);
        next_frame++;
    }

    const int TIMEOUT_LIMIT = 1;
    set<int> lostFrames;
    map<int, int> timeoutMap;

    while (!window.empty()) {
        cout << "\n";
        printWindow(window);

        for (int frame : window) {
            if (acks[frame]) continue;

            if (lostFrames.find(frame) != lostFrames.end() && timeoutMap[frame] < TIMEOUT_LIMIT) {
                timeoutMap[frame]++;
                continue;
            }

            cout << "Trimitem Frame " << frame << " -- ";

            if (isLost()) {
                cout << "Frame pierdut!\n";
                cout << "Receiver nu primeste Frame " << frame << ", nu trimite ACK.\n";
                lostFrames.insert(frame);
                timeoutMap[frame] = 0;
            }
            else {
                cout << "Frame primit cu succes.\n";
                receiver_frames.push_back(frame);
                cout << "Receiver trimite ACK pentru Frame " << frame << "\n";
                acks[frame] = 1;
                lostFrames.erase(frame);
                timeoutMap.erase(frame);
            }
        }

        if (!window.empty() && acks[window[0]]) {
            window.erase(window.begin());
            if (next_frame <= total_frames) {
                window.push_back(next_frame);
                next_frame++;
            }
        }
    }

    cout << "\n=== TOATE FRAME-URILE PRIMITE IN RECEIVER ===\n";
    //vector<int> received(receiver_frames.begin(), receiver_frames.end());
    //sort(received.begin(), received.end());
    for (int f : receiver_frames) cout << "Frame " << f << " ";
    cout << "\n\nToate pachetele au fost transmise si receptionate cu succes!\n";

    return 0;
}
