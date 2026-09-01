#include <iostream>
#include <string>
#include <vector>
#include <fstream>

enum class DropState{
    Disarmed,
    Armed,
    TakingOff,
    FlyingToTarget,
    ReadyToDrop,
    PayloadReleased,
    MissionComplete
};

struct DroneData{
    int index;
    bool armed;
    double altitude;
    double distance_to_target;
    bool release_command;
};

class DropController{
    private:
        DropState current_state;
        double takeoff_altitude;
        double ready_distance;
    public:
        DropController(double takeoff_altitude_value,double ready_distance_value);
        bool update(const DroneData& data);
        DropState get_state() const;
};


std::string state_to_string(DropState state);

int main()
{
    int index;
    bool armed;
    double altitude;
    double distance_to_target;
    bool release_command;
    char comma1;
    char comma2;
    char comma3;
    char comma4;
    const double takeoff_altitude = 30.0;
    const double ready_distance = 10.0;
    DropController controller(takeoff_altitude,ready_distance);
    std::string state = state_to_string(controller.get_state());
    std::cout<<"初始状态:"<<state<<std::endl;
    std::string filename;
    std::cout<<"请输入文件名：";
    if(!(std::cin>>filename)){
        std::cout<<"输入文件错误"<<std::endl;
        return 1;
    }
    std::vector<DroneData> datas;
    std::ifstream input_file(filename);
    if(!input_file.is_open()){
        std::cout<<"无法打开文件："<<filename<<std::endl;
        return 1;
    }

    while(input_file>>index
                    >>comma4
                    >>armed
                    >>comma1
                    >>altitude
                    >>comma2
                    >>distance_to_target
                    >>comma3
                    >>release_command){
                if(comma1!=','||comma2!=','|| comma3!=','||comma4!=','){std::cout<<"文件格式错误,请检查文件"<<std::endl;return 1;}
                
                datas.push_back({index,
                                armed,
                                altitude,
                                distance_to_target,
                                release_command
                });

    }

    if(datas.empty()){std::cout<<"没有数据"<<std::endl;return 1;}
    std::cout<<"数据大小："<<datas.size()<<std::endl;

    for(const DroneData& data : datas){
        
        if(controller.update(data)){
            std::cout<<"data"<<data.index<<": "<<"状态改变，"<<state_to_string(controller.get_state())<<std::endl;
        }else{
            std::cout<<"data"<<data.index<<": "<<"状态不变，"<<state_to_string(controller.get_state())<<std::endl;
        }
    }

    return 0;
}


std::string state_to_string(DropState state){
    switch(state)
    {
        case DropState::Disarmed:return "未解锁";
        case DropState::Armed:return "解锁";
        case DropState::TakingOff:return "起飞";
        case DropState::FlyingToTarget:return "飞向目标点";
        case DropState::ReadyToDrop:return "准备投放";
        case DropState::PayloadReleased:return "释放载荷";
        case DropState::MissionComplete:return "任务完成";
    }
     return "未知状态";
}

DropController::DropController(double takeoff_altitude_value,
                                double ready_distance_value)
    :current_state(DropState::Disarmed),
    takeoff_altitude(takeoff_altitude_value),
    ready_distance(ready_distance_value){}
bool DropController::update(const DroneData& data){
    switch(current_state)
    {
        case DropState::Disarmed:
            if(data.armed){
                current_state = DropState::Armed;
                return true;
            }
            return false;
        case DropState::Armed:
            if(data.altitude>0.0){
                current_state = DropState::TakingOff;
                return true;
            }
            return false;
        case DropState::TakingOff:
            if(data.altitude>=takeoff_altitude){
                current_state = DropState::FlyingToTarget;
                return true;
            }
            return false;
        case DropState::FlyingToTarget:
            if(data.distance_to_target<=ready_distance){
                current_state = DropState::ReadyToDrop;
                return true;
            }
            return false;
        case DropState::ReadyToDrop:
            if(data.release_command==true){
                current_state = DropState::PayloadReleased;
                return true;
            }
            return false;
        case DropState::PayloadReleased:
            current_state = DropState::MissionComplete;
            return true;
        case DropState::MissionComplete:
            return false;
    }
    return false;
}
DropState DropController::get_state() const{
    return current_state;
}