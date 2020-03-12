#include"wr_model.h"

int main()
{
    //创建一个读写模型对象
    wrmode wr;

    pthread_t writer;
    pthread_t reader1;
    pthread_t reader2;
    pthread_t reader3;
    pthread_t reader4;
    
    pthread_create(&writer, NULL, wrmode::writer, &wr);
    pthread_create(&reader1, NULL, wrmode::reader, &wr);
    pthread_create(&reader2, NULL, wrmode::reader, &wr);
    pthread_create(&reader3, NULL, wrmode::reader, &wr);
    pthread_create(&reader4, NULL, wrmode::reader, &wr);

    sleep(15);

    return 0;
}
