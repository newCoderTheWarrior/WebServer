//
// Created by wfh on 22-6-24.
//

#include "util.h"
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <memory>

//read from fd
ssize_t read(int fd, void* buffer, size_t n)
{
    size_t nLeft = n;
    ssize_t nRead = 0;
    ssize_t readSum = 0;
    auto ptr = std::make_unique<char*>(static_cast<char*>(buffer));
    while (nLeft > 0) {
        if((nRead = read(fd,ptr.get(),nLeft)) < 0) {
            if(errno = EINTR) {
                nRead = 0;
            }
            else {
                if(errno = EAGAIN) {
                    return readSum;
                }
                else return ssize_t (-1);
            }
        }
        else{
            if(nRead == 0) break;
        }
        readSum += n;
    }
}

//write to fd
ssize_t write(int fd, void* buffer, size_t n)
{
    auto nLeft = n;
    ssize_t nWrite = 0;
    auto writeSum = 0;
    auto ptr = std::make_unique<char*>(static_cast<char*>(buffer));
    while(nLeft > 0){
        if((nWrite = write(fd, ptr.get(), nLeft)) <= 0) {
            if(nWrite < 0) {
                if(errno == EINTR || errno == EAGAIN){
                    nWrite = 0;
                    continue;
                }
                else {
                    return -1;
                }
            }
        }
        writeSum += nWrite;
        nLeft -= nWrite;
        *ptr += nWrite;
    }
    return writeSum;
}

//handle sigpipe
auto sigpipeHandler()
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, nullptr)) {
        return;
    }
}

//set socket nonblocking
auto nonBlockingSocketSetter(int fd)
{
    auto flag = fcntl(fd, F_GETFL,0);
    if(flag == -1) {
        return -1;
    }
    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1) {
        return -1;
    }
    return 0;
}