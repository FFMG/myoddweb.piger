#pragma once
class ITray
{
public:
  ITray();
  virtual ~ITray();

  ITray(const ITray&) = delete;
  void operator=(const ITray&) = delete;

  virtual void SetActive() = 0;
  virtual void SetInactive() = 0;
};