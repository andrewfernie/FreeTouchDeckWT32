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

            // Get list of available serial ports and print it
            string[] ports = SerialPort.GetPortNames();
            Console.WriteLine("Available serial ports:");
            foreach (string port in ports)
            {
                Console.WriteLine(port);
            }
            Console.WriteLine("");


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
            int numCommands = 0;
            string[] commands = { "" };

            try
            {
                //Variables
                string serialValue = sp.ReadLine();
                IntPtr teamsWindow = IntPtr.Zero;


                // extract the text between "{" and "}" from serialValue
                string[] words = serialValue.Split('{', '}');

                // if "words" has 3 elements, then it contains the text between "{" and "}"
                if (words.Length == 3)
                {
                    commands = words[1].Split(',');
                    numCommands = commands.Length;
                }
                else
                {
                    numCommands = 0;
                }



                //Get the main Teams process of Microsoft Teams
                Process[] processes = Process.GetProcessesByName("Teams");
                for (int i = 0; i < processes.Length; i++)
                {
                    if (processes[i].MainWindowHandle.ToString() != "0")
                    {
                        teamsWindow = processes[i].MainWindowHandle;

                    }
                }



                //Loop through the menu options to make and send the keys
                if (numCommands > 1)
                {
                    string callingPage = "";
                    string newPage = "";
                    string buttonLogo = "";

                    switch (commands[0].Trim())
                    {
                        case "ButtonPress":
                            callingPage = commands[1].Trim(); 
                            buttonLogo = commands[2].Trim();

                            switch (callingPage)
                            {
                                case "Teams":
                                    if (currentWindow != teamsWindow)
                                    {
                                        ShowWindowAsync(teamsWindow, 1);
                                        SetForegroundWindow(teamsWindow);
                                    }
                                    
                                    switch (buttonLogo)
                                    {
                                        case "closeshare":
                                            break;

                                        case "mute":
                                        case "unmute":
                                            break;

                                        case "nosound":
                                            break;

                                        case "hangup":
                                            break;

                                        case "answer":
                                            break;

                                        case "raisehand":
                                            break;

                                        case "share":
                                            break;

                                        case "videooff":
                                        case "videoon":
                                            break;
                                    }
                                    break;
                                    
                                case "Zoom":
                                    break;

                                case "Media":
                                    break;

                                case "VSCode":
                                    break;

                                default:
                                    break;

                            }                          
                            break;

                        case "NewPage":
                            callingPage = commands[1].Trim();
                            newPage = commands[2].Trim();

                            switch (newPage)
                            {
                                case "Home":
                                    break;
                                case "Info":
                                    break;
                                case "Settings":
                                    break;
                                    
                                case "Teams":
                                    if (currentWindow != teamsWindow)
                                    {
                                        ShowWindowAsync(teamsWindow, 1);
                                        SetForegroundWindow(teamsWindow);
                                    }
                                    break;
                                    
                                default:
                                    break;
                            }

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
