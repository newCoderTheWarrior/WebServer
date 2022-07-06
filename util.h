//
// Created by wfh on 22-6-24.
//
#ifndef WEBSERVER_UTIL_H
#define WEBSERVER_UTIL_H

#include <cstdlib>

ssize_t read(int fd, void* buffer, size_t n);
ssize_t write(int fd, void* buffer, size_t n);
auto sigpipeHandler();
auto nonBlockingSocketSetter(int fd);

#endif //WEBSERVER_UTIL_H
