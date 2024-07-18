
#include "src/rea_tea.h"
#include "src/component_test.h"
#include "src/timerrtc.h"
//RotaryEncoder enc;
// bool btn_pressed = false;
// void updateEncoder(){
//     enc.ReadState();
// }
//MksMfc mfc;
REATEASystem sys;

int to_seconds(int time, TemporalUnit unit){
    switch(unit){
        case second:
            return time;
        case minute:
            return time * 60;
        case hour:
            return time * 3600;
        case day:
            return time * 3600 * 24;
    }
}
void setup(){
    // rtc.BeginRTC();
    // int time = 5;
    // TemporalUnit unit = minute;
    // Serial.println(rtc.StartTimer(time, unit));
    // char buffer[20];
    Serial.begin(115200);
    // int counter = 0;
    // while(counter < to_seconds(time, unit) * 10){
    //     if(rtc.HasSecondsPassed() && rtc.UpdateTimer()){
    //         rtc.GetElapsedTime(buffer);
    //         Serial.println(buffer);
    //     }
    //     anem.getData();
    //     counter++;   
    // }
    // Serial.print("Counter: ");
    // Serial.println(counter);
        
    // anem_test();
    // mfc_test();
    // relay_test(26, 33);
    // sd_test();
    // rtc_test();
    // pinMode(4, OUTPUT);
    // pinMode(5, OUTPUT);
    // pinMode(9, OUTPUT);

    // digitalWrite(4, HIGH);
    // digitalWrite(5, LOW);
    // analogWrite(9, 0);

    //pump_test(4, 5, 9);
    sys.InitializeSDRTC();
    sys.InitalRun();
    // sys.REASegregation();
    //  mfc.GetFullScaleAndUnits();
    //  if(mfc.SetFlow(0)) Serial.println("Success");
    //  Serial.println(mfc.GetFlow());
         // attachInterrupt(digitalPinToInterrupt(outA), updateEncoder, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(outB), updateEncoder, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(SW), updateButton, CHANGE); 
    
}

void loop(){
    //Serial.println(mfc.GetFlow());
    // anem.getData();
    // anem.printData();
    // delay(1000);
}

void stream_processing_test(){
    int N = 10;
    float test_data[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    float test_data2[N] = {13, 11, 14, 12, 15, 17, 20, 19, 18, 16};
    float mean = 0.0;
    float M2 = 0.0;
    float meanX = 0.0;
    float meanY = 0.0;
    float C = 0.0;
    for(int i = 0; i < N; i++){
        float delta = test_data[i] - mean;
        mean += delta / (float)(i+1);
        float delta2 = test_data[i] - mean;
        M2 += delta * delta2;
        delta2 = test_data2[i] - meanY;
        meanY += delta2 / (float)(i+1);
        C += delta * (test_data2[i] - meanY);
    } 
    Serial.print("Mean: ");
    Serial.println(mean);
    Serial.print("Var: ");
    Serial.println(M2 /(float)(N-1));
    Serial.print("Covar: ");
    Serial.println(C/(float)(N-1));
}