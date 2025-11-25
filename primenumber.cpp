#include <iostream>
using namespace std;

int n;
int main() {
int sum;
cout << "Enter a number: ";
cin >> n;
int i;
for ( i=2; i <=n; i++) {
    sum=0;
    for (int j=1; j<=i; j++) {
        if (i%j==0) {
            sum+=i;
      }
    }if (sum==i) {
    cout << i;
}

}
return 0;
}