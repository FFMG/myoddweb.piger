//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
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

  /**
   * \brief get the instance handle, it is safe because we do not throw...
   * \return the instance handle.
   */
  HINSTANCE GetSafeHinstance() const {
    return _hinstance;
  }
  
protected:
  /**
   * \brief Create the class if it does not exist
   */
  bool CreateClass();

  const std::wstring _szClassName;
  WNDCLASSEX _wc;
  HWND _hwnd;
  HINSTANCE _hinstance;

  /**
   * \brief called wnen a message is recieved to give the derived class a chance to handle the message
   * \param msg the message code
   * \param wParam the given wide parameter.
   * \param lParam long param
   * \return the result of the call.
   */
  virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam);

  /**
   * \brief called after the window was successfully created.
   * \return bool result to init dialog.
   */
  virtual bool OnInitDialog();

  /**
   * \brief called at regular intervals to repaint the window.
   */
  virtual void OnPaint();

private:
  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

