
// include the main heades
#include <LiquidCrystal.h>
#include <Servo.h>

#define trig_in_ultra_sonic 8
#define echo_in_ultra_sonic 9
#define trig_out_ultra_sonic 6
#define echo_out_ultra_sonic 7

#define IR_in_seneor 13
#define IR_out_seneor 10

#define places 3

#define opened_degree 0
#define closed_degree 90

// create Object from the LiquidCrystal class
LiquidCrystal my_lcd(12, 11, 5, 4, 3, 2);

// craete objects for both the in and out doors from servo calss
Servo out_door_servo;
Servo in_door_servo;

// Prototype of Functions
void control_servo(Servo& servo, int distance);
void distance_to_servo_action();
int distance(unsigned char trig_pin, unsigned char echo_pin);

// Flags
bool IR_in_value = 0;
bool IR_out_value = 0;
bool garage_is_completed = 0;
bool is_doors_opend = 0;

// init the empty places
int empty_places = places;

void setup()
{
  // init the lcd .
  my_lcd.begin(16, 2);
  Serial.begin(9600);

  // set the servos pin.
  in_door_servo.attach(A4);
  out_door_servo.attach(A5);

  // SET The pins As IN/OUT pins
  pinMode(trig_in_ultra_sonic, OUTPUT);
  pinMode(echo_in_ultra_sonic, INPUT);

  pinMode(trig_out_ultra_sonic, OUTPUT);
  pinMode(echo_out_ultra_sonic, INPUT);

  pinMode(IR_in_seneor, INPUT);
  pinMode(IR_out_seneor, INPUT);

}

void loop()
{


  // Function to manage the grage traffic
  garage_mangement();

  // function to read the ultrasonic sensors and control the servo motors
  distance_to_servo_action();

  // Display the info on the LCD.
  display_info_on_LCD();

  // freeze the microcontroller.
  //delay(200);
}

int distance(unsigned char trig_pin, unsigned char echo_pin)
{
  // set the trig pin high to send the waves
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  // set the echo pin as input to receive the waves.
  float duration = pulseIn(echo_pin, HIGH);

  // return the distance .
  return (int)((duration *  0.034) / 2);
}


void distance_to_servo_action()
{
  // readthe distance from the ultrasonic sensors.
  int distance_from_in_door = distance(trig_in_ultra_sonic, echo_in_ultra_sonic);
  int distance_from_out_door = distance(trig_out_ultra_sonic, echo_out_ultra_sonic);

  // Controle the servo based on the ultrasonic reading.
  control_servo(in_door_servo, distance_from_in_door);
  control_servo(out_door_servo, distance_from_out_door);

}

void control_servo(Servo& servo, int distance)
{
  //  if the distance <= 10 this meaning the cars is near by the door.
  if (distance <=  5)
  {
    // open the door in slow motion
    for (unsigned char i = 0; i <= opened_degree; i += 10)
    {
      servo.write(i);
delay(1000);

    }
    
    // set the flag
    is_doors_opend = 1;
  } else
  {
    // close the door
    servo.write(closed_degree);
    is_doors_opend = 0;
  }
}

void garage_mangement()
{
  // read the ir sensors and invert it because the default reading sensor is 1 if nothing
  IR_in_value = !digitalRead(IR_in_seneor);
  IR_out_value = !digitalRead(IR_out_seneor);
  
  //  decrese the empty places with any new reading from the in IR sensor and check if the garage is full or not.
  if (IR_in_value)
  {
    if (empty_places > 0)
    {
      Serial.print("IN: ");
      Serial.println(empty_places);
      garage_is_completed = false;
      empty_places--;
      delay(500);
    } else
    {
      garage_is_completed = true;
    }
  }

  // increase the empty places with any neew reading
  if (IR_out_value)
  {
    if (empty_places < places)
    {
            Serial.print("OUT: ");
            Serial.println(empty_places);

      empty_places++;
            delay(500);

    }
  }
}

void display_info_on_LCD()
{


  my_lcd.setCursor(0, 0);
  my_lcd.print("D_S: ");   // Doors Status if one of them or both is opend will show (1)
  my_lcd.setCursor(4, 0);
  my_lcd.print("  ");
  my_lcd.setCursor(4, 0);
  my_lcd.print(is_doors_opend);

  my_lcd.setCursor(8, 0);
  my_lcd.print("G_S: ");   // Garage Status
  my_lcd.setCursor(12, 0);
  my_lcd.print("  ");   // Garage Status
  my_lcd.setCursor(12, 0);
  my_lcd.print(garage_is_completed);



  my_lcd.setCursor(0, 1);
  my_lcd.print("C_I: ");  // Cars in the garage
  my_lcd.setCursor(4, 1);
  my_lcd.print("  ");  // Cars in the garage
  my_lcd.setCursor(4, 1);
  my_lcd.print(places - empty_places);


  my_lcd.setCursor(8, 1);
  my_lcd.print("EMP: ");  // Empty place in the garage
  my_lcd.setCursor(12, 1);
  my_lcd.print("  ");  // Empty place in the garage
  my_lcd.setCursor(12, 1);
  my_lcd.print(empty_places);
  
  //delay(500);
  //my_lcd.clear(); // clear the lcd to rewrite again
}
