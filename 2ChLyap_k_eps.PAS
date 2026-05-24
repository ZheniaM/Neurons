var ii,l,i,j,nk:longint;
    xc,yc,uc,vc,xi,coef,eps,k,lam,mu,p1,p2,p3,p4,d1,d2,d3,d4,mod,p,sum,
    x0,y0,u0,v0,x1,y1,u1,v1,xx0,xx1,yy0,yy1,uu0,uu1,vv0,vv1,
    yn,xn1,yn1,r1,r2,xn,r,w,aa,bb,cc:extended;
    mlx,mly,mlz:text;
    his:array[1..151,1..151] of extended;
   const n=1000000;
   a=0.89; c=0.28; Iapp=0.02; b=0.18;
   kn=0.0;kk=0.05; epsn=0.0001;epsk=0.02; kol=200000;
   nh=30; DELTA=0.000001;
   label lab,lab1;

function f1(x,y,u,v:extended):extended;
begin
  f1:=x*x*exp(y-x)+Iapp+k*(u-x)
end;

function f2(x,y,u,v:extended):extended;
begin
  f2:=a*y-b*x+c;
end;

function g1(x,y,u,v:extended):extended;
begin
  g1:=u*u*exp(v-u)+Iapp+k*(x-u)
end;

function g2(x,y,u,v:extended):extended;
begin
  g2:=a*v-b*u+c;
end;

begin
  assign(mlx,'e:\user\bash\Chialvo\Chialvo_coupled\mlx.txt'); rewrite(mlx);
  assign(mly,'e:\user\bash\Chialvo\Chialvo_coupled\mly.txt'); rewrite(mly);
  assign(mlz,'e:\user\bash\Chialvo\Chialvo_coupled\mlz.txt'); rewrite(mlz);
  randomize;
  coef:=exp(ln(epsk/epsn)/(nh));
  x0:=2; y0:=1; u0:=1; v0:=2;

  {pereh}
  for ii:=1 to 200000 do begin
    x1:=f1(x0,y0,u0,v0);
    y1:=f2(x0,y0,u0,v0);
    u1:=g1(x0,y0,u0,v0);
    v1:=g2(x0,y0,u0,v0);
    x0:=x1; y0:=y1; u0:=u1; v0:=v1;
  end;

  xc:=x0;yc:=y0;uc:=u0;vc:=v0;

  for i:=1 to nh+1 do begin
    k:=kn+(i-1)*(kk-kn)/nh;
    writeln(i,' ',k);
    xc:=x0;yc:=y0;uc:=u0;vc:=v0;
    eps:=epsn;

    for j:=1 to nh+1 do begin
      x0:=xc;y0:=yc;u0:=uc;v0:=vc;
      xx0:=x0+DELTA/sqrt(30);
      yy0:=y0+2*DELTA/sqrt(30);
      uu0:=u0+3*DELTA/sqrt(30);
      vv0:=v0+4*DELTA/sqrt(30);
      sum:=0;

      for ii:=1 to kol do begin
        r1:=random; r2:=random;

        x1:=f1(x0,y0,u0,v0)+eps*sqrt(-2*ln(r1))*sin(2*pi*r2);
        y1:=f2(x0,y0,u0,v0);
        u1:=g1(x0,y0,u0,v0)+eps*sqrt(-2*ln(r1))*cos(2*pi*r2);
        v1:=g2(x0,y0,u0,v0);

        xx1:=f1(xx0,yy0,uu0,vv0)+eps*sqrt(-2*ln(r1))*sin(2*pi*r2);
        yy1:=f2(xx0,yy0,uu0,vv0);
        uu1:=g1(xx0,yy0,uu0,vv0)+eps*sqrt(-2*ln(r1))*cos(2*pi*r2);
        vv1:=g2(xx0,yy0,uu0,vv0);

        d1:=xx1-x1; d2:=yy1-y1; d3:=uu1-u1; d4:=vv1-v1;
        mod:=sqrt(sqr(d1)+sqr(d2)+sqr(d3)+sqr(d4));

        p:=mod/DELTA;
        sum:=sum+ln(p);
        p1:=d1*DELTA/mod;  p2:=d2*DELTA/mod; p3:=d3*DELTA/mod;  p4:=d4*DELTA/mod;
        x0:=x1; y0:=y1; u0:=u1; v0:=v1;
        xx0:=x0+p1; yy0:=y0+p2; uu0:=u0+p3;  vv0:=v0+p4;
      end;

      lam:=sum/kol;
      lab1:  his[i,j]:=lam;
      eps:=eps*coef;
    end;
  end;

  for i:=1 to nh+1 do begin
    for j:=1 to nh+1 do begin
      write(mlz,his[j,i],' ')
    end;
    writeln(mlz);
  end;

  eps:=epsn;

  for l:=1 to nh+1 do begin
    writeln(mlx,kn+(l-1)*(kk-kn)/nh);
    writeln(mly,eps);
    eps:=eps*coef;
  end;

  lab:close(mlx);close(mly);close(mlz)
end.
