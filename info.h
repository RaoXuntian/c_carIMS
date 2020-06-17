#ifndef _info_h
#define _info_h

typedef struct OrderInfo    // 租车订单信息
{
    char id[20];            // 订单编号
    char clientName[20];    // 客户姓名
    char clientId[20];      // 客户身份证号码
    char clientPhone[20];	// 客户电话号码
    int carNo;          	// 租用的车辆编号
    char startTime[18];     // 取车时间
    float deposit;          // 押金
    char planEndTime[18];   // 预约还车时间
    char actualEndTime[18]; // 实际还车时间
    float planCost;         // 应缴费用
    float actualCost;       // 实缴费用
    struct OrderInfo* next; // 下一个订单
} order;

typedef struct CarInfo  		// 车辆基本信息
{
    int carNo;          		// 车辆编号
    char carId[3];         		// 车辆类型编码   '1'-'5'
    char license[10];   		// 车牌号 
    char carName[20];   		// 车辆名称
    char carWay[10];    		// 排档方式
    float price;        		// 每日租金
    char status[3];        		// 出租状态 'y' 'n'
    order* firstOrder;  		// 此车辆的订单追踪
    struct CarInfo* next;		// 指向同类型的下一个车辆
} car;

typedef struct CarType  		// 车辆分类信息
{
    char carId[3];         		// 车辆类型编码   '1'-'5'
    char type[20];      		// 车辆类型名称
    int count;          		// 库存数量
    struct CarType* next;      	// 下一个车辆分类信息
    car* sub;           		// 此类型车辆的具体车辆信息
} cartype;

// 暂时储存的车辆收入榜前十名
// 结构体
typedef struct query_most_income 
{
    char plate[20];         // 车牌号
    char name[20];          // 车辆名称
    float days;             // 出租天数
    float total_income;     // 总营业额
    float rate;             // 租用率
    int valid;
} query_most_income_t;

//全局变量
cartype* p_type_head;
query_most_income_t ans[10];

#define MAX_LINE_SIZE 256   			//fgets函数读取的最大字节数
#define CARTYPESIZE sizeof(cartype)		//计算车辆分类信息结构体的大小
#define CARINFOSIZE sizeof(car)			//计算车辆基本信息结构体的大小
#define ORDERINFOSIZE sizeof(order)		//计算租车订单信息结构体的大小

void menu();				// 菜单
void clean();				// 清屏
void show_menu(int option);	// 子菜单
int Scanf();				// 选择按钮
void Error(int i);			// 错误返回
void check();				// 查看整个head链表信息
void press_enter();         // 输入回车键

//数据载入保存
cartype* load_data();       // 从dat文件中加载数据
void save_data();           // 将修改后的数据保存至dat

//数据导入导出
void read_from_csv();       // 恢复备份-从源数据csv文件导入数据到系统
int read_car_type_csv();    // 车辆类型信息csv
int read_car_info_csv();    // 车辆基本信息csv
int read_order_info_csv();  // 租车订单信息csv
void write_to_csv();        // 数据备份-将数据查询的结果导出到csv文件
void write_head_to_csv(int select, char* filePath);         // 数据表头信息写入csv
void write_a_cartype_to_csv(char* filePath, cartype* p);    // 将cartype指针p的内容写入csv
void write_a_car_to_csv(char* filePath, car* p);            // 将car指针p的内容写入csv
void write_a_car_to_csv2(char* filePath, car* p, char* type);// 将car指针p的内容写入csv
void write_cars_to_csv2(char* filePath, car* p);            // 将car链表p的所有内容写入csv
void write_a_order_to_csv(char* filePath, order* p);        // 将order指针p的内容写入csv
void write_orders_to_csv(char* filePath, order* p);         // 将order链表p的所有内容写入csv

//数据维护
void car_type_mantain(int option);				// 车辆类型信息维护
void car_info_mantain(int option);				// 车辆基本信息维护
void order_info_mantain(int option);			// 租车订单信息维护

void add_car_type();		// 增加车辆分类信息
void change_car_type();		// 修改车辆分类信息
void del_car_type();		// 删除车辆分类信息

void add_car_info();		// 增加车辆基本信息
void change_car_info();		// 修改车辆基本信息
void del_car_info();		// 删除车辆基本信息

void add_order_info();		// 增加租车订单信息
void change_order_info();	// 修改租车订单信息
void del_order_info();		// 删除租车订单信息

//打印数据
void show_car_type(cartype* p);  // 打印单条车辆类型信息
void show_car_info(car* p);      // 打印单条车辆基本信息
void show_order_info(order* p);  // 打印单条租车订单信息
void list_car_type(cartype* p);  // 打印相关的所有车辆类型信息
void list_car_info(car* p);      // 打印相关的车辆基本信息
void list_order_info(order* p);  // 打印相关的租车订单信息

//数据查询
cartype* find_car_type(cartype* head, char* id);	// 寻找车辆分类信息
car* find_car_info(cartype* head, int no);			// 寻找车辆基本信息
order* find_order_info(cartype* head);				// 寻找租车订单信息
char* get_car_type(car* p);						    // 获取一个car结构体的车辆类型名称

void query_model(int option);					// 查询子模块
car* query_car_type();							// 查询车辆分类信息
car* query_car_info(int option);				// 查询车辆基本信息
order* query_order_info(int option);			// 查询租车订单信息
cartype* query_car_type_again();				// 再次次查询车辆分类信息
car* query_car_info_again(int option, car* p);	// 再次次查询车辆基本信息
order* query_order_info_again();				// 再次查询租车订单信息

car* car_copy_value(car* p);        // 复制car* p的值
order* order_copy_value(order* p);  // 复制order* p的值
char* rtrim(char* str);             // 去除尾部空白字符 包括\t \n \r  
char* ltrim(char* str);             // 去除首部空格
char* trim(char* str);              // 去除首尾空格
int pipei(char* a, char* b);		// 查询匹配板块 1 - a in b; 0 - a not in b; 
void Next(char* T,int* next);		// KMP算法辅助函数
int KMP(char* S, char* T);			// KMP算法 检查字符串S是否包含Tca
int time_cmp(char* time1, char* time2, char* time3);    // time3是否介于time1和time2之间

// 数据统计
void tongji(int option);
int tm_to_timestamp(char* a);           // 订单时间格式转换->秒
int my_time_diff(char* a, char* b);     // 返回两个时间的差（Duration)
int ans_lt(query_most_income_t *a, query_most_income_t *b); // 定义偏序小于关系
void sort_income_result();              // 冒泡排序
int insert_index(float tot_days);       // 得到插入挤占的位置

void Exit();				// 释放指针 退出程序


#endif