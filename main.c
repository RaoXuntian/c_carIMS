#include <stdio.h>
#include "info.h"
#include <memory.h> // for memset
#include <stdlib.h> // for malloc, free
#include <string.h> // for strtok
#include <ctype.h>

void menu()     // 菜单
{
    p_type_head = load_data();
    clean();
    printf("\n*****欢迎来到hust汽车租赁管理系统*****\n");
    printf("\n\n          按任意键进入系统...\n\n");
    getchar();
    show_menu(0);
}

void show_menu(int option)  // 子菜单
{
    int select;
    switch(option) {
    case 0:
        // 主菜单
        //p_type_head = load_data();    // 加载数据
        clean();
        printf("主菜单:\n");
        printf("1.数据管理\n");
        printf("2.数据维护\n");
        printf("3.数据查询\n");
        printf("4.数据统计\n");
        printf("5.退出系统\n");
        
        select = Scanf();

        if (select == 1) show_menu(1);
        if (select == 2) show_menu(2);
        if (select == 3) show_menu(3);
        if (select == 4) show_menu(4);
        if (select == 5) show_menu(5);
        break;
    case 1:
        // 数据管理
        clean();
        printf("数据管理列表:\n");
        printf("1.数据保存\n");
        printf("2.数据加载\n");
        printf("3.数据备份\n");
        printf("4.恢复备份\n");
        printf("5.返回主菜单\n");

        select = Scanf();

        if (select == 1) save_data();
        if (select == 2) p_type_head = load_data();
        if (select == 3) write_to_csv();
        if (select == 4) read_from_csv();
        if (select == 5) show_menu(0);
        press_enter();
        show_menu(1);
        break;
    case 2:
        // 数据维护
        clean();
        printf("数据维护列表:\n");
        printf("1.车辆分类信息维护\n");
        printf("2.车辆基本信息维护\n");
        printf("3.租车订单信息维护\n");
        printf("4.返回主菜单\n");

        select = Scanf();

        if (select == 1) car_type_mantain(0);
        if (select == 2) car_info_mantain(0);
        if (select == 3) order_info_mantain(0);
        if (select == 4) show_menu(0);
        press_enter();
        show_menu(2);
        break;
    case 3:
        // 数据查询
        clean();
        printf("数据查询列表:\n");
        printf("1.车辆分类信息查询\n");
        printf("2.车辆基本信息查询\n");
        printf("3.租车订单信息查询\n");
        printf("4.返回主菜单\n");

        select = Scanf();

        if (select == 1) query_model(1);
        if (select == 2) query_model(2);
        if (select == 3) query_model(3);
        if (select == 4) show_menu(0);
        press_enter();
        show_menu(3);
        break;
    case 4:
        // 数据统计
        clean();
        printf("数据统计列表:\n");  
        check();
        printf("1.车辆类型统计\n");
        printf("2.车辆类型营业额统计\n");
        printf("3.每辆车营业额统计\n");
        printf("4.出租天数最多十辆车统计\n");
        printf("5.返回主菜单\n");

        select = Scanf();
        if (select == 1) tongji(1);
        if (select == 2) tongji(2);
        if (select == 3) tongji(3);
        if (select == 4) tongji(4);
        if (select == 5) show_menu(0);
        press_enter();
        show_menu(4);
        break;
    case 5:
        // 数据管理
        clean();
        printf("确认退出系统？\n");
        printf("1.确认\n");
        printf("2.取消\n");
        select = Scanf();
        if (select == 1) Exit();
        if (select == 2) show_menu(0);
        break;
    }
}

void check() 
{
    // 注意p_type_head是否分配地址
    cartype* p = p_type_head->next;
    if (p == NULL) printf("[没有数据...]\n");//Error();
    while (p != NULL) 
    {
        show_car_type(p);
        car* p1 = p->sub;
        while (p1 != NULL) 
        {
            show_car_info(p1);
            order* p2 = p1->firstOrder;
            list_order_info(p2);
            p1 = p1->next;
        }
        p = p->next;
    }
}

void clean()
{
    system("clear");
    printf("***********汽车租赁管理系统***********\n\n");
}

int Scanf()
{
    int option;
    printf("请选择: ");
    scanf("%d", &option);
    setbuf(stdin, NULL);//使stdin输入流由默认缓冲区转为无缓冲区  
    return option;
}

void press_enter()  // 输入回车键
{
    setbuf(stdin, NULL);//使stdin输入流由默认缓冲区转为无缓冲区  
    printf("[按回车键继续...]\n" );
    getchar();
}

void Error(int i)
{
    printf("[操作失败...]");
    getchar();
    show_menu(i);
}

void Exit()         // 退出系统
{
    save_data();
    //free(p_type_head);          // 很复杂 需要逐个遍历
    // 释放所有指针
    cartype* p = p_type_head->next;
    while (p != NULL) 
    {
        car* p1 = p->sub;
        while (p1 != NULL) 
        {
            order* p2 = p1->firstOrder;
            while (p2 != NULL)
            {
                order* _p2 = p2;
                p2 = p2->next;
                free(_p2);
            }
            car* _p1 = p1;
            p1 = p1->next;
            free(_p1);
        }
        cartype* _p = p;
        p = p->next;
        free(p);
    }
    free(p_type_head);
    printf("[系统退出...]\n" );
    exit(0);
}

// 从txt文件中加载数据 返回链表的head节点
cartype* load_data()           
{
    FILE *fp1, *fp2, *fp3;
    int flag = 0;
    // 打开文件
    if ((fp1=fopen("./data/carTypeInfo.txt", "rb")) == NULL) {
        printf("[打开文件./data/carTypeInfo.dat失败...]\n");
        //Error(0);
        flag = 1;
    }
    if ((fp2=fopen("./data/carInfo.txt", "rb")) == NULL) {
        printf("[打开文件./data/carInfo.dat失败...]\n");
        //Error(0);
        flag = 1;
    }
    if ((fp3=fopen("./data/orderInfo.txt", "rb")) == NULL) {
        printf("[打开文件./data/orderInfo.dat失败...]\n");
        //Error(0);
        flag = 1;
    }
    // 开始加载数据

    // 先加载cartype的链表
    cartype* p, *p1, *head;
    p = head = (cartype*) malloc (CARTYPESIZE);
    if (flag) return p;
    head->sub = NULL;
    p1 = (cartype*) malloc (CARTYPESIZE);
    while ((fread(p1, CARTYPESIZE, 1, fp1)) == 1) 
    {
        p1->sub = NULL;
        p->next = p1;
        p = p1;
        p1 = (cartype*) malloc (CARTYPESIZE);
    }
    p->next = NULL;
    fclose(fp1);


    // 然后插入car的链表
    while (1) {
        car *p2;
        p2 = (car*) malloc (CARINFOSIZE);
        if ((fread(p2, CARINFOSIZE, 1, fp2)) != 1) break;
        p2->firstOrder = NULL;
        p2->next = NULL;
        //show_car_info(p2);
        cartype* p_type = find_car_type(head, p2->carId);
        if (p_type == NULL) 
        {
            show_car_info(p2);
            printf("[此条车辆基本信息添加失败...]\n");
        }
        else // 头插法
        {
            if (p_type->sub == NULL) 
            {
                p_type->sub = p2;
            }
            else 
            {
                p2->next = p_type->sub;
                p_type->sub = p2;
            }
        }
    }


    // 最后插入order的链表
    while (1) {
        order* p3;
        p3 = (order*) malloc (ORDERINFOSIZE);
        if ((fread(p3, ORDERINFOSIZE, 1, fp3)) != 1) break;
        p3->next = NULL;

        car* p_car = find_car_info(head, p3->carNo);
        if (p_car == NULL) 
        {
            show_order_info(p3);
            printf("[此条租车订单信息添加失败...]\n");
        }
        else // 尾插法
        {
            if (p_car->firstOrder == NULL) 
            {
                p_car->firstOrder = p3;
            }
            else 
            {
                order* p_order = p_car->firstOrder;
                while(p_order->next != NULL) 
                {
                    p_order = p_order->next;
                }
                p_order->next = p3;
            }
        }
    }

    printf("[从.txt文件加载数据完成...]\n");
    return head;
}

