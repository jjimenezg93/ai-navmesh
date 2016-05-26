#ifndef _STEERING_H_
#define _STEERING_H_

struct Accelerations;
class Character;

class Steering {
public:
	Steering();
	virtual ~Steering();

	virtual void Init(Character * ch);

	virtual void DrawDebug() = 0;

	//modifies character accelerations
	virtual void Update(Accelerations &acc, USVec2D target);
protected:
	Character * GetCh() { return mCh; }
private:
	Character * mCh;
};

#endif //!_STEERING_H_