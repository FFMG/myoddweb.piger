#pragma once
class CommonWnd
{
public:
  explicit CommonWnd( const std::wstring& className );
  virtual ~CommonWnd();

  CommonWnd(const CommonWnd&) = delete;
  void operator=(const CommonWnd&) = delete;

  /**
   * \brief close the window.
   * \return if there was an error or not.
   */
  virtual bool Close();

  /**
   * \brief create the window
   * \return if there was an error or not.
   */
  virtual bool Create();

  /**
   * \brief get the window handle, it is safe because we do not throw...
   * \return the created handle.
   */
  HWND GetSafeHwnd() const {
    return _hwnd;
  }

protected:
  /**
   * \brief Create the class if it does not exist
   */
  bool CreateClass();

  const std::wstring _szClassName;
  WNDCLASSEX _wc;
  HWND _hwnd;

  virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);

  virtual bool OnInitDialog();

  virtual void OnPaint();

private:
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

