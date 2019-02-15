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
    }

    private static int Main(string[] args)
    {
      try
      {
        var parser = new CommandlineParser(args, new Dictionary<string, CommandlineData>
        {
          { "uuid", new CommandlineData{ IsRequired = true}},   //  the unique id
          { "path", new CommandlineData{ IsRequired = true}}    //  the path to what we want to run from shell.
        });
        return ExitCode.Succeeded;
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
