#include "GPTree.h" 

struct timeval tv;
long long Additional_Memory = 0; //用于构建辅助矩阵的额外空间(int)
long long ts, te;

#define TIME_TICK_START      \
	gettimeofday(&tv, NULL); \
	ts = tv.tv_sec * 100000 + tv.tv_usec / 10;
#define TIME_TICK_END        \
	gettimeofday(&tv, NULL); \
	te = tv.tv_sec * 100000 + tv.tv_usec / 10;
#define TIME_TICK_PRINT(T) printf("%s RESULT: %lf Sec\r\n", (#T), (te - ts)/100000.0);

using namespace std ;

#define MAXP 400005 
double px[MAXP], py[MAXP] ;

void graphInit()
{
    char cwd[1000];
    getcwd(cwd, 1000);
    cout << "Current working dir is: " << cwd << endl;

	TIME_TICK_START
        init();
        FILE* dataFile, *nodeFile ;
        if((dataFile=fopen("data//GP_Tree.data","r"))!=NULL)
        {
            printf("GP_Tree.data already exist, loading...\n") ;
            fclose(dataFile) ;
            load() ;
            printf("Load completed\n") ;
        }
        else
        {
            printf("No data file existed, initializing...\n") ;
            read() ;
            Additional_Memory=2*Gn()*log2(Gn());
            printf("G.real_border:%d\n",Grealnode()) ;
            build() ;
            printf("Build complete\n") ;
            save() ;
        }
        nodeFile=fopen("data//road.cnode", "r") ;
        int idx ;
        double x, y ;
        while(fscanf(nodeFile, "%d %lf %lf", &idx, &x, &y) > 0)
            px[idx] = x, py[idx] = y ;
    TIME_TICK_END
	TIME_TICK_PRINT("Build")

    printf("Testing graph...\n") ;
    TIME_TICK_START
        for(int i=0;i<10000;i++)
        {
            if(i % 1000 == 0)
                printf("%d\n", i) ;
            int S=rand()%Gn();
            int T=rand()%Gn();
            int dis=search(S,T);
        }
    TIME_TICK_END
    TIME_TICK_PRINT("Test")
}

#define MaxCarNum 100005

int carNum ;
int pos[MaxCarNum], avaliable[MaxCarNum] ;
vector<int> passenger[MaxCarNum] ;
int cars[MaxCarNum] ;
int dist[MaxCarNum][5][5] ;
int D1[MaxCarNum], D2, D3, D4 ;
#define paNum(idx) (passenger[idx].size()) 

bool arrived[5] ;
int order[5] ;
int driveTime(int car, int deep, int current)
{
    if(deep == paNum(car)) return 0 ;
    int ret = 0x7fffffff ;
    for(int i = 0; i < paNum(car); i ++)
        if(!arrived[i]) 
        {
            int tmp = (deep ? dist[car][current][i] : search(passenger[car][i], current)) ;
            arrived[i] = 1 ;
            tmp = tmp+driveTime(car, deep+1, i) ;
            arrived[i] = 0 ;
            if(tmp < ret)
            {
                order[deep] = i ;
                ret = tmp ;
            }
        }    
    return ret ;
}

int driveTimeFast(int car, int deep, int current, int mxDist)
{
    if(deep == paNum(car)) return 0 ;
    for(int i = 0; i < paNum(car); i ++)
        if(!arrived[i]) 
        {
            int tmp = (deep ? dist[car][current][i] : search(passenger[car][i], current)) ;
            if(mxDist < tmp) continue ;
            arrived[i] = 1 ;
            tmp = tmp+driveTimeFast(car, deep+1, i, mxDist-tmp) ;
            arrived[i] = 0 ;
            if(tmp <= mxDist)
            {
                order[deep] = i ;
                return tmp ;
            }
        }    
    return mxDist+10000 ;
}

int driveTimeReallyFast(int car, int deep, int current, int mxDist)
{
    if(deep == paNum(car)) return 0 ;
    int mn = 1000000, best ;
    for(int i = 0; i < paNum(car); i ++)
        if(!arrived[i]) 
        {
            int tmp = (deep ? dist[car][current][i] : search(passenger[car][i], current)) ;
            if(tmp < mn) mn = tmp, best = i ;
        }    
    if(mn > mxDist) return mxDist+1000 ;
    order[deep] = best, arrived[best] = 1 ;
    int ret = mn+driveTimeReallyFast(car, deep+1, best, mxDist-mn) ;
    arrived[best] = 0 ;
    return ret ;
}

