package Game
{
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.system.ApplicationDomain;
	import Game.Figure;
	import Game.Cell;
	
	/**
	 * ...
	 * @author Rustam Ganeyev
	 * 
	 */
	public class Board extends Sprite
	{
		private var objectsOnBoard: uint = 0;
		private const horizName: Array =  new Array( "a", "b", "c", "d", "e", "f", "g", "h" );
		private const vertName: Array  =  new Array ( "1", "2", "3", "4", "5", "6", "7", "8" );
		
		private static var cells: Array = new Array(8);
	
		private var whiteTurn: Boolean = Figure.WHITE;
		private var field: Field;
		
		
		private var fogCells:Array = new Array();
		private var whites:  Array = new Array("Pa2","Pb2","Pc2","Pd2", "Pe2","Pf2","Pg2","Ph2","Ra1","Rh1","Nb1","Ng1","Bc1","Bf1","Qd1","Ke1");
		private var blacks:  Array = new Array("Pa7", "Pb7", "Pc7","Pd7","Pe7","Pf7","Pg7","Ph7","Ra8","Rh8","Nb8","Ng8","Bc8","Bf8","Qd8","Ke8");
		
		//constructor
		public function Board(whiteFigures: Array, blackFigures: Array, fog: Array): void {		
			whiteTurn = Figure.WHITE;
			fogCells = fog;
			whites = whiteFigures;
			blacks = blackFigures;
			
			//set cells
			initCells();
			
			//set fog
			initFog();
			
			//figure init
			initFigures(whiteFigures, Figure.WHITE);
			initFigures(blackFigures, Figure.BLACK);	
		}
		
		//sends move to server to validation
		public function sendMove(from: Cell, to: Cell): void {
			Main(parent).move(from.toString(), to.toString());
			
		}
		
		public function moveFigureToFront(fig: Figure): void {
			setChildIndex(fig, objectsOnBoard - 1);
		}
		
		//gets move from server
		public function getMove(whiteTurn: Boolean, from: String, to: String): void {
			//debug
			if (whiteTurn == this.whiteTurn) {
				return;
			}
			
			this.whiteTurn = whiteTurn;
			var toCell: Cell = getCellByString(to);
			var fromCell: Cell = getCellByString(from);
			
			//delete pawned figure
			var pawnedFigure: Figure = toCell.getFigure();
			var figure: Figure = fromCell.getFigure();
			
			if (pawnedFigure != null) {
				removeBoardChild(pawnedFigure);
			}
			fromCell.removeFigure();
			toCell.setFigure(figure);
			
		}
		
		public function initFog(): void {
			if (fogCells == null) {
				return;
			}
			for (var tmp: String in fogCells) {
				getCellByString(tmp).setFog(true);
			}
		}
		
		public function initCells(): void {
			//y - строчка
			//x - столбец
			var hPos: int = -Cell.size;
			for (var y: int = 7; y >= 0; y--) {
				hPos += Cell.size;
				var tmpArray: Array = new Array();
				var wPos: int = 0;
				for (var x: int = 0; x < 8; x++) {
					field = new Field();
					field.setPosition(wPos, hPos);
					field.name = horizName[x] + vertName[y];
					field.setColor((x % 2) !=  (y % 2));
					tmpArray.push(field);
					addBoardChild(tmpArray[x]);
					wPos += Cell.size;
				}
				
				cells[y] = tmpArray;
			}
		}
		
		private function addBoardChild(obj: DisplayObject): void {
			objectsOnBoard++;
			addChild(obj);
		}
		
		private function removeBoardChild(obj: DisplayObject): void {
			objectsOnBoard--;
			removeChild(obj);
		}
		/////////////////////////
		//figure initialization
		/////////////////////////
		private function initFigures(colorPiece: Array, isWhite: Boolean): void {
			for (var k: uint = 0 ; k < colorPiece.length; ++k) {
				var figure: Figure;
				switch(colorPiece[k].fig.charAt(0)) {
					case "R":
						if (isWhite) figure = new RookWhite(); 
						else		 figure = new RookBlack();
						break;
					case "B":
						if (isWhite) figure = new BishopWhite(); 
						else		 figure = new BishopBlack();
						break;
					case "N":
						if (isWhite) figure = new KnightWhite();
						else		 figure = new KnightBlack();
						break;
					case "Q":
						if (isWhite) figure = new QueenWhite();
						else 		 figure = new QueenBlack();
						break;
					case "K":		 
						if (isWhite) figure = new KingWhite();
						else 		 figure = new KingBlack();
						break;
					case "P":
						if (isWhite) figure = new PawnWhite();
						else		 figure = new PawnBlack();
						break;
					default:
						throw Error("unknown piece");
				}
				
				if (isWhite) {
					figure.setFigureColor(Figure.WHITE)
				} else {
					figure.setFigureColor(Figure.BLACK);
				}
				var cell: Cell = getCellByString(colorPiece[k].fig.substr(1, 2));
				figure.cell = cell;
				cell.setFigure(figure);
				addBoardChild(figure);
			}
		}
		
		private function getCellByString(cell : String): Cell {
			var tmpY : int = cell.charCodeAt(0) - String("a").charCodeAt(0);
			var tmpX : int = cell.charCodeAt(1) - String("1").charCodeAt(0);
			return (cells[tmpX][tmpY]);
		}
		
		private function getCell(x : int, y : int) : Cell {
			return cells[y][x];
		}
		
		public function getTurn(): Boolean {
			return whiteTurn;
		}	
	}

}