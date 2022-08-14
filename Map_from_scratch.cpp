#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <windows.h>
#include <fstream>
#include <bits/stdc++.h>
#include <list>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;
using namespace std;

//------------------DOC TEN FILE--------------------------------------------------------
typedef std::vector<std::string> stringvec;

using namespace std;

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

//---------------------CAU TRUC BAN BAM--------------------------------------------------
struct OneWord
{
    string Word;
    int TanSo;
};

class Hashing{
    private:
        int BUCKET;
        list<OneWord> *sc;
    public:
        Hashing(int N); //Luu Gia Tri duoc chia lay du
        void insertKey(string);
        void deleteKey(string);
        int hashFunction(string);
        void printHashTable();
        void findWord(string);
        int hashSearch(string);
        void browserHash(Hashing H_Add, int &sum);
};

Hashing::Hashing(int N = 156)
{
    this->BUCKET = N;
    sc = new list<OneWord>[BUCKET]; //Tao ra so luong phan tu
}

int Hashing::hashFunction(string string_okey)
{
    char c_okey_1 = string_okey[0];
    char c_okey_2 = string_okey[1];
    char c_okey_3 = string_okey[2];

    if( string_okey[0] <= 'z' && string_okey[0] >= 'a')
    {

    }
    else if(string_okey[0] == NULL)
        c_okey_1 = 'z' + 1;
    else
    {
        c_okey_1 = 'z' + 1;
    }

    if( string_okey[1] <= 'z' && string_okey[1] >= 'a')
    {

    }
    else if(string_okey[1] == NULL)
        c_okey_2 = 'z' + 1;
    else
    {
        c_okey_2 = 'z' + 1;
    }

    if( string_okey[2] <= 'z' && string_okey[2] >= 'a')
    {

    }
    else if(string_okey[2] == NULL)
        c_okey_3 = 'z' + 1;
    else
    {
        c_okey_3 = 'z' + 1;
    }


    return ((c_okey_1*1 + c_okey_2*2 + c_okey_3*3) % BUCKET);
}

void Hashing::insertKey(string key)
{
    int nkey = hashFunction(key); //nKey la index

    list<OneWord>::iterator j;

    int count = 0;

    if(sc[nkey].size() == 0)
    {
        OneWord Temp;
        Temp.Word = key;
        Temp.TanSo = 1;
        sc[nkey].push_back(Temp);
    }
    else
    {
        for(j = sc[nkey].begin(); j != sc[nkey].end(); j++)
        {
            if( (*j).Word == key)
            {
                (*j).TanSo += 1;
                count ++;
            }
        }

        if(count == 0)
        {
            OneWord Temp;
            Temp.Word = key;
            Temp.TanSo = 1;
            sc[nkey].push_back(Temp);

        }

    }

}

void Hashing::deleteKey(string key){
    int nkey = hashFunction(key);
    list<OneWord>::iterator i;
    for (i = sc[nkey].begin(); i != sc[nkey].end(); i++)
    {
        if( (*i).Word == key)
        {
            sc[nkey].erase(i);
        }
    }
}

void Hashing::printHashTable()
{
    for(int i = 0; i < BUCKET; i++){
        cout << i ;
        list<OneWord>::iterator j;
        for(j = sc[i].begin(); j != sc[i].end(); j++)
        {
            cout << "-->" << (*j).Word << " :  " << (*j).TanSo ;
        }
        cout << endl;
    }
}

int Hashing::hashSearch(string key)
{
    int nkey = hashFunction(key);
    list<OneWord>::iterator j;
    for(j = sc[nkey].begin(); j != sc[nkey].end(); j++)
    {
        if(key == (*j).Word)
        {
            return (*j).TanSo;
        }
    }
    return 0;
}


void Hashing::browserHash(Hashing H_Add, int &sum)
{
    for(int i = 0; i < BUCKET; i++)
    {
        list<OneWord>::iterator j;
        for(j = sc[i].begin(); j != sc[i].end(); j++)
        {
            int tmp;
            tmp = H_Add.hashSearch((*j).Word);

            if((*j).TanSo < tmp)
                sum = sum + (*j).TanSo;
            else
                sum = sum + tmp;
        }
    }
}


int main(){

//-----------DOC FILE-----------------------------------
    stringvec v;
    read_directory("C:/sm_doc_set/", v);
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

    if (system("CLS")) system("clear");
//-----------------DUA FILE TEXT VAO TRONG STRING---------------------------------------
    string txt[v.size()];
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

//-------XOA KI TU DAT BIET-----------------------------------
    char chars[] = ".,:;`'/+-(){}[]<>*&^%$#@!?~/\\=\r\t\n1234567890";
    for (unsigned int k = 0; k < strlen(chars); ++k)
    {
      for (int i = 0; i < v.size(); i++)
                txt[i].erase (std::remove(txt[i].begin(), txt[i].end(), chars[k]), txt[i].end());
    }

//------IN THUONG CAC KI TU------------------------------------
    for (int i = 0; i < v.size(); i++)
                std::for_each(txt[i].begin(), txt[i].end(), [](char & c){c = ::tolower(c);});

//------KHOI TAO BANG BAM -------------------------------------

    Hashing H_1_gram[v.size()];
    Hashing H_2_gram[v.size()];
    Hashing H_3_gram[v.size()];

 //----------DUA CAC CHU VAO TRONG BANG BAM 1-gram, 2-gram, 3-gram---------

    for( int i = 0; i < v.size() ; i++ ) //1-gram
    {
        string word;
        stringstream iss(txt[i]);
        while (iss >> word)
            H_1_gram[i].insertKey(word);
    }

    for( int i = 0; i < v.size(); i++ )  //2-gram
    {
        string word,old_word = "", tmp;
        stringstream iss(txt[i]);
        while (iss >> word)
        {
            tmp = old_word + " ";
            tmp = tmp + word;

            if(old_word != "")
                H_2_gram[i].insertKey(tmp);

            old_word = word;
        }
    }

    for( int i = 0; i < v.size(); i++ )  //3-gram
    {
        string word, old_word = "", oldest_world = "", tmp;
        stringstream iss(txt[i]);
        while (iss >> word)
        {
            tmp = oldest_world + " ";
            tmp = tmp + old_word;
            tmp = tmp + " ";
            tmp = tmp + word;

            if(oldest_world != "")
                H_3_gram[i].insertKey(tmp);

            oldest_world = old_word;
            old_word = word;
        }
    }


//-----------DUA MANG LUU TRU VE 0----------------------------

    int Num_Of_Cmp[v.size()][v.size()] ; // Noi Luu Tru Gia Tri So Sanh
    int Num_Of_Cmp2[v.size()][v.size()] ;
    int Num_Of_Cmp3[v.size()][v.size()] ;
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.size(); j++)
        {
            Num_Of_Cmp[i][j] = 0; // Dua gia tru luu tru ve 0
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
            H_1_gram[i].browserHash(H_1_gram[a], int_tmp);
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
            H_2_gram[i].browserHash(H_2_gram[a], int_tmp);
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
            H_3_gram[i].browserHash(H_3_gram[a], int_tmp);
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
