package 
{
	import fl.controls.Button;
	import fl.controls.List;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.Socket;
	import flash.net.ObjectEncoding;
	
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
		private var onlineList: List;
		private var challengeButton: Button;
		
		
		public function Main(): void {
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}

		private function init(e: Event = null): void {
			removeEventListener(Event.ADDED_TO_STAGE, init);
			
			// entry point
			
			//adding onlineList
			onlineList = new List();
			onlineList.x = 0;
			onlineList.y = 0;
			onlineList.width = 100;
			onlineList.height = 200;
			//addChild(onlineList);
			
			//adding challenge button
			challengeButton = new Button();
			challengeButton.x = 0;
			challengeButton.y = 400;
			challengeButton.width = 100;
			challengeButton.height = 20;
			challengeButton.addEventListener(MouseEvent.CLICK, inviteToPlay);
			stage.addChild(challengeButton);
			
			
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
			getOnline();
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
			//TODO: implement method
		}
		
		private function onGotInvitation(result: uint, response: Object): void {
			//TODO: implement method
		}
		
		private function register(): void {
			connection.send(GameEvent.Register, { id: myId } );
		}
		
		private function auth(): void {
			trace("Logging in.");
			connection.send(GameEvent.Auth, { id: myId, password: myPassword } );
		}
		
		private function getOnline() : void {
			trace("Getting online players");
			connection.send(GameEvent.GetOnlinePlayers, null);
		}
		
		private function inviteToPlay(e: MouseEvent) : void {
			//var opp_id: uint = onlineList.selectedItem.label;
			var opp_id: uint = 3771777;
			trace("Challenge " + id);
			connection.send(GameEvent.InviteToPlay, {oppId: oppId} );
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
		];
	}
}