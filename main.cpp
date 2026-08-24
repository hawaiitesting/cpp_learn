#include <iostream>
#include <string>
class Gua{
public:
    static const int qian[3];
    static const int dui[3];
    static const int li[3];
    static const int zhen[3];
    static const int xun[3];
    static const int kan[3];
    static const int gen[3];
    static const int kun[3];
    static const int* get_gua(int count) 
    {
        switch (count)
        {
            case 1 :return &qian[0];
            case 2 :return &dui[0];
            case 3 :return &li[0];
            case 4 :return &zhen[0];
            case 5 :return &xun[0];
            case 6 :return &kan[0];
            case 7 :return &gen[0];
            case 8 :return &kun[0];
            default:return nullptr;
        }
    }

    static int get_gua_number(const int target[]){
        for(int i=1;i<9;i++){
            const int *p=get_gua(i);
            if(target[0]==p[0]&&
                target[1]==p[1]&&
                target[2]==p[2]){
                return i;
            }   
        }
        return 0;
    }
};
const int Gua::qian[3]={0,0,0};   //乾
const int Gua::dui[3]={1,0,0};    //兑
const int Gua::li[3]={0,1,0};     //离
const int Gua::zhen[3]={1,1,0};   //震
const int Gua::xun[3]={0,0,1};    //巽
const int Gua::kan[3]={1,0,1};    //坎
const int Gua::gen[3]={0,1,1};    //艮
const int Gua::kun[3]={1,1,1};    //坤

class ChongGua{
public:
    int chong_gua[6];
    int shang_hu_gua[3];
    int xia_hu_gua[3];
    int bian_in_chong_gua[6];
    int bian_gua[3];
    int ti_gua[3];
    int yong_gua[3];

    ChongGua(const int shang_gua[],const int xia_gua[])
    {
        for (int i=0;i<3;i++)
        {
            chong_gua[i]=shang_gua[i];
        }
        for (int i=3;i<6;i++)
        {
            int j=i-3;
            chong_gua[i]=xia_gua[j];
        }
    }

    void generate_hu_gua(){
        for(int i=0;i<3;i++){
            shang_hu_gua[i]=chong_gua[i+1];
            xia_hu_gua[i]=chong_gua[i+2];
        }
    };

    bool generate_bian_yong_ti_gua(int dong_yao){
        if(dong_yao>6||dong_yao<1){
            return false;
        }

        for(int i=0;i<6;i++){
            bian_in_chong_gua[i]=chong_gua[i];
        }

        if(dong_yao<=3){
            bian_in_chong_gua[6-dong_yao]=1-bian_in_chong_gua[6-dong_yao];
            for(int i=3;i<6;i++){
                bian_gua[i-3]=bian_in_chong_gua[i];
            }
            for(int i=0;i<3;i++){
                ti_gua[i]=chong_gua[i];
            }
            for(int i=3;i<6;i++){
                yong_gua[i-3]=chong_gua[i];
            }
        }

        if(dong_yao>3){
            bian_in_chong_gua[6-dong_yao]=1-bian_in_chong_gua[6-dong_yao];
            for(int i=0;i<3;i++){
                bian_gua[i]=bian_in_chong_gua[i];
            }
            for(int i=3;i<6;i++){
                ti_gua[i-3]=chong_gua[i];
            }
            for(int i=0;i<3;i++){
                yong_gua[i]=chong_gua[i];
            }
        }
        return true;
    }
};

void print_dan_gua(
    const std::string& title,
    int number,
    const std::string gua_names[],
    const int gua[]){
        std::cout << title
                  << ":序号："
                  << number
                  << gua_names[number-1]
                  << "";
        
        for (int i=0;i<3;i++){
            std::cout<<gua[i];
        }
        std::cout << std::endl;
    }

int main()
{
    const std::string gua_name[8] = {"乾","兑","离","震","巽","坎","艮","坤",};
    const std::string yao_name[6] = {"上爻", "五爻", "四爻","三爻", "二爻", "初爻"};
    
    std::cout<<"请输入上卦"<<std::endl;
    int a = 0;
    std::cin >>a;
    const int* shang_gua=Gua::get_gua(a);
    if(shang_gua==nullptr){std::cout<<"上卦输入错误,编号必须为1到8"<<std::endl;return 1;}
    std::cout   <<"上卦"
                <<":"
                <<gua_name[a-1]
                <<std::endl;
    for (int i=0;i<3;i++){std::cout<<shang_gua[i]<<std::endl;}

    std::cout<<"请输入下卦"<<std::endl;
    int b = 0;
    std::cin >>b;
    const int *xia_gua=Gua::get_gua(b);
    if(xia_gua==nullptr){std::cout<<"下卦输入错误,编号必须为1到8"<<std::endl;return 1;}
    std::cout   <<"下卦"
                <<":"
                <<gua_name[b-1]
                <<std::endl;
    for (int i=0;i<3;i++){std::cout<<xia_gua[i]<<std::endl;}

    ChongGua chonggua(shang_gua,xia_gua);

    std::cout<<"以下是生成的重卦"<<std::endl;

    for (int i = 0; i <6; i++)
    {
        std::cout<<yao_name[i]<<":"<<chonggua.chong_gua[i]<<std::endl;
    }

    chonggua.generate_hu_gua();

    int c=Gua::get_gua_number(chonggua.shang_hu_gua);//上互卦的序号
    int d=Gua::get_gua_number(chonggua.xia_hu_gua);//下互卦的序号

    if(c==0||d==0){
        std::cout<<"互卦识别失败"<<std::endl;
        return 1;
    }

    print_dan_gua(
        "上互卦",c,gua_name,chonggua.shang_hu_gua
    );

    print_dan_gua(
    "下互卦", d, gua_name, chonggua.xia_hu_gua
    );

    std::cout<<"请输入动爻。初爻是1,上爻是6"<<std::endl;
    int dongyao;
    std::cout<<"动爻：";
    std::cin>>dongyao;
    bool success = chonggua.generate_bian_yong_ti_gua(dongyao);
    if(success==false){
        std::cout<<"动爻输入错误,编号必须为1到6"<<std::endl;
        return 1;
    }
    int e=Gua::get_gua_number(chonggua.ti_gua);//体卦的序号
    int f=Gua::get_gua_number(chonggua.yong_gua);//用卦的序号
    int g=Gua::get_gua_number(chonggua.bian_gua);//变卦的序号

    if(e==0||f==0||g==0){
        std::cout<<"体卦,用卦,变卦识别失败。"<<std::endl;
        return 1;
    }

    print_dan_gua(
        "体卦", e, gua_name, chonggua.ti_gua
    );

    print_dan_gua(
        "用卦", f, gua_name, chonggua.yong_gua
    );

    print_dan_gua(
        "变卦", g, gua_name, chonggua.bian_gua
    );

    return 0;
}