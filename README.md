# Radar-Sensing Autonomous Target System 🎯

## 🛰️ Technical Overview
This system implements a closed-loop autonomous tracking pipeline. It utilizes **Frequency-Modulated Continuous-Wave (FMCW)** radar to determine target range and velocity, feeding spatial data into a **PID controller** for servo-actuated precision targeting.

## ⚙️ Hardware Stack
- **Sensor:** FMCW Radar Module (24GHz)
- **Controller:** ESP32 / Arduino (Dual-core processing for signal filtering and motor control)
- **Actuation:** High-torque MG996R Servos for Pan/Tilt movement
- **Power:** External 5V 3A DC supply for high-current transients

## 🧠 Signal Processing & Logic
1. **FFT Analysis:** Raw radar data is processed via Fast Fourier Transform to isolate target peaks from background noise.
2. **PID Control:** A Proportional-Integral-Derivative algorithm minimizes the error between the target's spatial coordinate and the actuator's pointing vector.
3. **Prediction:** (Optional) A Kalman filter predicts target trajectory for high-speed tracking.