void carInit() 
{
    printf("Begin inputing car info...\n") ;
    FILE* carFile ;
    carFile = fopen("data//car.txt", "r") ;

    bool D1FileFlag = false ;
    FILE* D1File ;
    if((D1File=fopen("data//D1.data","r"))!=NULL)
    {
        printf("Detected D1 File...\n") ;
        D1FileFlag = true ;
    }
    else D1File=fopen("data//D1.data","w") ;

    int idx, passengerNum ;
    while(fscanf(carFile, "%d %d", &idx, &passengerNum) > 0)
    {
        double x, y ;
        int position ;
        fscanf(carFile, "%lf,%lf,%d", &x, &y, &position) ;
        //printf("%lf %lf %d\n", x, y, position) ;
        pos[carNum] = position ;
        for(int i = 0; i < passengerNum; i ++)
        {
            fscanf(carFile, "%lf,%lf,%d", &x, &y, &position) ;
            passenger[carNum].push_back(position) ;
        }
        avaliable[carNum] = (passengerNum < 4) ;
        for(int i = 0; i < passengerNum; i ++)
            for(int j = i+1; j < passengerNum; j ++)
            {
                if(!D1FileFlag)
                {
                    dist[carNum][i][j] = dist[carNum][j][i] = search(passenger[carNum][i], passenger[carNum][j]) ;
                    fprintf(D1File, "%d ", dist[carNum][i][j]) ;
                }
                else fscanf(D1File, "%d", &dist[carNum][i][j]), dist[carNum][j][i] = dist[carNum][i][j] ;
            }
        if(!D1FileFlag)
        {
            D1[carNum] = driveTime(carNum, 0, position) ;
            fprintf(D1File, "%d\n", D1[carNum]) ;
        }
        else fscanf(D1File, "%d", &D1[carNum]) ;
        cars[carNum ++] = carNum ;
        if(carNum %1000 == 0) printf("%d\n", carNum) ;
    }
    random_shuffle(cars, cars+carNum) ;
    printf("Cars input finished...\n") ;

}

double rawDist(int a, int b)
{
    return sqrt((px[a]-px[b])*(px[a]-px[b])+(py[a]-py[b])*(py[a]-py[b]))*100*1000 ;
}

int main()
{
    graphInit() ;
    carInit() ;
    printf("Initialization complete.\n") ;

    int S, T ;
    int start, dest ;

    while(scanf("%d %d", &start, &dest) > 0) 
    {
        if(start == -1) break ;
        D4 = search(start, dest) ;
        int res[1000], w1[1000], w2[1000], cnt = 0, mndist = 100000 ;
        int tt = 0, i ;
        for(int k = 0; k < carNum; k ++)
        {
            i = cars[k] ;
            // if(i %1000 == 0) printf("%d %d %d\n", i, cnt, mndist) ;
            if(!avaliable[i] || rawDist(start, pos[i]) > 10000) continue ;
            D2 = search(start, pos[i]) ;
            if(D2 > 10000 || D1[i]-D4 > 10000) continue ;
            tt ++ ;
            int t = passenger[i].size() ;
            for(int j = 0; j < t; j ++)
                dist[i][j][t] = dist[i][t][j] = search(dest, passenger[i][j]) ;
            passenger[i].push_back(dest) ;
            //D3 = driveTimeFast(i, 0, start, min(10000+D1[i]-D2, 10000+D4)) ;
            D3 = driveTimeFast(i, 0, start, min(10000+D1[i]-D2, 10000+D4)) ;
            passenger[i].pop_back() ;
            mndist = min(mndist, max(D2+D3-D1[i], D3-D4)) ;
            if(D2+D3-D1[i] > 10000 || D3-D4 > 10000) continue ;
            //printf("found one! %d, %d %d %d %d\n", passenger[i].size(), D1[i], D2, D3, D4) ;
            w1[cnt] = D2+D3-D1[i], w2[cnt] = D3-D4 ;
            res[cnt++] = i ;
            if(cnt == 5) break ;
        }
        //printf("!! %d\n", tt) ;
        for(int i = 0; i < cnt; i ++)
        {
            printf("Avaliable car #%d: %d\n", i, res[i]) ;
            printf("    Cost of other passengers: %d\n", w1[i]) ;
            printf("    Cost of yourself: %d\n", w2[i]) ;
        }
    }
    return 0 ;
}

