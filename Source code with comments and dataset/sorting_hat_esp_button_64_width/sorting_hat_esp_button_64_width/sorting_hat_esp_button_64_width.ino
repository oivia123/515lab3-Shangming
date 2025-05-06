#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sorting_hat_model.h"  // ML Model

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Pins (updated based on your wiring)
#define BUTTON_A  25  // A → D25
#define BUTTON_B  26  // B → D26
#define BUTTON_C  27  // C → D27
#define BUTTON_D  14  // D → D14

// Sorting Hat Questions
const char* questions[] = {
    "1. What do you value?",
    "2. What to do if someone cheats?",
    "3. Favorite subject?",
    "4. How do you face challenges?",
    "5. How do friends describe you?",
    "6. What to do with a mystery book?",
    "7. Preferred pet?",
    "8. How do you solve problems?",
    "9. What kind of friends do you like?",
    "10. Dream career?"
};

// Answer Options
const char* options[][4] = {
    {"A) Bravery", "B) Loyalty", "C) Intelligence", "D) Ambition"},
    {"A) Call them out", "B) Let them be", "C) Inform teacher", "D) Gain from it"},
    {"A) Defense Arts", "B) Herbology", "C) Charms", "D) Potions"},
    {"A) Face head-on", "B) Team up", "C) Plan first", "D) Outsmart it"},
    {"A) Bold", "B) Kind", "C) Smart", "D) Resourceful"},
    {"A) Read it now", "B) Check safety", "C) Study it", "D) Use for gain"},
    {"A) Owl", "B) Toad", "C) Cat", "D) Phoenix"},
    {"A) Act fast", "B) Find a compromise", "C) Analyze first", "D) Outsmart"},
    {"A) Adventurous", "B) Loyal", "C) Thoughtful", "D) Powerful"},
    {"A) Auror", "B) Healer", "C) Scholar", "D) Minister"}
};

// Store user responses (1-4)
int responses[10] = {0};
int questionIndex = 0;  // Current question index

// Initialize Machine Learning Model
Eloquent::ML::Port::DecisionTree clf;

void setup() {
    Serial.begin(115200);

    // Initialize OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }
    display.clearDisplay();

    // Welcome Message
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println("Sorting Hat Ready!");
    display.display();
    delay(2000);

    // Initialize buttons as input with pull-up resistors
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
    pinMode(BUTTON_D, INPUT_PULLUP);

    // Show the first question
    showQuestion();
}

void loop() {
    checkButtons();  // Wait for user input
}

// Display current question and answer choices
void showQuestion() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(questions[questionIndex]);
    
    for (int i = 0; i < 4; i++) {
        display.setCursor(10, 20 + (i * 10));
        display.println(options[questionIndex][i]);
    }

    display.display();
}

// Check button inputs and record answer
void checkButtons() {
    int buttonPins[] = {BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_D};

    for (int i = 0; i < 4; i++) {
        if (digitalRead(buttonPins[i]) == LOW) {
            delay(50);  // debounce
            if (digitalRead(buttonPins[i]) == LOW) {
                responses[questionIndex] = i + 1;
                Serial.print("Q");
                Serial.print(questionIndex + 1);
                Serial.print(": You chose option ");
                Serial.println(i + 1);
                delay(200);
                while (digitalRead(buttonPins[i]) == LOW);  // Wait until release
                nextQuestion();
                break;
            }
        }
    }
}

// Show next question or sort house if finished
void nextQuestion() {
    questionIndex++;
    if (questionIndex < 10) {
        showQuestion();
    } else {
        classifyHouse();
    }
}

// Use ML model to predict house and show result
void classifyHouse() {
    display.clearDisplay();
    display.setCursor(10, 10);
    display.println("Sorting...");

    // Prepare feature input
    float features[] = {
        (float)responses[0], (float)responses[1], (float)responses[2],
        (float)responses[3], (float)responses[4], (float)responses[5],
        (float)responses[6], (float)responses[7], (float)responses[8],
        (float)responses[9]
    };

    int house = clf.predict(features);

    // Show result on screen
    display.setCursor(10, 30);
    display.print("House: ");
    switch (house) {
        case 0: display.println("Gryffindor"); break;
        case 1: display.println("Ravenclaw"); break;
        case 2: display.println("Hufflepuff"); break;
        case 3: display.println("Slytherin"); break;
        default: display.println("Unknown"); break;
    }

    display.display();

    // Also print to Serial
    Serial.println("Sorting complete!");
    Serial.print("Predicted House: ");
    switch (house) {
        case 0: Serial.println("Gryffindor"); break;
        case 1: Serial.println("Ravenclaw"); break;
        case 2: Serial.println("Hufflepuff"); break;
        case 3: Serial.println("Slytherin"); break;
        default: Serial.println("Unknown"); break;
    }
}
