#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <chrono>  // 用於更精確的計時
using namespace std;
using namespace std::chrono;

class Polynomial {
private:
    struct Node {                                   //定義節點結構
        int coef;
        int exp;
        Node* next;                                 //指向下一個節點的指標
        Node(int c, int e, Node* n = nullptr)       //建構子 insertnode
            : coef(c), exp(e), next(n) {}
    };

    Node* head;                                     //指向開頭的指標
    int maxExp;                                     //記錄最高次方

    //插入新節點到正確位置
    static Node* insertNode(Node* head, int c, int e) {
        if (c == 0) return head;

        if (!head || e > head->exp)                 //如果串列是空的或新節點指數比第一個節點大
            return new Node(c, e, head);            //在最前面插入新節點

        Node* prev = nullptr;                       //記錄前一個節點，初始為空
        Node* cur = head;                           //紀錄目前檢查的節點
        //找插入位置  當還有節點且目前節點指數大於要插入的指數時，繼續往下找
        while (cur && cur->exp > e) {
            prev = cur;                             // 前一個節點移動到目前位置
            cur = cur->next;                // 目前節點移動到下一個
        }

        //如果找到指數相同的節點 合併
        if (cur && cur->exp == e) {
            cur->coef += c;                 // 係數相加
            if (cur->coef == 0) {           // 如果相加後係數變成 0
                if (prev) prev->next = cur->next;       // 如果有前一個節點，將它的 next 指向 cur 的下一個（跳過 cur  
                else head = cur->next;      // 如果沒有前一個節點（表示 cur 是 head），將 head 指向下一個  ?
                delete cur;
            }
        } 
        else {
            Node* n = new Node(c, e, cur);      // 建立新節點指向cur
            if (prev) prev->next = n;           // 如果有前一個節點，將它的 next 指向新節點
            else head = n;                      // 如果沒有前一個節點，新節點成為新的 head
        }
        return head;                //回傳更新後的head
    }

public:
    Polynomial() : head(nullptr), maxExp(-1) {}      // 預設建構子：初始化 head 為空指標，maxExp 為 -1

    void addTerm(int c, int e) {            // 新增一項到多項式
        head = insertNode(head, c, e);      // 把節點走訪head 插入該插的地方
        if (e > maxExp) maxExp = e;         // 如果新項的指數大於目前的最高次方，更新 maxExp
    }

    int degree() const { return maxExp; }   // 回傳多項式的次數（最高次方

    //印出
    void print() const {
        if (!head) {
            cout << "0";
            return;                         // 如果串列是空的，印出 "0" 並結束
        }
        Node* cur = head;                   // 從 head 開始
        bool first = true;                  // 記錄是否為第一項 用於控制 + 號
        
        //遍歷所有節點
        while (cur) {
            if (!first && cur->coef > 0) cout << "+";           // 如果不是第一項且係數為正，印出 "+"
                                                                // 負數係數會自動帶 "-"，所以不需要印
            if (cur->exp == 0)                                  // 如果指數是 0（常數項），只印係數

                cout << cur->coef;
            else {
                if (cur->coef != 1) cout << cur->coef;          // 避免印出1x
                cout << "x";
                if (cur->exp != 1) cout << "^" << cur->exp;     // 避免印出x＾1
            }
            first = false;          // 標記已不是第一項
            cur = cur->next;        // 移到下一個節點
        }
    }

    // Non-dense 乘法
    Polynomial multiplySparse(const Polynomial& B) const {
        Polynomial result;

        for (Node* a = head; a; a = a->next) {
            Polynomial temp;                                        // 建立暫時多項式，存放 A 的一項與 B 的所有項相乘的結果
            for (Node* b = B.head; b; b = b->next) {                // 遍歷 B 的每一項
                temp.addTerm(a->coef * b->coef, a->exp + b->exp);   // ab相乘 加到temp
            }
            result = result.add(temp);                  // 將 temp 加到 result

        }
        return result;                          //回傳結果
    }

    // Dense 乘法
    Polynomial multiplyDense(const Polynomial& B) const {
        Polynomial result;
        int maxDeg = maxExp + B.maxExp;         // 計算結果的最高次方
        vector<int> acc(maxDeg + 1, 0);         // 建立向量（動態陣列），大小為 maxDeg + 1 （包含 x^0 到 x^maxDeg）
                                                // acc[i] 儲存 x^i 的係數
        // 遍歷 A 的每一項
        for (Node* a = head; a; a = a->next)
            // 遍歷 B 的每一項
            for (Node* b = B.head; b; b = b->next)
                acc[a->exp + b->exp] += a->coef * b->coef;

        // 從最高次方往下遍歷（確保降冪輸出）把陣列加到linked list
        // --e: 遞減      
        for (int e = maxDeg; e >= 0; --e)
            if (acc[e] != 0)                    // 如果該項係數不為 0
                result.addTerm(acc[e], e);      // 將該項加入結果多項式

        return result;
    }

