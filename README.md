## A Rubik's Cube Intelligent Tutoring System
### Final Year Project
### Technologies Used: C++ and Cmake, OpenCV, OpenGL, Qt, SQLite
### Project Overview (Photos)
<img width="1252" height="663" alt="image" src="https://github.com/user-attachments/assets/3a0e55b1-6417-4dbf-9549-e4c47d7dd555" />
<img width="1252" height="664" alt="image" src="https://github.com/user-attachments/assets/7bfc36c5-326a-4352-b55a-ff1d6d39ee7e" />
<img width="1252" height="662" alt="image" src="https://github.com/user-attachments/assets/3caabf11-2942-4135-8caf-6bd12bac6ca2" />
<img width="1252" height="663" alt="image" src="https://github.com/user-attachments/assets/6dc21252-1f18-4fe5-8aa6-e96aa0608567" />

### Project Description

The Rubik’s Cube ITS is a desktop application designed to support absolute beginners a solving a classic 3x3 Rubik’s Cube. Unlike conventional solver applications that automatically generate optimal solutions, this system functions as a tutor, guiding users through a human-centered and comprehensible solving process. 

Given the hands-on nature of the task, the system must observe learner actions to provide appropriate feedback. To achieve this, the ITS employs computer vision to recognise the cube’s configuration via a standard webcam and reconstructs its state computationally. By comparing the user’s current cube configuration against a desired target state, the system can assess progress and deliver context-sensitive instructions.

In addition to webcam-based input, the system provides a 3D visualisation that mirrors the learner’s physical cube. This visualisation is complemented by step-by-step notational subtitles, which are gradually reduce in detail as the user’s proficiency improves. The system also incorporates a backtracking feature, allowing users to revert to previous checkpoints using a solver based on the IDA* algorithm, with animated transitions to support understanding of intermediate states.

To ensure a structured and pedagogically grounded learning experience, the system is developed in accordance with established models of psychomotor skill acquisition. By structuring the learning process around these principles, the application aims to replicate the guidance a human instructor might provide when teaching a physical, skill-based task. Behaviourist learning strategies are also integrated to reinforce progression through feedback and repetition.

The system is designed to eliminate the need for specialised hardware or modified Rubik’s Cubes, relying solely on a webcam. In doing so, it implicitly promotes accessibility, affordability, and sustainability.

The final system is intended to function as a fully operational desktop application capable of adapting instruction to specific cube configurations and the learner’s level of proficiency.

