package
{
	/**
	 * ...
	 * @author Rustam Ganeyev
	 */
	public class GameEvent {
		public static const Register: uint	 	 	 = 0; // id:int, password:string[8];
		public static const Auth: uint			 	 = 1; // id:int, password:string[8]
		public static const GetOnlinePlayers: uint	 = 2; // ответ online:[int]
		public static const InviteToPlay: uint 	 	 = 3; // oppId:int
		public static const GotInvitation: uint	 	 = 4; // oppId:int. ответ нужно слать с rc == 0 если принял, и 1, если отказал
		public static const AcceptInvite: uint	 	 = 5; // oppId:int response = 0, если принял
		public static const Move: uint 		 	 	 = 6; // from:string[2], to:string:[2]
		public static const GetState: uint		 	 = 7; // my_turn:bool(), whites:[string[3]], blacks:[string[3]]
		public static const GameStart: uint		     = 8; // oppId:int, и все содержимое GetState
		public static const GameEnd: uint		     = 9; 
		public static const AddOnlinePlayer: uint    = 10; //id
		public static const RemoveOnlinePlayer: uint = 11; //id
	}
}
