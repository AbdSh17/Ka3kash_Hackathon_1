# Proposed Solution

This project proposes a **smart, modular gas leakage detection system** designed for HVAC environments that rely on gas-powered heating equipment such as furnaces or boilers. The system continuously monitors gas levels and responds to unsafe conditions through local alerts, user notifications, and optional shutdown mechanisms.

## System Overview

The proposed solution consists of a **main detection device** and several **optional extensions** that enhance safety and flexibility depending on the installation requirements.

### Core Detection Unit

The main device is responsible for:
- Continuously monitoring gas concentration in HVAC environments
- Detecting abnormal gas levels during normal operation
- Classifying system states into:
  - **Safe**
  - **Warning**
  - **Danger**

When gas levels exceed predefined thresholds, the system reacts immediately to reduce risk.

## Response Mechanism

The system uses a **multi-stage response strategy**:

- **Warning Level**
  - Visual and audible alerts are activated
  - System remains operational while monitoring continues

- **Danger Level**
  - Strong audible alarm is triggered
  - Safety output is activated
  - System enters a locked emergency state until manual reset
## System Extensions

To enhance flexibility and adapt to different installation scenarios, the proposed system supports the following optional extensions.

### 1. Automatic / Manual Shutoff Valve

The system can be extended with a **shutoff valve module** that allows gas flow to be interrupted when dangerous gas levels are detected. This valve can operate in two ways:
- **Automatic closure**, triggered by the detection unit when gas concentration exceeds a critical threshold
- **Manual activation**, allowing users to shut down the gas supply using a physical control

For safety and regulatory reasons, the prototype demonstrates this functionality through a control signal. In real-world deployment, certified valves and professional installation are required.

### 2. HVAC Control Board Integration

If the HVAC system uses a **modern control board**, the detection device can communicate directly with it to request system shutdown. This is achieved through:
- A relay or dry-contact interface
- A shutdown or fault signal compatible with standard HVAC control inputs

When hazardous gas levels are detected, the system can automatically disable HVAC operation, preventing further gas circulation until safe conditions are restored.

### 3. User Notification Application

The system can be connected to a **user application** that provides real-time status updates and alerts. Through this application, users can:
- Receive notifications indicating the current system state (Safe, Warning, or Danger)
- Be alerted immediately when a gas leak is detected
- Monitor system status remotely, even when not present on-site

This extension improves user awareness and enables faster response to hazardous conditions.
