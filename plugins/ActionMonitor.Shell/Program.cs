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
using ActionMonitor.Shell.Runners;
using myoddweb.commandlineparser;

namespace ActionMonitor.Shell
{
  internal class Program
  {
    private static class ExitCode
    {
      /// <summary>
      /// The execution of the code worked fine.
      /// </summary>
      public static int Succeeded { get; }= 0;

      /// <summary>
      /// Something did not wor.
      /// </summary>
      public static int UnknownError { get; } = 1;

      /// <summary>
      /// There was an argument error
      /// </summary>
      public static int InvalidCommandLineArgument { get; } = 2;

      /// <summary>
      /// The path given could not be found.
      /// </summary>
      public static int InvalidPath { get; } = 3;
    }

    private static class Argument
    {
      /// <summary>
      /// The name of the UUID argument.
      /// </summary>
      public static string Uuid { get; } = "uuid";

      /// <summary>
      /// The path of the item we are trying to run, (dll, code etc...)
      /// </summary>
      public static string Path { get; } = "path";
    }

    private static IRunner CreateRunner(CommandlineParser parser )
    {
      // get the unique id.
      var uuid = parser.Get(Argument.Uuid).Trim();
      if (string.IsNullOrEmpty(uuid))
      {
        throw new ArgumentException( nameof(uuid ));
      }

      // then get the path.
      var path = parser.Get(Argument.Path);
      if (!File.Exists(path))
      {
        throw new FileNotFoundException( "Could not locate path", path );
      }
      var fileInfo = new FileInfo( path );

      // create the action monitor
      var actionMonitor = new Core( uuid );

      // create the pluggin.
      return new Plugin(actionMonitor, fileInfo );
    }

    private static int Main(string[] args)
    {
      try
      {
        var parser = new CommandlineParser(args, new Dictionary<string, CommandlineData>
        {
          {Argument.Uuid, new CommandlineData {IsRequired = true}}, //  the unique id
          {Argument.Path, new CommandlineData {IsRequired = true}} //  the path to what we want to run from shell.
        });

        // create the runner.
        var runner = CreateRunner(parser);


        return ExitCode.Succeeded;
      }
      catch (FileNotFoundException e)
      {
        Console.WriteLine($"The given path '{e.FileName}' does not exist: {e}");
        return ExitCode.InvalidPath;
      }
      catch (ArgumentException e)
      {
        Console.WriteLine( $"Invalid command line argument: {e}");
        return ExitCode.InvalidCommandLineArgument;
      }
      catch( Exception e )
      {
        Console.WriteLine(e);
        return ExitCode.UnknownError;
      }
    }
  }
}
