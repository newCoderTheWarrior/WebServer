//
// Created by wfh on 22-7-6.
//

#ifndef WEBSERVER_REQUESTDATA_H
#define WEBSERVER_REQUESTDATA_H
#include <string>
#include <unordered_map>

constexpr int STATE_PARSE_URI = 1;
constexpr int STATE_PARSE_HEADERS = 2;
constexpr int STATE_RECEIVE_BODY = 3;
constexpr int STATE_ANALYSIS = 4;
constexpr int STATE_FINISH = 5;

constexpr int MAX_BUFF_SIZE = 4096;

//request found but can not read, it maybe because request aborted
//or data did not arrive
//for these request, response them for some times then throw
constexpr int AGAIN_MAX_TIMES = 200;

constexpr int PARSE_URI_AGAIN = -1;
constexpr int PARSE_URI_ERROR = -2;
constexpr int PARSE_URI_SUCCESS = 0;

constexpr int PARSE_HEADER_AGAIN = -1;
constexpr int PARSE_HEADER_ERROR = -2;
constexpr int PARSE_HEADER_SUCCESS = 0;

constexpr int ANALYSIS_ERROR = -2;
constexpr int ANALYSIS_SUCCESS = 0;

constexpr int METHOD_POST = 1;
constexpr int METHOD_GET = 2;
constexpr int HTTP_10 = 1;
constexpr int HTTP_11 = 2;

constexpr int EPOLL_WAIT_TIME = 500;

class MimeType
{
private:
    static pthread_mutex_t lock;
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    MimeType(const MimeType& m);
public:
    static std::string getMime(const std::string &suffix);
};

enum class HeaderState
{
    h_start = 0,
    h_key,
    h_colon,
    h_spaces_after_colon,
    h_value,
    h_CR,
    h_LF,
    h_end_CR,
    h_end_LF
};

struct myTimer;
struct requestData;

struct requestData
{
private:
    int againTimes;
    std::string path;
    int fd;
    int epollFd;
    // content的内容用完就清
    std::string content;
    int method;
    int HTTPVersion;
    std::string file_name;
    int now_read_pos;
    int state;
    int h_state;
    bool isFinish;
    bool keep_alive;
    std::unordered_map<std::string, std::string> headers;
    myTimer *timer;

private:
    int parse_URI();
    int parse_Headers();
    int analysisRequest();

public:

    requestData();
    requestData(int _epollFd, int _fd, std::string _path);
    ~requestData();
    void addTimer(myTimer *mTimer);
    void reset();
    void seperateTimer();
    int getFd();
    void setFd(int _fd);
    void handleRequest();
    void handleError(int fd, int err_num, std::string short_msg);
};

struct myTimer
{
    bool deleted;
    size_t expired_time;
    requestData *request_data;

    myTimer(requestData *_request_data, int timeout);
    ~myTimer();
    void update(int timeout);
    bool isvalid();
    void clearReq();
    void setDeleted();
    bool isDeleted() const;
    size_t getExpTime() const;
};

struct timerCmp
{
    bool operator()(const myTimer *a, const myTimer *b) const;
};

#endif //WEBSERVER_REQUESTDATA_H
