package
{
	/**
	 * ...
	 * @author Rustam Ganeyev
	 */
	public class GameEvent {
		public static const Register: uint	 	 	 = 0; 
		public static const Auth: uint			 	 = 1; 
		public static const GetOnlinePlayers: uint	 = 2; 
		public static const InviteToPlay: uint 	 	 = 3; 
		public static const GotInvitation: uint	 	 = 4; 
		public static const AcceptInvite: uint	 	 = 5; 
		public static const Move: uint 		 	 	 = 6; 
		public static const InformMove: uint	 	 = 7; 
		public static const GameStart: uint		     = 8; 
		public static const GameEnd: uint		     = 9; 
		public static const AddOnlinePlayer: uint    = 10;
		public static const RemoveOnlinePlayer: uint = 11;
	}
}
