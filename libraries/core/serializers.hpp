#ifndef SERIALIZERS_HPP
#define SERIALIZERS_HPP

#include <vector>
#include <string>

#include "./messages.hpp"

extern int test_libcore(int x);

/* message schema:

- name: message_type
  type: u_int8_t
- name: data_size
  type: u_int64_t
- name: data
  len: char[`data_size`]
*/
class MessageBuilder {
public:
  MessageBuilder();

  void writeMessageType(u_int8_t code);

  void write(u_int8_t x);
  void write(int32_t x);
  void write(u_int64_t x);
  void write(char* buf, size_t size);
  void write(std::string str);

  std::vector<u_int8_t> build();

private:
  std::vector<u_int8_t> buffer_;
  size_t len_;
  const size_t DATA_OFFSET_ = 2;
};

class MessageParser {
public:
  MessageParser(std::vector<u_int8_t>);

  u_int8_t readMessageType();

  u_int8_t readUInt8T();
  u_int32_t readInt32T();
  u_int64_t readUInt64T();
  std::vector<u_int8_t> readBytes();
  std::string readString();
};

/* byte_open_request data schema:

- name: path_size
  type: u_int8_t
- name: path
  type: char[`path_size`]
- name: oflag
  type: int32_t
- name: mode
  type: int32_t
*/
extern std::vector<u_int8_t> SerializeOpenRequest(OpenRequest open_request);

extern OpenRequest DeserializeToOpenRequest(std::vector<u_int8_t> byte_request);

/* byte_open_response data schema:

  - name: result
    type: int32_t
  - name: error
    type: int32_t
*/
extern std::vector<u_int8_t> SerializeOpenResponse(OpenResponse open_response);

extern OpenResponse DeserializeToOpenResponse(std::vector<u_int8_t> byte_response);

#endif