    // 多項式加法
    Polynomial add(const Polynomial& B) const {
        Polynomial result;
        Node* a = head;
        Node* b = B.head;

        // 當兩個多項式都還有項時
        while (a && b) {
            if (a->exp == b->exp) {                             //同次方相加
                result.addTerm(a->coef + b->coef, a->exp);
                a = a->next;
                b = b->next;
            } else if (a->exp > b->exp) {                       // 如果 A 的指數較大
                result.addTerm(a->coef, a->exp);                // 將 A 的項加入結果
                a = a->next;                                    // A 指標移到下一項
            } else {
                result.addTerm(b->coef, b->exp);
                b = b->next;
            }
        }
        //把剩下的加進去
        while (a) {                                     // 如果 A 還有剩餘項（B 已經用完）
            result.addTerm(a->coef, a->exp);            // 將 A 的剩餘項全部加入結果
            a = a->next;
        }
        while (b) {
            result.addTerm(b->coef, b->exp);
            b = b->next;
        }
        return result;
    }

    Polynomial multiply(const Polynomial& B, bool dense) const {        //決定多項式乘法用的模式
        return dense ? multiplyDense(B) : multiplySparse(B);
    }
};

// 第一部分：手動輸入測試
void part1_manualTest() {
    cout << "\n========== 第一部分：程式功能確認 ==========\n\n";
    
    Polynomial A, B;

    int m;
    cout << "Terms of A: ";
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int c, e;
        cout << "  輸入第 " << i+1 << " 項的 coef 和 exp: ";
        cin >> c >> e;
        A.addTerm(c, e);                        // 將項加入多項式 A
    }

    cout << "Terms of B: ";
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int c, e;
        cout << "  輸入第 " << i+1 << " 項的 coef 和 exp: ";
        cin >> c >> e;
        B.addTerm(c, e);
    }

    cout << "\nA(x) = ";
    A.print();
    cout << "\nB(x) = ";
    B.print();

    Polynomial C = A.multiply(B, false);

    cout << "\nC(x) = ";
    C.print();
    cout << endl;
}

// 第二部分：時間複雜度測試
void part2_performanceTest() {
    cout << "\n========== 第二部分：時間複雜度分析 ==========\n\n";
    
    
    int m = 100;  // 固定 m
    int n_values[] = {10, 50, 100, 250, 500, 750, 1000, 1250, 1500, 1750};
    int num_tests = 10;             // 測試次數（陣列大小）

    
    cout << "固定 m = " << m << "，測試不同的 n 值\n";
    cout << "coef = 1，使用連續指數避免重複\n\n";
    
    // 表格標題
    cout << left << setw(8) << "m" 
         << setw(8) << "n" 
         << setw(20) << "Dense時間(毫秒)" 
         << setw(20) << "Non-dense時間(毫秒)" << endl; 
    cout << string(56, '-') << endl;
    
     // 迴圈測試不同的 n 值
    for (int i = 0; i < num_tests; i++) {
        int n = n_values[i];                // 取得目前測試的 n 值
        
        // 建立多項式 A (m 項，使用連續指數)
        Polynomial A;
        for (int j = 0; j < m; j++) {       // 指數j：0, 1, 2, ..., m-1
            A.addTerm(1, j);  
        }
        
        // 建立多項式 B (n 項，使用連續指數)
        Polynomial B;
        for (int j = 0; j < n; j++) {
            B.addTerm(1, j);  // 指數：0, 1, 2, ..., n-1
        }
        
        // 測試 Dense 方法（單次執行）
        auto start_dense = high_resolution_clock::now();
        Polynomial C1 = A.multiply(B, true);
        auto end_dense = high_resolution_clock::now();
        auto dense_duration = duration_cast<microseconds>(end_dense - start_dense);
        double dense_time = dense_duration.count() / 1000.0;  // 轉換成毫秒
        
        // 測試 Non-dense 方法（單次執行）
        auto start_nondense = high_resolution_clock::now();
        Polynomial C2 = A.multiply(B, false);
        auto end_nondense = high_resolution_clock::now();
        auto nondense_duration = duration_cast<microseconds>(end_nondense - start_nondense);
        double nondense_time = nondense_duration.count() / 1000.0;  // 轉換成毫秒
        
        // 輸出結果
        cout << left << setw(8) << m 
             << setw(8) << n 
             << setw(20) << fixed << setprecision(6) << dense_time
             << setw(20) << nondense_time << endl;
    }
}

int main() {
    int choice;
    
    cout << "======================================\n";
    cout << "    多項式相乘程式 (Linked List)\n";
    cout << "======================================\n\n";
    cout << "請選擇功能:\n";
    cout << "1. 第一部分：手動輸入測試（確認程式可以跑）\n";
    cout << "2. 第二部分：時間複雜度測試（自動產生資料）\n";
    cout << "3. 執行兩個部分\n";
    cout << "\n請輸入選項 (1/2/3): ";
    cin >> choice;
    
    switch(choice) {
        case 1:
            part1_manualTest();
            break;
        case 2:
            part2_performanceTest();
            break;
        case 3:
            part1_manualTest();
            cout << "\n\n";
            part2_performanceTest();
            break;
        default:
            cout << "無效的選項！\n";
    }
    
    return 0;
}