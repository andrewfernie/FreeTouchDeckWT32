using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Configuration;
using FreeDeckWindows.Properties;

namespace FreeDeck
{
    class FreeDeckWindows
    {
        [DllImport("user32.dll")]
        static extern IntPtr GetForegroundWindow();
        [DllImport("user32.dll")]
        static extern bool SetForegroundWindow(IntPtr hWnd);
        [DllImport("user32.dll")]
        private static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);

        [return: MarshalAs(UnmanagedType.Bool)]


        SerialPort sp;
        Boolean init = false;

        static void Main(string[] args)
        {
            Console.Title = "Free Teams Deck";
            //Open the Program function
            new FreeDeckWindows();

        }


        private FreeDeckWindows()
        {
            Console.ForegroundColor = ConsoleColor.DarkGray;

            Console.WriteLine("                                             ");
            Console.WriteLine("                 &&&&&&&&&&                  ");
            Console.WriteLine("                &&&      &&&    &&&&&&       Microsoft Teams Deck");
            Console.WriteLine("                &&&      &&&   &&    &&      ");
            Console.WriteLine("  &&&&&&&&&&&&&&&&&&&&&&&&&     &&&&&&       Control Microsoft Teams from your Free Touch Deck");
            Console.WriteLine(" &&&&           &&&&&                        #FreeTeamsDeck");
            Console.WriteLine(" &&&&           &&&&&&&&&&&&&&&&&&&&&&&&     ");
            Console.WriteLine(" &&&&&&&&   &&&&&&&&&        &&        &&    ");
            Console.WriteLine(" &&&&&&&&   &&&&&&&&&        &&        &&    ");
            Console.WriteLine(" &&&&&&&&   &&&&&&&&&        &&        &&    ");
            Console.WriteLine(" &&&&&&&&   &&&&&&&&&        &&        &&    Application developed by:");
            Console.WriteLine("  &&&&&&&&&&&&&&&&&&         &&       &&     João Ferreira");
            Console.WriteLine("            &&&               &&&&&&&&&      https://teams.handsnontek.net/teamsdeck");
            Console.WriteLine("             &&&            &&&              ");
            Console.WriteLine("               &&&&&&&&&&&&&&                ");
            Console.WriteLine("                                             ");


            Console.ResetColor();

            string line;

            //Serial communication parameters
            string comPort = Settings.Default.ComPort;
            Console.Write(" COM Port Number (" + comPort + "): ");
            line = Console.ReadLine();
            if (line != "")
            {
                comPort = line;
                Settings.Default.ComPort = comPort;
                Settings.Default.Save();
            }


            int boundRate = Settings.Default.BaudRate;
            Console.Write(" Bound Rate (" + boundRate.ToString() + "): ");
            line = Console.ReadLine();
            if (line != "")
            {
                boundRate = int.Parse(line);
                Settings.Default.BaudRate = boundRate;
                Settings.Default.Save();
            }

            Console.Write(" Status: ");

            //Set the datareceived event handler
            try
            {
                sp = new SerialPort("COM" + comPort, boundRate, Parity.None, 8, StopBits.One);
                sp.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);
                //Open the serial port
                sp.Open();

                Console.ForegroundColor = ConsoleColor.DarkGreen;
                Console.WriteLine("Connected");
                Console.ResetColor();
                Console.WriteLine("");
                Console.Write(" To control Microsoft Teams when the application is not in focus, keep this window open");
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.DarkRed;
                Console.WriteLine("Unable to connect to Free Touch Deck");
                Console.ResetColor();
                Console.WriteLine("");
                Console.Write(" To learn how to connect to Free Touch Deck visit - https://teams.handsnontek.net/teamsdeck");
            }



            //Read from the console, to stop it from closing.
            Console.Read();

        }


        private void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            IntPtr currentWindow = GetForegroundWindow();

            try
            {
                //Variables
                string serialValue = sp.ReadLine();
                IntPtr teamsWindow = IntPtr.Zero;

                int commandStart = serialValue.IndexOf('{');
                int commandEnd = serialValue.IndexOf('}');
                string[] commands;
                bool commandsFound = false;
                int numCommands = 0;

                // Initialize the first elements of commands to empty strings.
                commands = new string[] { "", "" };


                // if the starting "{" and ending "}" were found, and the end was after the start, then the string was valid. If the string is valid, split it into substrings.
                if (commandStart != -1)
                {

                    if (commandEnd != -1)
                    {
                        if (commandEnd > commandStart)
                        {
                            commands = serialValue.Substring(commandStart + 1, commandEnd - commandStart - 1).Split(',');
                            numCommands = commands.Length;
                            commandsFound = (numCommands == 2);
                        }
                    }
                }


                //Get the main Teams process of Microsoft Teams
                Process[] processes = Process.GetProcessesByName("Teams");
                for (int i = 0; i < processes.Length; i++)
                {
                    if (processes[i].MainWindowHandle.ToString() != "0")
                    {
                        teamsWindow = processes[i].MainWindowHandle;
                        if (currentWindow == teamsWindow)
                        {
                            //Teams is already the Window in focus
                            return;
                        }
                    }
                }

                //Loop through the menu options to make and send the keys
                if (commandsFound)
                {
                    switch (commands[0])
                    {
                        case "Home":
                            switch (commands[1])
                            {
                                case "teams.bmp":
                                    ShowWindowAsync(teamsWindow, 1);
                                    SetForegroundWindow(teamsWindow);
                                    break;


                                default:
                                    break;

                            }
                            break;

                        case "Settings":
                            switch (commands[1])
                            {
                                case "teams.bmp":
                                    ShowWindowAsync(teamsWindow, 1);
                                    SetForegroundWindow(teamsWindow);
                                    break;

                                default:
                                    break;

                            }
                            break;
                        case "Teams":
                            switch (commands[1])
                            {
                                case "closeshare.bmp":
                                    ShowWindowAsync(teamsWindow, 1);
                                    SetForegroundWindow(teamsWindow);
                                    break;

                                case "mute.bmp":
                                case "unmute.bmp":
                                    break;

                                case "nosound.bmp":
                                    break;

                                case "hangup.bmp":
                                    break;

                                case "answer.bmp":
                                    break;

                                case "raisehand.bmp":
                                    break;

                                case "share.bmp":
                                    break;

                                case "videooff.bmp":
                                case "videoon.bmp":
                                    break;


                            }
                            break;
                        case "Zoom":
                            break;

                        case "Media":
                            break;

                        case "VSCode":
                            break;
                    }
                }


            }
            catch (Exception ex)
            {
                //if there is no internet 
            }
        }

    }
}
