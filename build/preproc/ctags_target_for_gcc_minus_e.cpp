# 1 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\rea_tea.ino"
# 2 "D:\\ESS_projects\\true_eddy_accumulation\\rea_tea\\rea_tea.ino" 2
RTCDuration rtc;
void setup(){
    rtc.BeginRTC();
}

void loop(){
    DateTime test1 = rtc.GetCurrentTime();
    delay(500);
    DateTime test2 = rtc.GetCurrentTime();

}
