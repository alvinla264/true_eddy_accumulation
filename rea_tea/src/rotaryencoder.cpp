#include "rotaryencoder.h"

// RotaryEncoder::RotaryEncoder(int OUTA, int OUTB): outA(OUTA), outB(OUTB){
//     pinModeFast(outA, INPUT_PULLUP);
//     pinModeFast(outB, INPUT_PULLUP);
//     last_outA_value = currentStateA;
//     position = 0;
// }


RotaryEncoder::RotaryEncoder(){
    pinMode(outA, INPUT_PULLUP);
    pinMode(outB, INPUT_PULLUP);
    last_outA_value = (PIN_TYPE & (1 << OUTA_PORT)) >> OUTA_PORT; // Read CLK pin directly from port register
    last_outB_value = (PIN_TYPE & (1 << OUTB_PORT)) >> OUTB_PORT;
    position = 0;
}
void RotaryEncoder::ReadState(){
  uint8_t outA_value = (PIN_TYPE & (1 << OUTA_PORT)) >> OUTA_PORT; // Read CLK pin directly from port register
  uint8_t outB_value = (PIN_TYPE & (1 << OUTB_PORT)) >> OUTB_PORT;  // Read DT pin directly from port register
  if (outA_value != last_outA_value) {
    if (outB_value != outA_value) {
      position++;
    } 
    else {        
      position--;
    }
  }
  if (outB_value != last_outB_value) {
    if (outA_value == outB_value) {
      position++;
    } 
    else {        
      position--;
    }
  }
  last_outA_value = outA_value;
  last_outB_value = outB_value;
}


long RotaryEncoder::GetPositon(){ return position;}

void RotaryEncoder::SetPositionMax(){position = SLIDER_MAX_VALUE;}

void RotaryEncoder::SetPositionMin(){position = 0;}