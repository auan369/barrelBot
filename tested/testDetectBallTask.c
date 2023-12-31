#pragma config(Sensor, in1,    sharpLeft,      sensorAnalog)
#pragma config(Sensor, in2,    sharpRight,     sensorAnalog)
#pragma config(Sensor, in3,    sharpTop,       sensorAnalog)
#pragma config(Sensor, in4,    sharpBack,      sensorNone)
#pragma config(Sensor, in5,    ir1,            sensorAnalog)
#pragma config(Sensor, in6,    ir2,            sensorAnalog)
#pragma config(Sensor, in7,    ir3,            sensorAnalog)
#pragma config(Sensor, in8,    ir4,            sensorAnalog)
#pragma config(Sensor, dgtl1,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  limit1,         sensorTouch)
#pragma config(Sensor, dgtl6,  limit2,         sensorTouch)
#pragma config(Sensor, dgtl7,  limit3,         sensorTouch)
#pragma config(Sensor, dgtl8,  limit4,         sensorTouch)
#pragma config(Motor,  port2,           rightMotor,    tmotorServoContinuousRotation, openLoop, reversed, encoderPort, dgtl1)
#pragma config(Motor,  port3,           leftMotor,     tmotorServoContinuousRotation, openLoop, encoderPort, dgtl3)
#pragma config(Motor,  port5,           barrelMotor,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           barrelServo,   tmotorServoStandard, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
string console;
int backupLevel;
int sharpLeftVal;
int sharpRightVal;
int sharpTopVal;
int sharpBackVal;
int ir1Val;
int ir2Val;
int ir3Val;
int ir4Val;
int limit1Val;
int limit2Val;
int limit3Val;
int limit4Val;
int varSysTime;
int varSysTimeOld=0;
bool leftSense = false;
bool ballfound = false;
int turnTime;


void lookForBall(){

	//rotate while looking
	//if see ball left
	if (ballfound){


		if(time1[T1]>turnTime){
			//ballfound = false;
			console = "forward";
			motor[rightMotor]=100;
			motor[leftMotor]=100;
		}
		else{
			motor[rightMotor]=-50;
			motor[leftMotor]=50;
		}
	}

	else{
		//when left sensor nvr see yet
		if (!leftSense){
			motor[rightMotor]=50;
			motor[leftMotor]=-50;
			console = "lookB";

			if (SensorValue[sharpLeft]>500){
				console = "Lsense";
				leftSense=true;
				clearTimer(T1);
			}
		}

		else{
			console = "WaitR";
			if (time1[T1]<5000){
				if(SensorValue[sharpTop]>500){
					leftSense = false;
					ballfound = false;
					clearTimer(T1);
					console = "Tsense";
					//break;
				}

				else if(SensorValue[sharpRight]>500)
				{
					console = "Rsense";
					turnTime = time1[T1];
					clearTimer(T1);
					leftSense = false;
					ballfound = true;
				}
			}
			else{
				leftSense = false;
			}
		}
	}

}






task updateSensorState(){

	while(1){

		varSysTime = nSysTime;
		sharpLeftVal = SensorValue[sharpLeft];
		sharpRightVal= SensorValue[sharpRight];
		sharpTopVal= SensorValue[sharpTop];
		lookForBall();
		//sharpBackVal= SensorValue[sharpBack];
		//ir1Val= SensorValue[ir1];
		//ir2Val= SensorValue[ir2];
		//ir3Val= SensorValue[ir3];
		//ir4Val= SensorValue[ir4];
		//limit1Val= SensorValue[limit1];
		//limit2Val= SensorValue[limit2];
		//limit3Val= SensorValue[limit3];
		//limit4Val= SensorValue[limit4];
	}
}

task main()
{
	//startTask(lookForBall);
	startTask(updateSensorState);
	//startTask(updateSensorState);
	while(true){
		varSysTimeOld = varSysTime;
		backupLevel = BackupBatteryLevel;


	}



}
