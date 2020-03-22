#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
using namespace std;

// car1num car2num car3num 分别为车身数字
void task_allocation(int car1num,int car2num,int car3num);
int* numberControl();

//全局变量
int uav_targetCar[4];
int uav_isChange[4];

int main()
{
    int *carNumber;
    int i,j;
    while (1)
    {
        // printf("\x1b[;%dmhello world\n\x1b[0m", 31);
        carNumber = numberControl();
        task_allocation(carNumber[0],carNumber[1],carNumber[2]);

        cout << "car:" << carNumber[0] << "    " << carNumber[1] << "    " << carNumber[2] << endl;

        for ( i = 0; i < 4; i++)
        {
            for ( j = 0; j < uav_targetCar[i]*5-1; j++)
            {
                cout << " ";
            }
            // cout << "#";
            if(uav_isChange[i] == 1)
                printf("\x1b[;%dm#\x1b[0m", 31);
            else 
                cout << "#";
            cout << endl;
        }
        cout << "-----------------------" << endl ;

        sleep(5);
    }
    
    // task_allocation(0,1,3);
    // sleep(1);
    // task_allocation(2,0,2);
    // sleep(1);
    // task_allocation(2,0,2);  
    // sleep(1); 
    // task_allocation(2,0,2);  
    // sleep(1); 
    // task_allocation(2,0,2);
    // sleep(1);
    // task_allocation(2,2,1);

    
    return 0 ; 
}


int* numberControl()
{

    static int number[12][3] = {0,1,3,
                                0,2,2,
                                0,3,1,
                                1,0,3,
                                1,1,2,
                                1,2,1,
                                1,3,0,
                                2,0,2,
                                2,1,1,
                                2,2,0,
                                3,0,1,
                                3,1,0};
    //生成随机数 0~11
    srand((unsigned)time(NULL)); 
    int randnumber = rand()%(11-0+1);
    static int carShowNumber[3];
    // cout << "number : " << number[randnumber][0] << "  " << number[randnumber][1] << "  " << number[randnumber][2] << endl;
    carShowNumber[0] = number[randnumber][0];
    carShowNumber[1] = number[randnumber][1];
    carShowNumber[2] = number[randnumber][2];

    return carShowNumber;
}


