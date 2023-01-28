#include <limits.h>
struct Timer{
private:
	unsigned long e=0;
	unsigned long long l=0;
public:
	void Cancel(){l=0;}
	void Reset(unsigned long v){
		l=v;
		e=millis();
	}
	void Update(){
		unsigned long d,n=millis();
		if(e>n){
			d=(ULONG_MAX-e)+n;
		}else{
			d=n-e;
		}
		e=n;
		if(l>0){
			if(d>l){l=0;}else{l-=d;}
		}
	}
	bool Running(bool u=true){
		if (u) Update();
		return l>0;
	}
};
