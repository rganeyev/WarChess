package 
{
	import fl.controls.LabelButton;
	import fl.controls.Label;
	import fl.controls.List;
	import fl.controls.TextInput;
	import fl.core.UIComponent;
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.Socket;
	import flash.net.ObjectEncoding;
	import flash.ui.Mouse;
	import fl.events.ComponentEvent;
	import Game.Board;
	import Connection;
	
	/**
	 * ...
	 * @author Rustam Ganeyev
	 */
	[Frame(factoryClass="Preloader")]
	public class Main extends Sprite 
	{
		
		public var board : Board;
		public var connection: Connection;
		
		private var myId: uint = 3371777;
		private var myPassword: String = "PASSWORD";
		
		private var challengeLabelButton: LabelButton;
		private var playLabelButton: LabelButton;
		
		private var sentChallengeList: List;
		private var receivedChallengeList: List;
		private var onlineList: List;
		
		private var onlineLabel: Label;
		private var sentChallengeLabel: Label;
		private var receivedChallengeLabel: Label;
		private var textInput: TextInput;
		
		private var menuDisplayObjects: Array;
		
		
		
		public function Main(): void {
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}

		private function init(e: Event = null): void {
			removeEventListener(Event.ADDED_TO_STAGE, init);
			menuDisplayObjects = new Array();
			// entry point
			textInput = new TextInput();
			textInput.addEventListener(ComponentEvent.ENTER, start);
			addTextInput(textInput, 0, 0, 100, 30);
			
		}
		
		private function start(e: ComponentEvent): void {
			myId = uint(textInput.text);
			removeChild(textInput);
			menuDisplayObjects = new Array();
			//adding onlineList && label
			onlineList = new List();
			addList(onlineList, 0, 30, 100, 200);
			onlineLabel = new Label();
			addLabel(onlineLabel, 0, 0, 100, 30, "Online");
			
			//adding receivedChallengeList & label
			receivedChallengeList = new List();
			addList(receivedChallengeList, 150, 30, 100, 200);
			receivedChallengeLabel = new Label();
			addLabel(receivedChallengeLabel, 150, 0, 100, 30, "Received Challenges");
						
			//adding sentChallengeList & label
			sentChallengeList = new List();
			addList(sentChallengeList, 300, 30, 100, 200);
			sentChallengeLabel = new Label();
			addLabel(sentChallengeLabel, 300, 0, 100, 30, "Sent Challenges");
			
			
			//adding challenge button
			challengeLabelButton = new LabelButton();
			addLabelButton(challengeLabelButton, 0, 400, 100, 20, "Challenge!", inviteToPlay); 
			
			//adding play button
			playLabelButton = new LabelButton();
			addLabelButton(playLabelButton, 300, 400, 100, 20, "Play", acceptInvite);
			
			connection = new Connection();
			connection.addGameEventListener(GameEvent.Register, onRegisterComplete);
			connection.addGameEventListener(GameEvent.Auth, onAuthComplete);
			connection.addGameEventListener(GameEvent.GetOnlinePlayers, onGotOnlineList);
			connection.addGameEventListener(GameEvent.InviteToPlay, onReceivedAnswerOnInvitation);
			connection.addGameEventListener(GameEvent.GotInvitation, onGotInvitation);
			connection.addGameEventListener(GameEvent.AddOnlinePlayer, addPlayerToOnlineList);
			connection.addGameEventListener(GameEvent.RemoveOnlinePlayer, removePlayerFromOnlineList);
			connection.addGameEventListener(GameEvent.GameStart, startGame);
			connection.addGameEventListener(GameEvent.InformMove, onInformedMove);
			
			connection.onConnected = onConnected;
			connection.connect();
			
			//onConnected -> onRegisterComplete -> onAuthComplete
		}
		
		private function removePlayerFromOnlineList(result: uint, response: Object): void {
			checkResult(result);
			for (var i: uint = 0; i < onlineList.length; i++) {
				if (onlineList.getItemAt(i).data == response.id) {
					onlineList.removeItemAt(i);
					break;
				}
			}
		}
		
		private function addPlayerToOnlineList (result: uint, response: Object): void {
			getErrorDescription(result);
			onlineList.addItem( { label: response.id, data: response.id } );
		}
		
		private function addLabel(label: Label, x: int, y: int, width: int, height: int, text: String): void {
			label.text = text;
			addComponent(label, x, y, width, height);
		}
		private function addList(list : List, x: int, y: int, width: int, height: int): void {
			addComponent(list, x, y, width, height);
		}
		
		private function addTextInput(inp: TextInput, x: int, y: int, width: int, height: int): void {
			addComponent(inp, x, y, width, height);
		}
		
		private function addLabelButton(button: LabelButton, x: int, y: int, width: int, 
										height: int, text: String, clickFunction: Function): void {
			button.label = text;
			button.addEventListener(MouseEvent.CLICK, clickFunction);
			addComponent(button, x, y, width, height);
		}
		
		private function addComponent(component: UIComponent, x: int, y: int, width: int, height: int): void {
			component.setSize(width, height);
			component.x = x;
			component.y = y;
			menuDisplayObjects.push(component);
			addChild(component);
		}
		
		private function onConnected(): void {
			//trace("Connected. Now registering...");
			register();	
		}
		
		private function onRegisterComplete(result: uint, response: Object): void {
			checkResult(result);
			auth();
		}

		private function onAuthComplete(result:uint, response: Object): void {
			checkResult(result);
			getOnlinePlayers();
		}
		
		//response = Array of objects, containing id.
		private function onGotOnlineList(result:uint, response: Object): void {
			checkResult(result);
			
			trace("Online opponents: ");
			for (var i : uint = 0; i < response.length; i++) {
				trace(response[i].id);
				var caption: String = int(response[i].id).toString();
				if (response[i].id == myId) {
					caption += "me";
				}
				onlineList.addItem( {label: caption, data: response[i].id} );
			}
			
		}
		
		private function onReceivedAnswerOnInvitation(result: uint, response: Object): void {
			checkResult(result);
			if (result != 0) {
				return;
			}

			trace("invited " + response.id);
			sentChallengeList.addItem( {label: response.id} )
		}
		
		private function onGotInvitation(result: uint, response: Object): void {
			checkResult(result);
			trace("got invitation from " + response.id);
			receivedChallengeList.addItem( {label: response.id} );
		}
		
		private function register(): void {
			connection.send(GameEvent.Register, { id: myId } );
		}
		
		private function auth(): void {
			//trace("Logging in.");
			connection.send(GameEvent.Auth, { id: myId, password: myPassword } );
		}
		
		private function getOnlinePlayers() : void {
			//trace("Getting online players");
			connection.send(GameEvent.GetOnlinePlayers, {} );
		}
		
		private function inviteToPlay(e: MouseEvent) : void {
			if (onlineList.selectedItem == null) {
				//trace("select online player");
				return;
			}
			var oppId: uint = onlineList.selectedItem.data;
			//var oppId: uint = 3371777;
			//trace("Challenge " + oppId);
			connection.send(GameEvent.InviteToPlay, { id: oppId } );
		}
		
		private function acceptInvite(e: MouseEvent): void {
			if (receivedChallengeList.selectedItem == null) {
				return;
			}
			var oppId: uint = receivedChallengeList.selectedItem.label;
			
			connection.send(GameEvent.AcceptInvite, {id: oppId, response: 0} );
		}
		
		private function startGame(result: uint, response: Object): void {
			//TODO: implement start game
			for (var i: uint = 0; i < menuDisplayObjects.length; i++) {
				if (menuDisplayObjects[i].stage) { 
					removeChild(menuDisplayObjects[i]);
				}
			}
			
			var whiteFigures: Array = response.white;
			var blackFigures: Array = response.black;
			var fogCells: Array = response.fog;
			var whiteTurn: Boolean = response.whiteTurn;
			//board = new Board(whiteTurn, whiteFigures, blackFigures, fogCells);
			//addChild(board);
		}
		
		private function onInformedMove(result: uint, response: Object): void {
			var from: String = response.from;
			var to: String = response.to;
			board.getMove(from, to);
		}
		
		private function getErrorDescription(errorCode: uint): String {
			switch (errorCode) {
				case 0: return "Success";
				case 1: return "PlayerAlreadyExists";
				case 2: return "PlayerNotFound";
				case 3: return "PasswordIsIncorrect";
				case 4: return "PlayerNotAuthorized";
				case 5: return "IncorrectMessage";
				case 6: return "UnknownMethod";
				case 7: return "OutOfMemory";
				case 8: return "PlayerAlreadyAuthorized"; 
				case 9: return "PlayerAlreadyChallenged";
				default: return "Unknown error";
			}
		}
		
		private function checkResult(result: uint): void {
			if (result != 0) {
				trace(getErrorDescription(result));
			}
		}
		
		private var dummy: Object = [
			ScrollArrowDown_disabledSkin,
			ScrollArrowDown_upSkin,
			ScrollArrowDown_downSkin,
			ScrollArrowDown_overSkin,
			ScrollArrowUp_disabledSkin,
			ScrollArrowUp_upSkin,
			ScrollArrowUp_downSkin,
			ScrollArrowUp_overSkin,
			
			ScrollTrack_skin,
			ScrollThumb_upSkin,
			ScrollThumb_downSkin,
			ScrollThumb_overSkin,
			
			Button_overSkin,
			Button_upSkin,
			Button_downSkin,
			Button_selectedOverSkin,
			Button_selectedUpSkin,
			Button_selectedDownSkin,
			List_skin,
			
			ComboBox_upSkin,
			ComboBox_overSkin,
			ComboBox_downSkin,
			ComboBox_disabledSkin,
		];
	}
}