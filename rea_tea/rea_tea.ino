
#include "src/rea_tea.h"
#include "src/component_test.h"
//RotaryEncoder enc;
// bool btn_pressed = false;
// void updateEncoder(){
//     enc.ReadState();
// }
//MksMfc mfc;
//

void setup(){    // relay_test(26, 33);
    // while(true){}
    Serial.begin(115200);
    REATEASystem sys;
    // Serial.println("Initilizing System");
    sys.InitializeSDRTC();
    sys.InitialDataCollection();
    sys.REASampling();
    //sd_test();
    //rtc_test();
    //mfc_test();
    //anem_test();
}

void loop(){
}

