#include "lib.hpp"

#include "../core/messages.hpp"
#include "../core/serializers.hpp"


extern int test_libclient(int x) {
  return test_libcore(x) * 321;
};

extern int error;

NFSClient::NFSClient(char *host, int port, char *user, char *password) {};
int NFSClient::connect() {};

int NFSClient::open(char *path, int oflag, int mode) {
    OpenRequest open_request {path, oflag, mode};
    std::vector<u_int8_t> byte_request = SerializeOpenRequest(open_request);

    sendRequest_(byte_request);
    std::vector<u_int8_t> byte_response = receiveResponse_();

    OpenResponse open_response = DeserializeToOpenResponse(byte_response);
    error = open_response.error;
    return open_response.result;
}
ssize_t NFSClient::read(int fd, void *buf, size_t count) {}
ssize_t NFSClient::write(int fd, const void *buf, size_t count) {}
off_t NFSClient::lseek(int fd, off_t offset, int whence) {}
int NFSClient::close(int fd) {}
int NFSClient::unlink(const char *pathname) {}

int NFSClient::opendir(const char *name) {}
dirent *NFSClient::readdir(int dirfd) {}
int NFSClient::closedir(int dirfd) {}


void NFSClient::sendRequest_(std::vector<u_int8_t> request) {};
std::vector<u_int8_t> NFSClient::receiveResponse_() {};