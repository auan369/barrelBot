#pragma config(Sensor, in1,    sharpLeft,      sensorAnalog)
#pragma config(Sensor, in2,    sharpRight,     sensorAnalog)
#pragma config(Sensor, in3,    sharpTop,       sensorAnalog)
#pragma config(Sensor, in4,    sharpBack,      sensorAnalog)
#pragma config(Sensor, in5,    irFR,           sensorAnalog)
#pragma config(Sensor, in6,    irFL,           sensorAnalog)
#pragma config(Sensor, in7,    irBL,           sensorAnalog)
#pragma config(Sensor, in8,    irBR,           sensorAnalog)
#pragma config(Sensor, dgtl1,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  ballLimit,      sensorTouch)
#pragma config(Sensor, dgtl6,  leftLimit,      sensorTouch)
#pragma config(Sensor, dgtl7,  rightLimit,     sensorTouch)
#pragma config(Sensor, dgtl8,  topLimit,       sensorTouch)
#pragma config(Sensor, dgtl9,  compassWest,    sensorDigitalIn)
#pragma config(Sensor, dgtl10, compassSouth,   sensorDigitalIn)
#pragma config(Sensor, dgtl11, compassEast,    sensorDigitalIn)
#pragma config(Sensor, dgtl12, compassNorth,   sensorDigitalIn)
#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop, reversed, encoderPort, dgtl1)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop, encoderPort, dgtl3)
#pragma config(Motor,  port4,           barrelMotor,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           barrelServo,   tmotorServoStandard, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7

/*---------------------Global Variables-----------------------*/
string console;
int global_orientation;
int sharpLeftVal;
int sharpRightVal;
int sharpTopVal;
int sharpBackVal;
int irFLVal;
int irFRVal;
int irBLVal;
int irBRVal;
int ballLimitVal;
int leftLimitVal;
int rightLimitVal;
int topLimitVal;
int sharpSense;
int turnTime;
int compassWestVal;
int compassEastVal;
int compassNorthVal;
int compassSouthVal;
int search_orientation;

/*-------------------Default Function Declration---------------------------------*/

void move(int direction, int speedMode);
void rotate(int direction, int speedMode);
void lookForBall4();
void lookForBall3();
void read_orientation();
void align_orientation_with_collection_and_return();
void updateSensors();
void lineDetection();
void lineDetectionRear();
void releaseBall();
void resetServo();
void enemyDetectFront();
void enemyDetectRear();
void testMain();
/*-------------------State Variables---------------------------------*/
bool waitStart = true;
bool alignedBase = false;
bool ballInCage = false;
bool moveForward = false;
int stage_of_search = 0;
int ccwOrCw = 1; //1 is cw, -1 is ccw
bool justStart = true;
int enemyDetected = 0; // 1 is enemy front, 0 is no enemy, -1 is enemy back
bool startingTurn = true;



task main()
{
    while(1){
        //complete the following functions in this order, fix each as per required

		//lookForBall3();
        //updateSensors(); //works
        //read_orientation(); //works
        //lineDetection(); //works
        //align_orientation_with_collection_and_return();//testing release ball
				//resetServo();
    	//lookForBall3();
		//		lookForBall4();
        testMain();

    		//motor[barrelMotor]=127;
    		/*
    		if(SensorValue[ballLimit]==0)
    		{
    			resetServo();
    		}
    		else{releaseBall();}
				*/
    }

    //testMain();


}
/*-----------------------Main Parent Function---------------------------*/
void testMain(){
	resetServo();
	while(waitStart){ //initialisation stage
		if(SensorValue[leftLimit]==1){ //decide which way to turn based on start pos
			ccwOrCw = -1;
		}
		else if(SensorValue[rightLimit]==1){ //decide which way to turn based on start pos
			ccwOrCw = 1;
		}

		if (SensorValue[ballLimit]==1){
			//updateSensors();
			waitStart = false;
			clearTimer(T2);
		}
        else{
            updateSensors();
			read_orientation();
        }
	}

	while(1){ //main code
		lineDetection();
		read_orientation();
		//enemyDetectFront();
		if(justStart){
			if(time1[T2]<6500){
				move(1,3);
				lineDetection();
			}
			else{
				justStart = false;
				clearTimer(T2);
			}
		}
		else if(ballInCage){
			align_orientation_with_collection_and_return();
		}
		else{
			if(moveForward){
				if(time1[T2]>3000){
					moveForward = false;
					clearTimer(T2);
				}
				else{
					move(1,3);
				}
			}
			else if (startingTurn){
				read_orientation();
				search_orientation = global_orientation;
				startingTurn = false;
				clearTimer(T3);

			}
			else{
				lookForBall3();
				read_orientation();

				if(global_orientation==search_orientation && time1[T3]>5000){
					//rotate(1,3);
					//wait1Msec(850);
					moveForward = true;
					startingTurn = true;
					clearTimer(T2);
				}


				//lookForBall3();
			}
		}

	}
}
/*-------------------Default Function Defination---------------------------------*/

