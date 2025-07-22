#include "fsm.hpp"
#include <cstdint>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>
#include <chrono>

FSM::FSM(uint32_t delay) : currentState(SystemState::INIT), lastHeartbeat(0), 
             delay(delay), errorCount(0), moveCount(0) {}


FSM::~FSM() {
    stateHistory.clear();
}

std::string stateToString(SystemState state) {
    switch (state) {
        case SystemState::INIT:        return "INIT";
        case SystemState::IDLE:        return "IDLE";
        case SystemState::MOVEMENT:    return "MOVEMENT";
        case SystemState::SHOOTING:    return "SHOOTING";
        case SystemState::CALCULATION: return "CALCULATION";
        case SystemState::ERROR:       return "ERROR";
        case SystemState::STOPPED:     return "STOPPED";
        default:                       return "UNKNOWN";
    }
}

uint32_t millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

}

SystemState FSM::getCurrentState() const {
    return currentState;
}

void FSM::transitionToState(SystemState newState){
    currentState = newState;
    setLastHeartbeat(millis());
    addStateToHistory(newState, lastHeartbeat);
}

void FSM::setDelay(uint32_t delay){
    this->delay = delay;
}

void FSM::getDelay(uint32_t &delay) const {
    delay = this->delay;
}

void FSM::setErrorCount(int count){
    errorCount = count;
}

int FSM::getErrorCount() const {
    return errorCount; 
}

void FSM::setMoveCount(int count){
    moveCount = count;
}

int FSM::getMoveCount() const {
    return moveCount;
}

void FSM::addStateToHistory(SystemState state, uint32_t time) {
    stateHistory.emplace_back(state, time); // Tambahkan pair ke vector
}

vector<pair<SystemState, uint32_t>> FSM::getStateHistory() const{
    return stateHistory;
}

uint32_t FSM::getLastHeartbeat() const{
    return lastHeartbeat;
}

void FSM::setLastHeartbeat(uint32_t heartbeat){
    lastHeartbeat = heartbeat;
}

void FSM::start(){
    performInit();
    
    while (currentState != SystemState::STOPPED) {
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay)));

        std::cout << "Current state: " << stateToString(currentState) << std::endl;
    }


    shutdown();
}

void FSM::update(){
    setLastHeartbeat(millis());
    addStateToHistory(currentState, lastHeartbeat);

    switch(currentState){
        case SystemState::INIT:
            performInit();
            break;
        case SystemState::IDLE:
            performProcess();
            break;
        case SystemState::MOVEMENT:
            performMovement();
            break;
        case SystemState::SHOOTING:
            performShooting();
            break;
        case SystemState::CALCULATION:
            performCalculation();
            break;
        case SystemState::ERROR:
            performErrorHandling();
            break;
        case SystemState::STOPPED:
            shutdown();
            break;
        default:
            std::cerr << "Unknown state!" << std::endl;

    }
}

void FSM::printStatus(){
    std::cout << "current state: " << stateToString(currentState) << std::endl;
    std::cout << "last heartbeat time: " << lastHeartbeat << std::endl;
    std::cout << "delay: " << delay << std::endl;
    std::cout << "error count: " << errorCount << std::endl;
}

void FSM::printStateHistory() {
    if (stateHistory.empty()) {
        std::cout << "State history kosong." << std::endl;
        return;
    }

    std::cout << "=== State History ===" << std::endl;
    for (const auto& entry : stateHistory) {
        std::cout << "State: " << stateToString(entry.first) 
                  << " | Time: " << entry.second << " ms" << std::endl;
    }
}

void FSM::performInit(){
    delay = 1000;
    transitionToState(SystemState::IDLE);
    std::cout << "Initializing system..." << std::endl;
    printStatus();
}

void FSM::performProcess(){
    std::cout << "\nPilih aksi:\n"
              << "1. Status\n"
              << "2. Move\n"
              << "3. Shoot\n"
              << "4. Calculate\n"
              << "Pilihan: ";
    int choice;
    std::cin >> choice;

    if (std::cin.fail() || choice < 1 || choice > 4) {  // ✅ Batasi input 1-4
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Input harus angka 1-4!" << std::endl;
        return;
    }


    switch (choice) {
        case 1: printStatus(); break;
        case 2: transitionToState(SystemState::MOVEMENT); break;
        case 3: transitionToState(SystemState::SHOOTING); break;
        case 4: transitionToState(SystemState::CALCULATION); break;
        default: std::cout << "Input tidak valid!" << std::endl;
    }

}

void FSM::performMovement() {
    std::cout << "Moving..." << std::endl;
    moveCount++;

    if (moveCount >= 3) {
        transitionToState(SystemState::SHOOTING);
    } else {
        transitionToState(SystemState::IDLE);
    }
} 

void FSM::performShooting(){
    std::cout << "Shooting..." << std::endl;
    moveCount = 0;

    transitionToState(SystemState::IDLE);
}

void FSM::performCalculation(){
    std::cout << "Performing calculation..." << std::endl;

    if (moveCount == 0) {
        transitionToState(SystemState::ERROR);
    } else if (moveCount > 0) {
        transitionToState(SystemState::IDLE);
    }
}

void FSM::performErrorHandling(){
    std::cout << "Error occurred, performing error handling..." << std::endl;
    errorCount += 1;

    if (errorCount == 3) {
        transitionToState(SystemState::STOPPED);
    } else {
        transitionToState(SystemState::IDLE);
    }

}

void FSM::shutdown(){
    std::cout << "System stopped, shutting down..." << std::endl;
}