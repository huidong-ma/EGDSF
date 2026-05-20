// 参考自https://github.com/ArashPartow/bloom
#include<stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <vector>
#include <map>
#include <omp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "bloom_filter.hpp"
using namespace std;
void  decrypted_rsa(int threadCount, string input_Path);
void  encrypted_rsa(int threadCount, string input_Path);
// 用于获取字典行数目
int getDICTline(char *dictPath){
    ifstream file(dictPath);
    string str;
    int count = 0;
    while (file) {
    getline(file, str);//从文件中读取一行
    remove(str.begin(), str.end(), ' ');//这个算法函数在algorithm头文件中，删除一行中的空格
    remove(str.begin(), str.end(), '\t');//删除一行中的制表符，因为制表符和空格都是空的
    if (str.length() > 0) //如果删除制表符和空格之后的一行数据还有其他字符就算有效行
        count ++;
}
return count - 1;
}

int main(int argc, char** argv){
    string method=argv[1];
    int threadCount=std::stoi(argv[2]);
    string input_Path = std::string(argv[3]);
    if(method=="-c")
        encrypted_rsa(threadCount,input_Path);
    else if(method=="-d")
        decrypted_rsa(threadCount,input_Path);

    return 0;
}


void  encrypted_rsa(int threadCount, string input_Path){
    int debugModel = 1;
    char dictPath[1000] = "dataBaseSrf.txt";
    int elementNumber = getDICTline(dictPath) + 10000;
    int r=50;
    //cout <<elementNumber;
    float falsePostive = 0.0001; //1 in 10000
    // 定义布隆过滤器参数
    bloom_parameters parameters;
    parameters.projected_element_count = elementNumber;
    parameters.false_positive_probability = falsePostive;
    parameters.random_seed = 0xA5A5A5A5;
    // 错误检查
    if (!parameters)
    {
       std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
       return ;
    }

    parameters.compute_optimal_parameters();
    bloom_filter filter(parameters);//实例化Bloom筛选器
    //初始化bloom过滤器
    clock_t startTime = clock();
    ifstream inputFiles;   // 创建流对象用于读文件
    inputFiles.open(dictPath, ios::in);
    if(!inputFiles) {
        cout << "error" ;
        return ;
    }


    string myRead;
    while(!inputFiles.eof()){
        getline(inputFiles, myRead);
        filter.insert(myRead);

    }
    cout<<filter.size()<<endl;
    inputFiles.close();
    clock_t endTime = clock();
    double timeLoad = double(endTime - startTime)/CLOCKS_PER_SEC;
    if(debugModel == 1){
        cout <<"step1: SRF Bloom filter initialized successfully."<<endl;
        cout <<"       elementNumber is : "<<elementNumber<<endl;
        cout <<"       falsePostive is : "<<falsePostive<<endl;
        cout <<"       time spend is : "<< timeLoad<<" s "<<endl;
       }


    map<char,int> c_to_i;
    map<int,char> i_to_c;
    c_to_i['A']=0;i_to_c[0]='A';
    c_to_i['C']=1;i_to_c[1]='C';
    c_to_i['G']=2;i_to_c[2]='G';
    c_to_i['T']=3;i_to_c[3]='T';
    vector<string> rsa(threadCount); //密钥组

    string rsa_file="rsa_file.txt";
    ifstream input_Rsa_Files;
    input_Rsa_Files.open(rsa_file,ios::in);
    if(input_Rsa_Files.eof())
    {
        cout<<"rsa is not enough！！"<<endl;
        return ;
    }
    for(int i=0;i<threadCount;i++)
        getline(input_Rsa_Files,rsa[i]);
    input_Rsa_Files.close();
    int rsa_length=rsa[0].size();
    vector<int> tag(threadCount,0); //标记每个线程对应加密串标记点

    cout<< rsa_length << endl;
    //开始多文件测试
//定义一个vector存放名称

    string out_Folder = input_Path+".out";
    if (access(out_Folder.c_str(), 0) == 0)
        rmdir(out_Folder.c_str());
    int isCreate = mkdir(out_Folder.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
    if( !isCreate)
        cout<<"create path:"<<out_Folder<<"\n";
    else
        cout<<"create path failed! \n";


    string out_Path = out_Folder+"/encry.data";
    string out_Mark_Path = out_Folder+"/mark.txt";
    std::ofstream out_File,out_Mark_File;
    out_File.open(out_Path.c_str(),std::ios::trunc|std::ios::binary);
    out_Mark_File.open(out_Mark_Path.c_str(),std::ios::trunc);

    //查询过滤器
    //char testFile[100] = "./../../data/sens_1.txt";
    cout<<"**************************"<<endl;
    cout<<input_Path<<endl;
    int numberOfAll = 0;
    int numberOfSens = 0;
    long long wrong_num=0;

    clock_t startTimeTest = clock();
    ifstream input_fastq_Files;   // 创建流对象用于读文件
    input_fastq_Files.open(input_Path, ios::in);
        if(!input_fastq_Files) {
        cout << "error" ;
        return ;
    }

    string Read;
    getline(input_fastq_Files,Read);

    int len=Read.size();
    int partLength = len/ threadCount;
    int remainder = len % threadCount;

    vector<string> strBlock;

    //按照CPU核数分割序列
    int start=0;
    for(int i=0;i<threadCount;i++)
    {
        int currentLength = partLength + (i < remainder ? 1 : 0);
        if (start + currentLength > len) {
            currentLength = len - start;
        }
        strBlock.push_back(Read.substr(start, currentLength));
        start += currentLength;
    }


    vector<vector<int>> resultRecord(threadCount,vector<int>(strBlock[0].size(),0)); //敏感序列标识数组
    #pragma omp parallel for num_threads(threadCount)
    for (int index = 0; index < threadCount; index++) {
        string myread=strBlock[index];
        int myReadLen = myread.length();
        for(int i=0; i<myReadLen-r+1; i++){
            //提取第一段短序列
            string shortRead = myread.substr(i, r);  //每次50 50的取
            int pos;
            for(pos=49;pos>=0;pos--) //处理N的情况
                if(shortRead[pos]=='N')
                    break;
            if(pos>=0)
            {
                i+=pos;
                continue;
            }
            // 判断是否在布隆过滤器1中
            if(filter.contains(shortRead)){
                // 这个短序列是一条敏感短序列置res[i] = 1;
                resultRecord[index][i]  = 1 ;
                i+=49;
                //cout << "布隆过滤器1识别出！" << endl ;
                continue;
            }
        } // 识别完毕完毕
    }
    vector<int> wrong(threadCount,0);
    #pragma omp parallel for num_threads(threadCount) //schedule(static, (len+3)/threadCount)
    for (int index = 0; index < threadCount; index++)
    {
        string myread=strBlock[index];
        int thread_id = index;       //omp_get_thread_num();
        int myReadLen = myread.length();
        for(int i=0; i<myReadLen-r+1; i++){
            string shortRead = myread.substr(i, r);  //每次50 50的取
            // 判断是否在布隆过滤器1中
            if(resultRecord[index][i]==1){
                // 这个短序列是一条敏感短序列置res[i] = 1;
                wrong[thread_id]++;
                //cout<<strBlock[index]<<endl;
                int r1=0,r2;
                for(int j=0;j<48;j++)
                {
                    int num=c_to_i[shortRead[j]];
                    r1=r1*4+num;
                    if(j%4==3)
                    {
                        r2=rsa[thread_id][tag[thread_id]++];
                        tag[thread_id]%=rsa_length;
                        r1^=r2;
                        int k=4;
                        while(k--)
                        {
                            strBlock[index][i+j-3+k]=i_to_c[r1%4];
                            r1/=4;
                        }
                        r1=0;
                    }
                }
                i+=49;
                continue;
            }
        }
    }
    for(int i=0;i<threadCount;i++)
        wrong_num+=wrong[i];


    for (int index = 0; index < threadCount; index++)
    {
        out_File<<strBlock[index];
        int pre=0;
        int myReadLen = strBlock[index].length();
        for(int i=0; i<myReadLen-r+1; i++){
            if(resultRecord[index][i]==1){
                out_Mark_File<<pre<<" ";
                i+=49;
                pre=49;
            }
            pre++;
        }
        out_Mark_File<<'\n';
    }




    input_fastq_Files.close();
    out_File.close();
    clock_t endTimeTest = clock();
    double timeTest = double(endTimeTest - startTimeTest)/CLOCKS_PER_SEC;
    if(debugModel == 1){
       cout <<"step2: Test File successfully."<<endl;
       cout <<"       sensitive count is : "<<wrong_num<<endl;
       cout <<"       time spend is : "<< timeTest<<" s "<<endl;
       cout<<endl<<endl<<endl<<endl;
    }

    return ;
}
// srun -p gpu1 ./basicFunc >> test_err_0_to_10_in_gpu1.txt



void  decrypted_rsa(int threadCount, string input_Path){
    int debugModel = 1;
    int r=50;
    //cout <<elementNumber;

    map<char,int> c_to_i;
    map<int,char> i_to_c;
    c_to_i['A']=0;i_to_c[0]='A';
    c_to_i['C']=1;i_to_c[1]='C';
    c_to_i['G']=2;i_to_c[2]='G';
    c_to_i['T']=3;i_to_c[3]='T';
    vector<string> rsa(threadCount); //密钥组

    string rsa_file="rsa_file.txt";
    ifstream input_Rsa_Files;
    input_Rsa_Files.open(rsa_file,ios::in);
    if(input_Rsa_Files.eof())
    {
        cout<<"rsa is not enough！！"<<endl;
        return ;
    }
    for(int i=0;i<threadCount;i++)
        getline(input_Rsa_Files,rsa[i]);
    input_Rsa_Files.close();
    int rsa_length=rsa[0].size();
    vector<int> tag(threadCount,0); //标记每个线程对应加密串标记点

    cout<< rsa_length << endl;
    //开始多文件测试
    //定义一个vector存放名称

    string input_encry_Path = input_Path+"/encry.data";
    string input_Mark_Path = input_Path+"/mark.txt";
    std::ifstream input_encry_File,input_Mark_File;
    input_encry_File.open(input_encry_Path.c_str(),ios::in);
    input_Mark_File.open(input_Mark_Path.c_str(),ios::in);

    string out_Path = input_Path.substr(0,input_Path.rfind('.'))+".decry";
    std::ofstream out_File;
    out_File.open(out_Path.c_str(),std::ios::trunc|std::ios::binary);

    //查询过滤器
    //char testFile[100] = "./../../data/sens_1.txt";
    cout<<"**************************"<<endl;
    cout<<input_Path<<endl;

    clock_t startTimeTest = clock();

    vector<string> strBlock;

    string Read;
    getline(input_encry_File,Read);

    int len=Read.size();
    int partLength = len/ threadCount;
    int remainder = len % threadCount;

    //按照CPU核数分割序列
    int start=0;
    for(int i=0;i<threadCount;i++)
    {
        int currentLength = partLength + (i < remainder ? 1 : 0);
        if (start + currentLength > len) {
            currentLength = len - start;
        }
        strBlock.push_back(Read.substr(start, currentLength));
        start += currentLength;
    }


    vector<vector<int>> resultRecord(threadCount,vector<int>(strBlock[0].size(),0)); //敏感序列标识数组

    string markstr;
    for(int i=0;i<threadCount;i++)
    {
        getline(input_Mark_File,markstr);
        long long num=0;
        long long key=0;
        for(char c:markstr)
        {
            if(c==' ')
            {
                key+=num;
                resultRecord[i][key]=1;
                num=0;
            }
            else
                num=num*10+c-'0';
        }
    }

    #pragma omp parallel for num_threads(threadCount) //schedule(static, (len+3)/threadCount)
    for (int index = 0; index < threadCount; index++)
    {
        string myread=strBlock[index];
        int thread_id = index;       //omp_get_thread_num();
        int myReadLen = myread.length();
        for(int i=0; i<myReadLen-r+1; i++){
            string shortRead = myread.substr(i, r);  //每次50 50的取
            // 判断是否在布隆过滤器1中
            if(resultRecord[index][i]==1){
                // 这个短序列是一条敏感短序列置res[i] = 1
                int r1=0,r2;
                for(int j=0;j<48;j++)
                {
                    int num=c_to_i[shortRead[j]];
                    r1=r1*4+num;
                    if(j%4==3)
                    {
                        r2=rsa[thread_id][tag[thread_id]++];
                        tag[thread_id]%=rsa_length;
                        r1^=r2;
                        int k=4;
                        while(k--)
                        {
                            strBlock[index][i+j-3+k]=i_to_c[r1%4];
                            r1/=4;
                        }
                        r1=0;
                    }
                }
                i+=49;
                continue;
            }
        }
    }


    for (size_t index = 0; index < strBlock.size(); index++)
        out_File << strBlock[index];




    input_encry_File.close();
    input_Mark_File.close();
    out_File.close();
    clock_t endTimeTest = clock();
    double timeTest = double(endTimeTest - startTimeTest)/CLOCKS_PER_SEC;
    if(debugModel == 1){
       cout <<"step2: Test File successfully."<<endl;
       cout <<"       time spend is : "<< timeTest<<" s "<<endl;
       cout<<endl<<endl<<endl<<endl;
    }

    return ;
}