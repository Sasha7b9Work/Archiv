using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Threading;
using System.Drawing;

namespace Controller_S8_53 {

    public class RecievedEventArgs : EventArgs {

        public byte[] data;
        public int numBytes;

        public RecievedEventArgs(byte[] data, int numBytes) {
            this.data = data;
            this.numBytes = numBytes;
        }
    }

    class ComPort {

        enum TypeDisplay {
            None,
            Color,
            Monochrome
        };
        
        private static SerialPort port;
        private static string[] ports;
        private static Mutex mutex = new Mutex();
        
        private TypeDisplay typeDisplay = TypeDisplay.None;

        public ComPort () {
            port = new SerialPort();
            port.ReadTimeout = 100;
            port.BaudRate = 125000;
        }
        
        public void Stop() {
            port.Close();
        }

        public string[] GetPorts() {
            ports = SerialPort.GetPortNames();
            return ports;
        }

        public bool DeviceConnectToPort(int numPort) {
            port.PortName = ports[numPort];
            string answer;
            try {
                port.Open();
                if(port.IsOpen) {
                    SendString("REQUEST ?");
                    answer = ReadLine();
                    port.Close();
                    bool retValue = (answer == "S8-53" || answer == "S8-53/1");
                    return retValue;
                }
            } catch(SystemException) {
                port.Close();
            }
            return false;
        }

        public void SendByte ( byte data ) {
            if(port.IsOpen) {
                byte[] sendingData = new byte[1];
                sendingData[0] = data;
                port.Write(sendingData, 0, 1);
            }
        }

        public void SendString(string str) {
            mutex.WaitOne();

            if(port.IsOpen) {
                while(port.BytesToWrite != 0) { };
                port.Write(":" + str + "\x0d");
            }

            mutex.ReleaseMutex();
        }

        public void SendBytes(byte[] buffer) {
            port.Write(buffer, 0, buffer.Length);
        }

        static public string ReadLine() {
            string line;
            try {
                line = port.ReadLine();
                return line.Substring(0, line.Length - 2);
            } catch(SystemException) {

            }
            return null;
        }

        public void OnlyOpen(int numPort) {
            port.PortName = ports[numPort];
            port.Open();
        }

        public void OnlyClose() {
            port.Close();
        }

        public bool Open(int numPort)
        {
            try
            {
                port.PortName = ports[numPort];
                port.Open();
                if(port.IsOpen)
                {
                    SendString("REQUEST ?");
                    string answer = ReadLine();
                    if(answer == "S8-53")
                    {
                        typeDisplay = TypeDisplay.Monochrome;
                    }
                    else if(answer == "S8-53/1")
                    {
                        typeDisplay = TypeDisplay.Color;
                    }
                    else
                    {
                        typeDisplay = TypeDisplay.None;
                    }
                }
            }
            catch(SystemException)
            {
                port.Close();
            }
            return port.IsOpen;
        }

        private static byte[] data = new byte[16 * 1024];

        public SerialPort GetSerialPort()
        {
            return port;
        }

        public bool IsOpen() {
            return port.IsOpen;
        }
    }
}
