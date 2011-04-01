package Game
{
	import flash.display.Loader;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	
	/**
	 * ...
	 * @author Rustam Ganeyev
	 * 
	 */
	
	public class Figure extends Sprite 
	{
		public static const WHITE : int = 0;
		public static const BLACK : int = 1;
		public var cell: Cell;
		private var figureColor: int; 
		
		public function Figure() {
			addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
		}
		
		public function onMouseDown(evt : MouseEvent): void {
			if (figureColor != Board(parent).getTurn()) { 
				return;
			}		
			startDrag(true);
		}
		
		
		public function onMouseUp(evt : MouseEvent): void {
			//check, if available
			stopDrag();			
			if (dropTarget == null || dropTarget.parent == null) {
				cell.resetFigure();
				trace("wrong destination");
				return;
			}
			
			var cellToMove: Cell;		
			var pawnedFigure: Figure = null;
			if (dropTarget.parent is Cell) {
				cellToMove = dropTarget.parent as Cell;
			} else {
				pawnedFigure = dropTarget.parent as Figure;
				cellToMove = pawnedFigure.cell;
			} 
			cell.resetFigure();
			Board(parent).sendMove(cell, cellToMove);
		}
		
		
		override public function toString() : String {
			return "Figure";
		}
		
		
		/// returns figure color
		public function getFigureColor() : int {
			return figureColor;
		}
		
		/// sets figure color
		public function setFigureColor(color : int) : void {
			this.figureColor = color;
		}
		
		/// abstract function 
		/// gets all available moves of figure
		public function getAvailableMoves() : int {
			return 0;
		}
	}

}