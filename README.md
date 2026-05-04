# CogniCare – Hardware Module (Smartwatch System)

## 📌 Overview

The CogniCare Smartwatch is a wearable hardware system designed for Alzheimer’s patients. It continuously monitors health signals (heart rate + stress) and location (GPS), and intelligently triggers emergency communication (calls + WhatsApp alerts) using AI integration.

## ⌚ Hardware Components

🫀 MAX30102 (Heart Rate & SpO₂ Sensor)

+ Tracks:

  + Heart Rate (BPM)

  + Blood Oxygen (SpO₂)

+ Purpose:

  + Detect abnormal heart activity

  + Identify panic or distress conditions

## ⚡ GSR Sensor (Stress Detection)
+ Tracks:

  + Skin conductivity (stress indicator)
+ Purpose:

  + Detect emotional spikes (fear, anxiety)
Key Logic:

  + High GSR = High stress

## 🧠 ESP32 (Processing Unit)

+ Acts as the brain of the watch

+ Responsibilities:

  + Collects sensor data (HR + GSR)

  + Processes real-time conditions

  + Sends data to backend/AI system

  + Triggers emergency actions

## 📺 OLED Display
+ Displays:

  + Heart rate

  + Stress status

  + Guidance messages

+ Purpose:

  + Provides simple instructions for patients
## 📍 GPS Module
+ Tracks real-time location

+ Features:

  + Stores safe locations (home, hospital, etc.)

  + Tracks current position continuously

+ Smart Behavior:

  + When location changes, system retrieves 10–15 nearby saved locations

  + Helps guide user back to safe zones

## 🔗 Hardware + Intelligent Alert System

## 🚨 Emergency Detection (Triggered by Hardware)

+ The smartwatch detects critical situations using:

+ High Heart Rate (MAX30102)

+ High Stress (GSR)

+ Unsafe Location (GPS)

👉 When these conditions are met:

## 🤖 AI + Communication Trigger (What your system DOES)

+ The watch sends data to the AI system (Groq-powered LLM)

+ AI analyzes the situation and decides response

## 📞 Emergency Response Actions
## 1. 📲 Call to Caregiver (via Twilio)
+ Automatic voice call is triggered

+ Caregiver is informed:

  + Patient is in distress OR

  + Patient is in an unsafe location

## 2. 💬 WhatsApp Alert (via Twilio)
+ Instant message sent to caregiver

+ Includes:

  + Live location (from GPS)

  + Alert type (stress / heart rate / location)

## 3. 🧭 Navigation Assistance (GPS + AI)
+ AI guides the patient back to a safe location

+ Based on:

  + Stored safe places

  + Current GPS coordinates

+ Instructions shown on:

  + OLED display (simple directions)

## 4. 🧘 Calm Guidance (AI on Watch)
+ AI provides:

  + Relaxation exercises

  + Simple calming instructions

  + Displayed directly on the watch

## 🔁 Full Hardware Flow
+ Sensors (MAX30102 + GSR) collect real-time data

+ GPS tracks user location

+ ESP32 processes all inputs

+ If abnormal condition detected:

  + Data sent to AI (Groq)

  + AI evaluates situation

+ System triggers:

## 📞 Call (Twilio)

💬 WhatsApp alert (Twilio)

🧭 Navigation guidance

🧘 Relaxation support

## 🎯 Key Capabilities
✅ Real-time health monitoring

✅ Stress + panic detection

✅ Smart GPS tracking with safe zones

✅ Automatic emergency calling system

✅ WhatsApp alerts with live location

✅ AI-guided assistance directly on watch
