package Game
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.ui.Mouse;
	import Game.Figure;
	
	/**
	 * ...
	 * @author Rustam Ganeyev
	 * 
	 */
	
	public class Cell extends MovieClip 
	{
		public static const size: uint = 50;		// size to visualize
		
		private var figure: Figure = null; 			//figure
		private var isWhite : Boolean = false;		//color of theCell
		private var isFogged : Boolean = false;		//
		
		
		public function Cell(x: int = 0, y: int = 0, name: String = "a1", isWhite: Boolean = false): void {
			stop();
			addEventListener(MouseEvent.MOUSE_DOWN, onDown);
			this.name = name;
			setPosition(x, y);
			setColor(isWhite);
		}
		
		public function onDown(evt: MouseEvent): void {
			if (figure == null) {
				return; 
			} else {
				figure.onMouseDown(evt);
			}
		}
		
		/// puts figure into cell
		public function setFigure(newFigure: Figure): void {
			if (newFigure == null) {
				return;
			}
			if (figure != null) {
				removeFigure();
			}
			figure = newFigure;
			resetFigure();
		}
		
		public function setPosition(x: int, y: int): void {
			this.x = x;
			this.y = y;
		}
		
		public function resetFigure(): void {
			figure.x = x + size / 2;
			figure.y = y + size / 2;
		}
		
		public function removeFigure(): void {
			figure = null;
		}
		public function setFog(isFogged: Boolean): void {
			this.isFogged = isFogged;
			reDraw();
		}
		
		public function setColor(isWhite: Boolean): void {
			this.isWhite = isWhite;
			reDraw();
		}
		
		private function reDraw(): void {
			if (isFogged) {
				gotoAndStop(3);
				return;
			}
			if (isWhite) {
				gotoAndStop(1);
			}
			else {
				gotoAndStop(2);
			}
			
		}
		
		override public function toString(): String {
			return name;
		}
		
		public function hasFigure(): Boolean {
			return (figure != null);
		}
		
		public function getFigure(): Figure {
			return figure;
		}
	}
}