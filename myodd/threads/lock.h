#pragma once

#include <mutex>

namespace myodd {
  namespace threads {
    class Key : public std::mutex
    {
    public:
      Key() : Key(L"Unk.")
      {
      }
      Key(const std::wstring& name  ) :
        std::mutex(),
        _name(name)
      {
      }
      Key(const wchar_t* name) :
        std::mutex(),
        _name(name)
      {
      }

      const std::wstring _name;
    };

    class Lock final
    {
    public:
      explicit Lock( Key& key );
      virtual ~Lock();

      void Release();

    private:
      Key& _key;
      std::lock_guard<std::mutex>* _guard;
    };
  }
}