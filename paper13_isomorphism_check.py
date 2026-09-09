#!/usr/bin/env python3
from pathlib import Path

OUT = Path(__file__).with_name('paper13_local_rigidity_a12_fast_output.txt')

def A12():
    a=[1,0,0]+list(range(4,12))+[3]
    b=[0,2,3]+list(range(4,12))+[1]
    return a,b

def T12():
    a=[1,0,0]+list(range(4,12))+[3]
    b=[0,2,3,4,5,6,7,8,9,10,1,11]
    return a,b

def parse_survivors(text):
    rows=[]
    for ln in text.splitlines():
        if not (ln.startswith('S') and len(ln)>1 and ln[1].isdigit()):
            continue
        p=ln.split()
        idx=int(p[0][1:])
        ai=p.index('a'); bi=p.index('b',ai+1)
        a=[int(x)-1 for x in p[ai+1:bi]]
        b=[int(x)-1 for x in p[bi+1:]]
        rows.append((idx,a,b,p[3].split('=',1)[1]))
    return rows

def isomorphic(X,Y):
    ax,bx=X; ay,by=Y; n=len(ax)
    # Strong connectivity means the image of one seed determines the whole map.
    for sx in range(n):
        for sy in range(n):
            phi={sx:sy}; inv={sy:sx}; stack=[sx]; ok=True
            while stack and ok:
                u=stack.pop(); v=phi[u]
                for tx,ty in ((ax,ay),(bx,by)):
                    u2=tx[u]; v2=ty[v]
                    if u2 in phi:
                        if phi[u2]!=v2:
                            ok=False; break
                    else:
                        if v2 in inv:
                            ok=False; break
                        phi[u2]=v2; inv[v2]=u2; stack.append(u2)
            if ok and len(phi)==n:
                return True
    return False

rows=parse_survivors(OUT.read_text())
assert len(rows)==10
A=A12(); T=T12()
ca=ct=other=0
print('PAPER13_ISOMORPHISM_CHECK')
for idx,a,b,cyc in rows:
    ia=isomorphic((a,b),A)
    it=isomorphic((a,b),T)
    assert not (ia and it)
    cls='A12' if ia else ('T12' if it else 'OTHER')
    ca += ia; ct += it; other += (not ia and not it)
    print(f'S{idx}: cycle={cyc} class={cls}')
print(f'A12_class={ca}')
print(f'T12_class={ct}')
print(f'OTHER={other}')
assert (ca,ct,other)==(8,2,0)
print('ISOMORPHISM_CLASSIFICATION=PASS')
