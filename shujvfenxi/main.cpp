#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct FlightSample
{
    double time;
    double altitude;
    double voltage;
};

struct ClimbRateSample
{
    double start_time;
    double end_time;
    double climb_rate;
};

double find_max_altitude(const std::vector<FlightSample>& samples);
double calculate_average_altitude(const std::vector<FlightSample>& samples);
double find_min_voltage(const std::vector<FlightSample>& samples);
double calculate_average_voltage(const std::vector<FlightSample>& samples);
bool find_low_voltage_time(
    const std::vector<FlightSample>& samples,
    double threshold,
    double& event_time
);
int count_low_voltage_samples(
    const std::vector<FlightSample>& samples,
    double threshold
);
double calculate_duration(const std::vector<FlightSample>& samples);
std::vector<FlightSample> filter_low_voltage_samples(
    const std::vector<FlightSample>& samples,
    double threshold
);
double find_max_climb_rate(const std::vector<FlightSample>& samples);

std::vector<ClimbRateSample> calculate_climb_rates(const std::vector<FlightSample>& samples);

void print_flight_states(const std::vector<ClimbRateSample>& climb_rate);

std::vector<FlightSample> load_flight_samples(const std::string& filename);

int main()
{
    std::string filename;
    std::cout<<"请输入遥测数据文件名：";
    if(!(std::cin>>filename)){
        std::cout<<"文件名输入错误"<<std::endl;
        return 1;
    }
    std::vector<FlightSample> samples = load_flight_samples(filename);
    // samples.push_back({0.0, 0.0, 16.8});
    // samples.push_back({1.0, 8.5, 16.3});
    // samples.push_back({2.0, 15.2, 15.7});
    // samples.push_back({3.0, 18.6, 14.9});
    // samples.push_back({4.0, 17.1, 13.8});

    if(samples.empty()){std::cout<<"没有遥测数据"<<std::endl;return 1;}
    std::cout<<"样本数据："<<samples.size()<<std::endl;

    for (const FlightSample& sample : samples)
    {
        std::cout << "时间：" << sample.time
                << " 高度：" << sample.altitude
                << " 电压：" << sample.voltage
                << std::endl;
    }
    double low_voltage_time = 0.0;
    double low_voltage_threshold;
    std::cout<<"请输入低电压阈值：";
    if(!(std::cin>>low_voltage_threshold)){
        std::cout<<"低电压阈值输入错误"<<std::endl;
        return 1;
    }
    if(low_voltage_threshold<=0.0){std::cout<<"低电压阈值应该大于0.0";return 1;}
    double max_altitude = find_max_altitude(samples);
    double average_altitude = calculate_average_altitude(samples);
    double min_voltage = find_min_voltage(samples);
    bool low_voltage_detected = find_low_voltage_time(samples,low_voltage_threshold,low_voltage_time);
    double average_voltage = calculate_average_voltage(samples);
    int number_low_voltage = count_low_voltage_samples(samples,low_voltage_threshold);
    double duration = calculate_duration(samples);
    std::vector<FlightSample> low_voltage_samples = filter_low_voltage_samples(samples,low_voltage_threshold);
    double max_climb_rate = find_max_climb_rate(samples);
    std::vector<ClimbRateSample> climb_rates = calculate_climb_rates(samples);

    std::cout<<"高度的最大值是："<<max_altitude<<std::endl;
    std::cout<<"高度的平均值是："<<average_altitude<<std::endl;
    std::cout<<"电压的最小值是："<<min_voltage<<std::endl;
    if(low_voltage_detected){
        std::cout<<"第一次低电压时间："
                <<low_voltage_time
                <<std::endl;
    }else{
        std::cout<<"没有发生低电压"<<std::endl;
    }
    std::cout<<"电压的平均值是："<<average_voltage<<std::endl;
    std::cout<<"低于"<<low_voltage_threshold<<"V"<<"的数据的个数："<<number_low_voltage<<std::endl;
    std::cout<<"记录持续时间是："<<duration<<std::endl;

    std::cout<<"过滤后的数据："<<std::endl;
    for(const FlightSample& sample : low_voltage_samples){
        std::cout << "时间：" << sample.time
                << " 高度：" << sample.altitude
                << " 电压：" << sample.voltage
                << std::endl;
    }

    std::cout<<"最大爬升率："<<max_climb_rate<<std::endl;
    for(const ClimbRateSample& sample : climb_rates){
        std::cout<<sample.start_time<<"秒到"<<sample.end_time<<"秒,爬升率："<<sample.climb_rate<<std::endl;
    }

    print_flight_states(climb_rates);

    std::ofstream report_file("analysis_report.txt");
    if(!report_file.is_open()){std::cout << "无法创建分析报告"<<std::endl;return 1;}

    report_file<<"无人机遥测分析报告"<<std::endl
                <<"样本数量:"<<samples.size()<<std::endl
                <<"最大高度:"<<max_altitude<<std::endl
                <<"平均高度:"<<average_altitude<<std::endl
                <<"最低电压:"<<min_voltage<<std::endl
                <<"平均电压:"<<average_voltage<<std::endl
                <<"低电压阈值:"<<low_voltage_threshold<<std::endl
                <<"低电压样本数量:"<<number_low_voltage<<std::endl
                <<"记录持续时间:"<<duration<<std::endl
                <<"最大爬升率:"<<max_climb_rate<<std::endl;
                
    report_file << "低电压样本：" << std::endl;

    for(const FlightSample& sample : low_voltage_samples){
        report_file << "时间：" << sample.time
                << " 高度：" << sample.altitude
                << " 电压：" << sample.voltage
                << std::endl;
    }

    return 0;
}

