#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <windows.h>
#include <fstream>
#include<bits/stdc++.h>
#include <cstdlib>
#include <chrono>

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

//-----------------------CAU TRUC CAY-----------------------
struct data
{
    string Chu;
    int Solanlaplai = 1;
};

struct node
{
    data DuLieu;
    node * pLeft;
    node * pRight;
};

typedef struct node NODE;

typedef NODE* TREE;

void KhoiTaoCay(TREE &t)
{
    t = NULL;
}

void ThemNode(TREE &t, string str)
{
    if(t == NULL)
    {

        node *pNode = new node;
        pNode->DuLieu.Chu = str ;
        pNode->pLeft = NULL;
        pNode->pRight = NULL;

        t = pNode;
    }
    else
    {
        if(str > t->DuLieu.Chu)
            ThemNode(t->pRight, str);
        if(str < t->DuLieu.Chu)
            ThemNode(t->pLeft, str);
        if(str == t->DuLieu.Chu)
        {
            t->DuLieu.Solanlaplai++;
        }
    }
}

int search(TREE root, string key)
{
    if(root == NULL)
        return 0;

    if (root->DuLieu.Chu == key)
       return root->DuLieu.Solanlaplai;

    if (root->DuLieu.Chu < key)
       return search(root->pRight, key);

    return search(root->pLeft, key);
}

void DuyetRNL(TREE t, TREE t2, int &sum)
{
    if(t != NULL)
    {
        DuyetRNL(t->pRight, t2, sum);

        string str = t->DuLieu.Chu;

        int tmp;

        tmp = search(t2, str);

        if(t->DuLieu.Solanlaplai < tmp)
            sum = sum + t->DuLieu.Solanlaplai;
        else
            sum = sum + tmp;

        DuyetRNL(t->pLeft, t2, sum);
    }
}

int main()
{

//------------------DOC TEN FILE--------------------------------------------------------

    stringvec v;
    read_directory("C:/sm_doc_set/", v);
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

    if (system("CLS")) system("clear");

//-----------------DUA FILE TEXT VAO TRONG STRING---------------------------------------
    string txt[v.size()]; // moi string txt[i] dai dien cho 1 file
    for (int i = 0; i < v.size(); i++)
    {
        ifstream FileIn;
        FileIn.open("C:/sm_doc_set/" + v[i], ios_base::in);

        if(FileIn.fail() == 1)
        {
            cout << "Loi!";
            return 0;
        }

        while (FileIn.eof() == 0)
        {
            string chuoi_tam;
            FileIn >> chuoi_tam;
            txt[i].append(string(chuoi_tam));
            txt[i].append(" ");
        }
    }

//--------------------------XOA KI TU DAT BIET---------------------------------------------------------
    char chars[] = ".,:;`'/+-(){}[]<>*&^%$#@!?~/\\=\r\t\n1234567890";
    for (unsigned int k = 0; k < strlen(chars); ++k)
    {
      for (int i = 0; i < v.size(); i++)
                txt[i].erase (std::remove(txt[i].begin(), txt[i].end(), chars[k]), txt[i].end());
    }

//--------------------------IN THUONG CAC KI TU--------------------------------------------------------
    for (int i = 0; i < v.size(); i++)
                std::for_each(txt[i].begin(), txt[i].end(), [](char & c){c = ::tolower(c);});

//---------------------------KHOI TAO CAY--------------------------------------------------------------
    TREE t1_gram[v.size()], t2_gram[v.size()], t3_gram[v.size()];

    for (int i = 0; i < v.size(); i++)
    {
        KhoiTaoCay(t1_gram[i]);
        KhoiTaoCay(t2_gram[i]);
        KhoiTaoCay(t3_gram[i]);
    }

 //---------------------------DUA CAC CHU VAO TRONG VECTOR 1-gram, 2-gram, 3-gram-----------------------------------------

    for( int i = 0; i < v.size(); i++ ) //1-gram
    {
        string word;
        stringstream iss(txt[i]);
        while (iss >> word)
            ThemNode(t1_gram[i],word);
    }

    for( int i = 0; i < v.size(); i++ ) //2-gram
    {
        string word,old_word = "", tmp;
        stringstream iss(txt[i]);
        while (iss >> word)
        {
            tmp = old_word + " ";
            tmp = tmp + word;

            if(old_word != "")
                ThemNode(t2_gram[i],tmp);

            old_word = word;
        }
    }

    for( int i = 0; i < v.size(); i++ ) //3-gram
    {
        string word, old_word = "", oldest_world = "", tmp;
        stringstream iss(txt[i]);
        while (iss >> word)
        {
            tmp = oldest_world + " ";
            tmp = tmp + old_word;
            tmp = tmp + " ";
            tmp = tmp + word;

            if(old_word != "")
                ThemNode(t3_gram[i],tmp);

            oldest_world = old_word;
            old_word = word;
        }
    }
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

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size() ; a++)
        {
            int int_tmp = 0;
            DuyetRNL(t1_gram[i],t1_gram[a], int_tmp);
            Num_Of_Cmp[i][a] = int_tmp;

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

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size() ; a++)
        {
            int int_tmp = 0;
            DuyetRNL(t2_gram[i],t2_gram[a], int_tmp);
            Num_Of_Cmp[i][a] += int_tmp;
            Num_Of_Cmp2[i][a] += int_tmp;
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

    for (int i = 0; i < v.size(); i++)
    {
        for(int a = i + 1; a < v.size() ; a++)
        {
            int int_tmp = 0;
            DuyetRNL(t3_gram[i],t3_gram[a], int_tmp);
            Num_Of_Cmp[i][a] += int_tmp;
            Num_Of_Cmp3[i][a] += int_tmp;

            //cout << endl << v[i] << " " << v[a] << " " <<  Num_Of_Cmp[i][a];
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

    std::cout << "\n\nFinished computation at " << std::ctime(&end_time) << endl ;
    cout << " {1-grams}  " << duration_1gram.count() << endl;
    cout << " {2-grams}  " <<duration_2gram.count() << endl;
    cout << " {3-grams}  " <<duration_3gram.count() << endl;
    cout << " {1-grams, 2-grams}  " <<duration_1gram_2gram.count() << endl;
    cout << " {1-grams, 2-grams, 3-grams}  " <<duration_1gram_3gram.count() << endl;

    return 0;
}