void move(int direction, int speedMode)
{
	int voltageLeft;
	int voltageRight;
	if(speedMode==1)
	{
		voltageLeft=30;
		voltageRight=30;
	}
	else if(speedMode==2)
	{
		voltageLeft=60;
		voltageRight=60;
	}
	else if(speedMode==3)
	{
		voltageLeft=127;
		voltageRight=127;
	}
	else if(speedMode==0)
	{
		voltageLeft=0;
		voltageRight=0;
	}
	motor[rightMotor] = voltageRight*direction;
	motor[leftMotor]  = voltageLeft*direction;

}

void rotate(int direction, int speedMode) //ccw =1 , cw =-1
{
	int voltage;
	if(speedMode==1)
	{
		voltage=40;
	}
	else if(speedMode==2)
	{
		voltage=55;
	}
	else if(speedMode==3)
	{
		voltage=127;
	}
	else if(speedMode==0)
	{
		voltage=0;
	}
	motor[rightMotor] = voltage*direction;
	motor[leftMotor]  = -voltage*direction;
}

void read_orientation()
{
	int temp = 8*SensorValue(compassWest)+4*SensorValue(compassSouth)+2*SensorValue(compassEast)+SensorValue(compassNorth);
	switch(temp) {
	case 14:
		global_orientation = NORTH;
		break;
	case 13:
		global_orientation = EAST;
		break;
	case 11:
		global_orientation = SOUTH;
		break;
	case 7:
		global_orientation = WEST;
		break;
	case 12:
		global_orientation = NORTHEAST;
		break;
	case 9:
		global_orientation = SOUTHEAST;
		break;
	case 3:
		global_orientation = SOUTHWEST;
		break;
	case 6:
		global_orientation = NORTHWEST;
		break;
	default:
		global_orientation = -1;
	}
}

//bool stage_of_search=0; 0 is looking, 1 is L sense & waiting, 2 is R sense & turning back, 3 is aligned and moving forward
void lookForBall(){
		sharpTopVal = SensorValue[sharpTop];
		sharpLeftVal = SensorValue[sharpLeft];
		sharpRightVal = SensorValue[sharpRight];
    if (stage_of_search == 0){
        rotate(1,1);
        //motor[rightMotor]=50;
        //motor[leftMotor]=-50;
        console = "lookB";

        if (SensorValue[sharpLeft]>500 && SensorValue[sharpRight]<500){
            console = "Lsense";
            stage_of_search = 1;
            sharpSense = SensorValue[sharpLeft];
            //leftSense=true;
            clearTimer(T1);
        }
    }
    else if (stage_of_search == 1){
        console = "WaitR";
        if (time1[T1]<3000){
            if(SensorValue[sharpTop]>sharpSense)
            {
                clearTimer(T1);
                //leftSense = false;
                //ballfound = false;
                console = "Tsense";
                stage_of_search = 0;

            }
            else if(SensorValue[sharpRight]>500 && SensorValue[sharpLeft]<500)
            {
                console = "Rsense";
                turnTime = time1[T1];
                clearTimer(T1);
                //leftSense = false;
                //ballfound = true;
                move(1,0);
                stage_of_search = 2;
            }
        }
        else
        {
            //leftSense = false;
            stage_of_search = 0;
        }
    }
    else if (stage_of_search == 2){
        if(time1[T1]>turnTime){
			//ballfound = false;
			console = "forward";
            clearTimer(T1);
            move(1,0);
			stage_of_search = 3;
		}
		else{
			rotate(-1,1);
		}
    }
    else{
        if (time1[T1]<5000){
            move(1,1);
        }
        else{
            move(1,0);
            stage_of_search = 0;
        }
    }
}

