#pragma config(Sensor, in1,    sharpLeft,      sensorAnalog)
#pragma config(Sensor, in2,    sharpRight,     sensorAnalog)
#pragma config(Sensor, in3,    sharpTop,       sensorAnalog)
#pragma config(Sensor, in4,    sharpBack,      sensorAnalog)
#pragma config(Sensor, in5,    ir1,            sensorAnalog)
#pragma config(Sensor, in6,    ir2,            sensorAnalog)
#pragma config(Sensor, in7,    ir3,            sensorAnalog)
#pragma config(Sensor, in8,    ir4,            sensorAnalog)
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
#pragma config(Motor,  port5,           barrelMotor,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           barrelServo,   tmotorServoStandard, openLoop)
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
int global_orientation;
int sharpLeftVal;
int sharpRightVal;
int sharpTopVal;
int sharpBackVal;
int ir1Val;
int ir2Val;
int ir3Val;
int ir4Val;
int ballLimitVal;
int leftLimitVal;
int rightLimitVal;
int topLimitVal;
bool alignedBase = false;




/*-------------------Default Function---------------------------------*/
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

void rotate(int direction, int speedMode)
{
	int voltage;
	if(speedMode==1)
	{
		voltage=50;
	}
	else if(speedMode==2)
	{
		voltage=100;
	}
	else if(speedMode==0)
	{
		voltage=0;
	}
	motor[rightMotor] = voltage*direction;
	motor[leftMotor]  = -voltage*direction;
}

void align_orientation_with_collection()
{
	//rotate to align the orientation with the collection place, rotates to East
	if(global_orientation==0||global_orientation==1||global_orientation==7||global_orientation==6)
	{
		alignedBase = false;
		while(global_orientation!=2)
		{
			rotate(-1,1);//CW
		}
	}
	else if(global_orientation==2||global_orientation==3||global_orientation==4||global_orientation==5)
	{
		alignedBase = false;
		while(global_orientation!=1)
		{
			rotate(1,1);//CCW
		}
	}
	move(1,0);
	alignedBase = true;
}


void updateSensors(){
	sharpLeftVal = SensorValue[sharpLeft];
	sharpRightVal= SensorValue[sharpRight];
	sharpTopVal= SensorValue[sharpTop];
	sharpBackVal= SensorValue[sharpBack];
	ir1Val= SensorValue[ir1];
	ir2Val= SensorValue[ir2];
	ir3Val= SensorValue[ir3];
	ir4Val= SensorValue[ir4];
	ballLimitVal= SensorValue[ballLimit];
	leftLimitVal= SensorValue[leftLimit];
	rightLimitVal= SensorValue[rightLimit];
	topLimitVal= SensorValue[topLimit];
}


void lineDetection(){
	if (SensorValue[ir1]==0 && SensorValue[ir2]==0)
		{
			move(1,0);
			move(-1,1);
			wait1Msec(1000);
			rotate(-1,1);
			wait1Msec(6000);
			move(1,1);
			//wait1Msec(1000);
	}
	else if (SensorValue[ir1]==0)
		{
			move(1,0);
			move(-1,1);
			wait1Msec(1000);
			rotate(1,1);
			wait1Msec(3000);
			move(1,1);
			//wait1Msec(1000);
	}
	else if (SensorValue[ir2]==0)
		{
			move(1,0);
			move(-1,1);
			wait1Msec(1000);
			rotate(-1,1);
			wait1Msec(3000);
			move(1,1);
			//wait1Msec(1000);
	}

	else{
		move(1,1);
	}



}


void reverseDepositBall(){
	if (!(SensorValue[rightLimit] || SensorValue[leftLimit])){
		move(-1,1);
	}
	else{

		while(motor[barrelServo]>-70)
		{
			motor[barrelServo]= motor[barrelServo]-1;
			wait1Msec(10);
		}
		wait1Msec(1000);
		while(motor[barrelServo]<100)
		{
			motor[barrelServo]= motor[barrelServo]+1;
			wait1Msec(10);
		}

	}

}


/*-------------------Default Functions END---------------------------------*/

task main()
{

	while(1){
		read_orientation();
		updateSensors();
		reverseDepositBall();
	}

}
