#include <iostream>
#include <cmath>
#include <vector>
struct Position
{
    double x;
    double y;
};

struct Waypoint
{
    int id;
    Position position;
};

double calculate_distance(const Position& current,
                        const Position& target);
class Navigator{
    private:
        Position current_position;//保存无人机当前位置
        std::vector<Waypoint> waypoints;
        std::size_t current_waypoint_index;//接下来移动到那个航点游标
        bool move_toward(const Position& target,double step_distance);//让无人机朝目标移动
    public:
        Navigator(const Position& initial_position , const std::vector<Waypoint>& mission_waypoints);//通过构造函数在实例化的时候，初始化位置
        const Position& get_position() const;//通过两个const限制，只允许外部查看当前位置
        bool mission_complete() const;//判断任务是否完成
        const Waypoint* get_current_waypoint() const;
        bool update(double step_distance);//false:未到达或者任务完成,true:到达航点
};

int main()
{
    std::vector<Waypoint> waypoints;
    
    waypoints.push_back({1,{3.0,4.0}});
    waypoints.push_back({2,{6.0,8.0}});
    waypoints.push_back({3,{-3.0,4.0}});

    Navigator navigator({0.0,0.0},waypoints);

    const double step_distance = 1.0;

    while(!navigator.mission_complete()){
        const Waypoint* target = 
            navigator.get_current_waypoint();

        if(target==nullptr){
            std::cout<<"无法取得当前航点"<<std::endl;
            return 1;
        }

        int target_id = target->id;
        bool reached = 
            navigator.update(step_distance);
        const Position& position = 
            navigator.get_position();
        std::cout<<"无人机的位置："
                <<position.x
                <<","
                <<position.y
                <<std::endl;
        if(reached){
            std::cout<<"到达航点："
                    <<target_id
                    <<std::endl;
        }
    }
    std::cout<<"全部航点已完成"<<std::endl;

    return 0;
}

double calculate_distance(const Position& current,
                        const Position& target){
    double dx = target.x - current.x;
    double dy = target.y - current.y;
    double distance = std::sqrt(dx*dx+dy*dy);
    return distance;
}

const Position& Navigator::get_position() const{
    return current_position;
}

Navigator::Navigator(const Position& initial_position,const std::vector<Waypoint>& mission_waypoints)
    :current_position(initial_position),
     waypoints(mission_waypoints),
     current_waypoint_index(0)
    {}

bool Navigator::move_toward(const Position& target,double step_distance){
    const double tolerance = 1e-9;
    if(step_distance<=0.0){std::cout<<"移动距离必须大于0"<<std::endl;return false;}
    double distance = calculate_distance(current_position,target);
    if(distance<=step_distance+tolerance){
        current_position = target;
        return true;
    }else{
        double ratio = step_distance / distance;
        current_position.x = current_position.x + (target.x-current_position.x)*ratio;
        current_position.y = current_position.y + (target.y-current_position.y)*ratio;
    }
    return false;
}

bool Navigator::mission_complete() const{
    return current_waypoint_index >= waypoints.size();
}

const Waypoint* Navigator::get_current_waypoint() const{
    if(mission_complete()){return nullptr;}
    return &waypoints.at(current_waypoint_index);
}

bool Navigator::update(double step_distance){
    if(mission_complete()){return false;}
    const Waypoint& target = waypoints.at(current_waypoint_index);
    bool reached = move_toward(target.position,step_distance);//true:到达航点
    if(reached){current_waypoint_index++;}
    return reached;
}