void lookForBall3(){
	sharpTopVal = SensorValue[sharpTop];
	sharpLeftVal = SensorValue[sharpLeft];
	sharpRightVal = SensorValue[sharpRight];
	motor[barrelMotor] = 127;
	if (SensorValue[ballLimit]==1){
		ballInCage = true;
		motor[barrelMotor] = 0;
	}
	else{
		ballInCage = false;
	}


    if (stage_of_search == 0){
        rotate(1*ccwOrCw,1);
        //motor[rightMotor]=50;
        //motor[leftMotor]=-50;
        console = "lookB";
		if(ccwOrCw==1){
			if (SensorValue[sharpLeft]>500 && (SensorValue[sharpLeft]-SensorValue[sharpRight])>200){
				console = "Lsense";
				stage_of_search = 1;
				sharpSense = SensorValue[sharpLeft];
				//leftSense=true;
				clearTimer(T1);
			}
		}
		else{
			if (SensorValue[sharpRight]>500 && (SensorValue[sharpRight]-SensorValue[sharpLeft])>200){
				console = "Lsense";
				stage_of_search = 1;
				sharpSense = SensorValue[sharpLeft];
				//leftSense=true;
				clearTimer(T1);
			}

		}
    }
    else if (stage_of_search == 1){
        console = "WaitR";
        if (time1[T1]<3000){
            if(SensorValue[sharpTop]>sharpSense)
            {
                //clearTimer(T1);

                //console = "Tsense";
                //stage_of_search = 0;

            }
            else if(ccwOrCw ==1 && SensorValue[sharpRight]>500 && (SensorValue[sharpRight]-SensorValue[sharpLeft])>200)
            {
                console = "Rsense";
                turnTime = time1[T1];
                clearTimer(T1);
                //leftSense = false;
                //ballfound = true;
                move(1,0);
                stage_of_search = 2;
            }
			else if(ccwOrCw == -1 && SensorValue[sharpLeft]>500 && (SensorValue[sharpLeft]-SensorValue[sharpRight])>200)
            {
                console = "Rsense";
                turnTime = time1[T1];
                clearTimer(T1);
                //leftSense = false;
                //ballfound = true;
                move(1,0);
                stage_of_search = 2;
            }
        }
        else
        {
            //leftSense = false;
            stage_of_search = 0;
        }
    }
    else if (stage_of_search == 2){
        if(time1[T1]>turnTime){
			//ballfound = false;
			console = "forward";
            clearTimer(T1);
            move(1,0);
			stage_of_search = 3;
		}
		else{
			rotate(-1*ccwOrCw,1);
		}
    }
    else if (stage_of_search == 3){
        if (time1[T1]<3000){
            move(1,3);
        }
        else{
            move(1,0);
			clearTimer(T1);
			clearTimer(T2);
            stage_of_search = 4;
        }
    }
	else{//stage of search = 4
		clearTimer(T2);
		if(time1[T1]>turnTime*1.5){
			move(1,0);
            stage_of_search = 0;
		}
		else{
			rotate(-1*ccwOrCw,1);
		}


	}



}


void lookForBall4(){
	sharpTopVal = SensorValue[sharpTop];
	sharpLeftVal = SensorValue[sharpLeft];
	sharpRightVal = SensorValue[sharpRight];
	motor[barrelMotor] = 127;
	if (SensorValue[ballLimit]==1){
		ballInCage = true;
		motor[barrelMotor] = 0;
	}
	else{
		ballInCage = false;
	}


    if (stage_of_search == 0){
        rotate(1*ccwOrCw,2);
        //motor[rightMotor]=50;
        //motor[leftMotor]=-50;
        console = "lookB";
		if(ccwOrCw==1){
			if (SensorValue[sharpLeft]>500 && (SensorValue[sharpLeft]-SensorValue[sharpRight])>200){
				console = "Lsense";
				wait1Msec(150);
				stage_of_search = 3;
				sharpSense = SensorValue[sharpLeft];
				//leftSense=true;
				clearTimer(T1);
			}
		}
		else{
			if (SensorValue[sharpRight]>500 && (SensorValue[sharpRight]-SensorValue[sharpLeft])>200){
				console = "Lsense";
				wait1Msec(150);
				stage_of_search = 3;
				sharpSense = SensorValue[sharpLeft];
				//leftSense=true;
				clearTimer(T1);
			}
		}

    }
    else if (stage_of_search == 1){
        console = "WaitR";
        if (time1[T1]<3000){
            if(SensorValue[sharpTop]>sharpSense)
            {
                //clearTimer(T1);

                //console = "Tsense";
                //stage_of_search = 0;

            }
            else if(SensorValue[sharpRight]>500 && (SensorValue[sharpRight]-SensorValue[sharpLeft])>200)
            {
                console = "Rsense";
                turnTime = time1[T1];
                clearTimer(T1);
                //leftSense = false;
                //ballfound = true;
                move(1,0);
                stage_of_search = 2;
            }
        }
        else
        {
            //leftSense = false;
            stage_of_search = 0;
        }
    }
    else if (stage_of_search == 2){
        if(time1[T1]>turnTime){
			//ballfound = false;
			console = "forward";
            clearTimer(T1);
            move(1,0);
			stage_of_search = 3;
		}
		else{
			rotate(-1*ccwOrCw,2);
		}
    }
    else if (stage_of_search == 3){
        if (time1[T1]<3000){
            move(1,3);
        }
        else{
            move(1,0);
			clearTimer(T1);
			clearTimer(T2);
            stage_of_search = 4;
        }
    }
	else{//stage of search = 4
		clearTimer(T2);
		if(time1[T1]>800){
			move(1,0);
            stage_of_search = 0;
		}
		else{
			rotate(-1*ccwOrCw,2);
		}


	}



}



