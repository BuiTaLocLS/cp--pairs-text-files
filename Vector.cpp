#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <windows.h>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <tuple>

using namespace std::chrono;
using namespace std;

//------------------DOC TEN FILE--------------------------------------------------------
typedef std::vector<std::string> stringvec;

void read_directory(const std::string& name, stringvec& v)
{
    std::string pattern(name);
    pattern.append("\\*.txt");
    WIN32_FIND_DATA data;
    HANDLE hFind;
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            v.push_back(data.cFileName);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}
int main()
{
    stringvec v;
    read_directory("C:/sm_doc_set/", v);
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

    int row = v.size(); // So file co trong thu muc

    if (system("CLS")) system("clear");
//---------------------------DUA CAC CHU VAO TRONG VECTOR 1-gram-----------------------------------------
    vector<vector<string> > Data_1;  //Data_1[i] la tung File, Data_1[i][j] la tung chu co trong file
    for (int i = 0; i < row; i++)
    {
        ifstream FileIn;
        FileIn.open("C:/sm_doc_set/" + v[i], ios_base::in);

        if(FileIn.fail() == 1)
        {
            cout << "Loi!";
            return 0;
        }

        vector<string> v1;
        while (FileIn.eof() == 0)
        {
            string chuoi_tam;
            FileIn >> chuoi_tam;
            v1.push_back(string(chuoi_tam));
        }

        Data_1.push_back(v1);
    }
//--------------------------XOA KI TU DAT BIET---------------------------------------------------------
    char chars[] = ".,:;`'/+-(){}[]<>*&^%$#@!?~/\\= \r\t\n1234567890";
    for (unsigned int k = 0; k < strlen(chars); ++k)
    {
      for (int i = 0; i < Data_1.size(); i++)
            for (int j = 0; j < Data_1[i].size(); j++)
                Data_1[i][j].erase (std::remove(Data_1[i][j].begin(), Data_1[i][j].end(), chars[k]), Data_1[i][j].end());
    }

//--------------------------IN THUONG CAC KI TU---------------------------------------------------------------------
    for (int i = 0; i < Data_1.size(); i++)
            for (int j = 0; j < Data_1[i].size(); j++)
                std::for_each(Data_1[i][j].begin(), Data_1[i][j].end(), [](char & c){c = ::tolower(c);});

//---------------------------DUA CAC CHU VAO TRONG VECTOR 2-gram VA 3-gram-----------------------------------------
    vector<vector<string> > Data_2 = Data_1; //2-gram
    for (int i = 0; i < Data_2.size() ; i++)
    {
        for (int j = 0; j < Data_2[i].size() - 1; j++)
        {
            Data_2[i][j] = Data_2[i][j] + " ";
            Data_2[i][j] = Data_2[i][j] + Data_2[i][j + 1];
        }
        Data_2[i].pop_back();
        Data_2[i].pop_back();
    }


    vector<vector<string> > Data_3 = Data_2; //3-gram
    for (int i = 0; i < Data_3.size() ; i++)
    {
        for (int j = 0; j < Data_3[i].size() - 1; j++)
        {
            Data_3[i][j] = Data_3[i][j] + " ";
            Data_3[i][j] = Data_3[i][j] + Data_1[i][j + 2];
        }
        Data_3[i].pop_back();
    }

//-----------------SAP XEP LAI CAC PHAN TU TRONG VECTOR THEO THU TU TANG DAN------------------------------------

    for (int i = 0; i < Data_1.size(); i++)
        std::sort (Data_1[i].begin(), Data_1[i].end());
    for (int i = 0; i < Data_2.size(); i++)
        std::sort (Data_2[i].begin(), Data_2[i].end());
    for (int i = 0; i < Data_3.size(); i++)
        std::sort (Data_3[i].begin(), Data_3[i].end());


//-----------DUA MANG LUU TRU VE 0----------------------------

    int Num_Of_Cmp[v.size()][v.size()] ;
    int Num_Of_Cmp2[v.size()][v.size()] ;
    int Num_Of_Cmp3[v.size()][v.size()] ;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.size(); j++)
        {
            Num_Of_Cmp[i][j] = 0;
            Num_Of_Cmp2[i][j] = 0;
            Num_Of_Cmp3[i][j] = 0;
        }

    }

