// Created by James on 04/02/2022

#pragma once
#include <cstring>
#include <string>
#include <vector>

enum class PacketID : int
{
  STRING,
  INT,
  FLOAT,
  ONPLAYERCONNECT,
  ONCONNECTIONESTABLISHED,
  ONPLAYERDISCONNCT,
  ONPLAYERREADY,
  ONPLAYERUNREADY,
  ONPLAYERMOVE,
  ONPLAYERATTACK,
  ONGAMEOVER,
  ONPLAYERWIN
};

enum class PlayerID : int
{
  NONE,
  PLAYERONE,
  PLAYERTWO,
  SPECTATOR,
  SERVER,
};

namespace bcw::networking
{
  class Packet
  {
   public:
    Packet()    = default;
    ~Packet()   = default;
    using bytes = std::vector<std::byte>;

    explicit Packet(size_t capacity);
    Packet(const Packet&) = delete;
    [[nodiscard]] bool isEnd() const;
    [[nodiscard]] bytes& data();
    [[nodiscard]] size_t length() const;

    [[maybe_unused]] void resetReadHead();
    template<typename T>
    Packet& operator<<(const T& data)
    {
      static_assert(!std::is_pointer_v<T>, "Do not send pointers");
      auto size     = sizeof(data);
      auto* as_byte = reinterpret_cast<const std::byte*>(&data);
      _data.insert(_data.end(), as_byte, as_byte + size);
      return *this;
    }
    template<typename T>
    Packet& operator>>(T& data)
    {
      static_assert(!std::is_pointer_v<T>, "Do not send pointers");
      auto size    = sizeof(T);
      auto as_type = reinterpret_cast<T*>(&_data[read_pos]);
      data         = *as_type;
      read_pos += size;
      return *this;
    }
    Packet& operator<<(const std::string& str);
    Packet& operator<<(const std::string&& str);
    Packet& operator>>(std::string& str);

   private:
    size_t read_pos{};
    bytes _data{ 0 };
  };

}
