# **R-Type**

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)  
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![vcpkg](https://img.shields.io/badge/vcpkg-%23007ACC.svg?style=for-the-badge&logo=visualstudio&logoColor=white)
![GitHub Actions](https://img.shields.io/badge/github%20actions-%232671E5.svg?style=for-the-badge&logo=githubactions&logoColor=white)

---

## **Table of Contents**

- [Introduction](#introduction)
  - [Supported Platforms](#supported-platforms)
- [The Game](#the-game)
  - [Story](#story)
- [How to Play ?](#how-to-play-)
  - [Prerequisites](#prerequisites)
  - [Launch the Game](#launch-the-game)
- [For Developers](#for-developers)
  - [Build the Program](#build-the-program)
  - [Testing Program](#testing-program)
  - [Documentation](#documentation)
- [Technology Choices](#technology-choices)
- [Thank for Reading](#thank-for-reading)
- [Authors](#authors)

---

## **Introduction**

The **Epitech R-Type Project** is a reimagining of the classic shoot’em up **R-Type**, built from scratch using a **custom ECS (Entity Component System)** engine.  
It includes a **multithreaded C++ server** and a **graphical client**, with networking, physics, UI, and persistence handled through modern open-source libraries.

### **Main Libraries**
- **SFML** → 2D graphics rendering, sound, and event management  
- **Asio** → Networking and multithreading  
- **SQLite3** → Lightweight, embedded database for user and score management  
- **Libsodium** → Modern cryptographic library for secure password hashing and token management  
- **vcpkg** → Dependency manager for cross-platform builds  

---

### **Supported Platforms**

✅ **Windows**  
✅ **Linux**

---

## **The Game**

### **Story**

This story begins in 2442, after humanity barely survives the Fourth World War. The Earth, now fully radioactive, forces humankind to look to the stars.  
Five geniuses, known as *"Cold_as_Palmer"*, built ships capable of traveling to **Zelta**, a new habitable planet.

But space is not empty.  
Alien species, energy storms, and cosmic predators now stand between humanity and survival.  
Only you, as the last pilot, can help them defeat the alien forces and reach Zelta safely.

---

## **How to Play ?**

### **Prerequisites**

You need:
- **CMake ≥ 3.17**
- **C++20 compatible compiler**
- **vcpkg** (for dependency management)

### **Launch the Game**

After building (see below), simply run:
```bash
./r-type_server [port:int] &
./r-type_client [port:int] [adress:string]
```

## **For developers**

### **Build the program** 

On Linux : - You may execute the following commands :
```bash
    mkdir build
    cd build
    cmake ..
    make
```

On Windows : - You may execute the following commands :
```bash
    mkdir build
    cd build
    conan install .. --build=missing  -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
    cd ..
    cmake -B ./build -DCMAKE_BUILD_TYPE=Release
    cmake --build ./build
    copy ".\Debug\r-type_server.exe" .
    copy ".\Debug\r-type_client.exe" .
    copy ".\Debug\r-type_room.exe" .
    copy ".\Debug\r-type_admin_panel.exe" .
```

### **Testing program** 

The tests are available on Linux. You may can test the code executing the script build_tests.sh or using with the following commands.
```bash
    rm -rf ./build
    cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -DTESTING=ON
    cmake --build ./build
    cd ./build
    ctest --ouput-on-failure
```

### **Documentation**

🧠 **Code Reference:** Generated with [Doxygen](https://www.doxygen.nl/)  
📘 **Technical Docs:** [Notion Documentation](https://amazing-partridge-567)  
📜 **Communication Protocol:** [docs/protocol.md](./docs/protocol.md)  
💾 **Database Schema:** [docs/database.md](./docs/database.md)

### **Technologies choice**

Please consult the comparative study document [docs/comparatives_case_study.md](./docs/comparatives_case_study.md)
## **Thank for reading**

Feel free to read the CONTRIBUTING.md. Do not hesitate to contact any member for any questions or remarks. You can click on each following name.

## **Authors** 

<table> <tbody> <tr> <td align="center"><a href="https://github.com/FaroukJGRNT"><img src="https://avatars.githubusercontent.com/u/72017980?v=4" width="100px;" alt="FaroukJGRNT"/><br/><sub><b>Farouk OKANLA</b></sub></a><br/></td> <td align="center"><a href="https://github.com/amourguidi"><img src="https://avatars.githubusercontent.com/u/56249749?s=96&v=4" width="100px;" alt="amourguidi"/><br/><sub><b>Amour Guidi</b></sub></a><br/></td> <td align="center"><a href="https://github.com/Akanphi"><img src="https://avatars.githubusercontent.com/u/72011124?v=4" width="100px;" alt="Akanphi"/><br/><sub><b>Philippe ABIODOUN</b></sub></a><br/></td> <td align="center"><a href="https://github.com/racso27th"><img src="https://avatars.githubusercontent.com/u/72010794?v=4" width="100px;" alt="racso27th"/><br/><sub><b>Oscar Gbenou</b></sub></a><br/></td> <td align="center"><a href="https://github.com/Aimane01"><img src="https://avatars.githubusercontent.com/u/72015360?v=4" width="100px;" alt="Aimane01"/><br/><sub><b>Aïmane ALASSANE</b></sub></a><br/></td> </tr> </tbody> </table>