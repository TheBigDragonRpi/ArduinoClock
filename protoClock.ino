const int dim = 240

int u[10],d[10],h[10],m[10];
int z[240];




bool c(int x,int y,int r){
  return ((r-5)**2<(x-(dim-1)/2)**2+(y-(dim-1)/2)**2<r**2)
}
bool e(int x,int y,int r){
  return ((r**2)-(63*(sqrt(abs(y-((dim-1)/2)))))<((x-(dim-1)/2)/2)**2+((y-(dim-1)/2)/1.078)**2<r**2)
}
bool l(int y,int p){
  return(p+5>abs(y-(size-1)/2)>=p)
}







void draw(int dd, int ud, int du, int uu){

}

void setup() {
  for (x = 0; x<size;x++){
    for(y=0;y<size;y++){
        if((!(5>y-(size-1)/2>-5)) && !(((x-(size-1)/2)**2+(y-(size-1)/2)**2)<61**2)){
            if(l(x,0)){
                z.append((x,y));
            }
            if(y<=(size-1)/2){
                if(x>=(size-1)/2){ #u
                    setNum(u,x,y);
                }else{ #d
                    setNum(d,x,y);
                }
            }else{
                if(x>=(size-1)/2){ #u
                    setNum(h,x,y);
                }else{ #d
                    setNum(m,x,y);
                }
           }
      }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
