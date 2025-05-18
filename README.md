
# 📦 UCloud-SFML Documentation

## 📌 Overview
**UCloud-SFML** is a lightweight, multithreaded remote backup client-server application developed in C++ using SFML's networking module. It allows multiple clients to connect to a central server and upload files or folders over a local network. It features authentication, logging, and a GUI interface.

## 🚀 Features
- ✅ **Multi-client support** – multiple clients can simultaneously upload data  
- 🔐 **Password authentication** – restrict server access with a password  
- 📂 **File & folder upload** – supports recursive folder uploads  
- 📊 **Logging system** – records connection, disconnection, upload events with metadata  
- 🖥️ **Client GUI** – built with ImGui for file path entry and upload status feedback  
- 🔌 **SFML Sockets** – uses TCP sockets for communication  
- 📋 **Status display** – connection and file transfer states shown live in GUI  

## 🛠️ Build Instructions

### Requirements
- C++17 or later
- SFML (Graphics, Window, Network)
- ImGui + ImGui-SFML
- CMake 3.12+

### Build Steps (Linux/Mac/Windows)
```bash
git clone https://github.com/sahil-deo/UCloud-SFML.git
cd UCloud-SFML
mkdir build && cd build
cmake ..
make
```

## 📌 Usage

### 🖥️ Server
```bash
./ucloud-server
```
- Starts listening for clients.
- Logs saved in `logs/` directory.

### 👤 Client
```bash
./ucloud-client
```
- GUI prompts for server IP, password, and file/folder path.
- Click `Connect` then `Send`.

## 🔒 Authentication
- Server requires a password (editable in code).
- Invalid password = rejected connection.

## 🧾 Logging
- All connection events are logged:
  - Who connected/disconnected
  - File/folder name
  - Upload timestamps and duration
  - File size
- Logs saved in `logs/` with timestamps.

## 🧩 Future Enhancements
- [ ] Two-way file transfer (client downloads)
- [ ] Image previews before downloading
- [ ] Better UI: error messages, progress bars
- [ ] TLS encryption for secure file transfer

## 👨‍💻 Author
**Sahil Deo**  
GitHub: [@sahil-deo](https://github.com/sahil-deo)

## 📄 License
This project is licensed under the [MIT License](LICENSE).
