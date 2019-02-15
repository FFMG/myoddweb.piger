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
using System;

namespace ActionMonitor.Interfaces
{
  public interface IActionMonitor
  {
    /// <summary>
    /// Output a message on the screen.
    /// </summary>
    /// <param name="what">The message</param>
    /// <param name="elapse">For how long, (ms)</param>
    /// <param name="fadeout">The fadeout speed.</param>
    /// <returns></returns>
    bool Say(string what, uint elapse, uint fadeout);

    /// <summary>
    /// Get the Powershell version number.
    /// </summary>
    /// <returns></returns>
    string Version();

    /// <summary>
    /// Get a command at a certain index.
    /// We throw if the value is out of range, use GetCommandCount(...)
    /// Command #0 is the full path of this script.
    /// </summary>
    /// <param name="index">the index we want</param>
    /// <returns>The command</returns>
    string GetCommand(uint index);

    /// <summary>
    /// Get the action name.
    /// </summary>
    /// <returns>The name of the action that we called.</returns>
    string GetAction();

    /// <summary>
    /// Get the number of arguments entered after the command.
    /// </summary>
    /// <returns>The number of commands entered.</returns>
    int GetCommandCount();

    /// <summary>
    /// Execute a process.
    /// </summary>
    /// <param name="module">The process we want to execute.</param>
    /// <param name="args">the arguments to pass</param>
    /// <param name="priviledged">if we want it to be elevated or not.</param>
    /// <returns></returns>
    bool Execute(string module, string args, bool priviledged);

    /// <summary>
    /// Get the currently selected string, (if any).
    /// </summary>
    /// <param name="quote"></param>
    /// <returns></returns>
    string Getstring(bool quote);

    /// <summary>
    /// Get a selected file at an index.
    /// </summary>
    /// <param name="index">the index we want</param>
    /// <param name="quote">if we want to quote the text or not.</param>
    /// <returns></returns>
    string Getfile(uint index, bool quote);

    /// <summary>
    /// Get the currenctly selected folder by index.
    /// </summary>
    /// <param name="index">The folder we are looking for.</param>
    /// <param name="quote">If we want to quote the response or not.</param>
    /// <returns></returns>
    string Getfolder(uint index, bool quote);

    /// <summary>
    /// Get the url by index.
    /// </summary>
    /// <param name="index">The url we are looking for.</param>
    /// <param name="quote">If we want to quote the response or not.</param>
    /// <returns></returns>
    string Geturl(uint index, bool quote);

    /// <summary>
    /// Get the application version number.
    /// </summary>
    /// <returns>The app version number.</returns>
    string GetVersion();

    /// <summary>
    /// Log a message to the main application log.
    /// </summary>
    /// <param name="type"></param>
    /// <param name="message"></param>
    /// <returns>Success or not</returns>
    bool Log(int type, string message);

    /// <summary>
    /// Add an action
    /// </summary>
    /// <param name="action">The action we want to add.</param>
    /// <param name="path">The path of the action we are adding.</param>
    /// <returns>Success or not</returns>
    bool AddAction(string action, string path);

    /// <summary>
    /// Remove an action given a path
    /// </summary>
    /// <param name="action">The action we want to remove.</param>
    /// <param name="path">The path of the action we want to remove.</param>
    /// <returns>Success or not</returns>
    bool RemoveAction(string action, string path);

    /// <summary>
    /// Find an action at a given index.
    /// So if we are looking for a command "stuff", there could be more than one.
    /// And index 0, 1, ..., x are the posible numbers.
    /// </summary>
    /// <param name="index">The action index we are looking for.</param>
    /// <param name="action">The action we want.</param>
    /// <returns>Success or not</returns>
    string FindAction(uint index, string action);

    /// <summary>
    /// Get the foreground window at the time of the call. 
    /// </summary>
    /// <returns>IntPtr the current fogregorund window.</returns>
    IntPtr GetForegroundWindow();
  }
}
