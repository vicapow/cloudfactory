package {
    import flash.display.Sprite;
	import flash.display.DisplayObjectContainer;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.events.Event;
	import flash.text.TextField;

    public class BluePrintDetection extends Sprite {
	
		public var blueprints:Array = new Array();
		public var clouds:Array = new Array();
		
		public var isSubmit:Boolean = false;
		public var startPoint:Point = null;
		
		public var scoreBoard:TextField = new TextField();
		

        public function BluePrintDetection() {
			
			
			/*this.graphics.beginFill(0xffffff,1);
			this.graphics.drawRect(0,0,stage.stageWidth,stage.stageHeight);
			this.graphics.endFill();*/
			
			//add the button
			var btn:MyButton = new MyButton("Calculate Score");
			addChild(btn);
			btn.x = 10;
			btn.y = stage.stageHeight-btn.height;
			
			var submit:MyButton = new MyButton("Switch to Submission");
			addChild(submit);
			submit.x = btn.x + btn.width + 5;
			submit.y = btn.y;
			
			scoreBoard.text = "The Score: ";
			addChild(scoreBoard);
			scoreBoard.x = submit.x + submit.width;
			scoreBoard.y = submit.y;
			
			submit.addEventListener(MouseEvent.CLICK,function(event:MouseEvent):void{
				isSubmit = !isSubmit;
				submit.text = "Switch to "+((isSubmit)?"Blueprint":"Submission");
			});
			
			var cloudDrag:Boolean = false;
			var self:BluePrintDetection = this;
		
			
			stage.addEventListener(MouseEvent.MOUSE_DOWN,function(event:MouseEvent):void {
				//drag start
				if(!cloudDrag){
					startPoint = new Point(event.stageX,event.stageY);
					cloudDrag = true;
					stage.addEventListener(MouseEvent.MOUSE_MOVE,mouseMove);
				}
			});
			
			stage.addEventListener(MouseEvent.MOUSE_UP,function(event:MouseEvent):void {
				
				//add the cloud
				if(cloudDrag){
					var radius:Number = Point.distance(startPoint,new Point(event.stageX,event.stageY));
					if(radius<10) radius = 10;
					addCloud(startPoint,radius,!isSubmit);
					stage.removeEventListener(MouseEvent.MOUSE_MOVE,mouseMove);
					self.graphics.clear();
					if(blueprints.length>0){
						var blueprintsCOM:Point = computeCenterOfMass(blueprints);
						trace("blueprintsCOM x: "+blueprintsCOM.x+" y: "+blueprintsCOM.y);
						self.graphics.beginFill(0xff0000,1);
						self.graphics.drawCircle(blueprintsCOM.x,blueprintsCOM.y,5);
						self.graphics.endFill();
					}
					if(clouds.length>0){
						var cloudsCOM:Point = computeCenterOfMass(clouds);
					}
				}
				cloudDrag = false;
			});
			
			btn.addEventListener(MouseEvent.MOUSE_DOWN,function(e:MouseEvent):void{
				e.stopPropagation();
			},false,0,true);
			
			btn.addEventListener(MouseEvent.MOUSE_UP,function(e:MouseEvent):void{
				e.stopPropagation();
			},false,0,true);
			
			
			submit.addEventListener(MouseEvent.MOUSE_DOWN,function(e:MouseEvent):void{
				e.stopPropagation();
			},false,0,true);
			
			submit.addEventListener(MouseEvent.MOUSE_UP,function(e:MouseEvent):void{
				e.stopPropagation();
			},false,0,true);
			
			btn.addEventListener(MouseEvent.CLICK,function(event:MouseEvent):void{
				
				var error:Number = calculateError(self.blueprints,self.clouds);
				trace('ERROR: '+error);
				var pass:Boolean = (error>=0.5);
				
				if(pass){
					trace("you passed!");
				}else{
					trace("you failed!");
				}
				
				scoreBoard.text = ((pass)?"pass":"fail") + " score: "+error;
				scoreBoard.width = scoreBoard.textWidth + 4;
				
				event.stopPropagation();
			},false,0,true);
        }

		public function computeCenterOfMass(clouds:Array):Point{
			//for this calculat
			var totalMass:Number = 0;
			var weightedPos:Point = new Point(0,0);
			for each( var cloud:Cloud in clouds ){
				trace('adding cloud pos: x:'+cloud.x+' y:'+cloud.y+' radius: '+cloud.radius);
				weightedPos = weightedPos.add(new Point(cloud.x*cloud.radius,cloud.y*cloud.radius));
				trace('new weihted pos: '+weightedPos);
				if(cloud.radius==0){
					throw new Error("radius is zero!");
				}
				totalMass += cloud.radius;
			}
			weightedPos.x = weightedPos.x / totalMass;
			weightedPos.y = weightedPos.y / totalMass;
			return weightedPos;
		}

		public function calculateError(blueprints:Array,clouds:Array):Number {
			clouds = clouds.slice();//shallow copy
			var diff:Point = computeCenterOfMass(blueprints).subtract(computeCenterOfMass(clouds));
			trace('difference in COM: '+diff);
			var score:Number = 0;
			for each( var blueprint:Cloud in blueprints ){
				if(clouds.length>0){
					var closest:Cloud = getClosestNeighbor(blueprint,clouds,diff);
					clouds.splice(clouds.lastIndexOf(closest),1);//remove the found cloud from the clouds array
					score += cloudsDeviation(blueprint,closest,diff);
				}else{
					trace("error, ran out of clouds");
					break;
				}
			}
			return score / blueprints.length;//normalize
		}
		
		public var sizeTolerence:Number = 50;
		public var posTolerance:Number = 50;
		public function cloudsDeviation(cloud:Cloud,closest:Cloud,offset:Point):Number{
			//one is good, zero is bad
			var sizeDiff:Number = Math.abs(cloud.radius - closest.radius); // 0 -> inf
			sizeDiff = sizeDiff / sizeTolerence; // 0 -> 1
			if(sizeDiff>1) sizeDiff = 1;//cap it at 1 // 0good -> 1
			sizeDiff = 1 - sizeDiff;//1 is good, 0 is bad

			var posDiff:Number = getDistance(cloud,closest,offset); // 0 -> inf
			posDiff = posDiff / posTolerance;
			if(posDiff>1) posDiff = 1;
			posDiff = 1 - posDiff;
			
			return (sizeDiff + posDiff)/2; //noramlize
		}
		
		public function getDistance(obj1:DisplayObjectContainer,obj2:DisplayObjectContainer,offset:Point = null ):Number{
			if(offset==null) offset = new Point();
			var x:Number = obj1.x - (obj2.x+offset.x);
			var y:Number = obj1.y - (obj2.y+offset.y);
			return Math.sqrt( x*x + y*y ); //calculate the distance between the two clouds
		}
		
		public function getClosestNeighbor(cloud:Cloud,clouds:Array,offset:Point = null ):Cloud{
			if(offset==null) offset = new Point();
			var closest:Cloud = null;
			var min_d:Number = Number.MAX_VALUE;
			for each( var neighbor:Cloud in clouds ){
				var d:Number = getDistance(cloud,neighbor,offset); //calculate the distance between the two clouds
				if( d < min_d ){
					closest = neighbor;
					min_d = d;
				}
			}
			return closest;
		}

		public function mouseMove(e:MouseEvent):void{
			trace("mouse moved!");
			this.graphics.clear();
			this.graphics.lineStyle(1,0x333333,1);
			this.graphics.drawCircle(startPoint.x,startPoint.y,Point.distance(startPoint,new Point(e.stageX,e.stageY)));
		}

		public function addCloud(p:Point,r:int,isBlueprint:Boolean):void {
			var cloud:Cloud = null;
			if(isBlueprint){
				cloud = new Cloud(p,r,0x0000ff);
				blueprints.push(cloud);
			}else{
				cloud = new Cloud(p,r,0x00ff00);
				clouds.push(cloud);
			}
			this.addChild(cloud);
		}
    }
}

