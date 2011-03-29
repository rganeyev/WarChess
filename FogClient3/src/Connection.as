package
{
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.ProgressEvent;
	import flash.net.Socket;
	import flash.utils.ByteArray;
	import GameEvent;
	import flash.net.ObjectEncoding;
	import fl.controls.List;
	
	/**
	 * ...
	 * @author Rustam Ganeyev
	 */
	public class Connection
	{
		
		//init 
		public function Connection() {
			gameEvents = new Array();
			buffer = new ByteArray();
			buffer.objectEncoding = ObjectEncoding.AMF0;

			socket = new Socket();
			socket.objectEncoding = ObjectEncoding.AMF0;

			socket.addEventListener(IOErrorEvent.IO_ERROR, onError);
			socket.addEventListener(Event.CONNECT, _onConnected);
			socket.addEventListener(Event.CLOSE, _onDisconnected);
			socket.addEventListener(ProgressEvent.SOCKET_DATA, onDataReceived);
		}

		/**
		 * Function to process received message
		 * Firstly, we read the messageLength
		 * and wait until we have all message to come
		 * 
		 * MessageLength must be >= 8 (code + result + message(may be null)>= 8)
		 */
		private function onDataReceived(e:ProgressEvent): void {
			var bytesAvailable: uint = socket.bytesAvailable;
			trace("received " + bytesAvailable + " bytes");
			while (true) {
				if (!sizeRead) {
					if (bytesAvailable < 4) {
						return;
					}
					messageLength = socket.readUnsignedInt() - 4;
					if (messageLength < 8) {
						return;
					}
					bytesAvailable -= 4;		
					sizeRead = true;
				}
				
				if (bytesAvailable < messageLength) {
					return;
				}
				
				var code: uint  = socket.readUnsignedInt();
				var result: uint = socket.readUnsignedInt();
				var message: Object = null;
				//we have an object
				if (messageLength > 8) {
					message = socket.readObject();
				}
				onEvent(code, result, message);
				sizeRead = false;
				bytesAvailable -= messageLength;
			}
		}

		/**
		 * process incoming message
		 * @param	code the code. @see GameEvent.as
		 * @param	result 
		 * @param	obj
		 */
		private function onEvent(code: uint, result: uint, message: Object): void {
			var callback: Function = gameEvents[code];
			if (callback != null) {
				callback(result, message);
			}
		}

		private function _onDisconnected(e: Event): void {
			if (onDisconnected != null) {
				onDisconnected();
			}
		}

		private function _onConnected(e:Event):void {
			if (onConnected != null) {
				onConnected();
			}
		}

		private function onError(e:IOErrorEvent):void {
			trace("Connection failure");
		}

		public function get connected(): Boolean {
			return socket.connected;
		}
		
		public function connect(): void {
			if (!socket.connected) {
				socket.connect(host, port);
			}
		}

		/**
		 * Function to send message to server
		 * @param	code the code. @see GameEvent
		 * @param	obj  object to send.
		 * 
		 * Firstly we send the messageLength, then we send code and object
		 */
		public function send(code: uint, obj: Object): void {
			if (!socket.connected) {
				appendLog("ERROR: Socket not connected");
				return;
			}
			
			buffer.position = 0;
			buffer.writeUnsignedInt(code);
			buffer.writeObject(obj);

			trace("sent " + (buffer.position + 4) + " bytes");
			socket.writeUnsignedInt(buffer.position + 4);
			socket.writeBytes(buffer);
			socket.flush();
		}

		public function addGameEventListener(event: uint, callback: Function): void {
			gameEvents[event] = callback;
		}

		private function appendLog(text: String): void {
			trace(text);
		}
		
		private var buffer: ByteArray;
		private var socket: Socket;
		private var host: String = "localhost";
		private var port: int = 3113;
		private var sizeRead: Boolean = false;
		private var messageLength: uint = 0;
		private var gameEvents: Array;

		public var onConnected: Function;
		public var onDisconnected: Function;
	}
}