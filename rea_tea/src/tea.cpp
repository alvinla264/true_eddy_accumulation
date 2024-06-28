#include "tea.h"

REATEASystem::REATEASystem(): anem(Serial2), pump(PUMP_PIN), w_prime(0),
prev_avg(0.0), prev_std_dev(0.0), N(0){
    for(int i = 0; i < NUM_OF_VALVES; i++){
        valves[i] = Relay(VALVE_PIN + i);
    }
    file_read_str.reserve(STR_BUFF_SIZE);
    lcd_line[0].reserve(STR_BUFF_SIZE);
    lcd_line[1].reserve(STR_BUFF_SIZE);
}

bool REATEASystem::InitializeSDRTC(){
    if(!rtc.BeginRTC() || !sd.begin(53, SPI_SPEED)) return false;
    folder_name = rtc.now().timestamp();
    sd.mkdir(folder_name);
    sd.chdir(folder_name);

}
void REATEASystem::WriteDataTOSD(){
    anem_data[U] = anem.getNorthSouth();
    anem_data[V] = anem.getWestEast();
    anem_data[W] = anem.getUpDown();
    anem_data[Temp] = anem.getTempK();
    anem_data[windSpd] = anem.getWindSpeed();
    anem_data[windDir] = anem.getWindDir();
    anem_data[wPrime] = w_prime;
    for(int i = 0; i < ANEM_DATA_SIZE; i++){
        data_file.print(anem_data[i]);
        data_file.print(",");
    }
    data_file.println(WindStatusToString(wind_status));
}

void REATEASystem::InitalRun(){
    in_initial_run = true;
    File initial_run_file;
    initial_run_file.open("initial_run_data.csv", FILE_WRITE);
    rtc.StartTimer(INTIAL_RUN_TIME, minute);
    char second_line[17];
    rtc.GetRemainingTime(second_line);
    lcd.PrintBothLine("Initial Run", second_line);
    prev_avg = 0;
    N = 0;
    float var_sum = 0.0;
    float delta, delta2;
    while(rtc.UpdateTimer()){
        anem.getData();
        N++;
        delta = anem.getUpDown() - prev_avg;
        prev_avg += delta / (float)N;
        delta2 = anem.getUpDown() - prev_avg;
        var_sum += delta * delta2;
        // initial_run_file.println(anem_data[W]);
        // initial_run_file.flush();
        if(rtc.HasSecondsPassed()){
            rtc.GetRemainingTime(second_line);
            lcd.PrintSecondLine(second_line, true);
        }
    }
    initial_run_file.close();
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
        pump.TurnRelayOn();
        float mean = 0.0;
        float var_sum = 0.0;
        float meanY = 0.0;
        float C = 0.0;
        float delta, delta2;
        float flow_avg = 0.0;
        mfc.SetFlow(200);
        pump.TurnRelayOn();
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
                valves[i].TurnRelayOff();
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

        pump.TurnRelayOff();
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
