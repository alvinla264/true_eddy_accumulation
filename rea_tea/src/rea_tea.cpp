#include "rea_tea.h"

REATEASystem::REATEASystem(): anem(Serial2), w_prime(0),
prev_avg(0.0), prev_std_dev(0.0), N(0), pump(PUMP_IN1, PUMP_IN2, PUMP_EN){
    for(int i = 0; i < NUM_OF_VALVES; i++){
        valves[i] = Relay(VALVE_PIN + i);
    }
    pump.turn_off();
}

void REATEASystem::InitializeSDRTC(){
    if(!rtc.BeginRTC()) {
        lcd.PrintBothLine("Error RTC", "Initialization");
        while(true){
        }
    }
    if(!sd.begin(53, SPI_SPEED)){
        lcd.PrintBothLine("Error SD", "Initialization");
        while(true){
        }
    }
    DateTime now = rtc.now();
    sprintf(data_file_name, "%0.02d_%0.02d_%d_%0.02d_%0.02d_%0.02d", 
        now.month(), now.day(), now.year(), now.hour(), now.minute(), now.second());
    if(sd.mkdir(data_file_name)){
        Serial.print("Directory ");
        Serial.print(data_file_name);
        Serial.println(" created Successfully");
        if(!sd.chdir(data_file_name)){
            Serial.println("Unable to change directiory");
            lcd.PrintBothLine("Failed To Change", "Directory");
            while(true){}
        }
    }
    else{
        Serial.print("Failed to create directiory ");
        Serial.println(data_file_name);
        lcd.PrintBothLine("Failed To Create", "Directory");
        while(true){}
    } 
    return;
}
void REATEASystem::WriteDataTOSD(){
    data_file.print(rtc.now().timestamp() + ",");
    data_file.print(anem.getNorthSouth());
    data_file.print(",");
    data_file.print(anem.getWestEast());
    data_file.print(",");
    data_file.print(anem.getUpDown());
    data_file.print(",");
    data_file.print(anem.getTempK());
    data_file.print(",");
    data_file.print(anem.getWindSpeed());
    data_file.print(",");
    data_file.print(anem.getWindDir());
    data_file.print(",");
    data_file.print(w_prime);
    data_file.print(",");
    data_file.println(WindStatusToString(wind_status));
}
void REATEASystem::InitialDataCollection(){
    int total_seconds = rtc.StartTimer(INTIAL_RUN_TIME, minute);
    char second_line[17];
    rtc.GetRemainingTime(second_line);
    sprintf(data_file_name, "%s0.csv", DATA_FILE_NAME);
    data_file.open(data_file_name, FILE_WRITE);
    data_file.println("Date and Time,U,V,W,Temp,W_Prime,Wind Speed, Wind Direction, Wind Status");
    lcd.PrintBothLine("Data Collect", second_line);
    prev_avg = 0;
    N = 0;
    float var_sum = 0.0;
    float delta, delta2;
    //N < total_seconds * DATA_FREQ
    while(rtc.UpdateTimer()){
        anem.getData();
        N++;
        delta = anem.getUpDown() - prev_avg;
        prev_avg += delta / (float)N;
        delta2 = anem.getUpDown() - prev_avg;
        var_sum += delta * delta2;
        WriteDataTOSD();
        if(rtc.HasSecondsPassed()){
            rtc.GetRemainingTime(second_line);
            lcd.PrintSecondLine(second_line, true);
            //Serial.println(second_line);
        }
    }
    data_file.close();
    prev_std_dev = sqrt(var_sum / (float)(N-1));
    if(DEBUG){
        Serial.print("Average: ");
        Serial.println(prev_avg);
        Serial.print("Standard Dev: ");
        Serial.println(prev_std_dev);
    }
}

void REATEASystem::REASampling(){
    for(int i = 0; i < NUM_OF_RUNS; i++){
        sprintf(data_file_name, "%s%d.csv", DATA_FILE_NAME, i + 1);
        data_file.open(data_file_name, FILE_WRITE);
        data_file.println("Date and Time,U,V,W,Temp,W_Prime,Wind Speed, Wind Direction, Wind Status");
        N = 0;
        float threshold = K * prev_std_dev; 
        unsigned long int num_of_t[2] = {0, 0}; //0: t-up 1: t-down
        long double t_sum[2] = {0.0, 0.0};//0: t-up 1: t-down
        int total_seconds = rtc.StartTimer(SEGREGATION_TIME, minute);
        char first_line[17];
        char second_line[17];
        sprintf(first_line, "Sampling %d", i + 1);
        rtc.GetRemainingTime(second_line);
        lcd.PrintBothLine(first_line, second_line);
        float mean = 0.0;
        float var_sum = 0.0;
        float meanY = 0.0;
        float C = 0.0;
        float delta, delta2;
        float flow_avg = 0.0;
        valves[neutral].TurnRelayOn();
        mfc.SetFlow(DEFAULT_FLOW_RATE);
        pump.update_motor(forward, 255);
        while(mfc.GetFlow() < DEFAULT_FLOW_RATE * 0.99){}
        valves[neutral].TurnRelayOff();
        //N < DATA_FREQ * total_seconds
        while(rtc.UpdateTimer()){
            if(rtc.HasSecondsPassed()){
                rtc.GetRemainingTime(second_line);
                lcd.PrintBothLine(first_line, second_line);
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
            float flow = mfc.GetFlow();
            flow_avg += flow;
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
                int index = (wind_status == down) ? 1 : 0;
                num_of_t[index]++;
                t_sum[index] += anem.getTempK();
            }
            WriteDataTOSD();
            if(DEBUG){
                Serial.println(WindStatusToString(wind_status));
                Serial.print("Flow: ");
                Serial.println(flow);
            }
        }

        pump.turn_off();
        mfc.SetFlow(0);
        for(int j = 0; j < NUM_OF_VALVES; j++){
            valves[j].TurnRelayOff();
        }
        for(int i = 0; i < 2; i++)
            temperature_avg[i] = (num_of_t[i] > 0) ? t_sum[i] / (float)num_of_t[i] : 0;
        flow_avg /= (float)N;
        float denominator = prev_std_dev * (temperature_avg[0] - temperature_avg[1]);
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
        data_file.print(temperature_avg[0]);
        data_file.print(",");
        data_file.println(temperature_avg[1]);
        data_file.close();
        if(DEBUG){
            Serial.print("New Threshold: ");
            Serial.println(threshold);
            Serial.print("B_Coeff: ");
            Serial.println(b_coefficient);
            Serial.print("Heat Flux: ");
            Serial.println(heat_flux);
            Serial.print("Avg_flow: ");
            Serial.println(flow_avg);
            Serial.print("Standard Dev: ");
            Serial.println(prev_std_dev);
            Serial.print("Average: ");
            Serial.println(prev_avg);
            Serial.print("Up temp: ");
            Serial.println(temperature_avg[0]);
            Serial.print("Down Temp");
            Serial.println(temperature_avg[1]);
        }
    }
    lcd.PrintBothLine("Sampling", "Finished");
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
