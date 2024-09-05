#include "component_test.h"

// void anem_test(){
//     Anemometer anem(Serial2);
//     for(int i = 0; i < 5; i++){
//         anem.getData();
//         anem.printData();
//     }
// }

void sd_test(){
    SdFat sd;
    if(!sd.begin(53, SPI_SPEED)){
        Serial.println("Error Initializing SD");
        return;
    }
        
    File test_file;
    if(sd.exists("test_file.txt")) sd.remove("test_file.txt");
    if(!test_file.open("test_file.txt", FILE_WRITE)) {
        Serial.println("Error Opening test file for writing");
        test_file.close();
        return;
    }
    String write_str = "hello world";
    test_file.println(write_str);
    test_file.close();
    if(!test_file.open("test_file.txt", FILE_READ)){
        Serial.println("Error Opening test file for reading");
        test_file.close();
        return;
    }
    String read_str = test_file.readStringUntil('\n');
    read_str.trim();
    if(read_str == write_str) 
        Serial.println("read successful");
    else {
        Serial.println("read unsuccessful");
        Serial.print("read str: |");
        Serial.print(read_str);
        Serial.println("|");
        Serial.print("write str: |");
        Serial.print(write_str);
        Serial.println("|");
    }
    test_file.close();
}

void rtc_test(){
    TimerRTC rtc;
    char time[20];
    if(!rtc.BeginRTC()) 
        Serial.println("Error Initializing RTC");
    Serial.println("Testing Get remaining time func for 5 seconds");
    rtc.StartTimer(5, second);
    while(rtc.UpdateTimer()){
        if(rtc.HasSecondsPassed()){
            rtc.GetRemainingTime(time);
            Serial.println(time);
        }
    }
    Serial.println("Testing Get elapsed time func for 5 seconds");
    rtc.StartTimer(5, second);
    while(rtc.UpdateTimer()){
        if(rtc.HasSecondsPassed()){
            rtc.GetElapsedTime(time);
            Serial.println(time);
        }
    }

}

void mfc_test(){
    MksMfc mfc;
    mfc.GetFullScaleAndUnits();
    Relay valve(26);
    L298N pump(4, 5, 9);
    for(int i = 0; i < 3; i++){
        Serial.println(mfc.GetFlow());
    }
}

void relay_test(int start, int end){
    for(int i = start; i <= end; i++){
        Relay temp_relay(i);
        temp_relay.TurnRelayOn();
        delay(1000);
        temp_relay.TurnRelayOff();
        delay(1000);
    }
}

void pump_test(int in1, int in2, int enA = -1){
    L298N pump(in1, in2, enA);
    for(int i = 0; i < 256; i++){
        pump.update_motor(forward, i);
        delay(50);
    }
    delay(5000);
    pump.turn_off(); 
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