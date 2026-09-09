#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

static constexpr int N = 12;
static constexpr int FULL = (1<<N)-1;
struct Aut { std::array<uint8_t,N> a{}, b{}; };

static Aut known_A12(){
    Aut x;
    x.a[0]=1; x.b[0]=0;
    x.a[1]=0; x.b[1]=2;
    x.a[2]=0; x.b[2]=3;
    for(int i=3;i<=10;i++){ x.a[i]=i+1; x.b[i]=i+1; }
    x.a[11]=3; x.b[11]=1;
    return x;
}
static inline uint16_t image_mask(uint16_t m,const std::array<uint8_t,N>& t){
    uint16_t out=0; while(m){ unsigned i=__builtin_ctz((unsigned)m); m&=(uint16_t)(m-1); out|=(uint16_t)(1u<<t[i]); } return out;
}
static bool strongly_connected(const Aut& x){
    bool adj[N][N]={}, radj[N][N]={};
    for(int i=0;i<N;i++){adj[i][x.a[i]]=adj[i][x.b[i]]=true;radj[x.a[i]][i]=radj[x.b[i]][i]=true;}
    auto reach=[&](bool g[N][N]){bool seen[N]={};int q[N],h=0,t=0;q[t++]=0;seen[0]=true;while(h<t){int u=q[h++];for(int v=0;v<N;v++)if(g[u][v]&&!seen[v]){seen[v]=true;q[t++]=v;}}return t==N;};
    return reach(adj)&&reach(radj);
}
static int avoiding_threshold(const Aut& x,bool* sync_out=nullptr){
    static uint16_t q[1<<N]; static uint16_t stamp[1<<N]; static uint16_t gen=1;
    if(++gen==0){std::memset(stamp,0,sizeof(stamp));gen=1;}
    int dist[N];for(int i=0;i<N;i++)dist[i]=-1;int missing=0;bool sync=false;
    int h=0,t=0,depth=0;q[t++]=FULL;stamp[FULL]=gen;
    while(h<t){int end=t;while(h<end){uint16_t m=q[h++]; if((m&(m-1))==0)sync=true;
        uint16_t absent=(uint16_t)(FULL^m);while(absent){int i=__builtin_ctz((unsigned)absent);absent&=(uint16_t)(absent-1);if(dist[i]<0){dist[i]=depth;missing++;}}
        if(missing==N&&sync){int mx=0;for(int i=0;i<N;i++)mx=std::max(mx,dist[i]);if(sync_out)*sync_out=true;return mx;}
        uint16_t m1=image_mask(m,x.a),m2=image_mask(m,x.b);if(stamp[m1]!=gen){stamp[m1]=gen;q[t++]=m1;}if(stamp[m2]!=gen){stamp[m2]=gen;q[t++]=m2;}
    } depth++;}
    if(sync_out)*sync_out=sync;if(missing<N)return -1;int mx=0;for(int i=0;i<N;i++)mx=std::max(mx,dist[i]);return mx;
}
static bool is_permutation(const std::array<uint8_t,N>& t){int c[N]={};for(int i=0;i<N;i++)c[t[i]]++;for(int i=0;i<N;i++)if(c[i]!=1)return false;return true;}
static std::vector<int> cycle_lengths(const std::array<uint8_t,N>& t){std::vector<int> out;bool seen[N]={};for(int i=0;i<N;i++)if(!seen[i]){int u=i,len=0;do{seen[u]=true;u=t[u];len++;}while(!seen[u]);out.push_back(len);}std::sort(out.begin(),out.end(),std::greater<int>());return out;}
static std::string cycstr(const std::vector<int>& v){std::string s;for(size_t i=0;i<v.size();i++){if(i)s+="+";s+=std::to_string(v[i]);}return s;}
struct Surv{Aut x;int d;int at;std::string pcyc;};

int main(){
    Aut base=known_A12(); long long total[4]={},valid[4]={},ge21[4]={},ge22[4]={}; std::vector<Surv> surv;
    auto eval=[&](Aut& x,int d){total[d]++;if(!strongly_connected(x))return;bool sync=false;int at=avoiding_threshold(x,&sync);if(!sync)return;valid[d]++;if(at>=21){ge21[d]++;if(at>=22)ge22[d]++;std::string p="none";if(is_permutation(x.b))p="b:"+cycstr(cycle_lengths(x.b));else if(is_permutation(x.a))p="a:"+cycstr(cycle_lengths(x.a));surv.push_back({x,d,at,p});}};
    auto setp=[](Aut& x,int p,int v){if(p<N)x.a[p]=v;else x.b[p-N]=v;};
    auto getp=[](const Aut& x,int p){return p<N?(int)x.a[p]:(int)x.b[p-N];};
    for(int p1=0;p1<24;p1++){int o1=getp(base,p1);for(int v1=0;v1<N;v1++)if(v1!=o1){Aut x=base;setp(x,p1,v1);eval(x,1);}}
    for(int p1=0;p1<24;p1++)for(int p2=p1+1;p2<24;p2++){int o1=getp(base,p1),o2=getp(base,p2);for(int v1=0;v1<N;v1++)if(v1!=o1)for(int v2=0;v2<N;v2++)if(v2!=o2){Aut x=base;setp(x,p1,v1);setp(x,p2,v2);eval(x,2);}}
    for(int p1=0;p1<24;p1++)for(int p2=p1+1;p2<24;p2++)for(int p3=p2+1;p3<24;p3++){int o1=getp(base,p1),o2=getp(base,p2),o3=getp(base,p3);for(int v1=0;v1<N;v1++)if(v1!=o1)for(int v2=0;v2<N;v2++)if(v2!=o2)for(int v3=0;v3<N;v3++)if(v3!=o3){Aut x=base;setp(x,p1,v1);setp(x,p2,v2);setp(x,p3,v3);eval(x,3);}}
    std::cout<<"PAPER13_LOCAL_RIGIDITY_A12\n";long long T=0,V=0,G21=0,G22=0;for(int d=1;d<=3;d++){T+=total[d];V+=valid[d];G21+=ge21[d];G22+=ge22[d];std::cout<<"distance="<<d<<" total="<<total[d]<<" sc_sync="<<valid[d]<<" at1_ge_21="<<ge21[d]<<" at1_ge_22="<<ge22[d]<<"\n";}std::cout<<"distance_le_3 total="<<T<<" sc_sync="<<V<<" at1_ge_21="<<G21<<" at1_ge_22="<<G22<<"\n";
    std::map<std::string,int> sig;for(auto&s:surv)sig[s.pcyc]++;std::cout<<"survivor_cycle_type_counts:\n";for(auto&kv:sig)std::cout<<"  "<<kv.first<<" -> "<<kv.second<<"\n";
    std::cout<<"survivors="<<surv.size()<<"\n";for(size_t k=0;k<surv.size();k++){auto&s=surv[k];std::cout<<"S"<<k+1<<" d="<<s.d<<" at1="<<s.at<<" perm_cycle="<<s.pcyc<<"\n a:";for(int i=0;i<N;i++)std::cout<<" "<<(int)s.x.a[i]+1;std::cout<<"\n b:";for(int i=0;i<N;i++)std::cout<<" "<<(int)s.x.b[i]+1;std::cout<<"\n";}
}
