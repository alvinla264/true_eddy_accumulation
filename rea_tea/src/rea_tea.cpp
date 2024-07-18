#include "rea_tea.h"

REATEASystem::REATEASystem(): anem(Serial2), w_prime(0),
prev_avg(0.0), prev_std_dev(0.0), N(0), pump(PUMP_IN1, PUMP_IN2, PUMP_EN){
    for(int i = 0; i < NUM_OF_VALVES; i++){
        valves[i] = Relay(VALVE_PIN + i);
    }
    file_read_str.reserve(STR_BUFF_SIZE);
    lcd_line[0].reserve(STR_BUFF_SIZE);
    lcd_line[1].reserve(STR_BUFF_SIZE);
    pump.turn_off();
}

bool REATEASystem::InitializeSDRTC(){
    if(!rtc.BeginRTC() || !sd.begin(53, SPI_SPEED)) return false;
    DateTime now = rtc.now();
    sprintf(data_file_name, "%0.02d_%0.02d_%d_%0.02d_%0.02d_%0.02d", 
        now.month(), now.day(), now.year(), now.hour(), now.minute(), now.second());
    if(sd.mkdir(data_file_name)){
        Serial.print("Directory ");
        Serial.print(data_file_name);
        Serial.println(" created Successfully");
        if(!sd.chdir(data_file_name)){
            Serial.println("Unable to change directiory");
        }
    }
    else{
        Serial.print("Failed to create directiory ");
        Serial.println(data_file_name);
    }
    

}
void REATEASystem::WriteDataTOSD(){
    anem_data[U] = anem.getNorthSouth();
    anem_data[V] = anem.getWestEast();
    anem_data[W] = anem.getUpDown();
    anem_data[Temp] = anem.getTempK();
    anem_data[windSpd] = anem.getWindSpeed();
    anem_data[windDir] = anem.getWindDir();
    anem_data[wPrime] = w_prime;
    data_file.print(rtc.now().timestamp() + ",");
    for(int i = 0; i < ANEM_DATA_SIZE + 1; i++){
        data_file.print(anem_data[i]);
        data_file.print(",");
    }
    data_file.println(WindStatusToString(wind_status));
}
void REATEASystem::InitalRun(){
    int total_seconds = rtc.StartTimer(INTIAL_RUN_TIME, minute);
    char second_line[17];
    rtc.GetRemainingTime(second_line);
    sprintf(data_file_name, "%s0.csv", DATA_FILE_NAME);
    data_file.open(data_file_name, FILE_WRITE);
    data_file.println("Date and Time,U,V,W,Temp,W_Prime,Wind Speed, Wind Direction, Wind Status");
    lcd.PrintBothLine("Initial Run", second_line);
    prev_avg = 0;
    N = 0;
    float var_sum = 0.0;
    float delta, delta2;
    int data_counter = 0;
    while(data_counter < total_seconds * DATA_FREQ){
        anem.getData();
        data_counter++;
        N++;
        delta = anem.getUpDown() - prev_avg;
        prev_avg += delta / (float)N;
        delta2 = anem.getUpDown() - prev_avg;
        var_sum += delta * delta2;
        // initial_run_file.println(anem_data[W]);
        // initial_run_file.flush();
        WriteDataTOSD();
        if(rtc.HasSecondsPassed() && rtc.UpdateTimer()){
            rtc.GetRemainingTime(second_line);
            lcd.PrintSecondLine(second_line, true);
            Serial.println(second_line);
        }
    }
    data_file.close();
    prev_std_dev = sqrt(var_sum / (float)(N-1));
}