void task_allocation(int car1num,int car2num,int car3num)
{
    /* 无人机目标追踪车辆 
     * uav_target[i] 对应的值 表示 要追踪的车辆编号
     * 1 -- 去追踪车辆1 
     * 2 -- 去追踪车辆2 
     * 3 -- 去追踪车辆3
     * uav_target[0] -- uav1
     * uav_target[1] -- uav2
     * uav_target[2] -- uav3 
     * 
     * 例： uav_target[0] = 3  无人机1将要去跟踪车辆3
    */
    int uav_target[4] = {1,2,3,3};

    /* 无人机当前追踪车辆 
     * uav_now[i] 对应的值 表示 正在追踪的车辆编号
     * 1 -- 正在追踪车辆1 
     * 2 -- 正在追踪车辆2 
     * 3 -- 正在追踪车辆3
     * uav_now[0] -- uav1
     * uav_now[1] -- uav2
     * uav_now[2] -- uav3 
     * 
     * 例： uav_now[0] = 3  无人机1正在跟踪车辆3
    */
    static int uav_now[4] = {1,2,3,3};  // 运行过程中只能初始化一次

    int uav_change[4] = {0,0,0,0}; 

    int uav[4]={1,2,3,4}; //存储无人机编号
    // int uav_2[4]={1,2,3,4};
    int uav_distance[4];//存储无人机与车辆的逻辑距离
    // int uav_distance2[4];
    int minDistance =0; //

    int car[3]={1,2,3}; //存储车辆编号
    int carnum[3];      //存储车身数字

    carnum[0] = car1num;
    carnum[1] = car2num;
    carnum[2] = car3num;  

    int temp;   //排序算法临时变量

    // 按照从大到小排序 保持车辆编号与车身数字保持一致
    for(int i=0;i<2;i++)
    {
        for (int j = 2; j > i; j--)
        {
            if(carnum[j]>carnum[j-1])
            {
                temp = carnum[j];
                carnum[j] = carnum[j-1];
                carnum[j-1] = temp;

                temp = car[j];
                car[j] = car[j-1];
                car[j-1] = temp;             
            }
        }   
    }  
    // cout << "car : " << car[0] << " " << car[1] << " " << car[2] << endl;
    //包含数字3 
    if (carnum[0] == 3)
    {

        uav_distance[0] = abs(uav_now[0] - car[0]);
        uav_distance[1] = abs(uav_now[1] - car[0]);
        uav_distance[2] = abs(uav_now[2] - car[0]);
        uav_distance[3] = abs(uav_now[3] - car[0]);

            for(int i=0;i<3;i++)
            {
                for (int j = 3; j > i; j--)
                {
                    if(uav_distance[j]< uav_distance[j-1])
                    {
                        temp = uav_distance[j];
                        uav_distance[j] = uav_distance[j-1];
                        uav_distance[j-1] = temp;

                        temp = uav[j];
                        uav[j] = uav[j-1];
                        uav[j-1] = temp;             
                    }
                    /* 添加对车身数字为1的车辆的距离判断 */
                }   
            }

        //car[0]为车身数字为3的车辆编号
        uav_target[uav[0]-1] = car[0];
        uav_target[uav[1]-1] = car[0];
        uav_target[uav[2]-1] = car[0];
        //car[1]为车身数字为1的车辆编号
        uav_target[uav[3]-1] = car[1];

    }
    else if (carnum[0] == 2)
    {
        if (carnum[1] == 2)// 含有两个数字2 一个数字0
        { 
            //三个无人机总体上距离 carnum[0] 号车辆较近
            if(  (abs(uav_now[0]-car[0]) + abs(uav_now[1]-car[0]) + abs(uav_now[2]-car[0]) + abs(uav_now[3]-car[0]))
               <= (abs(uav_now[0]-car[1]) + abs(uav_now[1]-car[1]) + abs(uav_now[2]-car[1]) + abs(uav_now[3]-car[1])))
                minDistance = 0;
            else    //三个无人机总体上距离 carnum[1] 号车辆较近
                minDistance = 1;
         
            uav_distance[0] = abs(uav_now[0] - car[minDistance]);
            uav_distance[1] = abs(uav_now[1] - car[minDistance]);
            uav_distance[2] = abs(uav_now[2] - car[minDistance]);
            uav_distance[3] = abs(uav_now[3] - car[minDistance]);
            // cout << "minDistance: " << minDistance << endl;
            //按照距离远近排序
            for(int i=0;i<3;i++)
            {
                for (int j = 3; j > i; j--)
                {
                    if(uav_distance[j]< uav_distance[j-1])
                    {
                        temp = uav_distance[j];
                        uav_distance[j] = uav_distance[j-1];
                        uav_distance[j-1] = temp;

                        temp = uav[j];
                        uav[j] = uav[j-1];
                        uav[j-1] = temp;             
                    }
                }   
            }
            // cout << "uav : " << uav[0] << " " << uav[1] << " " << uav[2] << endl;
            // 无人机编号 1 2 3 
            // 对应数组   0 1 2    因此需要减1
            uav_target[uav[0]-1] = car[minDistance];
            uav_target[uav[1]-1] = car[minDistance];
            if(minDistance == 1)
            {
                uav_target[uav[2]-1] = car[0];
                uav_target[uav[3]-1] = car[0];
            }   
            else 
            {
                uav_target[uav[2]-1] = car[1];
                uav_target[uav[3]-1] = car[1];
            }                   
        }
        else    // 含有一个数字2 两个数字1
        {
            // 寻找重复跟踪的无人机
            int k =0;
            for (int i = 0; i < 4; i++)
            {
                uav_target[i] = uav_now[i];
            }
            
            for(int i=0;i<3;i++)
            {
                for (int j = i+1; j < 4; j++)
                {
                    if(uav_target[i] == uav_target[j])
                    {
                        uav_target[i] = car[0];
                        k++;
                        break;
                    }
                } 
                if(k==2)
                    break;  
            }            



    //         uav_distance[0] = abs(uav_now[0] - car[0]);
    //         uav_distance[1] = abs(uav_now[1] - car[0]);
    //         uav_distance[2] = abs(uav_now[2] - car[0]);
    //         uav_distance[3] = abs(uav_now[3] - car[0]);
    //         //按照距离远近排序
    //         for(int i=0;i<3;i++)
    //         {
    //             for (int j = 3; j > i; j--)
    //             {
    //                 if(uav_distance[j]< uav_distance[j-1])
    //                 {
    //                     temp = uav_distance[j];
    //                     uav_distance[j] = uav_distance[j-1];
    //                     uav_distance[j-1] = temp;

    //                     temp = uav[j];
    //                     uav[j] = uav[j-1];
    //                     uav[j-1] = temp;             
    //                 }
    //             }   
    //         }
    //         //距离车身数字为2的车辆 最近的两架无人机去追踪
    //         uav_target[uav[0]-1] = car[0];
    //         uav_target[uav[1]-1] = car[0];

    //         if(carnum[uav_now[uav[2]-1]-1] != 0)//此架无人机不需要换位置
    //         {
    //             if ((uav_now[uav[2]-1] - car[1]) <= (uav_now[uav[2]-1] - car[2]))
    //             {
    //                 uav_target[uav[2]-1] = car[1]; 
    //                 uav_target[uav[3]-1] = car[2];  
    //             }    
    //             else
    //             {
    //                 uav_target[uav[2]-1] = car[2];
    //                 uav_target[uav[3]-1] = car[1]; 
    //             }    
    //         }
    //         else
    //         {
    //             if ((uav_now[uav[3]-1] - car[1]) <= (uav_now[uav[3]-1] - car[2]))
    //             {
    //                 uav_target[uav[3]-1] = car[1]; 
    //                 uav_target[uav[2]-1] = car[2];  
    //             }    
    //             else
    //             {
    //                 uav_target[uav[3]-1] = car[2];
    //                 uav_target[uav[2]-1] = car[1]; 
    //             }    
    //         }            
        }
    }
    
    //判断 每架无人机 是否需要 变换跟踪车辆
    if(uav_now[0] != uav_target[0])
        uav_change[0] = 1;
    if(uav_now[1] != uav_target[1])
        uav_change[1] = 1;
    if(uav_now[2] != uav_target[2])
        uav_change[2] = 1;
    if(uav_now[3] != uav_target[3])
        uav_change[3] = 1;
    //更新
    uav_now[0] = uav_target[0];
    uav_now[1] = uav_target[1];        
    uav_now[2] = uav_target[2];    
    uav_now[3] = uav_target[3]; 

    // cout << "uav1 uav2 uav3 uav4 : " << uav_target[0] << " " << uav_target[1] << " " << uav_target[2] << " " << uav_target[3] << endl;
    // cout << "uav_change : " << uav_change[0] << " " << uav_change[1] << " " << uav_change[2] << endl;

    uav_targetCar[0] = uav_target[0];
    uav_targetCar[1] = uav_target[1];        
    uav_targetCar[2] = uav_target[2];  
    uav_targetCar[3] = uav_target[3];

    uav_isChange[0] = uav_change[0];
    uav_isChange[1] = uav_change[1];        
    uav_isChange[2] = uav_change[2]; 
    uav_isChange[3] = uav_change[3];
}
