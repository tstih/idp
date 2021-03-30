using System;
using System.Linq;
using System.Text;

using Idp.Gpx.Snatch.Commands;
using Idp.Gpx.Common.CmdLine;

namespace Idp.Gpx.Snatch
{
    class Program
    {
        const int SUCCESS=0;
        const int MISSING_COMMAND = 1;
        const int INVALID_COMMAND = 2;
        const int INVALID_ARGS = 3;
        const int EXEC_FAILED = 4;

        static Cmd[] _commands=new Cmd[] {
            new ArrayCmd()
        };

        static public void Main(string[] args)
        {
            // Display welcome msg.
            Welcome();

            // Must have at least 1 arg.
            if (args.Length<1) 
                Error("Missing command.", MISSING_COMMAND);

            // Parse other args.
            string cmdName=args[0];
            Cmd cmd=_commands.FirstOrDefault(c=>cmdName.Equals(c.Name, StringComparison.CurrentCultureIgnoreCase));
            if (cmd==null) Error(string.Format("Invalid command {0}.", cmdName), INVALID_COMMAND);

            // Now parse the rest of arguments.
            ArgumentParser parser=new ArgumentParser();
            string result=parser.Parse(cmd,args,1);
            if (result!=null) 
                Error(result, INVALID_ARGS);
        
            // If we are here, we have evrything.
            StringBuilder std=new StringBuilder();
            StringBuilder err=new StringBuilder();
            var retCode = cmd.Execute(std,err);
            Report(std.ToString());
            if (retCode==0) 
                Environment.Exit(SUCCESS);
            else 
                Error(err.ToString(), EXEC_FAILED);
        }

        static void Welcome() {
            Console.WriteLine("Welcome to Snatch.");
            Console.WriteLine("MIT License, (c) 2021 Tomaz Stih, All rights reserved.");
            Console.WriteLine();
        }

        static void Usage() {
            Console.Error.WriteLine("Usage: snatch <command> <command args>.");
            Console.Error.Write("   <command> can be any of: {0}.", string.Join(',',_commands.Select(c=>c.Name)));
            Console.Error.WriteLine();
        }

        static void Error(string err, int retCode) {
            Console.Error.WriteLine(err);
            Console.Error.WriteLine();
            Usage();
            Environment.Exit(retCode);
        }

        static void Report(string msg) {
            Console.WriteLine(msg);
        }
        
    }              
}