double find_max_altitude(const std::vector<FlightSample>& samples){
    double max_altitude = samples.at(0).altitude;

    for(const FlightSample& sample : samples){
        if(sample.altitude > max_altitude){
            max_altitude = sample.altitude;
        }
    }

    return max_altitude;
}

double calculate_average_altitude(const std::vector<FlightSample>& samples){
    double sum = 0.0;

    for(const FlightSample& sample : samples){
        sum = sum + sample.altitude;
    }

    return sum/static_cast<double>(samples.size());
}

double find_min_voltage(const std::vector<FlightSample>& samples){
    double min_voltage = samples.at(0).voltage;

    for(const FlightSample& sample : samples){
        if(sample.voltage <= min_voltage){
            min_voltage = sample.voltage;
        }
    }

    return min_voltage;
}

bool find_low_voltage_time(
    const std::vector<FlightSample>& samples,
    double threshold,
    double& event_time
){
    for(const FlightSample& sample : samples){
        if(sample.voltage < threshold){
            event_time = sample.time;
            return true;
        }
    }
    return false;//false：没有检测到低于要求电压值
}

double calculate_average_voltage(const std::vector<FlightSample>& samples){
    double sum = 0.0;
    for(const FlightSample& sample : samples){
        sum = sum + sample.voltage;
    }
    return sum / static_cast<double>(samples.size());
}

int count_low_voltage_samples(
    const std::vector<FlightSample>& samples,
    double threshold
){
    int count = 0;
    for(const FlightSample& sample : samples){
        if(sample.voltage < threshold){
            count++;
        }
    }
    return count;
}

double calculate_duration(const std::vector<FlightSample>& samples){
    double start = samples.front().time;
    double end   = samples.back().time;
    return (end-start);
}

std::vector<FlightSample> filter_low_voltage_samples(
    const std::vector<FlightSample>& samples,
    double threshold
){
    std::vector<FlightSample> samples_;
    for(const FlightSample& sample : samples){
        if(sample.voltage<threshold){
            samples_.push_back(sample);
        }
    }
    return samples_;
}

double find_max_climb_rate(const std::vector<FlightSample>& samples){
    if(samples.size()<2){return 0.0;}
    double climb_rate = -999999999.0;
    for(std::size_t i=1;i<samples.size();i++){
        if(0>=(samples.at(i).time-samples.at(i-1).time)){std::cout<<"时间差无效";continue;}
        double current_climb_rate = (samples.at(i).altitude - samples.at(i-1).altitude)/(samples.at(i).time-samples.at(i-1).time);
        if(current_climb_rate>climb_rate){
            climb_rate = current_climb_rate;
        }
    }
    return climb_rate;
}

std::vector<ClimbRateSample> calculate_climb_rates(const std::vector<FlightSample>& samples){
    std::vector<ClimbRateSample> climbratesample;
    if(samples.size()<2){std::cout<<"数据不足两个，无法计算爬升率"<<std::endl;return climbratesample;}
    for(std::size_t i=1;i<samples.size();i++){
        if(0>=(samples.at(i).time-samples.at(i-1).time)){std::cout<<"时间差无效";continue;}
        double current_climb_rate = (samples.at(i).altitude - samples.at(i-1).altitude)/(samples.at(i).time-samples.at(i-1).time);
        climbratesample.push_back({samples.at(i-1).time,samples.at(i).time,current_climb_rate});
    }

    return climbratesample; 
}

void print_flight_states(const std::vector<ClimbRateSample>& climb_rate){
    for(const ClimbRateSample& sample : climb_rate){
        if(sample.climb_rate > 0.5){std::cout<<sample.start_time<<"秒到"<<sample.end_time<<"秒：爬升"<<std::endl;}
        else if(sample.climb_rate < -0.5){std::cout<<sample.start_time<<"秒到"<<sample.end_time<<"秒：下降"<<std::endl;}
        else{std::cout<<sample.start_time<<"秒到"<<sample.end_time<<"秒：平飞"<<std::endl;}
    }
}

std::vector<FlightSample> load_flight_samples(const std::string& filename){
    std::vector<FlightSample> samples;
    std::ifstream input_file(filename);
    if(!input_file.is_open()){
        std::cout<<"无法打开文件："<<filename<<std::endl;
        return samples;
    }
    double time;
    double altitude;
    double voltage;
    char comma1;
    char comma2;
    while(input_file>>time>>comma1>>altitude>>comma2>>voltage){
        if(comma1!=','||comma2!=','){std::cout<<"文件格式错误,请检查文件"<<std::endl;return samples;}
        samples.push_back({time,altitude,voltage});
    }
    return samples;
}
