// ------------------ PIN SETUP -------------------
#define STEP_PIN 3
#define DIR_PIN 4
#define SENSOR A0
#define BUZZER 9     // MUST be a PWM pin (3,5,6,9,10,11)

// ------------------ MOTOR SPEED -----------------
int motorSpeed = 3000;                 // steps per second
unsigned long stepDelay;

// ------------------ HEIGHTS (Lead screw pitch 8mm) ------------------
// 400 steps per mm (1/16 microstep)
long height0 = 4000;                      // 0 cm
long height1 = 20000;                  // 7 cm  = 70mm × 400
long height2 = 40000;                  // 14 cm = 140mm × 400

long currentPos = 0;

// ------------------ BUZZER VOLUME LEVELS ------------------
int lowVolume = 50;        // very soft beep
int mediumVolume = 70;     // soft-moderate
int highVolume = 100;      // louder (but not max)

// -------------------------------------------------------------

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);

  stepDelay = 1000000UL / motorSpeed;   // speed control
}

void loop() {

  int water = analogRead(SENSOR);
  Serial.print("Water Level: ");
  Serial.println(water);

  long targetPos = currentPos;

  // ---------------- WATER LEVEL → HEIGHT + BUZZER ----------------
  if (water >= 30 && water <= 300) {
    targetPos = height0;

    // Softest buzzer
    analogWrite(BUZZER, lowVolume);
  }
  else if (water >= 300 && water <= 600) {
    targetPos = height1;

    // Medium buzzer intensity
    analogWrite(BUZZER, mediumVolume);
  }
  else if (water > 600) {
    targetPos = height2;

    // Slightly louder warning buzzer
    analogWrite(BUZZER, highVolume);
  }
  else {
    analogWrite(BUZZER, 0);  // OFF if water < 50
  }

  // Move stepper to the required height
  moveToPosition(targetPos);

  delay(150);
}

// ------------------ MOVE STEPPER FUNCTION ------------------

void moveToPosition(long target) {

  if (target > currentPos) {
    digitalWrite(DIR_PIN, HIGH);   // UP
    for (long i = currentPos; i < target; i++) {
      stepMotor();
    }
  }
  else if (target < currentPos) {
    digitalWrite(DIR_PIN, LOW);    // DOWN
    for (long i = currentPos; i > target; i--) {
      stepMotor();
    }
  }

  currentPos = target;
}

// ------------------ ONE STEP PULSE ------------------

void stepMotor() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(stepDelay);
}