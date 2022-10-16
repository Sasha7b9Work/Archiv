/*
 * На передачу
 * 0xff, 0xfe, 0x00, _id, 0x01/0x10 (press/release), 0xfe, 0xff - нажата/отпущена кнопка
 * 0xff, 0xfe, 0x01, _id, 0x01/0x10 (left/right),    0xfe, 0xff - поворот регулятора влево/вправо
 * 
 * На приём
 * 0x80 : byte, _color : byte - установить цвет заливки
 * 0x81 : byte,	_color : byte - установить цвет рисования
 * 0x82 : byte - очистить экран
 * 0x83 : byte, _x0 : int16, _y0 : int16, _x1 : int16, _y1 : int16 - нарисовать линию
 * 0x84, _x0, _y0, _x1, _y1 - нарисовать закрашенный прямогольник
 * 0x85, _x0, _y0, _x1, _y1 - нарисовать векторный прямоугольник
 * 0x86, _x0, _y0, string - вывести строку в заданном месте
 * 0x85, _x0, _y0 - поставить точку
 * 
 * На приём для отладки
 * 0xf0, string - строка в окно вывода сообщений
 * 
 * Каждое сообщение начинается с 0xff, 0xfe, а заканчивается 0xfe, 0xff
 */

package  {
	import flash.errors.*;
	import flash.net.*;
	import flash.events.*;
	import flash.utils.*;
	import flash.system.*;
	
	public class Transmitter extends EventDispatcher {
		
		private var socket : Socket,
					numMessage : int = 0,
					queue : Vector.<int> = new Vector.<int>(),
                    transmittedBytes : int = 0,
                    recievedBytes : int = 0,
                    numBytesForOne : int = 100;
		
		public function Transmitter() {
		
			socket = new Socket();
			socket.addEventListener(Event.CONNECT, OnConnect);
			socket.addEventListener(IOErrorEvent.IO_ERROR, OnError);
			socket.addEventListener(ProgressEvent.SOCKET_DATA, OnRecieve);
			socket.addEventListener(SecurityErrorEvent.SECURITY_ERROR, OnSecurityError);
			socket.addEventListener(Event.CLOSE, OnClose);
	
			//socket.connect("192.168.1.200", 9999);
		}
		
		private function OnClose(_event : Event) : void {
			Main.debugWindow.AddString("Соединение закрыто");
		}
		
		/*
		private function OnTimerEvent(_event : TimerEvent) : void {
			
			var bytes : ByteArray = new ByteArray();
			while (bytes.length < numBytesForOne) {
				bytes.writeByte(0xff);
			}
			socket.writeBytes(bytes);
			transmittedBytes += numBytesForOne;
			socket.flush();
			
		}
		*/
		
		private function OnRecieve(_event : ProgressEvent) : void {
			
			/*
			var bytes : ByteArray = new ByteArray();
			socket.readBytes(bytes);
			socket.flush();
			recievedBytes += bytes.length;
			if(transmittedBytes % numBytesForOne == 0) {
				Main.debugWindow.AddString("Передано : " + transmittedBytes.toString() + ", принято : " + recievedBytes.toString());
			}
			*/
			
			
			GF.DBG_MSG("Enter OnRecieve()");
			var bytes : ByteArray = new ByteArray();
			socket.readBytes(bytes);
			while (bytes.bytesAvailable) {
				queue.push(bytes.readByte());
			}
			//Main.debugWindow.AddString("В принятой последоваетльности " + queue.length + " байт : " + GF.VectorBytesToString(queue, 16));
			socket.flush();
			var numCommandsInQueue : int = 0;
			while (DecodeMessage()) {
				numCommandsInQueue++;
			}
			
			//if (numCommandsInQueue > 1) {
			//	Main.debugWindow.AddString("Принято " + numCommandsInQueue + " комманд за раз");
			//}
			
			GF.DBG_MSG("Out OnRecieve()");
			
		}
		
		private function DecodeMessage() : Boolean {
			GF.DBG_MSG("Enter DecodeMessage()");
			var command : ByteArray = GetNextCommand();
			
			var code : int;
			code = command.readByte() & 0xff;
			if (code == 0xfa) {
				var string : String = new String();
				string = command.readUTFBytes(command.bytesAvailable);
				Main.debugWindow.AddString("Устройство : " + string);
			} else {
				Main.debugWindow.AddString("Неизвестная команда с кодом 0x" + code.toString(16));
			}
			
			GF.DBG_MSG("Out DecodeMessage()");
			return false;
		}
		
		private function GetNextCommand() : ByteArray {
			var command : ByteArray = new ByteArray();
			
			var startIndex : int = IndexStartMarker(),
				endIndex : int = IndexEndMarker(startIndex);
				
			if (endIndex > 0) {
				for (var i : int = startIndex + 2; i < endIndex; i++) {
					command.writeByte(queue[i]);
				}
				
				queue.splice(0, endIndex + 2);
				command.position = 0;
			}
			
			return command;
		}
		
		private function IndexStartMarker() : int {
			var size : int = queue.length;
			
			for (var i : int = 0; i < size - 1; i++) {
				if (queue[i] == -1 && queue[i + 1] == -2) {
					return i;
				}
			}
			
			return -1;
		}
		
		private function IndexEndMarker(_startMarker : int) : int {
			if (_startMarker == -1) {
				return - 1;
			}
			
			var size : int = queue.length;
			
			for (var i : int = _startMarker + 2; i < size - 1; i++) {
				if (queue[i] == -2 && queue[i + 1] == -1) {
					return i;
				}
			}
			
			return -1;
		}
	
		public function IsConnected() : Boolean {
			return socket.connected;
		}
		
		public function Connect() : void {
			socket.connect("192.168.1.200", 9999);
		}
		
		public function Disconnect() : void {
			socket.close();
		}
		
		public function SendBytes(_array : ByteArray) : void {
			if (socket.connected) {
				//Main.debugWindow.AddString("Send " + _array.length + " bytes : " + GF.ByteArrayToStringINT(_array, 16));
				socket.writeBytes(_array);
				socket.flush();
			}
		}
		
		private function WritePrefix() : void {
			socket.writeByte(0xff);
			socket.writeByte(0xfe);
		}
		
		private function WritePostfix() : void {
			socket.writeByte(0xfe);
			socket.writeByte(0xff);
			socket.flush();
		}
		
		public function SendButtonMessage(_button : TextButton) : void {
			WritePrefix();
			socket.writeByte(0x00);
			socket.writeByte(_button.ID());
			WritePostfix();
		}
		
		public function SendRegulatorMessage(_regulator : Regulator, _type : String) : void {
			WritePrefix();
			socket.writeByte(0x01);
			socket.writeByte(_regulator.ID());
			if (_type == RegulatorEvent.DECREASE) {
				socket.writeByte(0x10);
			} else {
				socket.writeByte(0x01);
			}
			WritePostfix();
		}
		
		private function OnConnect(_event : Event) : void {
			Main.debugWindow.AddString("Соединение установлено ");
		}
		
		private function OnError(_event : IOErrorEvent) : void {
			Main.debugWindow.AddString(_event.text);
			socket.connect("192.168.1.200", 9999);
		}
		
		private function OnSecurityError(_event : SecurityErrorEvent) : void {
			Main.debugWindow.AddString(_event.text);
			socket.connect("192.168.1.200", 9999);
		}
		
	}

}