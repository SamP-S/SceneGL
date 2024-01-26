#pragma once

#include <chrono>
#include <cstdint>

class TickTimer {
    private:
        std::chrono::time_point<std::chrono::system_clock> _startTime;
        std::chrono::time_point<std::chrono::system_clock> _endTime;
        std::chrono::time_point<std::chrono::system_clock> _lastFrameTime;
        uint32_t _tickCount = 0;
        bool _running = true;

    public: 
        TickTimer() {
            Start();
        }

        void Start() {
            _startTime = std::chrono::system_clock::now();
            _lastFrameTime = _startTime;
            _running = true;
        }

        void Stop() {
            _endTime = std::chrono::system_clock::now();
            _running = false;
        }
        
        void Tick() {
            _tickCount += 1;
            if (_running) {
                _lastFrameTime = std::chrono::system_clock::now();
            }
        }

        int GetTickCount() {
            return _tickCount;
        }

        double GetTickElapsed(bool isMilli = false) {
            std::chrono::time_point<std::chrono::system_clock> endTime;
            if(_running) {
                endTime = std::chrono::system_clock::now();
            } else {
                endTime = _endTime;
            }
            double dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - _lastFrameTime).count();
            return isMilli ? dt : dt / 1000;
        }
    
        double GetTotalElapsed(bool isMilli = false) {
            std::chrono::time_point<std::chrono::system_clock> endTime;
            if(_running) {
                endTime = std::chrono::system_clock::now();
            } else {
                endTime = _endTime;
            }
            double dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - _startTime).count();
            return isMilli ? dt : dt / 1000;
        }
};