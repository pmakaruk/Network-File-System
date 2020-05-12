#include "lib.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#include "../core/messages.hpp"
#include "../core/serializers.hpp"


#include <sstream>
#include <string>
#include <vector>

std::string bytes_to_string2(std::vector<u_int8_t> bytes) {
  std::stringstream sstream;
  for (std::vector<u_int8_t>::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
    sstream << int(*i) << ' ';

  std::string result;
  getline(sstream, result);
  return result;
}

extern int test_libclient(int x) { return test_libcore(x) * 321; };

extern int error = -1;

NFSClient::NFSClient(){};
int NFSClient::connect4(char *host, int port, char *user, char *password) {
  struct sockaddr_in server;
  struct hostent *hp;
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ == -1) {
    perror("opening stream socket");
    exit(1);
  }
  server.sin_family = AF_INET;
  hp = gethostbyname(host);

  if (hp == (struct hostent *)0) {
    fprintf(stderr, "%s: unknown host\n", host);
    exit(2);
  }

  memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);
  server.sin_port = htons(port);
  if (connect(socket_fd_, (struct sockaddr *)&server, sizeof server) == -1) {
    perror("connecting stream socket");
    exit(1);
  }
};

int NFSClient::open(char *path, int oflag, int mode) {
  OpenRequest open_request{path, oflag, mode};
  std::vector<u_int8_t> byte_request = SerializeOpenRequest(open_request);

  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  OpenResponse open_response = DeserializeToOpenResponse(byte_response);
  error = open_response.error;
  return open_response.result;
}
ssize_t NFSClient::read(int fd, void *buf, size_t count) {
  ReadRequest read_request{fd, count};
  std::vector<u_int8_t> byte_request = SerializeReadRequest(read_request);

  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  ReadResponse read_response = DeserializeToReadResponse(byte_response);
  error = read_response.error;
  memcpy(buf, read_response.buf.data(), read_response.result);
  return read_response.result;
}
//+
ssize_t NFSClient::write(int fd, const void *buf, size_t count) {
  
  const u_int8_t *uintBuff = (u_int8_t*) buf;
  std::vector<u_int8_t>vectorBuff(uintBuff, uintBuff + count);
  WriteRequest write_request{fd, vectorBuff};
  std::vector<u_int8_t> byte_request = SerializeWriteRequest(write_request); 
 
  sendRequest_(byte_request);
  std::vector<u_int8_t> byte_response = receiveResponse_();

  WriteResponse read_response = DeserializeToWriteResponse(byte_response);
  error = read_response.error;
  return read_response.result;

}
//
off_t NFSClient::lseek(int fd, off_t offset, int whence) {

}

int NFSClient::close(int fd) {
  CloseRequest close_request{fd};

}

int NFSClient::unlink(const char *pathname) {}

int NFSClient::opendir(const char *name) {}
dirent *NFSClient::readdir(int dirfd) {}
int NFSClient::closedir(int dirfd) {}


void NFSClient::sendRequest_(std::vector<u_int8_t> request) {
  sendMessage(socket_fd_, request);
}; 

std::vector<u_int8_t> NFSClient::receiveResponse_() {
  return receiveMessage(socket_fd_);
};
