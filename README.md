# UCloud: A Lightweight Home Network Backup Server

UCloud is a multithreaded networking application built entirely in C++ using SFML sockets. Designed as a home network backup server, UCloud allows multiple clients to connect to a central server to upload files and folders seamlessly.  

## Features  
- **Client-Server Architecture**: A single main server can manage multiple simultaneous client connections.  
- **File Upload**: Clients can upload files and folders to the server.  
- **Authentication**: Password protection for server access.  
- **Status Monitoring**: Real-time status updates for connection and file transfer events.  
- **Logging**: Detailed logs, including connection events and file metadata (e.g., file size, upload duration).  
- **Cross-Platform Compatibility**: Developed using SFML, UCloud runs on Windows, macOS, and Linux.  

## Planned Features  
- **Two-Way File Transfer**: Enable clients to download files from the server.  
- **Image Previews**: Clients can preview images in lower resolution before downloading.  
- **Improved Scalability**: Enhance performance to handle higher client and data volumes.  
- **Cross-Platform Compatibility**: Client and Server for Linux and Android devices.
---

## System Requirements  (for compiling the project yourself)
- **Operating System**: Windows
- **Dependencies**:  
  - SFML (Simple and Fast Multimedia Library)  
  - A C++ compiler supporting C++17 or later  

---

## Installation  

### 1. Clone the Repository  
```bash  
git clone https://github.com/<username>/UCloud.git  
cd UCloud  
