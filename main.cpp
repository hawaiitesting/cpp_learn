#include <iostream>
#include <string>
#include <array>//标准库容器
class Gua{
private:
    static const std::array<int,3> qian;
    static const std::array<int,3>  dui;
    static const std::array<int,3>  li;
    static const std::array<int,3>  zhen;
    static const std::array<int,3>  xun;
    static const std::array<int,3>  kan;
    static const std::array<int,3>  gen;
    static const std::array<int,3>  kun;
public:
    static const std::array<int,3>* get_gua(int count)
    {
        switch (count)
        {
            case 1 :return &qian;
            case 2 :return &dui;
            case 3 :return &li;
            case 4 :return &zhen;
            case 5 :return &xun;
            case 6 :return &kan;
            case 7 :return &gen;
            case 8 :return &kun;
            default:return nullptr;
        }
    }

    static int get_gua_number(const std::array<int,3>& target){
        for(int i=1;i<9;i++){
            const std::array<int,3>* p=get_gua(i);
            if(p != nullptr&&*p==target){
                return i;
            }
        }
        return 0;
    }
};
const std::array<int,3> Gua::qian={0,0,0};   //乾
const std::array<int,3>  Gua::dui={1,0,0};    //兑
const std::array<int,3>  Gua::li={0,1,0};     //离
const std::array<int,3>  Gua::zhen={1,1,0};   //震
const std::array<int,3>  Gua::xun={0,0,1};    //巽
const std::array<int,3>  Gua::kan={1,0,1};    //坎
const std::array<int,3>  Gua::gen={0,1,1};    //艮
const std::array<int,3>  Gua::kun={1,1,1};    //坤

class ChongGua{
    private:
        std::array<int,6> chong_gua{};
        std::array<int,3> shang_hu_gua{};
        std::array<int,3> xia_hu_gua{};
        std::array<int,6> bian_in_chong_gua{};
        std::array<int,3> bian_gua{};
        std::array<int,3> ti_gua{};
        std::array<int,3> yong_gua{};

        void generate_hu_gua(){
            for(int i=0;i<3;i++){
                shang_hu_gua[i]=chong_gua[i+1];
                xia_hu_gua[i]=chong_gua[i+2];
            }
        };
    public:
        const std::array<int,6>& get_chong_gua() const{
            return chong_gua;
        }
        const std::array<int,3>& get_shang_hu_gua() const{
            return shang_hu_gua;
        }
        const std::array<int,3>& get_xia_hu_gua() const{
            return xia_hu_gua;
        }
        const std::array<int,3>& get_bian_gua() const{
            return bian_gua;
        }
        const std::array<int,3>& get_ti_gua() const{
            return ti_gua;
        }
        const std::array<int,3>& get_yong_gua() const{
            return yong_gua;
        }

        ChongGua(const std::array<int,3>& shang_gua,const std::array<int,3>& xia_gua)
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
            generate_hu_gua();
        }

        bool generate_bian_yong_ti_gua(int dong_yao){
            if(dong_yao>6||dong_yao<1){
                return false;
            }

            bian_in_chong_gua=chong_gua;
            bian_in_chong_gua[6-dong_yao]=1-bian_in_chong_gua[6-dong_yao];

            if(dong_yao<=3){
                for(int i=0;i<3;i++){
                    bian_gua[i]=bian_in_chong_gua[i+3];
                    ti_gua[i]=chong_gua[i];
                    yong_gua[i]=chong_gua[i+3];
                }
            }
            else
            {
                for(int i=0;i<3;i++){
                    bian_gua[i]=bian_in_chong_gua[i];
                    ti_gua[i]=chong_gua[i+3];
                    yong_gua[i]=chong_gua[i];
                }
            }
            return true;
        }
};

void print_dan_gua(
    const std::string& title,
    int number,
    const std::array<std::string,8>& gua_names,
    const std::array<int,3>& gua){
        std::cout << title
                  << ":序号："
                  << number
                  << gua_names.at(number-1)
                  << "";
        
        for (int i=0;i<3;i++){
            std::cout<<gua[i];
        }
        std::cout << std::endl;
    }

int main(){

    const std::array<std::string,8> gua_name = {"乾","兑","离","震","巽","坎","艮","坤",};
    const std::array<std::string,6> yao_name = {"上爻", "五爻", "四爻","三爻", "二爻", "初爻"};
    
    std::cout<<"请输入上卦"<<std::endl;
    int a = 0;
    if (!(std::cin >> a))
    {
        std::cout << "上卦必须输入整数" << std::endl;
        return 1;
    }
    const std::array<int,3>* shang_gua=Gua::get_gua(a);//解决方案是指向std::array类型的指针
    if(shang_gua==nullptr){std::cout<<"上卦输入错误,编号必须为1到8"<<std::endl;return 1;}
    std::cout   <<"上卦"
                <<":"
                <<gua_name.at(a-1)
                <<std::endl;
    for (int i=0;i<3;i++){std::cout<<shang_gua->at(i)<<std::endl;}

    std::cout<<"请输入下卦"<<std::endl;
    int b = 0;

    if (!(std::cin >> b))
    {
        std::cout << "下卦必须输入整数" << std::endl;
        return 1;
    }
    const std::array<int,3>* xia_gua=Gua::get_gua(b);
    if(xia_gua==nullptr){std::cout<<"下卦输入错误,编号必须为1到8"<<std::endl;return 1;}
    std::cout   <<"下卦"
                <<":"
                <<gua_name.at(b-1)
                <<std::endl;
    for (int i=0;i<3;i++){std::cout<<xia_gua->at(i)<<std::endl;}

    ChongGua chonggua(*shang_gua,*xia_gua);

    std::cout<<"以下是生成的重卦"<<std::endl;

    for (int i = 0; i <6; i++)
    {
        std::cout<<yao_name.at(i)<<":"<<chonggua.get_chong_gua().at(i)<<std::endl;
    }

    int c=Gua::get_gua_number(chonggua.get_shang_hu_gua());//上互卦的序号
    int d=Gua::get_gua_number(chonggua.get_xia_hu_gua());//下互卦的序号

    if(c==0||d==0){
        std::cout<<"互卦识别失败"<<std::endl;
        return 1;
    }

    print_dan_gua(
        "上互卦",c,gua_name,chonggua.get_shang_hu_gua()
    );

    print_dan_gua(
    "下互卦", d, gua_name, chonggua.get_xia_hu_gua()
    );

    std::cout<<"请输入动爻。初爻是1,上爻是6"<<std::endl;
    int dongyao = 0;
    std::cout<<"动爻：";
    if(!(std::cin>>dongyao)){//如果数据流读取失败
        std::cout<<"动爻必须输入整数"<<std::endl;
        return 1;
    }
    bool success = chonggua.generate_bian_yong_ti_gua(dongyao);
    if(success==false){
        std::cout<<"动爻输入错误,编号必须为1到6"<<std::endl;
        return 1;
    }
    int e=Gua::get_gua_number(chonggua.get_ti_gua());//体卦的序号
    int f=Gua::get_gua_number(chonggua.get_yong_gua());//用卦的序号
    int g=Gua::get_gua_number(chonggua.get_bian_gua());//变卦的序号

    if(e==0||f==0||g==0){
        std::cout<<"体卦,用卦,变卦识别失败。"<<std::endl;
        return 1;
    }

    print_dan_gua(
        "体卦", e, gua_name, chonggua.get_ti_gua()
    );

    print_dan_gua(
        "用卦", f, gua_name, chonggua.get_yong_gua()
    );

    print_dan_gua(
        "变卦", g, gua_name, chonggua.get_bian_gua()
    );

    return 0;
}