//------------SO SANH 1-gram -------------------------
    auto start_1gram = high_resolution_clock::now();

    for (int i = 0; i < Data_1.size(); i++)
    {
        for(int a = i + 1; a < Data_1.size() ; a++)
        {
            for (int j = 0; j < Data_1[i].size(); j++)
            {
                int sum = 1, sum_cmp = 0;

                for (int k = j+1; k < Data_1[i].size(); k++) //Tim so luong chu co trong file
                {
                    if(Data_1[i][j] == Data_1[i][k])
                        sum = sum + 1;
                    else break;
                }


                for (int k = 0; k < Data_1[a].size(); k++)  //So sanh voi file khac va tim so luong chu co trong file do
                {
                    if(Data_1[i][j] == Data_1[a][k])
                        sum_cmp = sum_cmp + 1;
                }


                if(sum_cmp < sum) Num_Of_Cmp[i][a] = Num_Of_Cmp[i][a] + sum_cmp; //Lay gia tri min
                    else Num_Of_Cmp[i][a] += sum;

                j = j + sum - 1;
            }
        }
    }

    auto stop_1gram = high_resolution_clock::now();
//------------------------------------------XUAT RA 20 TRUONG HOP TRUNG CAO NHAT 1-gram-----------------------
    cout << "---1 GRAM---";
    vector<tuple<int, int, int>> vp;

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size(); a++)
        {
            vp.push_back(make_tuple(Num_Of_Cmp[i][a], i, a));
        }
    }

    sort(vp.begin(), vp.end() );

    for (int i = vp.size() - 1; i >= vp.size() - 20 ; i--) {
            cout << endl << v[get<1>(vp[i])] << "               " << v[get<2>(vp[i])] << "          " << get<0>(vp[i]);
    }
//------------SO SANH 2-gram -------------------------
    auto start_2gram = high_resolution_clock::now();

    for (int i = 0; i < Data_2.size(); i++)
    {
        for(int a = i + 1; a < Data_2.size() ; a++)
        {
            for (int j = 0; j < Data_2[i].size(); j++)
            {
                int sum = 1, sum_cmp = 0;

                for (int k = j+1; k < Data_2[i].size(); k++)
                {
                    if(Data_2[i][j] == Data_2[i][k])
                        sum = sum + 1;
                    else break;
                }

                for (int k = 0; k < Data_2[a].size(); k++)
                {
                    if(Data_2[i][j] == Data_2[a][k])
                        sum_cmp = sum_cmp + 1;
                }


                if(sum_cmp < sum) Num_Of_Cmp[i][a] = Num_Of_Cmp[i][a] + sum_cmp;
                    else
                    {
                        Num_Of_Cmp[i][a] += sum;
                        Num_Of_Cmp2[i][a] += sum;
                    }

                j = j + sum - 1;
            }
        }
    }

    auto stop_2gram = high_resolution_clock::now();
//------------------------------------------XUAT RA 20 TRUONG HOP TRUNG CAO NHAT 2 gram-----------------------
    cout << "\n\n\n--2 GRAM--";
    vector<tuple<int, int, int>> vp2;

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size(); a++)
        {
            vp2.push_back(make_tuple(Num_Of_Cmp2[i][a], i, a));
        }
    }

    sort(vp2.begin(), vp2.end() );

    for (int i = vp2.size() - 1; i >= vp2.size() - 20 ; i--) {
            cout << endl << v[get<1>(vp2[i])] << "               " << v[get<2>(vp2[i])] << "          " << get<0>(vp2[i]);
    }
//------------------------------------------XUAT RA 20 TRUONG HOP TRUNG CAO NHAT 1-2 gram-----------------------
    cout << "\n\n\n--1-2 GRAM--";
    vector<tuple<int, int, int>> vp12;

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size(); a++)
        {
            vp12.push_back(make_tuple(Num_Of_Cmp[i][a], i, a));
        }
    }

    sort(vp12.begin(), vp12.end() );

    for (int i = vp12.size() - 1; i >= vp12.size() - 20 ; i--) {
            cout << endl << v[get<1>(vp12[i])] << "               " << v[get<2>(vp12[i])] << "          " << get<0>(vp12[i]);
    }
