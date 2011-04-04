package {
	
	import flash.display.Sprite;
	import flash.geom.Point;

    public class Cloud extends Sprite {
	
		private var _radius:int = 20;
		private var _color:uint = 0x0000ff;

        public function Cloud(point:Point,radius:int,color:uint) {
			this.x = point.x;
			this.y = point.y;
			_radius = radius;
			_color = color;
			draw();
		}
		
		public function draw():void {
			this.graphics.beginFill(_color,0.5);
			this.graphics.drawCircle(0,0,_radius);
			this.graphics.endFill();
		}
		public function get radius():int{
			return _radius;
		}
    }
}