void REATEASystem::REASegregation(){
    for(int i = 0; i < NUM_OF_RUNS; i++){
        sprintf(data_file_name, "%s%d.csv", DATA_FILE_NAME, i + 1);
        data_file.open(data_file_name, FILE_WRITE);
        data_file.println("U,V,W,Temp,W_Prime,Wind Speed, Wind Direction, Wind Status");
        N = 0;
        float threshold = K * prev_std_dev; 
        unsigned long int num_of_t[2] = {0, 0}; //0: t-up 1: t-down
        long double t_sum[2] = {0.0, 0.0};//0: t-up 1: t-down
        rtc.StartTimer(SEGREGATION_TIME, minute);
        char second_line[17];
        rtc.GetRemainingTime(second_line);
        lcd.PrintBothLine("Segregation", second_line);
        float mean = 0.0;
        float var_sum = 0.0;
        float meanY = 0.0;
        float C = 0.0;
        float delta, delta2;
        float flow_avg = 0.0;
        mfc.SetFlow(200);
        pump.update_motor(forward, 255);
        while(rtc.UpdateTimer()){
            if(rtc.HasSecondsPassed()){
                rtc.GetRemainingTime(second_line);
                lcd.PrintBothLine("Segregation", second_line);
            }
            anem.getData();

            N++;
            delta = anem.getUpDown() - mean;
            mean += delta / (float)(N);
            delta2 = anem.getUpDown() - mean;
            var_sum += delta * delta2;
            delta2 = anem.getTempK() - meanY;
            meanY += delta2 / (float)(N);
            C += delta * (anem.getTempK() - meanY);
            flow_avg += mfc.GetFlow();
            w_prime = anem.getUpDown() - prev_avg; //w - avg
            if(threshold < w_prime && anem.getIsValid()){
                wind_status = up;
                valves[i].TurnRelayOn();
                valves[down + i].TurnRelayOff();
                valves[neutral].TurnRelayOff();
                valves[bypass].TurnRelayOff();
            }
            //down
            else if(w_prime < (threshold * -1) && anem.getIsValid()){
                wind_status = down;
                valves[up + i].TurnRelayOff();
                valves[down + i].TurnRelayOn();
                valves[neutral].TurnRelayOff();
                valves[bypass].TurnRelayOff();
            }
            //neutral
            else if(abs(w_prime) < threshold || !anem.getIsValid()){
                valves[i].TurnRelayOff();
                valves[down + i].TurnRelayOff();
                valves[neutral].TurnRelayOn();
                valves[bypass].TurnRelayOff();
            }
            //this calculates avergae Temperature in the Up or Down
            if(wind_status != neutral){
                num_of_t[wind_status]++;
                t_sum[wind_status] += anem.getTempK();
            }
            WriteDataTOSD();
        }

        pump.turn_off();
        mfc.SetFlow(0);
        for(int j = 0; j < NUM_OF_VALVES; j++){
            valves[j].TurnRelayOff();
        }
        temperature_avg[up] = (num_of_t[up] > 0) ? t_sum[up] / (float)num_of_t[up] : 0;
        temperature_avg[down] = (num_of_t[down] > 0) ? t_sum[down] / (float) num_of_t[down] : 0;
        flow_avg /= (float)N;
        float denominator = prev_std_dev * (temperature_avg[up] - temperature_avg[down]);
        prev_avg = mean;
        prev_std_dev = sqrt(var_sum/(float)(N - 1));
        covariance = C / (float)(N-1);
        heat_flux = RHO_AIR * CP_AIR * covariance;
        b_coefficient = (denominator == 0) ? 0 : covariance / denominator;
        data_file.close();
        sprintf(data_file_name, "%s%d.csv", STATISTIC_FILE_NAME, i + 1);
        data_file.open(data_file_name, FILE_WRITE);
        data_file.println("Threshold,B-Coefficient, Heat Flux, Average Flow, Standard Dev, Average, Avg Up Temp, Avg Down Temp");
        data_file.print(threshold);
        data_file.print(",");
        data_file.print(b_coefficient);
        data_file.print(",");
        data_file.print(heat_flux);
        data_file.print(",");
        data_file.print(flow_avg);
        data_file.print(",");
        data_file.print(prev_std_dev);
        data_file.print(",");
        data_file.print(prev_avg);
        data_file.print(",");
        data_file.print(temperature_avg[up]);
        data_file.print(",");
        data_file.println(temperature_avg[down]);
        data_file.close();
    }
}

const char *WindStatusToString(WindStatus status){
    switch(status){
        case up:
            return "up";
        case down:
            return "down";
        case neutral:
            return "neut";
        default:
            return "none";
    }
}
