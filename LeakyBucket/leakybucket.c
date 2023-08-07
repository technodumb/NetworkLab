#include<stdio.h>
#include<stdlib.h>

int min(int a, int b) {
    return a < b ? a : b;
}

int main(){
    int bucketValue=0, bucketSize=0, totalCount=0, noOfPackets, outputRate;
    int incomingPackets[100], remainingPackets[100], outgoingPackets[100], droppedPackets[100];
    
    printf("Enter Bucket size: ");
    scanf("%d", &bucketSize);
    printf("Enter Output rate: ");
    scanf("%d", &outputRate);
    printf("Enter No of Packets: ");
    scanf("%d", &noOfPackets);

    for(int i=0; i<noOfPackets; i++){
        printf("Enter Packet no. %d: ", i+1);
        scanf("%d", &incomingPackets[i]);
        remainingPackets[i]=0;
    }

    for(int i=0; i<noOfPackets; i++){
        if(bucketValue+incomingPackets[i]<=bucketSize){
            bucketValue += incomingPackets[i];
            droppedPackets[i] = 0;
        }
        else {
            droppedPackets[i] = incomingPackets[i];
        }

        if(bucketValue < outputRate){
            outgoingPackets[i] = bucketValue;
            bucketValue = 0;
        }
        else{
            outgoingPackets[i] = outputRate;
            bucketValue = bucketValue - outputRate;
        }

        remainingPackets[i] = bucketValue;
        totalCount++;
    }


    while(bucketValue > 0){
        if(bucketValue < outputRate){
            outgoingPackets[totalCount] = bucketValue;
            bucketValue = 0;
        }
        else{
            outgoingPackets[totalCount] = outputRate;
            bucketValue = bucketValue - outputRate;
        }
        incomingPackets[totalCount] = 0;
        droppedPackets[totalCount] = 0;
        remainingPackets[totalCount] = bucketValue;
        totalCount++;
    }

    printf("Time\tRecv\tSent\tDrop\tRem\n");
    

    for(int i=0;i<totalCount;i++){
        printf("%d\t%d\t%d\t%d\t%d\n", i+1, incomingPackets[i], outgoingPackets[i], droppedPackets[i], remainingPackets[i]);
    }
}