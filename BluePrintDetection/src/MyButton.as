package {
	
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFormat;

    public class MyButton extends Sprite{
	
		private var tf:TextField = new TextField();

        public function MyButton(label:String) {
			tf.textColor = 0xffffff;
			tf.antiAliasType = flash.text.AntiAliasType.ADVANCED;
			tf.selectable = false;
			tf.mouseEnabled = false;
			addChild(tf);
			this.text = label;
        }
		public function set text(label:String):void{
			tf.text = label;
			tf.width = tf.textWidth + 3;
			tf.height = tf.textHeight + 3;
			redraw();
		}
		public function redraw():void{
			this.graphics.clear();
			this.graphics.beginFill(0x333333,1);
			this.graphics.drawRoundRect(tf.x-5,tf.y-5,tf.textWidth+10,tf.textHeight+10,5,5);
			this.graphics.endFill();
		}
    }
}

