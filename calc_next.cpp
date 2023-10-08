#include <iostream>
using namespace std;

int main(){
    time_t now = time(0);
    
    tm *cur = localtime(&now);
    int cur_val = (cur->tm_min) *60 + (cur->tm_hour) *60*60;

//    cur_val = 10 * 60 * 60;   //for testing

    int comming_val = 0;
    int comming_idx = 0;

    FILE *F = freopen("table.txt", "r", stdin);
    for(int i = 0; i < 6; i++){
        string name, time;
        int secs;
        cin >> name >> time >> secs;
        if(i == 0) comming_val = secs;
        if(secs > cur_val){
            comming_idx = i;
            comming_val = secs;
            break;
        }
    }
    fclose(F);

    int dff = comming_val - cur_val;
    if(dff < 0){
        dff += 24 * 60 * 60;
    }
    
    //cout << cur_val << ' ' << comming_val << '\n';
    cout << comming_idx << '\n'  << dff << '\n';


}
