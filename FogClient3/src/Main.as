package 
{
	import fl.controls.LabelButton;
	import fl.controls.Label;
	import fl.controls.List;
	import fl.core.UIComponent;
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.Socket;
	import flash.net.ObjectEncoding;
	import flash.ui.Mouse;
	
	/**
	 * ...
	 * @author Rustam Ganeyev
	 */
	[Frame(factoryClass="Preloader")]
	public class Main extends Sprite 
	{
		
		//public var board : Board;
		public var connection: Connection;
		
		private var myId: int = 3371777;
		private var myPassword: String = "PASSWORD";
		
		private var challengeLabelButton: LabelButton;
		private var playLabelButton: LabelButton;
		
		private var sentChallengeList: List;
		private var receivedChallengeList: List;
		private var onlineList: List;
		
		private var onlineLabel: Label;
		private var sentChallengeLabel: Label;
		private var receivedChallengeLabel: Label;
		
		
		
		public function Main(): void {
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}

		private function init(e: Event = null): void {
			removeEventListener(Event.ADDED_TO_STAGE, init);
			
			// entry point
			
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
			
			connection.onConnected = onConnected;
			connection.connect();
			
			//onConnected -> onRegisterComplete -> onAuthComplete
		}
		
		private function addLabel(label: Label, x: int, y: int, width: int, height: int, text: String): void {
			label.text = text;
			addObj(label, x, y, width, height);
		}
		private function addList(list : List, x: int, y: int, width: int, height: int): void {
			addObj(list, x, y, width, height);
		}
		
		private function addLabelButton(button: LabelButton, x: int, y: int, width: int, 
										height: int, text: String, clickFunction: Function): void {
			button.label = text;
			button.addEventListener(MouseEvent.CLICK, clickFunction);
			addObj(button, x, y, width, height);
		}
		
		private function addObj(obj: UIComponent, x: int, y: int, width: int, height: int): void {
			obj.setSize(width, height);
			obj.x = x;
			obj.y = y;
			addChild(obj);
		}
		
		private function onConnected(): void {
			trace("Connected. Now registering...");
			register();	
		}
		
		private function onRegisterComplete(result: uint, response: Object): void {
			checkResult(result);
			auth();
		}

		private function onAuthComplete(result:uint, response: Object): void {
			checkResult(result);
			//getOnlinePlayers();
		}
		
		private function onGotOnlineList(result:uint, response: Object): void {
			//response = Array of objects, containing id.
			checkResult(result);
			
			trace("Online opponents: ");
			for (var i : uint = 0; i < response.length; i++) {
				trace(response[i].id);
				onlineList.addItem( {label: response[i].id} );
			}
			
		}
		
		private function onReceivedAnswerOnInvitation(result: uint, response: Object): void {
			checkResult(result);
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
			trace("Logging in.");
			connection.send(GameEvent.Auth, { id: myId, password: myPassword } );
		}
		
		private function getOnlinePlayers() : void {
			trace("Getting online players");
			connection.send(GameEvent.GetOnlinePlayers, null );
		}
		
		private function inviteToPlay(e: MouseEvent) : void {
			//var opp_id: uint = onlineList.selectedItem.label;
			var oppId: uint = 3371777;
			trace("Challenge " + oppId);
			connection.send(GameEvent.InviteToPlay, { id: oppId } );
		}
		
		private function acceptInvite(e: MouseEvent): void {
			if (receivedChallengeList.selectedItem == null) {
				return;
			}
			var oppId: uint = receivedChallengeList.selectedItem.label;
			
			connection.send(GameEvent.AcceptInvite, {id: oppId} );
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