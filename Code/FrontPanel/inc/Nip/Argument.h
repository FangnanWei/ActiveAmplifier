#ifndef Argument_H__
#define Argument_H__

extern "C" {
#include "stm8s.h"
}

#include "Util/Array.h"

// 该方法的声明应该位于类声明的私有部分
#define DISALLOW_COPY_AND_ASSIGN(TypeName)	\
private: \
    TypeName(const TypeName &);				\
    void operator = (const TypeName &)
	  
class BranchArg;

class Argument{
public:
	Argument(BranchArg *parent, const char *fieldName);
	virtual ~Argument();
	void SetFieldName(const char *fieldName);
protected:
	char 		*fieldName_;
	BranchArg	*parent_;

private:
	DISALLOW_COPY_AND_ASSIGN(Argument);
};


class BranchArg : public Argument {
public:
	BranchArg(BranchArg *parent, const char *fieldName, int initSize, int maxSize);
	bool AddSon(Argument *arg);

	
private:
  	Array<Argument *> args_;
	friend class Argument;
	
	DISALLOW_COPY_AND_ASSIGN(BranchArg);
};


#endif