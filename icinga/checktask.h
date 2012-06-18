#ifndef CHECKTASK_H
#define CHECKTASK_H

namespace icinga
{

enum CheckState
{
	StateOK,
	StateWarning,
	StateCritical,
	StateUnreachable,
	StateUncheckable,
	StateUnknown
};

struct CheckResult
{
	time_t StartTime;
	time_t EndTime;

	CheckState State;
	string Output;
	Dictionary::Ptr PerformanceData;
};

struct CheckTaskType;

class I2_ICINGA_API CheckTask : public Object
{
public:
	typedef shared_ptr<CheckTask> Ptr;
	typedef weak_ptr<CheckTask> WeakPtr;

	typedef function<CheckTask::Ptr(const Service&)> Factory;
	typedef function<void()> QueueFlusher;

	Service GetService(void) const;

	virtual void Enqueue(void) = 0;
	virtual bool IsFinished(void) const = 0;
	virtual CheckResult GetResult(void) = 0;

	static void RegisterType(string type, Factory factory, QueueFlusher qflusher);
	static CheckTask::Ptr CreateTask(const Service& service);
	static void Enqueue(const CheckTask::Ptr& task);
	static void FlushQueue(void);

protected:
	CheckTask(const Service& service);

private:
	Service m_Service;

	static map<string, CheckTaskType> m_Types;
};

struct CheckTaskType
{
	CheckTask::Factory Factory;
	CheckTask::QueueFlusher QueueFlusher;
};

}

#endif /* CHECKTASK_H */
