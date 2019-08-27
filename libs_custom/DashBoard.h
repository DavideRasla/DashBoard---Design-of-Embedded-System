#define HsmGetState(me_) ((me_)->state_)
#define CLASS(class_) typedef struct class_ class_;\ struct class_{
#define SUBCLASS(class_, superclass_) \CLASS(class_)\ superclass_ super;
#define METHODS };
#define END_CLASS


CLASS(Hsm)
	State state_;
	State source_;
METHODS
	Hsm *HsmCtor_();
	void HsmXtor_();

	void HsmInit();
	void HsmDispatch();
	void HsmTran_();

	State Hsm_top();
END_CLASS

SUBCLASS(ChildHS)
