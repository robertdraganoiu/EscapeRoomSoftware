// JP1 is an input
byte rows[] = {8,9, 10, 11};
const int rowCount = sizeof(rows)/sizeof(rows[0]);
 
// JP2 and JP3 are outputs
byte cols[] = {5, 4, 3, 2};
const int colCount = sizeof(cols)/sizeof(cols[0]);
 
byte keys[colCount][rowCount];
 
void setup() {
    Serial.begin(9600);
 
    for(int x=0; x<rowCount; x++) {
        pinMode(rows[x], INPUT);
    }
 
    for (int x=0; x<colCount; x++) {
        pinMode(cols[x], INPUT_PULLUP);
    }
         
}
 
void readMatrix() {
    // iterate the columns
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        byte curCol = cols[colIndex];
        pinMode(curCol, OUTPUT);
        digitalWrite(curCol, LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            byte rowCol = rows[rowIndex];
            pinMode(rowCol, INPUT_PULLUP);
            keys[colIndex][rowIndex] = digitalRead(rowCol);
            pinMode(rowCol, INPUT);
        }
        // disable the column
        pinMode(curCol, INPUT);
    }
}

int code[] = {1, 1, 10, 6, 6, 6, 13};
int codeSize = 7;
int currentIndex = 0;

void checkNextCodeElement(int elem) {
  if (code[currentIndex] == elem) {
    currentIndex++;
  } else {
    currentIndex = 0;
  }
}
 
void checkPress() {
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        for (int colIndex=0; colIndex < colCount; colIndex++) {
            if (keys[colIndex][rowIndex] == 0) {
              String num = String(colIndex + rowIndex * colCount);
              if (num.length() == 1) {
                String code = "0" + num;
                Serial.println("colors_play" + code);
              } else {
                Serial.println("colors_play" + num);
              }
              checkNextCodeElement(colIndex + rowIndex * colCount);
              if (currentIndex == codeSize) {
                Serial.println("pcolor_solved");
              }
            }
        } 
    }
}

void loop() {
  delay(200);
  readMatrix();
  checkPress();
}
