#include <stdio.h>
#include <stdbool.h>

int main() {

    while(true){
        loop();
    }
    return 0;
}

void loop(){

    double i;
    scanf("%lf", &i);
    if(i>=-1 && i<1){
        printf("%f appartient � I1\n", i);
    }else{
        printf("%f n'appartient pas � I1\n", i);
    }

    bool condition = (!(i < -10) && (i<-2 || i == 2)) || ( !(i<0 || i==0) && (i<1 && i == 1) ) || ( !(i<2) && i<3 );
    if(condition){
        printf("%f appartient � I2\n", i);
    }else{
        printf("%f n'appartient pas � I2\n", i);
    }

    return;
}
