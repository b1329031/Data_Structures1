#include <iostream>
using namespace std;

//CppArray類別：一個動態浮點數陣列的封裝
class CppArray{
private:
    int size;
    float initvalue;
    float *arr;                                                       //指向動態陣列的指標

//建構子：建立指定大小和初始值的陣列
public:

    // (a) 一個建構子 CppArray (int size = defaultSize, float initvalue = default Value)
    // 這個建構子能夠建立一個大小為 size 並且每個元素的初始值都設定為 initvalue 的陣列

    CppArray(int defaultSize, float defaultValue): size(defaultSize) ,initvalue(defaultValue) {  //初始化
        arr = new float[defaultSize];                                  //動態分配記憶體
        for(int i = 0 ; i < defaultSize; i++){
            arr[i] = defaultValue;                                     //將每個元素初始化為預設值
        }
    };

    // (b) 一個複製的建構子 CppArray (const CppArray& cp2)
    // 它會建立一個和 cp2 一樣的陣列；複製建構子可以用一個陣列來初始化另一個陣列
    // 用法如下：CppArray a = b;

    CppArray(const CppArray& cp2) {                                    //從另一個 CppArray 物件複製內容, cp2: 要複製的來源物件
        size = cp2.size;                                               // 複製大小
        arr = new float[size];                                         // 分配新的記憶體空間
        for (int i = 0; i < size; i++){  
            arr[i] = cp2.arr[i];                                       // 逐一複製元素
        }
    }

    // (c) 一個指派運算子 CppArray& operator = (const CppArray& cp2)
    // 它能夠將原本的陣列以 cp2 取代

    CppArray& operator=(const CppArray& cp2) {                         // 將另一個 CppArray 的內容賦值給當前物件
        delete[] arr;                                                  // 先釋放舊的記憶體
        size = cp2.size;                                               // 設定新的大小
        arr = new float[size];                                         //分配新的記憶體空間
        for (int i = 0; i < size; i++){
            arr[i] = cp2.arr[i];                                       // 逐一複製元素
        } 
        return *this;                                                  // 回傳參考
    }

    //d 解構子
    ~CppArray() {
        delete[] arr;                                                         //釋放動態分配的記憶體
    }                                                                         

    // (e) 一個下標運算子 float& operator[] (int i)
    // 必須實作它才能作索引範圍的檢測（譯者註：一個陣列元素可以表示成 ai 或 a[i]，
    // 前者的 i 稱為下標，後者的 i 稱為索引）
    float& operator[](int i) {
        return arr[i];                                                        //回傳索引值
    } 

    //f
    int GetSize() {
        return size;                                                          // 回傳陣列大小
    }

    // (g) 重載cin運算子 << 與 >>
    // 使其能輸入與列印 CppArray 中的元素。這些函式必須是 CppArray 的友伴函式
    friend istream& operator>>(istream& is,const CppArray& cp2) {              // 從輸入串流讀取資料到陣列
        for (int i = 0; i < cp2.size; i++){                                    // os: 輸入串流（如 cin）
            is >> cp2.arr[i];                                                  // cp2: 要輸入資料的 CppArray 物件
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const CppArray& cp2) {             // 將陣列內容輸出到輸出串流
        for (int i = 0; i < cp2.size; i++){                                    // os: 輸出串流（如 cout）
             os << cp2.arr[i] << " ";                                          // cp2: 要輸出的 CppArray 物件
        }
        return os;
    }


};

int main() {
    CppArray arr1(5, 1.0);   // 建立大小=5，每個元素=1.0
    cout << "arr1: " << arr1 << endl;

    CppArray arr2 = arr1;    // 測試複製建構子
    arr2[2] = 10;            // 修改 arr2 的第 3 個元素
    cout << "arr2 (after modification): " << arr2 << endl;

    CppArray arr3(2, 2.0);   // 先建一個小陣列
    arr3 = arr2;             // 測試指定運算子
    cout << "arr3 (after assignment): " << arr3 << endl;

    cout << "Size of arr1 = " << arr1.GetSize() << endl;

    cout << "請輸入 arr1 的 " << arr1.GetSize() << " 個新值: ";
    cin >> arr1;             // 測試輸入
    cout << "arr1 (after input): " << arr1 << endl;
    cout << arr1[4];
    return 0;
}