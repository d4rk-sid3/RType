/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-8
** File description:
** server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
    #include <poll.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdbool.h>
    #include <ctype.h>
    #include <time.h>
    #include <limits.h>
    #include <string>
    #include <sstream>
    #include <iostream>
    #include <filesystem>
    #include <bits/stdc++.h>
    #include <cstdlib>
    #include <vector>
    #include <thread>
    #include <sys/ioctl.h>
    #include <asio.hpp>


    class Server {
        private:
            int fd;
            int port;

        public:
            Server(int _port);
            ~Server();
    };

#endif /* !SERVER_HPP_ */
