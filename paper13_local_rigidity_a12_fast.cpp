#include <algorithm>
#include <array>
#include <atomic>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#ifdef _OPENMP
#include <omp.h>
#endif

static constexpr int N=12, FULL=(1<<N)-1;
struct Aut{std::array<uint8_t,N>a{},b{};};
struct LUT{uint16_t x[3][16];};
struct Scratch{uint32_t seen[1<<N]{};uint16_t q[1<<N];uint32_t gen=0;};
struct Surv{Aut x;int d,at;std::string cyc;};

static Aut A12(){Aut x;x.a[0]=1;x.b[0]=0;x.a[1]=0;x.b[1]=2;x.a[2]=0;x.b[2]=3;for(int i=3;i<=10;i++){x.a[i]=i+1;x.b[i]=i+1;}x.a[11]=3;x.b[11]=1;return x;}
static LUT make_lut(const std::array<uint8_t,N>&t){uint16_t pre[N]={};for(int i=0;i<N;i++)pre[t[i]]|=(1u<<i);LUT L{};for(int g=0;g<3;g++){L.x[g][0]=0;for(int m=1;m<16;m++){int bit=__builtin_ctz((unsigned)m);L.x[g][m]=L.x[g][m&(m-1)]|pre[4*g+bit];}}return L;}
static inline uint16_t preim(uint16_t s,const LUT&L){return L.x[0][s&15]|L.x[1][(s>>4)&15]|L.x[2][(s>>8)&15];}
static int avoid_dist_q(int target,const LUT&A,const LUT&B,Scratch&sc,int limit){if(++sc.gen==0){std::memset(sc.seen,0,sizeof(sc.seen));sc.gen=1;}uint32_t G=sc.gen;int h=0,t=0,dep=0;uint16_t st=1u<<target;sc.q[t++]=st;sc.seen[st]=G;while(h<t&&dep<limit){int end=t;while(h<end){uint16_t s=sc.q[h++];uint16_t u=preim(s,A),v=preim(s,B);if(u==0||v==0)return dep+1;if(sc.seen[u]!=G){sc.seen[u]=G;sc.q[t++]=u;}if(sc.seen[v]!=G){sc.seen[v]=G;sc.q[t++]=v;}}dep++;}return limit+1;}
static bool any_ge21(const Aut&x,Scratch&sc){LUT A=make_lut(x.a),B=make_lut(x.b);static const int order[N]={0,1,2,3,4,5,6,7,8,9,10,11};for(int k=0;k<N;k++)if(avoid_dist_q(order[k],A,B,sc,20)>20)return true;return false;}
static int exact_at(const Aut&x,Scratch&sc){LUT A=make_lut(x.a),B=make_lut(x.b);int mx=0;for(int q=0;q<N;q++){int d=avoid_dist_q(q,A,B,sc,4095);if(d>4095)return -1;mx=std::max(mx,d);}return mx;}
static bool sc(const Aut&x){bool g[N][N]={},r[N][N]={};for(int i=0;i<N;i++){g[i][x.a[i]]=g[i][x.b[i]]=1;r[x.a[i]][i]=r[x.b[i]][i]=1;}auto f=[&](bool z[N][N]){bool s[N]={};int q[N],h=0,t=0;q[t++]=0;s[0]=1;while(h<t){int u=q[h++];for(int v=0;v<N;v++)if(z[u][v]&&!s[v]){s[v]=1;q[t++]=v;}}return t==N;};return f(g)&&f(r);}
static inline uint16_t img(uint16_t m,const std::array<uint8_t,N>&t){uint16_t o=0;while(m){int i=__builtin_ctz((unsigned)m);m&=m-1;o|=1u<<t[i];}return o;}
static bool sync(const Aut&x){bool seen[1<<N]={};uint16_t q[1<<N];int h=0,t=0;q[t++]=FULL;seen[FULL]=1;while(h<t){uint16_t m=q[h++];if((m&(m-1))==0)return true;uint16_t u=img(m,x.a),v=img(m,x.b);if(!seen[u]){seen[u]=1;q[t++]=u;}if(!seen[v]){seen[v]=1;q[t++]=v;}}return false;}
static bool perm(const std::array<uint8_t,N>&t){int c[N]={};for(int i=0;i<N;i++)c[t[i]]++;for(int i=0;i<N;i++)if(c[i]!=1)return false;return true;}
static std::string cyc(const std::array<uint8_t,N>&t){std::vector<int>v;bool s[N]={};for(int i=0;i<N;i++)if(!s[i]){int u=i,l=0;while(!s[u]){s[u]=1;u=t[u];l++;}v.push_back(l);}std::sort(v.rbegin(),v.rend());std::string z;for(int x:v){if(!z.empty())z+='+';z+=std::to_string(x);}return z;}

