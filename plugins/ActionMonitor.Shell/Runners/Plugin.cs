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
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Threading;
using System.Threading.Tasks;
using ActionMonitor.Interfaces;

namespace ActionMonitor.Shell.Runners
{
  internal class Plugin : IRunner
  {
    /// <inheritdoc />
    public IActionMonitor ActionMonitor { get; }

    /// <inheritdoc />
    public FileSystemInfo FileSystemInfo { get; }

    public Plugin(IActionMonitor actionMonitor, FileSystemInfo fileSystemInfo)
    {
      // set the action monitor interface.
      ActionMonitor = actionMonitor ?? throw new ArgumentNullException( nameof(actionMonitor) );

      // set the file infor.
      FileSystemInfo = fileSystemInfo ?? throw new ArgumentNullException(nameof(fileSystemInfo));
    }

    /// <inheritdoc />
    public async Task GoAsync()
    {
      try
      {
        // get all the instances
        var instances = LoadInstances<IAction>(FileSystemInfo);

        // create a cancelation token
        var cts = new CancellationTokenSource();

        // then tell them all to initialise.
        await InitializeAllActionsAsync(instances, cts.Token).ConfigureAwait(false);

        // then run them all
        await RunAllActionsAsync(instances, cts.Token).ConfigureAwait(false);
      }
      catch( Exception e )
      {
        ActionMonitor.Log(1, $"Exception running the plugin {FileSystemInfo.FullName}: {e.ToString()}");
        throw;
      }
    }

    #region Private function 
    /// <summary>
    /// Initialise all the actions.
    /// </summary>
    /// <param name="actions">The actions we will be calling</param>
    /// <param name="token">The token to pass to the action.</param>
    /// <returns></returns>
    private async Task InitializeAllActionsAsync(IList<IAction> actions, CancellationToken token)
    {
      var tasks = new List<Task>(actions.Count);
      foreach (var action in actions)
      {
        tasks.Add(action.InitializeAsync(ActionMonitor, token));
      }
      await Task.WhenAll(tasks).ConfigureAwait(false);
    }

    /// <summary>
    /// Runn all the actions.
    /// </summary>
    /// <param name="actions">The actions we will be calling</param>
    /// <param name="token">The token to pass to the action.</param>
    /// <returns></returns>
    private async Task RunAllActionsAsync(IList<IAction> actions, CancellationToken token)
    {
      var tasks = new List<Task>(actions.Count);
      foreach (var action in actions)
      {
        tasks.Add(action.GoAsync(token));
      }
      await Task.WhenAll(tasks).ConfigureAwait(false);
    }

    /// <summary>
    /// Load all the instances of a certain interface.
    /// There could be more than one per file... so we get them al,
    /// </summary>
    /// <typeparam name="T">The interface we are looking for.</typeparam>
    /// <param name="fileSystemInfo">The file we are looking for the interfaces in.</param>
    /// <returns></returns>
    private static IList<T> LoadInstances<T>(FileSystemInfo fileSystemInfo)
    {
      var instances = new List<T>();
      var assembly = Assembly.LoadFile(fileSystemInfo.FullName);
      var types = assembly.GetTypes();
      foreach (var type in types.Where(t => t.IsClass && t.IsPublic))
      {
        var interfaces = type.GetInterfaces();
        if (!interfaces.Contains(typeof(T)))
        {
          continue;
        }

        var obj = Activator.CreateInstance(type);
        var t = (T) obj;
        instances.Add(t);
      }
      return instances;
    }
    #endregion
  }
}
