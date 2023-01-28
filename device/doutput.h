struct DOutput{
private:
	unsigned char p;
public:
	bool Get(){
		return digitalRead(p);
	}
	bool Set(bool v){
		if (Get()==v) return false;
		digitalWrite(p,v);
		return true;
	}
	DOutput(unsigned char _p, bool _i=false){
		p=_p;
		pinMode(p,OUTPUT);
		digitalWrite(p,_i);
	}
};