int main(){Aut base=A12(); long long total[4]={},gate[4]={},valid[4]={},ge21[4]={},ge22[4]={};std::vector<Surv>surv;std::mutex mu;
 auto get=[](const Aut&x,int p){return p<N?(int)x.a[p]:(int)x.b[p-N];};auto set=[](Aut&x,int p,int v){if(p<N)x.a[p]=v;else x.b[p-N]=v;};
 auto handle=[&](Aut&x,int d,Scratch&scratch,long long*ltotal,long long*lgate,long long*lvalid,long long*lge21,long long*lge22,std::vector<Surv>&ls){ltotal[d]++;if(!any_ge21(x,scratch))return;lgate[d]++;if(!sc(x)||!sync(x))return;lvalid[d]++;int at=exact_at(x,scratch);if(at>=21){lge21[d]++;if(at>=22)lge22[d]++;std::string c="none";if(perm(x.b))c="b:"+cyc(x.b);else if(perm(x.a))c="a:"+cyc(x.a);ls.push_back({x,d,at,c});}};

 // Enumerate by changed-position combinations; OpenMP parallel over combination index loops.
 #pragma omp parallel
 { Scratch scratch; long long lt[4]={},lg[4]={},lv[4]={},l21[4]={},l22[4]={};std::vector<Surv>ls;
   #pragma omp for schedule(dynamic,1)
   for(int p1=0;p1<24;p1++){int o1=get(base,p1);for(int v1=0;v1<N;v1++)if(v1!=o1){Aut x=base;set(x,p1,v1);handle(x,1,scratch,lt,lg,lv,l21,l22,ls);}}
   #pragma omp for schedule(dynamic,1)
   for(int code=0;code<276;code++){int c=0,p1=-1,p2=-1;for(int i=0;i<24&&p1<0;i++)for(int j=i+1;j<24;j++){if(c++==code){p1=i;p2=j;break;}}int o1=get(base,p1),o2=get(base,p2);for(int v1=0;v1<N;v1++)if(v1!=o1)for(int v2=0;v2<N;v2++)if(v2!=o2){Aut x=base;set(x,p1,v1);set(x,p2,v2);handle(x,2,scratch,lt,lg,lv,l21,l22,ls);}}
   #pragma omp for schedule(dynamic,1)
   for(int code=0;code<2024;code++){int c=0,p1=-1,p2=-1,p3=-1;for(int i=0;i<24&&p1<0;i++)for(int j=i+1;j<24&&p1<0;j++)for(int k=j+1;k<24;k++){if(c++==code){p1=i;p2=j;p3=k;break;}}int o1=get(base,p1),o2=get(base,p2),o3=get(base,p3);for(int v1=0;v1<N;v1++)if(v1!=o1)for(int v2=0;v2<N;v2++)if(v2!=o2)for(int v3=0;v3<N;v3++)if(v3!=o3){Aut x=base;set(x,p1,v1);set(x,p2,v2);set(x,p3,v3);handle(x,3,scratch,lt,lg,lv,l21,l22,ls);}}
   #pragma omp critical
   {for(int d=1;d<=3;d++){total[d]+=lt[d];gate[d]+=lg[d];valid[d]+=lv[d];ge21[d]+=l21[d];ge22[d]+=l22[d];}surv.insert(surv.end(),ls.begin(),ls.end());}
 }
 std::cout<<"PAPER13_LOCAL_RIGIDITY_A12_FAST\n";long long T=0,G=0,V=0,A=0,B=0;for(int d=1;d<=3;d++){T+=total[d];G+=gate[d];V+=valid[d];A+=ge21[d];B+=ge22[d];std::cout<<"distance="<<d<<" total="<<total[d]<<" gate_ge21="<<gate[d]<<" sc_sync_gate="<<valid[d]<<" at1_ge21="<<ge21[d]<<" at1_ge22="<<ge22[d]<<"\n";}std::cout<<"distance_le3 total="<<T<<" gate_ge21="<<G<<" sc_sync_gate="<<V<<" at1_ge21="<<A<<" at1_ge22="<<B<<"\n";
 std::map<std::string,int>m;for(auto&s:surv)m[s.cyc]++;std::cout<<"cycle_types\n";for(auto&kv:m)std::cout<<kv.first<<" "<<kv.second<<"\n";std::cout<<"survivors="<<surv.size()<<"\n";std::sort(surv.begin(),surv.end(),[](auto&x,auto&y){if(x.cyc!=y.cyc)return x.cyc<y.cyc;for(int i=0;i<N;i++)if(x.x.a[i]!=y.x.a[i])return x.x.a[i]<y.x.a[i];return x.x.b<y.x.b;});for(size_t k=0;k<surv.size();k++){auto&s=surv[k];std::cout<<"S"<<k+1<<" d="<<s.d<<" at="<<s.at<<" cyc="<<s.cyc<<" a";for(auto v:s.x.a)std::cout<<" "<<(int)v+1;std::cout<<" b";for(auto v:s.x.b)std::cout<<" "<<(int)v+1;std::cout<<"\n";}
}
