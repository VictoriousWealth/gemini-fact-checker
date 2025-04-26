# Gemini Fact Checker Badge

🚀 **A wearable fact-checking badge that fights misinformation in real-time using Google's Gemini AI.**

---

## Inspiration

The idea for **Gemini Fact Checker** was born from a conversation with **Josh**, a Robotics Engineering student, about the trauma caused by misinformation and lying individuals in today's world.

This inspired the team to design a **wearable badge** that empowers people to **fact-check information instantly**.

---

## What It Does

- Listens to speech input (currently simulated)
- Sends the text to Google's **Gemini 2.0 Flash** model via API
- Displays a **"True" ✅** or **"False" ❌** verdict on the badge screen
- Provides instant visual (and future audio) feedback to users

---

## How We Built It

- Programmed the **M5StickC Plus** microcontroller in **C++** using **Arduino IDE**
- Integrated **Wi-Fi connectivity** to access the **Gemini API**  
- Displayed dynamic feedback on the **OLED screen**
- (Future) Planned to implement real audio capture and speaker feedback

---

## Built With

- Arduino IDE
- C++
- Gemini 2.0 Flash API
- M5StickC Plus Microcontroller
- OLED Display Libraries
- Wi-Fi Client + HTTPS Libraries
- Team collaboration between **Software, Robotics, and Mechatronics engineers**

---

## Challenges We Ran Into

- Severe **memory limitations** on the M5StickC Plus hardware
- Inability to send real **audio files to the API** due to resource constraints
- **Cross-field communication** between team members from very different engineering backgrounds
- Simulating audio-to-text functionality for demo purposes

---

## Accomplishments We're Proud Of

- Getting a **working prototype** built in under 24 hours
- **OLED screen color feedback** to signal fact-check results
- **Successfully integrating Gemini AI** with embedded hardware
- **Collaborating seamlessly** between **Software**, **Robotics**, and **Mechatronics** disciplines
- Being **one of 3 winning teams** out of 9+ competitors at the iForge Hackathon 🎉

---

## What We Learned

- Importance of researching **physical hardware limitations** deeply before building
- Best practices for **cross-disciplinary collaboration**
- Techniques for **working around limitations** creatively in hackathons
- Basics of **real-time microcontroller networking** with AI services

---

## What's Next for Gemini Fact Checker

- Fully implementing **real-time audio capture** and processing
- Reducing badge size and power consumption
- Designing a **wearable commercial prototype**
- Open-sourcing the project to inspire others in the fight against misinformation

---

## Contributors

| Name | Field | Contribution |
|:-----|:------|:-------------|
| **Nick Efe Oni** | Software Engineering | Lead Developer, Hardware Integration, Gemini API |
| **Leroy Barnie** | Software Engineering | Support Developer, Networking Assistant |
| **Josh** | Robotics Engineering | Idea Originator, Hardware Insights |
| **Arj** | Mechatronics Engineering | Hardware Support, Troubleshooting Assistance |

---

## Commit and Line Stats (for transparency)

| Contributor | Commits | % of Total | Lines Changed | % of Total |
|:-----------|:--------|:----------|:--------------|:----------|
| Nick Efe Oni | 14 commits | 82.4% | 924 lines | 66.4% |
| Leroy Barnie | 3 commits | 17.6% | 467 lines | 33.6% |

_(Josh and Arj assisted directly with hardware setup, testing, and ideation rather than Git commits.)_

---

# 🌟 Try it Out

GitHub Repo: ![here](https://github.com/VictoriousWealth/gemini-fact-checker/)

---

# Badges

![Arduino IDE](https://img.shields.io/badge/Built%20with-ArduinoIDE-informational)
![Powered by Gemini](https://img.shields.io/badge/Powered%20by-GeminiAI-brightgreen)

---

> "Fighting misinformation, one fact at a time."