void align_orientation_with_collection_and_return()
{
	//rotate to align the orientation with the collection place, rotates to South, when not 4
    read_orientation();
    resetServo();
	if(global_orientation==7||global_orientation==6||global_orientation==5)
	{//NW,W,SW
		alignedBase = false;
		rotate(1,2);//CW

	}
	else if(global_orientation==1||global_orientation==2||global_orientation==3||global_orientation==0)
	{//NE,E,SE,N
		alignedBase = false;
		rotate(-1,2);//CCW

	}

	else{
		move(-1,3);
		alignedBase = true;
		if(SensorValue[rightLimit]==1 || SensorValue[leftLimit]==1){ //if reach base
			//wait1Msec(400); // reverse 1 second more
			move(1,0);
			releaseBall();

			resetServo();
			ballInCage = false;

			justStart = true;
			wait1Msec(1000);
			clearTimer(T2);
		}
		else{
			if(!justStart){ //to prevent this from running after ball deposited
				lineDetectionRear();
				//enemyDetectRear();
			}
		}


	}
}

void align_orientation_with_east_and_forward()
{
	//rotate to align the orientation with the collection place, rotates to South, when not 4
    read_orientation();
	if(global_orientation==7||global_orientation==6||global_orientation==5)
	{//NW,W,SW
		alignedBase = false;
		rotate(1,2);//CW

	}
	else if(global_orientation==1||global_orientation==2||global_orientation==3||global_orientation==0)
	{//NE,E,SE,N
		alignedBase = false;
		rotate(-1,2);//CCW

	}

	else{
		move(-1,3);
		alignedBase = true;
		if(SensorValue[rightLimit]==1 || SensorValue[leftLimit]==1){ //if reach base
			//wait1Msec(400); // reverse 1 second more
			move(1,0);
			releaseBall();

			resetServo();
			ballInCage = false;

			justStart = true;
			wait1Msec(1000);
			clearTimer(T2);
		}
		else{
			if(!justStart){ //to prevent this from running after ball deposited
				lineDetectionRear();
				enemyDetectRear();
			}
		}


	}
}

void updateSensors(){
    compassWestVal = SensorValue[compassWest];
    compassEastVal = SensorValue[compassEast];
    compassNorthVal = SensorValue[compassNorth];
    compassSouthVal = SensorValue[compassSouth];
	sharpLeftVal = SensorValue[sharpLeft];
	sharpRightVal= SensorValue[sharpRight];
	sharpTopVal= SensorValue[sharpTop];
	sharpBackVal= SensorValue[sharpBack];
	irFLVal= SensorValue[irFL];
	irFRVal= SensorValue[irFR];
	irBLVal= SensorValue[irBL];
	irBRVal= SensorValue[irBR];
	ballLimitVal= SensorValue[ballLimit];
	leftLimitVal= SensorValue[leftLimit];
	rightLimitVal= SensorValue[rightLimit];
	topLimitVal= SensorValue[topLimit];
}


