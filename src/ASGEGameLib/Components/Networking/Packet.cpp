// Created by James on 04/02/2022

#include "../../../../include/ASGEGameLib/Components/Networking/Packet.hpp"
bcw::networking::Packet::Packet(size_t capacity)
{
  _data.reserve(capacity);
}
bool bcw::networking::Packet::isEnd() const
{
  return read_pos >= _data.size();
}
bcw::networking::Packet::bytes& bcw::networking::Packet::data()
{
  return _data;
}
size_t bcw::networking::Packet::length() const
{
  return _data.size();
}
void bcw::networking::Packet::resetReadHead()
{
  read_pos = 0;
}
/*
template<typename T>
bcw::networking::Packet& bcw::networking::Packet::operator<<(const T& data)
{
  static_assert(!std::is_pointer_v<T>, "Do not send pointers");
  auto size = sizeof(data);
  auto* as_byte = reinterpret_cast<std::byte*>(&data);
  _data.insert(_data.end(),as_byte, as_byte+size);
  return *this;
}

template<typename T>
bcw::networking::Packet& bcw::networking::Packet::operator>>( T& data)
{
  static_assert(!std::is_pointer_v<T>, "Do not send pointers");
  auto size = sizeof(T);
  auto as_type = reinterpret_cast<T*>(&_data[read_pos]);
  data = *as_type;
  read_pos += size;
  return *this;

}*/

bcw::networking::Packet& bcw::networking::Packet::operator<<(const std::string& str)
{
  const auto* str_data = reinterpret_cast<const std::byte*>(str.data());
  auto length          = str.length() + 1;
  _data.insert(_data.end(), str_data, str_data + length);
  return *this;
}
bcw::networking::Packet& bcw::networking::Packet::operator<<(const std::string&& str)
{
  return *this << str;
}
bcw::networking::Packet& bcw::networking::Packet::operator>>(std::string& str)
{
  auto* as_char = reinterpret_cast<char*>(&_data[read_pos]);
  auto length   = std::strlen(as_char);
  str           = std::string(as_char, length);
  read_pos += length;
  return *this;
}
