#include "fsm.hpp"

using namespace std;

int main() {
    cout << "Program dimulai!" << endl; // Pastikan ini muncul

    FSM robotFSM(2000);
    robotFSM.start();
    
    cout << "Program selesai" << endl; // Pastikan ini muncul
    
    system("pause");
    return 0;
}