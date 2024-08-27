#include<iostream>
#include<queue>
using namespace std;

class Register {
    public:
    bool value;
    Register* next;
    Register* prev;
    Register(bool v) { 
        value = v; 
        next = nullptr;
        prev = nullptr;
    }
};

class LFSR {
    Register* head;
    Register* tail;
    queue<Register*> q;
    int closeNum;
    public:
    LFSR(int size, bool* fdr, bool* initialV) {
        closeNum = 0;

        head = new Register{initialV[0]};
        tail = head;

        if (fdr[0] == 1) {                
            closeNum++;
            q.push(head);
        }

        for (int i = 1; i<size; i++) {
            Register* temp = new Register{initialV[i]};
            temp->next = head;
            head->prev = temp;
            head = temp;

            if (fdr[i] == 1) {
                closeNum++;
                q.push(head);
            }
        }

    }

    void generate() {
        Register* temp;
        bool input = 0;
        for (int i = 0; i<closeNum; i++) {
            temp = q.front();
            q.pop();

            input = (input + temp->value) % 2;

            if (temp->prev != nullptr) {
                temp = temp->prev;
                q.push(temp);
            }
            else {
                q.push(tail);
            }
        }

        if (tail->prev != nullptr) {
            temp = tail;
            tail = tail->prev;
            temp->prev = nullptr;
            tail->next = nullptr;

            temp->next = head;
            head->prev = temp;
            head = temp;
        }

        head->value = input;
    }

    bool output() {
        return tail->value;
    }

    ~LFSR() {
        while (head == tail) {
            head = head->next;
            delete head->prev;
        }
        delete tail;
    }
};

int main() {
    bool init[] = {0, 0, 1};
    bool p[] = {1, 1, 0};
    LFSR numGenerator(3, p, init);

    for (int i = 0; i<10; i++) {
        cout << numGenerator.output() << endl;
        numGenerator.generate();
    }
    
    return 0;
}