//------------SO SANH 3-gram -------------------------
    auto start_3gram = high_resolution_clock::now();

    for (int i = 0; i < Data_3.size(); i++)
    {
        for(int a = i + 1; a < Data_3.size() ; a++)
        {
            for (int j = 0; j < Data_3[i].size(); j++)
            {
                int sum = 1, sum_cmp = 0;

                for (int k = j+1; k < Data_3[i].size(); k++)
                {
                    if(Data_3[i][j] == Data_3[i][k])
                        sum = sum + 1;
                    else break;
                }

                for (int k = 0; k < Data_3[a].size(); k++)
                {
                    if(Data_3[i][j] == Data_3[a][k])
                        sum_cmp = sum_cmp + 1;
                }

                if(sum_cmp < sum) Num_Of_Cmp[i][a] = Num_Of_Cmp[i][a] + sum_cmp;
                    else
                    {
                        Num_Of_Cmp[i][a] += sum;
                        Num_Of_Cmp3[i][a] += sum;
                    }
                j = j + sum - 1;
            }
            //cout << v[i] << " " << v[a] << " " << Num_Of_Cmp[i][a] << endl;
        }
    }

    auto stop_3gram = high_resolution_clock::now();
//------------------------------------------XUAT RA 20 TRUONG HOP TRUNG CAO NHAT 3 gram-----------------------
    cout << "\n\n\n--3 GRAM--";
    vector<tuple<int, int, int>> vp3;

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size(); a++)
        {
            vp3.push_back(make_tuple(Num_Of_Cmp3[i][a], i, a));
        }
    }

    sort(vp3.begin(), vp3.end() );

    for (int i = vp3.size() - 1; i >= vp3.size() - 20 ; i--) {
            cout << endl << v[get<1>(vp3[i])] << "               " << v[get<2>(vp3[i])] << "          " << get<0>(vp3[i]);
    }
//------------------------------------------XUAT RA 20 TRUONG HOP TRUNG CAO NHAT 1-2-3 gram-----------------------
    cout << "\n\n\n--1-2-3 GRAM--";
    vector<tuple<int, int, int>> vp13;

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size(); a++)
        {
            vp13.push_back(make_tuple(Num_Of_Cmp[i][a], i, a));
        }
    }

    sort(vp13.begin(), vp13.end() );

    for (int i = vp13.size() - 1; i >= vp13.size() - 20 ; i--) {
            cout << endl << v[get<1>(vp13[i])] << "               " << v[get<2>(vp13[i])] << "          " << get<0>(vp13[i]);
    }
//--------------------IN RA THOI GIAN------------------------------
    std::chrono::duration<double> duration_1gram = duration_cast<microseconds>(stop_1gram - start_1gram);
    std::chrono::duration<double> duration_2gram = duration_cast<microseconds>(stop_2gram - start_2gram);
    std::chrono::duration<double> duration_3gram = duration_cast<microseconds>(stop_3gram - start_3gram);
    std::chrono::duration<double> duration_1gram_2gram = duration_cast<microseconds>(stop_2gram - start_1gram);
    std::chrono::duration<double> duration_1gram_3gram = duration_cast<microseconds>(stop_3gram - start_1gram);

    std::time_t end_time = std::chrono::system_clock::to_time_t(stop_3gram);

    std::cout << "\nFinished computation at " << std::ctime(&end_time) << endl ;
    cout << " {1-grams}  " << duration_1gram.count() << endl;
    cout << " {2-grams}  " <<duration_2gram.count() << endl;
    cout << " {3-grams}  " <<duration_3gram.count() << endl;
    cout << " {1-grams, 2-grams}  " <<duration_1gram_2gram.count() << endl;
    cout << " {1-grams, 2-grams, 3-grams}  " <<duration_1gram_3gram.count() << endl;
}