void lineDetection(){
	irFLVal = SensorValue[irFL];
	irFRVal = SensorValue[irFR];
	irBRVal = SensorValue[irBR];
	irBLVal = SensorValue[irBL];
	read_orientation();
	if (SensorValue[irFL]<300)
		{
			if (global_orientation==0){
				console	= "leftIR";
				//ccwOrCw = -1;
				move(1,0);
				wait1Msec(100);
				move(-1,3);
				wait1Msec(500);
				rotate(-1,3);
				wait1Msec(1800);
				//move(1,3);
				moveForward=false;
				clearTimer(T2);
				justStart=true;
			}

			else{
				console	= "leftIR";
				//ccwOrCw = -1;
				move(1,0);
				wait1Msec(100);
				move(-1,3);
				wait1Msec(500);
				rotate(-1,3);
				wait1Msec(800);
				move(1,3);
				wait1Msec(1000);
				move(1,0);
				//clearTimer(T2);
		}
	}
	else if (SensorValue[irFR]<300)
		{
			if(global_orientation==0){
				console	= "righttIR";
				//ccwOrCw = -1;
				move(1,0);
				wait1Msec(100);
				move(-1,3);
				wait1Msec(500);
				rotate(1,3);
				wait1Msec(1800);
				//move(1,3);
				moveForward=false;
				clearTimer(T2);
				justStart=true;
			}
			else{
				console	= "rightIR";
				//ccwOrCw = 1;
				move(1,0);
				wait1Msec(100);
				move(-1,3);
				wait1Msec(500);
				rotate(1,3);
				wait1Msec(800);
				move(1,3);
				wait1Msec(1000);
				move(1,0);
				//clearTimer(T2);
			}
	}
	//else{
	//	move(-1,1);
	//}
}
void lineDetectionRear(){
	  if (SensorValue[irBL]<300)
		{
			console	= "leftBackIR";
			move(1,0);
			move(1,3);
			wait1Msec(500);
			rotate(1,3);
			wait1Msec(500);
			move(-1,3);
			wait1Msec(1000);
			move(1,0);
	}
	else if (SensorValue[irBR]<300)
		{
			console	= "rightBackIR";
			move(1,0);
			move(1,3);
			wait1Msec(500);
			rotate(-1,3);
			wait1Msec(500);
			move(-1,3);
			wait1Msec(1000);
			move(1,0);
	}
}

void releaseBall(){
	motor[barrelMotor]=127;
	while(motor[barrelServo]>-90)
	{
		motor[barrelServo]= motor[barrelServo]-1;
		wait1Msec(5);
	}
	wait1Msec(400);
	motor[barrelMotor]=0;
}

void resetServo(){
	while(motor[barrelServo]<60)
	{
		motor[barrelServo]= motor[barrelServo]+1;
		wait1Msec(10);

	}
	wait1Msec(200);
}

void enemyDetectFront(){
	if (enemyDetected==0){
		if (SensorValue[sharpTop]>1500 || (SensorValue[sharpLeft]>1500 && SensorValue[sharpRight]>1500)){
			move(1,0);
			enemyDetected = 1;
			clearTimer(T3);
		}
	}
	else if(enemyDetected==1){
		while(time1[T3]<4500){
			lineDetection();
			if (SensorValue[sharpTop]<1500 || (SensorValue[sharpLeft]<1500 && SensorValue[sharpRight]<1500)){
				move(1,0); //if enemy not present anymore
				enemyDetected = 0;
				clearTimer(T3);
				break;
			}
			/*
			else if (time1[T3]>4400){
				move(1,0);
				enemyDetected = 0;
				clearTimer(T3);
			}
			*/
			else if (time1[T3]>3500){
				move(1,3);
			}
			else if (time1[T3]>2500){
				rotate(1*ccwOrCw,1);
			}
			else if (time1[T3]>1500){ //move back if enemy detected and wait for 1.5 sec
				move(-1,3);
			}
		}
		move(1,0);
		enemyDetected = 0;
		clearTimer(T3);

	}

}

void enemyDetectRear(){
	if (enemyDetected==0){
		if (SensorValue[sharpBack]>1500){
			move(1,0);
			enemyDetected = -1;
			clearTimer(T3);
		}
	}
	else if(enemyDetected==-1){
		while(time1[T3]<4500){
			lineDetection();
			if (SensorValue[sharpBack]<1500){
				move(1,0); //if enemy not present anymore
				enemyDetected = 0;
				clearTimer(T3);
				break;
			}
			/*
			else if (time1[T3]>4400){
				move(1,0);
				enemyDetected = 0;
				clearTimer(T3);
			}
			*/
			else if (time1[T3]>3500){
				move(-1,3);
			}
			else if (time1[T3]>2500){
				rotate(-1*ccwOrCw,1);
			}
			else if (time1[T3]>1500){ //move back if enemy detected and wait for 1.5 sec
				move(1,3);
			}
		}
		move(1,0);
		enemyDetected = 0;
		clearTimer(T3);

	}

}

/*



*/

/*
void enemyDetectFront(){
	if (SensorValue[sharpTop]>1500 || (SensorValue[sharpLeft]>1500 && SensorValue[sharpRight]>1500)){
		move(1,0);
		clearTimer(T3);

	}
}
*/