// 将修改后的数据保存至txt
void save_data()            
{
    FILE *fp1, *fp2, *fp3;
    if ((fp1=fopen("./data/carTypeInfo.txt", "wb")) == NULL) {
        printf("[打开文件./data/carTypeInfo.txt失败...]\n");
        Error(0);
    }
    if ((fp2=fopen("./data/carInfo.txt", "wb")) == NULL) {
        printf("[打开文件./data/carInfo.txt失败...]\n");
        Error(0);
    }
    if ((fp3=fopen("./data/orderInfo.txt", "wb")) == NULL) {
        printf("[打开文件./data/orderInfo.txt失败...]\n");
        Error(0);
    }

    printf("[数据保存ing...]\n");
    cartype* p = p_type_head->next;
    while (p) 
    {
        fwrite(p, CARTYPESIZE, 1, fp1);
        car* p1 = p->sub;
        while (p1) 
        {
            fwrite(p1, CARINFOSIZE, 1, fp2);
            order* p2 = p1->firstOrder;
            while (p2) 
            {
                fwrite(p2, ORDERINFOSIZE, 1, fp3);
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        p = p->next;
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    printf("[相关数据保存成功...]\n");
}

// 获取车辆类型信息csv
int read_car_type_csv()     
{
    cartype* p;
    p = p_type_head = (cartype*) malloc (CARTYPESIZE);
    FILE *fp;
    char *filePath = "./output/车辆分类信息表_.csv";
    fp = fopen(filePath, "r");
    if (!fp) 
    {
        fprintf(stderr, "[打开文件『 %s 』失败...]\n", filePath);
        return -1;
    }
    printf("[正在读取%s...]\n", filePath);
    char buff[MAX_LINE_SIZE];
    char *info;
    char *tok;
    info = fgets(buff, MAX_LINE_SIZE, fp);   // 读取csv内容到buff
    //printf("%s\n", buff);
    tok = strtok(buff, ",");
    tok = strtok(NULL, ",");
    tok = strtok(NULL, ",");
    while ((info = fgets(buff, MAX_LINE_SIZE, fp)) != NULL) 
    {
        //printf("%s\n", buff);
        cartype* cur;
        cur = (cartype*) malloc (CARTYPESIZE+1);
        tok = strtok(buff, ",");
        if (tok == NULL) break;
        strcpy(cur->carId, tok);
        tok = strtok(NULL, ",");
        if (tok == NULL) break;
        strcpy(cur->type, tok);
        tok = strtok(NULL, ",");
        if (tok == NULL) break;
        cur->count = atoi(tok);
        cur->sub = NULL;
        p->next = cur;
        p = cur;
        show_car_type(cur);
        //printf("%s | %s | %d\n", cur->carId, cur->type, cur->count);
    }
    p->next = NULL;
    p->sub = NULL;
    printf("[%s读取成功...]\n", filePath);
    fclose(fp);
    return 1;
}

// 获取车辆基本信息csv
int read_car_info_csv()     
{
    FILE *fp;
    char *filePath = "./output/车辆基本信息表_.csv";
    fp = fopen(filePath, "r");
    if (!fp) 
    {
        fprintf(stderr, "[打开文件『 %s 』失败...]\n", filePath);
        return -1;
    }
    printf("[正在读取%s...]\n", filePath);
    char buff[10*MAX_LINE_SIZE];
    char *info;
    char *tok;
    info = fgets(buff, MAX_LINE_SIZE, (FILE*)fp);   // 读取csv内容到buff
    tok = strtok(buff, ",");
    tok = strtok(NULL, ",");
    tok = strtok(NULL, ",");
    tok = strtok(NULL, ",");
    tok = strtok(NULL, ",");
    tok = strtok(NULL, ",");
    tok = strtok(NULL, ",");
    while ((info = fgets(buff, MAX_LINE_SIZE, (FILE*)fp)) != NULL) 
    {
        car* cur;
        cur = (car*) malloc (CARINFOSIZE);
        tok = strtok(buff, ",");    // 车辆编号
        if (tok == NULL) break;
        cur->carNo = atoi(tok);
        tok = strtok(NULL, ",");    // 车牌号
        if (tok == NULL) break;
        strcpy(cur->license, tok);
        tok = strtok(NULL, ",");    // 车辆类型编号
        if (tok == NULL) break;
        strcpy(cur->carId, tok);
        tok = strtok(NULL, ",");    // 车辆名称
        if (tok == NULL) break;
        strcpy(cur->carName, tok);
        tok = strtok(NULL, ",");    // 排挡方式
        if (tok == NULL) break;
        strcpy(cur->carWay, tok);
        tok = strtok(NULL, ",");    // 每日租金
        if (tok == NULL) break;
        cur->price = atof(tok);
        tok = strtok(NULL, ",");    // 出租状态
        if (tok == NULL) break;
        strcpy(cur->status, tok);
        
        cur->firstOrder = NULL;
        show_car_info(cur);
        //printf("%d | %s | %s | %s | %s | %f | %s\n", cur->carNo, cur->license, cur->carId, cur->carName, cur->carWay, cur->price, cur->status);

        cartype* p_type = find_car_type(p_type_head, cur->carId);
        if (p_type == NULL) 
        {
            printf("Error!\n");
        }
        else // 头插法
        {
            if (p_type->sub == NULL) 
            {
                p_type->sub = cur;
            }
            else 
            {
                cur->next = p_type->sub;
                p_type->sub = cur;
            }
        }
    } 
    
    printf("[%s读取成功...]\n", filePath);
    fclose(fp);
    return 1;
}

// 获取租车订单信息csv
int read_order_info_csv()   
{

    FILE *fp;
    char *filePath = "./output/租车订单信息表_.csv";
    fp = fopen(filePath, "r");
    if (!fp) 
    {
        fprintf(stderr, "[打开文件『 %s 』失败...]\n", filePath);
        return -1;
    }
    printf("[正在读取%s...]\n", filePath);
    char buff[10*MAX_LINE_SIZE];
    char *info;
    char *tok;
    info = fgets(buff, 10*MAX_LINE_SIZE, (FILE*)fp);   // 读取csv内容到buff
    tok = strtok(buff, ",");
    for (int i = 0; i < 10; ++i) {
        tok = strtok(NULL, ",");
    }
    while ((info = fgets(buff, 10*MAX_LINE_SIZE, (FILE*)fp)) != NULL) 
    {
        order* cur;
        cur = (order*) malloc (ORDERINFOSIZE);
        tok = strtok(buff, ",");    // 订单编号
        if (tok == NULL) break;
        strcpy(cur->id, tok);
        tok = strtok(NULL, ",");    // 身份证号
        if (tok == NULL) break;
        strcpy(cur->clientId, tok);
        tok = strtok(NULL, ",");    // 客人姓名
        if (tok == NULL) break;
        strcpy(cur->clientName, tok);
        tok = strtok(NULL, ",");    // 客人电话号码
        if (tok == NULL) break;
        strcpy(cur->clientPhone, tok);
        tok = strtok(NULL, ",");    // 租用车辆编号
        if (tok == NULL) break;
        cur->carNo = atoi(tok);
        tok = strtok(NULL, ",");    // 取车时间
        if (tok == NULL) break;
        strcpy(cur->startTime, tok);
        tok = strtok(NULL, ",");    // 预约还车时间
        if (tok == NULL) break;
        strcpy(cur->planEndTime, tok);
        tok = strtok(NULL, ",");    // 押金
        if (tok == NULL) break;
        cur->deposit = atof(tok);
        tok = strtok(NULL, ",");    // 实际还车时间
        if (tok == NULL) break;
        strcpy(cur->actualEndTime, tok);
        tok = strtok(NULL, ",");    // 应缴费用
        if (tok == NULL) break;
        cur->planCost = atof(tok);
        tok = strtok(NULL, ",");    // 实缴费用
        if (tok == NULL) break;
        cur->actualCost = atof(tok);
        cur->next = NULL;
        show_order_info(cur);
        //printf("%s | %s | %s | %s | %s | %s | %s | %f | %s | %f | %f\n", 
        //    cur->id, cur->clientId, cur->clientName, cur->clientPhone, cur->carNo, cur->startTime, cur->planEndTime, cur->deposit, cur->actualEndTime, cur->planCost, cur->actualCost);
        

        car* p_car = find_car_info(p_type_head, cur->carNo);
        if (p_car == NULL) 
        {
            printf("Error!\n");
        }
        else // 头插法
        {
            if (p_car->firstOrder == NULL) 
            {
                p_car->firstOrder = cur;
            }
            else 
            {
                order* p_order = p_car->firstOrder;
                while(p_order->next != NULL) 
                {
                    p_order = p_order->next;
                }
                p_order->next = cur;
            }
        }
    }
    printf("[%s读取成功...]\n", filePath);
    fclose(fp);
    return 1;
}

// 从3个csv文件中读取数据组成链表
void read_from_csv()
{
    int f1 = read_car_type_csv();
    if (f1 == 1) {
        int f2 = read_car_info_csv();
        if (f2 == 1) {
            int f3 = read_order_info_csv();
            if (f3 == 1) {
                printf("[数据恢复备份成功...!]\n");
            }
            else {
                printf("[数据恢复备份失败...]\n");
                printf("[请检查备份文件...]\n" );
            }
        }
        else {
            printf("[数据恢复备份失败...]\n");
            printf("[请检查备份文件...]\n" );
        }
    }
    else {
        printf("[数据恢复备份失败...]\n");
        printf("[请检查备份文件...]\n" );
    }
    //check();
    //save_data();
}

// 将cartype指针p的内容写入csv
void write_a_cartype_to_csv(char* filePath, cartype* p)
{   
    FILE* fp = fopen(filePath, "a+");
    if (!fp) 
    {
        printf("[文件打开失败...]\n");
        return ;
    }
    if (p != NULL)
    {
        fputs(p->carId, fp);
        fputs(",", fp);
        fputs(p->type, fp);
        fputs(",", fp);
        char c1[10];
        sprintf(c1, "%d", p->count);
        fputs(c1, fp);
        fputs(",\n", fp);
    }
    fclose(fp);
}

// 将car指针p的内容写入csv
void write_a_car_to_csv(char* filePath, car* p)
{
    FILE* fp = fopen(filePath, "a+");
    if (!fp) 
    {
        printf("[文件打开失败...]\n");
        return ;
    }
    if (p != NULL)
    {
        //char *c1 = itoa(p->carNo, c1, 10);
        char c1[10];
        sprintf(c1, "%d", p->carNo);
        fputs(c1, fp);
        fputs(",", fp);
        fputs(p->license, fp);
        fputs(",", fp);
        fputs(p->carId, fp);
        fputs(",", fp);
        fputs(p->carName, fp);
        fputs(",", fp);
        fputs(p->carWay, fp);
        fputs(",", fp);
        char c2[10];
        sprintf(c2, "%f", p->price);
        fputs(c2, fp);
        fputs(",", fp);
        fputs(p->status, fp);
        fputs(",\n", fp);
    }
    fclose(fp);
}

// 将car指针p的内容写入csv（查询信息格式）
void write_a_car_to_csv2(char* filePath, car* p, char* type)
{
    FILE* fp = fopen(filePath, "a+");
    if (!fp) 
    {
        printf("[文件打开失败...]\n");
        return ;
    }
    if (p != NULL)
    {
        char c1[10];
        sprintf(c1, "%d", p->carNo);
        fputs(c1, fp);
        fputs(",", fp);
        fputs(p->license, fp);
        fputs(",", fp);
        fputs(type, fp);
        fputs(",", fp);
        fputs(p->carName, fp);
        fputs(",", fp);
        fputs(p->carWay, fp);
        fputs(",", fp);
        fputs(p->status, fp);
        fputs(",\n", fp);
    }
    fclose(fp);
}

// 将order指针p的内容写入csv
void write_a_order_to_csv(char* filePath, order* p)
{
    FILE* fp = fopen(filePath, "a+");
    if (!fp) 
    {
        printf("[文件打开失败...]\n");
        return ;
    }
    if (p != NULL)
    {
        //char *c1 = itoa(p->carNo, c1, 10);
        fputs(p->id, fp);
        fputs(",", fp);
        fputs(p->clientId, fp);
        fputs(",", fp);
        fputs(p->clientName, fp);
        fputs(",", fp);
        fputs(p->clientPhone, fp);
        fputs(",", fp);
        char c1[10];
        sprintf(c1, "%d", p->carNo);
        fputs(c1, fp);
        fputs(",", fp);
        fputs(p->startTime, fp);
        fputs(",", fp);
        fputs(p->planEndTime, fp);
        fputs(",", fp);
        char c2[10];
        sprintf(c2, "%f", p->deposit);
        fputs(c2, fp);
        fputs(",", fp);
        fputs(p->actualEndTime, fp);
        fputs(",", fp);
        char c3[10];
        sprintf(c3, "%f", p->planCost);
        fputs(c3, fp);
        fputs(",", fp);
        char c4[10];
        sprintf(c4, "%f", p->actualCost);
        fputs(c4, fp);
        fputs(",\n", fp);
    }
    fclose(fp);
}

// 将order链表p的所有内容写入csv
void write_orders_to_csv(char* filePath, order* p)
{
    //write_head_to_csv(3, filePath);
    while(p != NULL)
    {
        write_a_order_to_csv(filePath, p);
        p = p->next;
    }
    printf("[查询结果已保存至%s中...]\n", filePath);
}

// 将car链表p的所有内容写入csv
void write_cars_to_csv2(char* filePath, car* p)
{
    // 将查询结果写进文件
    car* pc = p;
    while(pc != NULL)
    {
        char* type = get_car_type(pc);
        write_a_car_to_csv2(filePath, pc, type);
        pc = pc->next;
    }
}

// 数据表头信息写入csv
void write_head_to_csv(int select, char* filePath)  // 数据表头信息写入csv
{
    FILE* fp = fopen(filePath, "w");
    if (!fp) 
    {
        printf("[文件打开失败...]\n");
        return ;
    }
    char head[MAX_LINE_SIZE];
    if (select == 1)        // 车辆信息分类表
    {
        strcpy(head, "车辆类型编码,车辆类型名称,库存数量,\n");
    }
    if (select == 2)        // 车辆基本信息表
    {
        strcpy(head, "车辆编号,车牌号,车辆类型编码,车辆名称,排挡方式,每日租金（元）,出租状态,\n");
    }
    if (select == 3)        // 租车订单信息表
    {
        strcpy(head, "订单编号,身份证号,客人姓名,手机号码,租用车辆编号,取车时间,预约还车时间,押金（元）,实际还车时间,应缴费用（元）,实缴费用（元）,\n");
    }
    if (select == 4)        // 车辆基本信息查询
    {
        strcpy(head, "序号,车牌号,车辆类型,车辆名称,排挡方式,出租状态,\n");
    }
    fputs(head, fp);
    fclose(fp);
}

// 数据备份-将数据查询的结果导出到csv文件
void write_to_csv()
{
    char* filePath1 = "./output/车辆分类信息表_.csv";
    char* filePath2 = "./output/车辆基本信息表_.csv";
    char* filePath3 = "./output/租车订单信息表_.csv";
    write_head_to_csv(1, filePath1);
    write_head_to_csv(2, filePath2);
    write_head_to_csv(3, filePath3);

    //FILE* fp1 = fopen(filePath1, "a+");
    //FILE* fp2 = fopen(filePath2, "a+");
    //FILE* fp3 = fopen(filePath3, "a+");

    cartype* p = p_type_head->next;
    if (p == NULL) printf("[没有数据...]\n");//Error();
    while (p != NULL) 
    {
        write_a_cartype_to_csv(filePath1, p);
        car* p1 = p->sub;
        while (p1 != NULL) 
        {
            write_a_car_to_csv(filePath2, p1);
            order* p2 = p1->firstOrder;
            while (p2 != NULL)
            {
                write_a_order_to_csv(filePath3, p2);
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        p = p->next;
    }
    //fclose(fp1);
    //fclose(fp2);
    //fclose(fp3);
    printf("[数据备份完成...]\n");
}

// 车辆类型信息维护
void car_type_mantain(int option)     
{
    int select;
    switch(option) {
    case 0:
        clean();
        printf("车辆分类信息维护\n");
        printf("1.添加车辆类型信息\n");
        printf("2.修改车辆类型信息\n");
        printf("3.删除车辆类型信息\n");
        printf("4.返回\n");
        select = Scanf();
        if (select == 1) car_type_mantain(1);
        if (select == 2) car_type_mantain(2);
        if (select == 3) car_type_mantain(3);
        if (select == 4) car_type_mantain(4);
        break;
    case 1:
        // 添加车辆类型信息
        add_car_type();
        break;
    case 2:
        // 修改车辆类型信息
        change_car_type();
        break;
    case 3:
        // 删除车辆类型信息
        del_car_type();
        break;
    case 4:
        show_menu(2);
        break;
    }
}

// 增加车辆类型信息
void add_car_type()                    
{
    clean();
    // 定义新的cartype
    list_car_type(p_type_head->next);
    cartype* new_p;
    new_p = (cartype*) malloc (CARTYPESIZE);
    printf("添加车辆类型信息\n");
    printf("车辆类型编号:");      scanf("%s", new_p->carId);  printf("\n");
    printf("车辆类型名称:");      scanf("%s", new_p->type);   printf("\n");
    printf("车辆类型库存数量:");   scanf("%d", &new_p->count);  printf("\n");
    new_p->next = NULL;
    new_p->sub = NULL;

    int tmp;
    printf("是否确定添加:\n");
    show_car_type(new_p);
    printf("1.确定\n");
    printf("2.我再想想吧...\n");
    tmp = Scanf();
    if (tmp == 2) return ;
    // 插入
    cartype *p = p_type_head;
    while (p->next != NULL) {   // 尾插法
        if (atoi(p->carId) == atoi(new_p->carId))  // 检测车辆类型编号是否冲突
        {
            printf("[warning:车辆类型编号发生冲突!!!]\n");
            Error(2);
        }
        p = p->next;
    }
    p->next = new_p;
    printf("[插入成功...]\n");
    printf("当前车辆类型信息如下:\n");
    list_car_type(p_type_head->next);
    press_enter();
    car_type_mantain(0);
}

// 修改车辆类型信息
void change_car_type()                  
{   
    clean();
    printf("修改车辆类型信息\n");
    list_car_type(p_type_head->next);
    int id;
    printf("\n请选择您要修改的车辆类型编号:\n");
    scanf("%d", &id);
    cartype* p = p_type_head->next;
    while (p != NULL)
    {
        if ( atoi(p->carId) == id ) break;
        p = p->next;
    }
    if (p == NULL) 
    {
        printf("[没有查到您输入的车辆类型编号...]\n"); // 递归
        press_enter();
        car_type_mantain(0);
    }
    int select = 4;
    while(select != 5)
    {
        clean();
        printf("请选择要修改的车辆类型信息:\n");
        show_car_type(p);
        printf("1.车辆类型编号\n");
        printf("2.车辆类型名称\n");
        printf("3.车辆类型剩余数量\n");
        printf("4.不修改了...\n");
        
        select = Scanf();
        if (select == 4) return ;  // 有bug
        if (select == 1) 
        {
            //printf("请输入车辆类型编号:");
            //scanf("%s", p->carId);
            printf("[暂不支持修改车辆类型编号...]\n");
        }
        if (select == 2) 
        {
            printf("请输入车辆类型名称:");
            char ch[20];
            scanf("%s", ch);
            strcpy(p->type, ch);
        }
        if (select == 3) 
        {
            printf("请输入车辆类型剩余数量:");
            int i;
            scanf("%d", &i);
            p->count = i;
        }
        int tmp;
        printf("是否继续修改？\n");
        printf("1.是\n");
        printf("2.否\n");
        tmp = Scanf();
        if (tmp == 2) 
        {
            select = 5;
            printf("修改完成:\n");
            show_car_type(p);
            press_enter();
        }
    }
    car_type_mantain(0);
}

// 删除车辆类型信息
void del_car_type()                     
{
    clean();
    printf("删除车辆类型信息\n");
    list_car_type(p_type_head->next);
    printf("请输入要删除的车辆类型编号:");
    int id;
    scanf("%d", &id);
    setbuf(stdin, NULL);
    
    char mima[10];
    printf("请输入管理员密码执行删除操作...\n密码: ");  // 考虑网络安全 增加密码保护数据
    /*  
    // 输入时不显示密码，但不能回删
    // 需要conio.h Linux上无
    int i;
    for (int i=0; i<10; ++i) {
        mima[i] = getch();
        if (mima[i] == 'r')
            break;
        printf("*");
    }
    mima[i] = '\0';
    */
    scanf("%s", mima);
    getchar();          // 接收回车的
    if (strcmp(mima, "saye.") != 0) {
        printf("[warning: 密码输入错误!!!]\n");
        Error(2);
    }
    
    cartype* pre = p_type_head;
    cartype* p = pre->next;
    while (p != NULL)
    {
        if ( atoi(p->carId) == id ) break;
        pre = p;
        p = p->next;
    }
    if (p == NULL)  // 没有找到要删除的节点
    {
        printf("[没找到要删除的车辆类型...]\n");
        Error(2);
    }
    else
    {
        pre->next = p->next;
        printf("[删除成功...]\n");
        list_car_type(p_type_head->next);
        press_enter();
    }
    car_type_mantain(0);
}

// 车辆基本信息维护
void car_info_mantain(int option)     
{
    int select;
    switch(option) {
    case 0:
        clean();
        printf("车辆基本信息维护\n");
        printf("1.添加车辆基本信息\n");
        printf("2.修改车辆基本信息\n");
        printf("3.删除车辆基本信息\n");
        printf("4.返回\n");
        select = Scanf();
        if (select == 1) car_info_mantain(1);
        if (select == 2) car_info_mantain(2);
        if (select == 3) car_info_mantain(3);
        if (select == 4) car_info_mantain(4);
        break;
    case 1:
        add_car_info();
        break;
    case 2:
        change_car_info();
        break;
    case 3:
        del_car_info();
        break;
    case 4:
        show_menu(2);
        break;
    }
}

// 增加车辆基本信息
void add_car_info()         
{
    clean();
    printf("添加车辆基本信息\n");
    // 定义新的car类
    car* new_p;
    new_p = (car*) malloc (CARINFOSIZE);
    printf("添加车辆基本信息\n");
    printf("车辆编号:");              scanf("%d", &new_p->carNo);  printf("\n");
    printf("车辆类型编号:");           scanf("%s", new_p->carId);   printf("\n");
    printf("车牌号:");                scanf("%s", new_p->license);  printf("\n");
    printf("车辆名称:");              scanf("%s", new_p->carName);  printf("\n");
    printf("车辆排挡方式:");           scanf("%s", new_p->carWay);   printf("\n");
    printf("每日租金:");              scanf("%f", &new_p->price);  printf("\n");
    printf("车辆出租状态(y/n):");      scanf("%s", new_p->status);   printf("\n");
    new_p->next = NULL;
    new_p->firstOrder = NULL;

    int tmp;
    printf("是否确定添加:\n");
    show_car_info(new_p);
    printf("1.确定\n");
    printf("2.我再想想吧...\n");
    tmp = Scanf();
    if (tmp == 2) return ;
    
    // 寻找车辆类型cartype父节点pre
    // 检测车辆编号是否冲突
    cartype* p = p_type_head->next;
    while (p != NULL) 
    {
        car* p1 = p->sub;
        while (p1 != NULL)
        {
            if (p1->carNo == new_p->carNo) // carNo发生冲突
            {
                printf("[warning:车辆编号发生冲突!!!]\n");
                Error(2);
            }
            p1 = p1->next;
        }
        p = p->next;
    }

    // 插入new_p
    cartype* pre = find_car_type(p_type_head, new_p->carId);
    if (pre == NULL) 
    {
        show_car_info(new_p);
        printf("[warning:数据库里没有车辆类型编号为%s的车辆类型...]\n", new_p->carId);
        printf("[请先添加相关车辆类型信息...]\n");
        Error(2);
    }
    else // 头插法
    {
        if (pre->sub == NULL) 
        {
            pre->sub = new_p;
        }
        else 
        {
            new_p->next = pre->sub;
            pre->sub = new_p;
        }
    }

    printf("[插入成功...]\n");
    printf("[此类型车辆信息如下...]\n");
    list_car_info(pre->sub);
    press_enter();
    car_info_mantain(0);
}

// 修改车辆基本信息
void change_car_info()      
{
    clean();
    printf("修改车辆基本信息\n");
    // 展示现有车辆基本信息
    cartype* pp = p_type_head->next;
    while (pp != NULL) 
    {
        list_car_info(pp->sub);   
        pp = pp->next;
    }
    int no;
    printf("\n请选择您要修改的车辆编号:\n");
    scanf("%d", &no);
    // 查找是否有这个车
    cartype* p1 = p_type_head->next;
    car* p = NULL;
    while (p1 != NULL)
    {
        int flag = 0;
        car* p2 = p1->sub;
        while (p2 != NULL) 
        {
            if (p2->carNo == no)
            {
                p = p2;
                flag = 1;
                break;
            }
            p2 = p2->next;
        }
        if (flag) break;
        p1 = p1->next;
    }
    if (p == NULL) 
    {
        printf("[没有查到您输入的车辆编号...]\n"); // 递归
        press_enter();
        car_info_mantain(0);
    }

    int select = 8;
    while(select != 9)
    {
        clean();
        printf("请选择要修改的车辆类型信息:\n");
        show_car_info(p);
        printf("1.车辆编号:\t%d\n", p->carNo);
        printf("2.车辆类型编号:\t%s\n", p->carId);
        printf("3.车牌号:\t%s\n", p->license);
        printf("4.车辆名称:\t%s\n", p->carName);
        printf("5.排档方式:\t%s\n", p->carWay);
        printf("6.每日租金:\t%f\n", p->price);
        printf("7.出租状态(y/n):\t%s\n", p->status);
        printf("8.不修改了...\n");
        
        select = Scanf();
        if (select == 8) return ;
        if (select == 1) 
        {
            printf("[暂不支持修改车辆编号...]\n");
        }
        if (select == 2) 
        {
            printf("[暂不支持修改车辆类型编号...]\n");
        }
        if (select == 3) 
        {
            printf("请输入车牌号:");
            scanf("%s", p->license);
        }
        if (select == 4) 
        {
            printf("请输入车辆名称:");
            scanf("%s", p->carName);
        }
        if (select == 5) 
        {
            printf("请输入车辆排档方式:");
            scanf("%s", p->carWay);
        }
        if (select == 6) 
        {
            printf("请输入车辆每日租金:");
            scanf("%f", &p->price);
        }
        if (select == 7) 
        {
            printf("请输入车辆出租状态(y/n):");
            scanf("%s", p->status);
        }
        int tmp;
        printf("是否继续修改？\n");
        printf("1.是\n");
        printf("2.否\n");
        tmp = Scanf();
        if (tmp == 2) 
        {
            select = 9;
            printf("修改完成:\n");
            show_car_info(p);
            press_enter();
        }
    }

    car_info_mantain(0);
}

// 删除车辆基本信息
void del_car_info()         
{
    clean();
    printf("删除车辆基本信息\n");
    // 展示现有车辆基本信息
    cartype* pp = p_type_head->next;
    while (pp != NULL) 
    {
        list_car_info(pp->sub);   
        pp = pp->next;
    }
    printf("请输入要删除的车辆编号:");
    int no;
    scanf("%d", &no);
    
    char mima[10];
    printf("请输入管理员密码执行删除操作...\n密码: ");  // 考虑网络安全 增加密码保护数据
    scanf("%s", mima);
    getchar();          // 接收回车的
    if (strcmp(mima, "saye.") != 0) {
        printf("[warning: 密码输入错误!!!]\n");
        Error(2);
    }

    car* p = find_car_info(p_type_head, no);    // 先寻找对应的car类型
    if (p == NULL)
    {
        printf("[没有找到要删除的车辆信息...]\n");
        Error(2);
    }
    cartype *pre = find_car_type(p_type_head, p->carId);    // 再寻找cartype类型
    car* p1 = pre->sub;
    int flag = 0;
    while (p1 != NULL )
    {
        if (p1->next == p) 
        {
            p1->next = p1->next->next;
            flag = 1;
        }
        p1 = p1->next;
    }
    if (flag)
    {
        printf("[删除成功...]\n");
        list_car_info(pre->sub);
        
    }
    else 
    {
        printf("[删除失败...]\n");
        printf("[错误原因未知...]\n");
    }
    press_enter();
    car_info_mantain(0);
}

// 租车订单信息维护
void order_info_mantain(int option)   
{
    int select;
    switch(option) {
    case 0:
        clean();
        printf("租车订单信息维护\n");
        printf("1.添加租车订单信息\n");
        printf("2.修改租车订单信息\n");
        printf("3.删除租车订单信息\n");
        printf("4.返回\n");
        select = Scanf();
        if (select == 1) order_info_mantain(1);
        if (select == 2) order_info_mantain(2);
        if (select == 3) order_info_mantain(3);
        if (select == 4) order_info_mantain(4);
        break;
    case 1:
        add_order_info();
        break;
    case 2:
        change_order_info();
        break;
    case 3:
        del_order_info();
        break;
    case 4:
        show_menu(2);
        break;
    }
}

// 增加租车订单信息
void add_order_info()       
{
    clean();
    printf("增加租车订单信息\n");
    // 定义新的car类
    order* new_p;
    new_p = (order*) malloc (ORDERINFOSIZE);
    printf("添加订单基本信息\n");
    printf("订单编号(格式:2020061501):");           scanf("%s", new_p->id);  printf("\n");
    printf("客户姓名:");           scanf("%s", new_p->clientName);   printf("\n");
    printf("客户身份证号码:");      scanf("%s", new_p->clientId);  printf("\n");
    printf("客户电话号码:");        scanf("%s", new_p->clientPhone);  printf("\n");
    printf("租用车辆编号:");        scanf("%d", &new_p->carNo);   printf("\n");
    printf("取车时间(格式:2019/02/15-13:00):");           scanf("%s", new_p->startTime);  printf("\n");
    printf("预约还车时间(格式:2019/02/15-13:00):");        scanf("%s", new_p->planEndTime);   printf("\n");
    printf("实际还车时间(格式:2019/02/15-13:00):");        scanf("%s", new_p->actualEndTime);   printf("\n");
    printf("应缴费用:");           scanf("%f", &new_p->planCost);  printf("\n");
    printf("实缴费用:");           scanf("%f", &new_p->actualCost);   printf("\n");
    printf("押金:");              scanf("%f", &new_p->deposit);  printf("\n");
    new_p->next = NULL;

    int tmp;
    printf("是否确定添加:\n");
    show_order_info(new_p);
    printf("1.确定\n");
    printf("2.我再想想吧...\n");
    tmp = Scanf();
    if (tmp == 2) return ;
    
    // 寻找车辆car父节点pre
    // 检测订单编号是否冲突
    cartype* p = p_type_head->next;
    while (p != NULL) 
    {
        car* p1 = p->sub;
        while (p1 != NULL)
        {
            order* p2 = p1->firstOrder;
            while (p2 != NULL)
            {
                if (atol(p2->id) == atol(new_p->id) )     // 订单编号发生冲突
                {
                    printf("[warning:订单编号发生冲突!!!]\n");
                    Error(2);
                }
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        p = p->next;
    }

    car* pre = find_car_info(p_type_head, new_p->carNo);
    if (pre == NULL) 
    {
        show_order_info(new_p);
        printf("[warning:数据库里没有车辆编号为%d的车辆信息...]\n", new_p->carNo);
        printf("[请先添加相关车辆基本信息...]\n");
        Error(2);
    }
    else // 尾插法
    {
        if (pre->firstOrder == NULL) 
        {
            pre->firstOrder = new_p;
        }
        else 
        {
            order* p_order = pre->firstOrder;
            while(p_order->next != NULL) 
            {
                p_order = p_order->next;
            }
            p_order->next = new_p;
        }
    }

    printf("[插入成功...]\n");
    printf("[同类型车辆租车订单信息如下...]\n");
    list_order_info(pre->firstOrder);
    press_enter();
    order_info_mantain(0);
}

// 修改租车订单信息
void change_order_info()    
{
    clean();
    printf("修改租车订单信息\n");
    list_order_info(find_order_info(p_type_head));  // 展现现有的订单
    long unsigned id;
    printf("\n请选择您要修改的订单编号:\n");
    scanf("%lu", &id);
    // 查找是否有这个订单
    cartype* p1 = p_type_head->next;
    order* p = NULL;
    while (p1 != NULL)
    {
        int flag = 0;
        car* p2 = p1->sub;
        while (p2 != NULL) 
        {
            order* p3 = p2->firstOrder;
            while(p3 != NULL)
            {
                if (atol(p3->id) == id)   // 找到了
                {   
                    p = p3;
                    flag = 1;
                    break;
                }
                p3 = p3->next;
            }
            if (flag) break;
            p2 = p2->next;
        }
        if (flag) break;
        p1 = p1->next;
    }
    if (p == NULL) 
    {
        printf("[没有查到您输入的订单编号...]\n"); // 递归
        press_enter();
        order_info_mantain(0);  // 返回上一级
    }

    int select = 12;
    while(select != 13)
    {
        clean();
        printf("请选择要修改的车辆类型信息:\n");
        show_order_info(p);
        printf("1.订单编号(格式:2020061501):\t%s\n", p->id);
        printf("2.客户姓名:\t%s\n", p->clientName);
        printf("3.客户身份证号码:\t%s\n", p->clientId);
        printf("4.客户电话号码:\t%s\n", p->clientPhone);
        printf("5.租用车辆编号:\t%d\n", p->carNo);
        printf("6.取车时间(格式:2019/02/15-13:00):\t%s\n", p->startTime);
        printf("7.押金:\t%f\n", p->deposit);
        printf("8.预约还车时间(格式:2019/02/15-13:00):\t%s\n", p->planEndTime);
        printf("9.实际还车时间(格式:2019/02/15-13:00):\t%s\n", p->actualEndTime);
        printf("10.应缴费用:\t%f\n", p->planCost);
        printf("11.实缴费用:\t%f\n", p->actualCost);
        printf("12.不修改了...\n");
        
        select = Scanf();
        if (select == 12) return ;
        if (select == 1) 
        {
            printf("请输入订单编号:\n");   // 暂不考虑订单编号冲突的情况!!!
            scanf("%s", p->id);
        }
        if (select == 2) 
        {
            printf("请输入客户姓名:");
            scanf("%s", p->clientName);
        }
        if (select == 3) 
        {
            printf("请输入客户身份证号:");
            scanf("%s", p->clientId);
        }
        if (select == 4) 
        {
            printf("请输入客户电话号码:");
            scanf("%s", p->clientPhone);
        }
        if (select == 5) 
        {
            printf("[暂不支持修改租用车辆编号...]\n");
        }
        if (select == 6) 
        {
            printf("请输入取车时间:");
            scanf("%s", p->startTime);
        }
        if (select == 7) 
        {
            printf("请输入押金:");
            scanf("%f", &p->deposit);
        }
        if (select == 8) 
        {
            printf("请输入预约还车时间:");
            scanf("%s", p->planEndTime);
        }
        if (select == 9) 
        {
            printf("请输入实际还车时间:");
            scanf("%s", p->actualEndTime);
        }
        if (select == 10) 
        {
            printf("请输入应缴费用:");
            scanf("%f", &p->planCost);
        }
        if (select == 11) 
        {
            printf("请输入实缴费用:");
            scanf("%f", &p->actualCost);
        }
        int tmp;
        printf("是否继续修改？\n");
        printf("1.是\n");
        printf("2.否\n");
        tmp = Scanf();
        if (tmp == 2) 
        {
            select = 13;
            printf("修改完成:\n");
            show_order_info(p);
            press_enter();
        }
    }


    order_info_mantain(0);
}

// 删除租车订单信息
void del_order_info()       
{
    clean();
    printf("删除租车订单信息\n");
    list_order_info(find_order_info(p_type_head));  // 展现现有的订单
    printf("请输入要删除的订单编号:");
    long id;
    scanf("%lu", &id);
    //getchar();
    
    char mima[10];
    printf("请输入管理员密码执行删除操作...\n密码: ");  // 考虑网络安全 增加密码保护数据
    scanf("%s", mima);
    getchar();          // 接收回车的
    if (strcmp(mima, "saye.") != 0) {
        printf("[warning: 密码输入错误!!!]\n");
        Error(2);
    }

    int flag = 0;       //  0-未发现节点 1-发现节点 2-成功删除节点
    cartype* p1 = p_type_head->next;
    while (p1 != NULL)
    {
        car* p2 = p1->sub;
        while(p2 != NULL)
        {
            order* p3 = p2->firstOrder;
            while(p3 != NULL) 
            {
                if (atol(p3->id) == id)   // 找到了
                {
                    flag = 1;
                    break;
                }
                p3 = p3->next;
            }
            if (flag)   // 删除
            {
                order* pre = p2->firstOrder;
                if (pre == p3) 
                {
                    p2->firstOrder = pre->next;
                    flag = 2;
                    printf("[删除成功...]\n");
                    list_order_info(p2->firstOrder);
                }
                else
                {
                    while(pre != NULL)
                    {
                        if (pre->next == p3)
                        {
                            pre->next = pre->next->next;
                            flag = 2;
                            printf("[删除成功...]\n");
                            list_order_info(p2->firstOrder);
                            break;
                        }
                        pre = pre->next;
                    } 
                }
                break;
            }
            p2 = p2->next;
        }
        if (flag)
        {
            break;
        }
        p1 = p1->next;
    }

    if (flag == 0) 
    {
        printf("[没有找到要删除的订单信息...]\n");
        Error(2);
    }
    else if (flag ==1)
    {
        printf("[删除失败...]\n");
        printf("[错误原因未知...]\n");
        Error(2);
    }
    press_enter();
    order_info_mantain(0);
}

// 查询子模块
void query_model(int option)        
{
    int select;
    switch(option) {
    case 1:
        clean();
        printf("车辆分类信息查询\n");
        list_car_type(p_type_head->next);
        car* p1 = query_car_type();
        list_car_info(p1);
        char* filePath0 = "./output/车辆基本信息查询结果.csv";
        write_head_to_csv(4, filePath0);
        write_cars_to_csv2(filePath0, p1);
        int select = -1;
        while (select != 2)
        {
            printf("是否继续查询？\n");
            printf("1.继续查询\n");
            printf("2.结束查询\n");
            select = Scanf();
            if (select == 1) 
            {
                car* p1 = query_car_type();
                list_car_info(p1);
                write_cars_to_csv2(filePath0, p1);
            }
        }
        
        
        break;
    case 2:
        clean();
        printf("车辆基本信息查询\n");
        printf("请选择查询条件:\n");
        printf("1.车牌号码\n");
        printf("2.车辆名称\n");
        printf("3.出租状态\n");
        printf("4.不查询了...\n");
        
        select = Scanf();
        car* p;     // 查询结果链表表头
        if (select >= 1 && select <= 3)
        {
            p = query_car_info(select);
            if (p != NULL)
            {
                list_car_info(p);
            }
            else 
            {
                printf("[没查到相关信息...]\n");
                printf("[退出查询板块...]\n");
                select = 4;
            }
            
        }
        while (select >= 1 && select <= 3)
        {
            int tmp = -1;
            printf("是否继续查找?\n");
            printf("1.继续添加查找条件\n");
            printf("2.结束查询并将查询结果导出到csv文件\n");
            tmp = Scanf();
            if (tmp == 2) 
            {
                select = 4;
                // 将查询结果写进文件
                char* filePath = "./output/车辆基本信息查询结果.csv";
                write_head_to_csv(4, filePath);
                write_cars_to_csv2(filePath, p);
                printf("[查询结果写入%s文件...]\n", filePath);
            }
            else 
            {
                clean();
                printf("车辆基本信息查询\n");
                list_car_info(p);
                printf("请继续选择查询条件:\n");
                printf("1.车牌号码\n");
                printf("2.车辆名称\n");
                printf("3.出租状态\n");
                printf("4.不查询了...\n");
                
                select = Scanf();
                if (select == 4) break;
                p = query_car_info_again(select, p);
                if (p != NULL)
                {
                    list_car_info(p);
                }
                else 
                {
                    printf("[没查到相关信息...]\n");
                    printf("[退出查询板块...]\n");
                    select = 4;
                }
            }
        }
        if (select == 4)
        {
            press_enter();
            show_menu(3);
        }
        
        list_car_info(p);

        break;
    case 3:
        clean();
        printf("租车订单信息查询\n");   // 不支持多条件查找
        printf("请选择查询条件:\n");
        printf("1.客人信息\n");
        printf("2.车辆信息\n");
        printf("3.租车时间范围\n");
        printf("4.不查询了...\n");
        
        select = Scanf();
        char* filePath = "./output/租车订单信息查询结果.csv";
        write_head_to_csv(3, filePath); // 写csv的表头
        order* pp = (order*) malloc (ORDERINFOSIZE);
        pp->next = NULL;
        if (select == 1)
        {
            int tmp;
            printf("1.客人身份证号码\n");
            printf("2.客人手机号码\n");
            printf("3.不查询了...\n");
            tmp = Scanf();
            if (tmp == 1)
            {
                pp->next = query_order_info(1);        // 根据客人身份证号码查询
            }
            else if (tmp == 2)
            {
                pp->next = query_order_info(2);        // 根据客人手机号码查询
            }
            else 
            {
                printf("[返回上层菜单...]\n");
                query_model(3);
            }
        }
        if (select == 2)
        {
            int tmp;
            printf("1.车辆号码\n");
            printf("2.车辆名称\n");
            printf("3.不查询了...\n");
            tmp = Scanf();
            if (tmp == 1)
            {
                pp->next = query_order_info(3);        // 根据车牌号码查询
            }
            else if (tmp == 2)
            {
                pp->next = query_order_info(4);        // 根据车辆名称查询
            }
            else 
            {
                printf("[返回上层菜单...]\n");
                query_model(3);
            }
        }
        if (select == 3)
        {
            pp->next = query_order_info(5);            // 根据租车时间查询
        }
        list_order_info(pp->next);
        // 查询结果写进csv
        if (pp->next != NULL)
        {
            write_orders_to_csv(filePath, pp->next);
        }
        printf("[查询结束...]\n");
        break;
    }
}

// 查询车辆分类信息  模糊查询 综合查询
car* query_car_type()                     
{
    //printf("车辆分类信息查询\n");
    printf("请输入要查询的车辆类型:");
    char target[20];
    scanf("%s", target);
    car* res = (car*) malloc (CARINFOSIZE);
    res->next = NULL;
    res->firstOrder = NULL;
    car* pre = res;
    cartype* p = p_type_head->next;
    while(p != NULL)
    {
        char* type = p->type;
        if (pipei(type, target) != -1)
        {
            car* p1 = p->sub;
            //list_car_info(p1);
            
            while(p1 != NULL)
            {
                car* tmp = car_copy_value(p1);
                pre->next = tmp;
                pre = tmp;
                tmp = pre->next;
                p1 = p1->next;
            }
        }
        p = p->next;
    }
    return res->next;
}

// 查询车辆基本信息
car* query_car_info(int option)                         
{
    //从p_type_head开始按条件查找
    car* p;
    p = (car*) malloc (CARINFOSIZE);
    p->firstOrder = NULL;
    p->next = NULL;
    car* p0 = p;
    if (option == 1)    // 车牌号码
    {
        char target[10];
        printf("请输入要查询的车牌号码:\n");
        scanf("%s", target);

        cartype* p1 = p_type_head->next;
        while (p1 != NULL)
        {
            car* p2 = p1->sub;
            while (p2 != NULL)
            {
                char *s = p2->license;
                if (pipei(s, target) != -1)     // p2是我们要查询的值
                {
                    car *find_p = car_copy_value(p2);
                    p0->next = find_p;
                    p0 = find_p;
                    find_p = p0->next;
                }
                p2 = p2->next;
            }
            p1 = p1->next;
        }

    }
    if (option == 2)    // 车辆名称
    {
        char target[20];
        printf("请输入要查询的车牌名称:\n");
        scanf("%s", target);

        cartype* p1 = p_type_head->next;
        while (p1 != NULL)
        {
            car* p2 = p1->sub;
            while (p2 != NULL)
            {
                char *s = p2->carName;
                if (pipei(s, target) != -1)     // p2是我们要查询的值
                {
                    car *find_p = car_copy_value(p2);
                    p0->next = find_p;
                    p0 = find_p;
                    find_p = p0->next;
                }
                p2 = p2->next;
            }
            p1 = p1->next;
        }
    }
    if (option == 3)    // 出租状态
    {
        char target[3];
        printf("请输入要查询的车牌出租状态:\n");
        scanf("%s", target);

        cartype* p1 = p_type_head->next;
        while (p1 != NULL)
        {
            car* p2 = p1->sub;
            while (p2 != NULL)
            {
                char *s = p2->status;
                if (pipei(s, target) != -1)     // p2是我们要查询的值
                {
                    car *find_p = car_copy_value(p2);
                    p0->next = find_p;
                    p0 = find_p;
                    find_p = p0->next;
                }
                p2 = p2->next;
            }
            p1 = p1->next;
        }
    }

    return p->next;
}

// 再次查询车辆基本信息
car* query_car_info_again(int option, car* p)           
{
    //从p开始按条件查找
    car* pre = (car*) malloc (CARINFOSIZE);
    pre->next = p;
    car* p0 = pre;

    if (option == 1)        // 车牌号
    {
        char target[10];
        printf("请输入要查询的车牌号码:");
        scanf("%s", target);

        while (p != NULL) 
        {
            char* s = p->license;
            if (pipei(s, target) == -1)
            {
                p0->next = p->next;
            }
            else
            {
                p0 = p;
            }
            p = p0->next;
        }
    }
    if (option == 2)        // 车辆名称
    {
        char target[20];
        printf("请输入要查询的车辆名称:");
        scanf("%s", target);

        while (p != NULL) 
        {
            char* s = p->carName;
            if (pipei(s, target) == -1)
            {
                p0->next = p->next;
            }
            else
            {
                p0 = p;
            }
            p = p0->next;
        }
    }
    if (option == 3)        // 出租状态
    {
        char target[3];
        printf("请输入要查询的车辆出租状态:");
        scanf("%s", target);

        while (p != NULL) 
        {
            char* s = p->status;
            if (pipei(s, target) == -1)
            {
                p0->next = p->next;
            }
            else
            {
                p0 = p;
            }
            p = p0->next;
        }
    }

    return pre->next;
}

// 查询租车订单信息
order* query_order_info(int option)                     
{
    //从p_type_head开始按条件查找
    order* p;
    p = (order*) malloc (ORDERINFOSIZE);
    p->next = find_order_info(p_type_head);
    order* pre = p, *cur = p->next;
    list_order_info(cur);
    // 1.2可以直接在链表p上进行核对并进行操作
    if (option == 1)        // 根据客人身份证号码查询
    {
        char target[20];
        printf("请输入要查询的客人身份证号码:\n");
        scanf("%s", target);

        while (cur != NULL)
        {
            char t[20];
            strcpy(t, cur->clientId);
            if (pipei(t, target) == -1)     // cur不符合查询条件
            {
                pre->next = cur->next;
            }
            else
            {
                pre = cur;
            }
            cur = pre->next;
        }
        return p->next;
    }
    if (option == 2)        // 根据客人手机号码查询
    {
        char target[20];
        printf("请输入要查询的客人手机号码:\n");
        scanf("%s", target);

        while (cur != NULL)
        {
            char t[20];
            strcpy(t, cur->clientPhone);
            if (pipei(t, target) == -1)     // cur不符合查询条件
            {
                pre->next = cur->next;
            }
            else
            {
                pre = cur;
            }
            cur = pre->next;
        }
        return p->next;
    }
    // 与1.2方式不一样
    if (option == 3)        // 根据车牌号码查询
    {
        char target[20];
        printf("请输入要查询的车牌号码:\n");
        scanf("%s", target);

        cartype* p1 = p_type_head->next;
        char* filePath = "./output/租车订单信息查询结果.csv";
        while(p1 != NULL)
        {
            car* p2 = p1->sub;
            while (p2 != NULL)
            {
                char s[20];
                strcpy(s, p2->license);
                if (pipei(s, target) != -1)
                {
                    list_order_info(p2->firstOrder);
                    write_orders_to_csv(filePath, p2->firstOrder);
                }
                p2 = p2->next;
            }
            p1 = p1->next;
        }

    }
    if (option == 4)        // 根据车辆名称查询
    {
        char target[20];
        printf("请输入要查询的车辆名称:\n");
        scanf("%s", target);

        cartype* p1 = p_type_head->next;
        char* filePath = "./output/租车订单信息查询结果.csv";
        while(p1 != NULL)
        {
            car* p2 = p1->sub;
            while (p2 != NULL)
            {
                char s[20];
                strcpy(s, p2->license);
                if (pipei(s, target) != -1)
                {
                    list_order_info(p2->firstOrder);
                    write_orders_to_csv(filePath, p2->firstOrder);
                }
                p2 = p2->next;
            }
            p1 = p1->next;
        }

    }
    if (option == 5)        // 根据租车时间查询
    {
        char time1[9], time2[9];
        printf("请输入要查询的租车时间范围:\n");
        printf("起始时间(例如20200202):");
        scanf("%s", time1);
        printf("终止时间(例如20200502):");
        scanf("%s", time2);

        while (cur != NULL)
        {
            char t[20];
            strcpy(t, cur->id);
            if (time_cmp(time1, time2, t) == 0)     // cur不符合查询条件
            {
                pre->next = cur->next;
            }
            else
            {
                pre = cur;
            }
            cur = pre->next;
        }
        return p->next;

    }
    return NULL;
}

// 根据car的caiId寻找cartype父节点
cartype* find_car_type(cartype* head, char* id)     
{
    char a[3];
    strcpy(a, id);
    cartype* p = head->next;
    while (p != NULL)
    {
        char b[3];
        strcpy(b, p->carId);
        if (strcmp(trim(a), trim(b)) == 0) 
        {
            return p;
        }
        p = p->next;
    }
    printf("未查询到车辆类型为%s的汽车\n", id);
    return NULL;
}

// 根据order的carNo寻找car父节点
car* find_car_info(cartype* head, int no)           
{
    cartype* p = head->next;
    while (p != NULL) 
    {
        car* p1 = p->sub;
        while(p1 != NULL)
        {
            if (p1->carNo == no)
            {
                return p1;
            }
            p1 = p1->next;
        }
        p = p->next;
    }
    return NULL;
}

// 寻找租车订单信息,返回所有租车订单的链表
order* find_order_info(cartype* head)               
{
    order* pre = (order*) malloc (ORDERINFOSIZE);
    pre->next = NULL;
    order* p0 = pre;
    cartype* p1 = head->next;
    while (p1 != NULL)
    {
        car* p2 = p1->sub;
        while (p2 != NULL) 
        {
            order* p3 = p2->firstOrder;
            while(p3 != NULL)
            {
                order* tmp = order_copy_value(p3);
                p0->next = tmp;
                p0 = tmp;
                tmp = p0->next;

                p3 = p3->next;
            }
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    return pre->next;
}

// time3是否介于time1和time2之间
int time_cmp(char* time1, char* time2, char* time3)     
{
    int a = atoi(time1), b = atoi(time2);
    char t[9];
    strcpy(t, trim(time3));
    t[8] = '\0';
    int c = atoi(t);
    printf("%d %d %d\n",a, b, c);
    if (a <= c && c <= b)
        return 1;
    return 0;
}

// 复制car* p的值
car* car_copy_value(car* p) 
{
    car* new_p = (car*) malloc (CARINFOSIZE);
    if (p != NULL)
    {
        new_p->carNo = p->carNo;
        strcpy(new_p->carId, p->carId);
        strcpy(new_p->license, p->license);
        strcpy(new_p->carName, p->carName);
        strcpy(new_p->carWay, p->carWay);
        new_p->price = p->price;
        strcpy(new_p->status, p->status);
        new_p->firstOrder = NULL;
        new_p->next = NULL;
    }
    return new_p;
}

// 复制order* p的值
order* order_copy_value(order* p) 
{
    order* new_p = (order*) malloc (ORDERINFOSIZE);
    if (p != NULL)
    {
        new_p->carNo = p->carNo;
        strcpy(new_p->id, p->id);
        strcpy(new_p->clientName, p->clientName);
        strcpy(new_p->clientId, p->clientId);
        strcpy(new_p->clientPhone, p->clientPhone);
        new_p->deposit = p->deposit;
        strcpy(new_p->startTime, p->startTime);
        strcpy(new_p->planEndTime, p->planEndTime);
        strcpy(new_p->actualEndTime, p->actualEndTime);
        new_p->planCost = p->planCost;
        new_p->actualCost = p->actualCost;
        new_p->next = NULL;
    }
    return new_p;
}

// 获取一个car结构体的车辆类型名称
char* get_car_type(car* p)      
{
    if (p == NULL) return NULL;
    char* res = find_car_type(p_type_head, p->carId)->type;
    return res;
}

// 打印单条车辆类型信息
void show_car_type(cartype* p)  
{
    printf("{ %s | %s | %d }\n", 
        p->carId, p->type, p->count);
}

// 打印单条车辆基本信息
void show_car_info(car* p)      
{
    printf("[ %d | %s | %s | %s | %s | %f | %s ]\n", 
        p->carNo, p->license, p->carId, p->carName, p->carWay, p->price, p->status);
}

// 打印单条租车订单信息
void show_order_info(order* p)  
{
    printf("( %s | %s | %s | %s | %d | %s | %s | %f | %s | %f | %f )\n", 
        p->id, p->clientId, p->clientName, p->clientPhone, p->carNo, p->startTime, p->planEndTime, p->deposit, p->actualEndTime, p->planCost, p->actualCost);          
}

// 打印相关的所有车辆类型信息
void list_car_type(cartype* p)  
{
    while (p != NULL) 
    {
        show_car_type(p);
        p = p->next;
    }
}

// 打印相关的车辆基本信息
void list_car_info(car* p)      
{
    while (p != NULL)
    {
        show_car_info(p);
        p = p->next;
    }
}

// 打印相关的租车订单信息
void list_order_info(order* p)  
{
    while (p != NULL)
    {
        show_order_info(p);
        p = p->next;
    }
}

//去除尾部空白字符 包括\t \n \r  
/*
标准的空白字符包括：
' '     (0x20)    space (SPC) 空格符
'\t'    (0x09)    horizontal tab (TAB) 水平制表符    
'\n'    (0x0a)    newline (LF) 换行符
'\v'    (0x0b)    vertical tab (VT) 垂直制表符
'\f'    (0x0c)    feed (FF) 换页符
'\r'    (0x0d)    carriage return (CR) 回车符
//windows \r\n linux \n mac \r
*/
char *rtrim(char *str)
{
    if (str == NULL || *str == '\0')
    {
        return str;
    }

    int len = strlen(str);
    char *p = str + len - 1;
    while (p >= str  && isspace(*p))
    {
        *p = '\0';
        --p;
    }

    return str;
}

//去除首部空格
char *ltrim(char *str)
{
    if (str == NULL || *str == '\0')
    {
        return str;
    }

    int len = 0;
    char *p = str;
    while (*p != '\0' && isspace(*p))
    {
        ++p;
        ++len;
    }

    memmove(str, p, strlen(str) - len + 1);

    return str;
}

//去除首尾空格 改变原指针的
char *trim(char *str)
{
    str = rtrim(str);
    str = ltrim(str);

    return str;
}

// KMP算法的辅助数组
void Next(char *T,int *next)
{
    int i=1;
    next[1]=0;
    int j=0;
    while (i<strlen(T)) {
        if (j==0||T[i-1]==T[j-1]) {
            i++;
            j++;
            next[i]=j;
        }else{
            j=next[j];
        }
    }
}

// 模糊查询的核心算法 KMP算法
int KMP(char *S, char *T)
{
    int next[MAX_LINE_SIZE];
    Next(T,next);//根据模式串T,初始化next数组
    int i=1;
    int j=1;
    while (i<=strlen(S)&&j<=strlen(T)) {
        //j==0:代表模式串的第一个字符就和当前测试的字符不相等；S[i-1]==T[j-1],如果对应位置字符相等，两种情况下，指向当前测试的两个指针下标i和j都向后移
        if (j==0 || S[i-1]==T[j-1]) {
            i++;
            j++;
        }
        else{
            j=next[j];//如果测试的两个字符不相等，i不动，j变为当前测试字符串的next值
        }
    }
    if (j>strlen(T)) {//如果条件为真，说明匹配成功
        return i-(int)strlen(T);
    }
    return -1;
}

// a是否包含b？ 不包含返回-1
int pipei(char *a, char *b)     
{
    a = trim(a);
    b = trim(b);
    return KMP(a, b);
}

int tm_to_timestamp(char* a);
// 返回两个时间的差（Duration)
// return time in seconds
// a, b: '2019/02/15-13:00'
int my_time_diff(char* a, char* b) 
{
    return tm_to_timestamp(a) - tm_to_timestamp(b);
}

// 转换字符串格式的时间到时间戳
// 考虑了闰年等格里高利历历法细节
// (a的格式:2019/02/15-13:00)
int tm_to_timestamp(char* a) 
{
    int day_in_month[] = { 0, 31,28,31,30,31,30,31,31,30,31,30,31 };
    for (size_t mm = 1; mm <= 12; ++mm) {
        day_in_month[mm] += day_in_month[mm - 1];
    }
    int ans = 0;
    int y, m, d, h, min;
    sscanf(a, "%d/%d/%d-%d:%d", &y, &m, &d, &h, &min);
    int day_in_year = day_in_month[m - 1] + d;
    if (y % 4 == 0 && y % 400 != 0) { // leap year
        if (m > 2) {
            day_in_year += 1;
        }
    }
    return ((y - 1970) * 365 + day_in_year) * 86400 + (h * 3600 + min * 60);
}

// 定义偏序小于关系
int ans_lt(query_most_income_t *a, query_most_income_t *b) 
{
    if (a->valid == 0) return 1;
    if (b->valid == 0) return 1;
    return a->days < b->days ? 1 : 0;
}

// 冒泡排序
void sort_income_result() 
{
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 9; j > i; --j) {
            if (ans_lt(&ans[j], &ans[j - 1]) == 1) {
                query_most_income_t tmp = ans[j];
                ans[j] = ans[j - 1];
                ans[j - 1] = tmp;
            }
        }
    }
}

// 得到插入挤占的位置
int insert_index(float tot_days) 
{
    for (size_t i = 0; i < 10; ++i) 
    {
        if (ans[i].valid == 0) 
        {
            return i;
        }
        if (tot_days >= ans[i].days) 
        {
            return i;
        }
    }
    return -1;
}

void tongji(int select)
{
    // 查询所有的类型
    if (select == 1)
    {
        cartype* p = p_type_head->next;
        printf("| 车辆类型\t | 总数\t | 已出租\t | 未出租\t |\n");
        while (p != NULL) {
            car* p1 = p->sub;
            int tot = 0, y = 0, n = 0;
            while (p1 != NULL) {
                if (strcmp(p1->status, "y")) y++;
                else n++;
                tot++;
                p1 = p1->next;
            }
            printf("| %s\t | %d\t | %d\t\t | %d\t\t |\n", p->type, tot, y, n);
            p = p->next;
        }
    }
    // 查询每辆车的营业额
    if (select == 2)
    {
        cartype* p = p_type_head->next;
        printf("| 车辆类型\t | 营业额\t |\n");
        while (p != NULL) {
            car* p1 = p->sub;
            float tot = 0.0;
            while (p1 != NULL) {
                order* p2 = p1->firstOrder;
                while (p2 != NULL) {
                    tot += p2->actualCost;
                    p2 = p2->next;
                }
                p1 = p1->next;
            }
            printf("| %s\t | %f\t |\n", p->type, tot);
            p = p->next;
        }
    }
    // 查询特定年份所有的车的收入
    if (select == 3)
    {
        printf("请输入要查询的年份(2019):");
        int year;
        scanf("%d", &year);
        cartype* p = p_type_head->next;
        printf("| 车牌号\t | 名称  \t | 营业额\t | 租用率\t |\n");
        while (p != NULL) {
            car* p1 = p->sub;
            while (p1 != NULL) {
                order* p2 = p1->firstOrder;
                float tot = 0.0;
                int secs = 0;
                while (p2 != NULL) {
                    long id_order;
                    char id[20];
                    strcpy(id, p2->id);
                    sscanf(id, "%lu", &id_order);
                    //printf("%lu\n", id_order);
                    if (id_order >= year * 1000000 &&
                        id_order <= (year + 1) * 1000000) {
                        //printf("*****\n");
                        tot += p2->actualCost;
                        secs += my_time_diff(p2->actualEndTime, p2->startTime);
                    }
                    p2 = p2->next;
                }
                printf("| %s\t | %s\t | %f\t | %f%%\t |\n", 
                    p1->license, p1->carName,
                    tot, ((float)secs) / (365.0*86400.0) * 100.0
                );
                p1 = p1->next;
            }
            p = p->next;
        }
    }
    // 查询出租天数最多十辆车（降序）
    if (select == 4)
    {
        int yr;
        printf("请输入要查询的年份(2019):");
        scanf("%d", &yr);
        memset(ans, 0, sizeof(ans));

        cartype* type = p_type_head->next;
        while (type != NULL) {
            car* meta = type->sub;
            while (meta != NULL) {
                order* order = meta->firstOrder;
                sort_income_result();
                float tot = 0.0;
                int secs = 0;
                while (order != NULL) {
                    int id_order;
                    char id[20];
                    strcpy(id, order->id);
                    sscanf(id, "%d", &id_order);
                    if (id_order >= yr * 1000000 &&
                        id_order <= (yr + 1) * 1000000) {
                        tot += order->actualCost;
                        secs += my_time_diff(order->actualEndTime, order->startTime);
                    }
                    order = order->next;
                }
                
                float rate = ((float)secs) / (365.0 * 86400.0) * 100.0;
                int index = insert_index(((float)secs) / 86400.0);
                //printf("I: %d\n", index);
                if (index != -1 && tot != 0.0) {
                    ans[index].days = ((float)secs) / 86400.0f;
                    ans[index].total_income = tot;
                    ans[index].rate = ((float)secs) / (365.0 * 86400.0) * 100.0f;
                    strcpy((&ans[index])->plate, meta->license);
                    strcpy((&ans[index])->name, meta->carName);
                    ans[index].valid = 1;
                }
                meta = meta->next;
            }
            type = type->next;
        }

        printf("| 车牌号\t | 名称  \t | 出租天数\t | 营业额\t | 租用率\t |\n");
        for (size_t i = 0; i < 10; ++i) {
            if (ans[i].valid == 1)
            printf("| %s\t | %s\t | %.2f天\t | %.2f元\t | %.2f%%\t |\n",
                ans[i].plate, ans[i].name, ans[i].days, ans[i].total_income, ans[i].rate
            );
        }
    }
}

int main(int argc, char const *argv[])
{
    p_type_head = load_data();    // 加载数据
    menu();
    //read_from_csv();
    //check();
    
    return 0;
}