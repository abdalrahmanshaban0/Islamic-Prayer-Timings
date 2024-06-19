#include <iostream>
using namespace std;

struct day{
    string name;
    string time;
    int val;
};

int main(){
    const int N = 7;
    day arr[N];

    FILE *f = freopen("/home/abdalrahman/.src/Islamic-Prayer-Timings/table.txt", "r", stdin);

    char usless; cin >> usless;

    for(int i = 0 ; i < N; i++){
        string a, b; 
        cin >> a >> b;
        arr[i].name = a.substr(1, a.size()-3);
        arr[i].time = b.substr(1, b.size()-3);
        arr[i].val = stoi(arr[i].time.substr(0, 2))*60*60 + stoi(arr[i].time.substr(3, 2))*60;
    }

    fclose(f);
    f = freopen("/home/abdalrahman/.src/Islamic-Prayer-Timings/table.txt", "w", stdout);

    for(int i = 0 ; i < N; i++){
        if(i!=4) //(no need to Sunset, just Maghrib)
            cout << arr[i].name << ' ' << arr[i].time << ' ' << arr[i].val << '\n';
    }

    return 0;
}
