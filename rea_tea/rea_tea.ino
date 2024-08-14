
#include "src/rea_tea.h"
#include "src/component_test.h"
//RotaryEncoder enc;
// bool btn_pressed = false;
// void updateEncoder(){
//     enc.ReadState();
// }
//MksMfc mfc;
//

void setup(){
    //relay_test(26, 33);
    REATEASystem sys;
    Serial.begin(115200);
    // Serial.println("Initilizing System");
    sys.InitializeSDRTC();
    sys.InitialDataCollection();
    sys.REASampling();
}

void loop(){
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